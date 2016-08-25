#include "attrwidget.h"
#include <metadatamodel/dbxmlstruct.h>
#include "xmldelegate.h"
#include <QStringListModel>
#include <QMessageBox>
#include <QToolTip>
#include <QCompleter>
#include "treefilterproxymodel.h"
#include "regexpvalidator.h"

using namespace RTPTechGroup::MetaDataModel;
using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

AttrWidget::AttrWidget(QWidget *parent) :
    AbstractItemWidget(parent)
{
    setupUi(this);

    RegExpValidator *validator =
            new RegExpValidator(QRegExp("^[A-Za-z]{1}[A-Za-z0-9_]{0,26}|^[A-Za-z]{0}"));
    lineEditAttrName->setValidator(validator);
    connect(validator,SIGNAL(stateChanged(QValidator::State)),
            this,SLOT(validateAttrName(QValidator::State)));

    QStringList tags;
    tags << DBATTRXML::ATTR;
    proxyModel()->setAttributeTags(tags);

    m_attrGroupModel = new AttrGroupProxyModel();
    QCompleter *groupCompleter = new QCompleter(lineEditAttrGroup);
    groupCompleter->setModel(m_attrGroupModel);
    lineEditAttrGroup->setCompleter(groupCompleter);

    m_typeAttrModel = new QStringListModel();
    const QStringList attrTypeList = (QStringList()
                                       << DBTYPEXML::BOOLEAN
                                       << DBTYPEXML::BINARY
                                       << DBTYPEXML::CHAR
                                       << DBTYPEXML::DATE
                                       << DBTYPEXML::DECIMAL
                                       << DBTYPEXML::DOUBLE
                                       << DBTYPEXML::INTEGER
                                       << DBTYPEXML::STRING
                                       << DBTYPEXML::REFERENCE
                                       << DBTYPEXML::TIME
                                       << DBTYPEXML::TIMESHTAMP
                                       );
    m_typeAttrModel->setStringList(attrTypeList);

    AbstractItemWidget::setItemView(tableViewAttr);
    comboBoxLov->setItemDelegate(new XmlDelegate(this));
    comboBoxNumerator->setItemDelegate(new XmlDelegate(this));

    connect(tableViewAttr,SIGNAL(clicked(QModelIndex)),
            this,SLOT(setCurrent(QModelIndex)));
    connect(toolButtonAddAttr,SIGNAL(clicked()),this,SLOT(add()));
    connect(toolButtonDeleteAttr,SIGNAL(clicked()),this,SLOT(remove()));

    connect(comboBoxTypeAttr,SIGNAL(currentIndexChanged(QString)),this,SLOT(changeType(QString)));
    connect(pushButtonAttrSave,SIGNAL(clicked()),this,SLOT(submit()));
    connect(pushButtonAttrCancel,SIGNAL(clicked()),this,SLOT(revert()));
    connect(toolButtonEditAttr,SIGNAL(clicked()),this,SLOT(edit()));
    connect(checkBoxInInherited,SIGNAL(clicked(bool)),this,SLOT(showParentAttr(bool)));

    connect(toolButtonUnitAttrClean, SIGNAL(clicked()), this, SLOT(cleanUnit()));
    connect(toolButtonLovClean, SIGNAL(clicked()), this, SLOT(cleanLov()));
    connect(toolButtonNumeratorClean, SIGNAL(clicked()), this, SLOT(cleanNumerator()));

    connect(toolButtonUpAttr,SIGNAL(clicked()),this,SLOT(up()));
    connect(toolButtonDownAttr,SIGNAL(clicked()),this,SLOT(down()));
}

AttrWidget::~AttrWidget()
{
    delete lineEditAttrName->validator();

    delete m_typeAttrModel;
    delete m_attrGroupModel;
}

void AttrWidget::setModel(TreeXmlHashModel *model)
{
    AbstractItemWidget::setModel(model);

    proxyModel()->setHeaderData(0,  Qt::Horizontal, tr("Имя атрибута"));
    proxyModel()->setHeaderData(1,  Qt::Horizontal, tr("Псевдоним"));
    proxyModel()->setHeaderData(2,  Qt::Horizontal, tr("Тип"));
    proxyModel()->setHeaderData(3,  Qt::Horizontal, tr("Длина строки"));
    proxyModel()->setHeaderData(4,  Qt::Horizontal, tr("Точность"));
    proxyModel()->setHeaderData(5,  Qt::Horizontal, tr("Ссылочный класс"));
    proxyModel()->setHeaderData(6,  Qt::Horizontal, tr("Класс"));
    proxyModel()->setHeaderData(7,  Qt::Horizontal, tr("ЕИ"));
    proxyModel()->setHeaderData(8,  Qt::Horizontal, tr("По умолчанию"));
    proxyModel()->setHeaderData(9,  Qt::Horizontal, tr("Нижняя граница"));
    proxyModel()->setHeaderData(10, Qt::Horizontal, tr("Верхняя гранница"));
    proxyModel()->setHeaderData(11, Qt::Horizontal, tr("Список значений"));
    proxyModel()->setHeaderData(12, Qt::Horizontal, tr("Нумератор"));
    proxyModel()->setHeaderData(13, Qt::Horizontal, tr("Группа"));
    proxyModel()->setHeaderData(14, Qt::Horizontal, tr("Нулевые значения"));
    proxyModel()->setHeaderData(15, Qt::Horizontal, tr("Уникальный"));
    proxyModel()->setHeaderData(16, Qt::Horizontal, tr("Кандидат в ключ"));
    proxyModel()->setHeaderData(17, Qt::Horizontal, tr("Индетификатор"));

    proxyModel()->setDynamicSortFilter(true);
    tableViewAttr->setColumnHidden(17,true);
    m_attrGroupModel->setModel(model);

    TableXMLProxyModel *lovFilterModel = new TableXMLProxyModel(this);
    QStringList tags;
    tags << DBLOVLISTXML::LOVLIST;
    lovFilterModel->setAttributeTags(tags);
    lovFilterModel->setSourceModel(model);
    lovFilterModel->setFilterIndex(model->index(0,0));
    lovFilterModel->setFilterRole(Qt::EditRole);
    lovFilterModel->setFilterKeyColumn(model->columnDisplayedAttr(
                                           DBLOVXML::LOV,DBLOVXML::TYPE));
    lovFilterModel->setDynamicSortFilter(true);
    lovFilterModel->sort(0);
    comboBoxLov->setModel(lovFilterModel);
    comboBoxLov->setRootModelIndex(lovFilterModel->index(0,0).child(0,0));
    lovFilterModel->setFilterIndex(lovFilterModel->mapToSource(lovFilterModel->index(0,0).child(0,0)));
    tags << DBLOVXML::LOV;
    lovFilterModel->setAttributeTags(tags);

    TableXMLProxyModel *numeratorFilterModel = new TableXMLProxyModel(this);
    tags.clear();
    tags << DBNUMERATORLISTXML::NUMERATORLIST;
    numeratorFilterModel->setAttributeTags(tags);
    numeratorFilterModel->setSourceModel(model);
    numeratorFilterModel->setFilterIndex(model->index(0,0));
    numeratorFilterModel->setFilterRole(Qt::EditRole);
    numeratorFilterModel->setDynamicSortFilter(true);
    numeratorFilterModel->sort(0);
    comboBoxNumerator->setModel(numeratorFilterModel);
    comboBoxNumerator->setRootModelIndex(numeratorFilterModel->index(0,0).child(0,0));
    numeratorFilterModel->setFilterIndex(
                numeratorFilterModel->mapToSource(
                    numeratorFilterModel->index(0,0).child(0,0))
                );
    tags << DBNUMERATORXML::NUMERATOR;
    numeratorFilterModel->setAttributeTags(tags);

    QSortFilterProxyModel *classFilterModel = new QSortFilterProxyModel(this);
    classFilterModel->setFilterKeyColumn(0);
    classFilterModel->setFilterRole(TreeXmlModel::TagRole);
    classFilterModel->setFilterRegExp(DBCLASSXML::CLASS + "|" +
                                      DBMODELXML::MODEL + "|" +
                                      DBCLASSLISTXML::CLASSLIST);
    classFilterModel->setSourceModel(model);
    classFilterModel->setDynamicSortFilter(true);
    classFilterModel->sort(0);

    comboBoxLinkAttr->setModel(classFilterModel);
    comboBoxLinkAttr->setRootModelIndex(classFilterModel->index(0,0).child(0,0));
    comboBoxLinkAttr->setIndexColumn(model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                                DBCLASSXML::ID));

    QSortFilterProxyModel *unitFilterModel = new QSortFilterProxyModel(this);
    unitFilterModel->setFilterKeyColumn(0);
    unitFilterModel->setFilterRole(TreeXmlModel::TagRole);
    unitFilterModel->setFilterRegExp(DBUNITXML::UNIT + "|" +
                                     DBMODELXML::MODEL + "|" +
                                     DBQUANTITYLISTXML::QUANTITYLIST + "|" +
                                     DBQUANTITYGROUPXML::QUANTITYGROUP + "|" +
                                     DBQUANTITYXML::QUANTITY);
    unitFilterModel->setSourceModel(model);
    unitFilterModel->setDynamicSortFilter(false);

    comboBoxUnitAttr->setModel(unitFilterModel);
    comboBoxUnitAttr->setRootModelIndex(unitFilterModel->index(0,0).child(0,0));
    comboBoxUnitAttr->setIndexColumn(
                model->columnDisplayedAttr(DBUNITXML::UNIT,
                                           DBUNITXML::ID));

    comboBoxTypeAttr->setModel(m_typeAttrModel);

    dataMapper()->addMapping(lineEditAttrName,
                             model->columnDisplayedAttr(DBATTRXML::ATTR,
                                                        DBATTRXML::NAME));
    dataMapper()->addMapping(comboBoxTypeAttr,
                             model->columnDisplayedAttr(DBATTRXML::ATTR,
                                                        DBATTRXML::TYPE));
    dataMapper()->addMapping(spinBoxStringLen,
                             model->columnDisplayedAttr(DBATTRXML::ATTR,
                                                        DBATTRXML::MAXSTRLEN));
    dataMapper()->addMapping(spinBoxAccuracy,
                             model->columnDisplayedAttr(DBATTRXML::ATTR,
                                                        DBATTRXML::ACCURACY));
    dataMapper()->addMapping(comboBoxLinkAttr,
                             model->columnDisplayedAttr(DBATTRXML::ATTR,
                                                        DBATTRXML::REFCLASS));
    dataMapper()->addMapping(lineEditDefaultValue,
                             model->columnDisplayedAttr(DBATTRXML::ATTR,
                                                        DBATTRXML::INITIALVAL));
    dataMapper()->addMapping(lineEditLowerBound,
                             model->columnDisplayedAttr(DBATTRXML::ATTR,
                                                        DBATTRXML::LOWERBOUND));
    dataMapper()->addMapping(lineEditUpperBound,
                             model->columnDisplayedAttr(DBATTRXML::ATTR,
                                                        DBATTRXML::UPPERBOUND));
    dataMapper()->addMapping(checkBoxIsNull,
                             model->columnDisplayedAttr(DBATTRXML::ATTR,
                                                        DBATTRXML::ISNULLALLOWED));
    dataMapper()->addMapping(checkBoxIsUnique,
                             model->columnDisplayedAttr(DBATTRXML::ATTR,
                                                        DBATTRXML::ISUNIQUE));
    dataMapper()->addMapping(checkBoxIsCandidateKey,
                             model->columnDisplayedAttr(DBATTRXML::ATTR,
                                                        DBATTRXML::ISCANDIDATEKEY));
    dataMapper()->addMapping(lineEditAttrAlias,
                             model->columnDisplayedAttr(DBATTRXML::ATTR,
                                                        DBATTRXML::ALIAS));
    dataMapper()->addMapping(lineEditAttrGroup,
                             model->columnDisplayedAttr(DBATTRXML::ATTR,
                                                        DBATTRXML::GROUP));
    dataMapper()->addMapping(comboBoxUnitAttr,
                             model->columnDisplayedAttr(DBATTRXML::ATTR,
                                                        DBATTRXML::REFUNIT));

    dataMapper()->addMapping(comboBoxLov,
                             model->columnDisplayedAttr(DBATTRXML::ATTR,
                                                        DBATTRXML::REFLOV));
    dataMapper()->addMapping(comboBoxNumerator,
                             model->columnDisplayedAttr(DBATTRXML::ATTR,
                                                        DBATTRXML::REFNUMERATOR));

}

void AttrWidget::setRootIndex(const QModelIndex &index)
{
    AbstractItemWidget::setRootIndex(index);

    qint32 groupColumn = model()->columnDisplayedAttr(DBATTRXML::ATTR, DBATTRXML::GROUP);
    m_attrGroupModel->setUniqueColumn(groupColumn);
    m_attrGroupModel->setRootModelIndex(index);
    m_attrGroupModel->reset();
}

void AttrWidget::validateAttrName(QValidator::State state) const
{
    if(state != QValidator::Acceptable)
        QToolTip::showText(lineEditAttrName->mapToGlobal(QPoint(0,5)),
                           tr("Имя атрибута должно содержать только латинские\n"
                              "символы и цифры длиной не более 27 символов"));
    else
        QToolTip::hideText();
}

void AttrWidget::cleanUnit()
{
    comboBoxUnitAttr->setDisplayText("");
    comboBoxUnitAttr->setCurrentIndex(-1);
}

void AttrWidget::cleanLov()
{
    comboBoxLov->setCurrentIndex(-1);
}

void AttrWidget::cleanNumerator()
{
    comboBoxNumerator->setCurrentIndex(-1);
}

void AttrWidget::add()
{
    AbstractItemWidget::add(DBATTRXML::ATTR);
}

void AttrWidget::submit()
{
    QModelIndex rootIndex = (tableViewAttr->rootIndex());
    QModelIndex srcIndex = dataMapper()->rootIndex().child(dataMapper()->currentIndex(),0);
    qint32 nameColumn = model()->columnDisplayedAttr(DBATTRXML::ATTR, DBATTRXML::NAME);

    qint32 row = 0;
    QModelIndex childIndex = proxyModel()->index(row, nameColumn, rootIndex);
    while (childIndex.isValid())
    {
        if (lineEditAttrName->text().toUpper() == childIndex.data().toString().toUpper()
                && childIndex.sibling(row,0)!=srcIndex) {
            QMessageBox::warning(this,tr("Предуреждение"),
                                 tr("Атрибут с таким имененм уже существует"));
            return;
        }
        childIndex = proxyModel()->index(++row, nameColumn, rootIndex);
    }

    AbstractItemWidget::submit();

    // Занулить если выбраны неверные данные в comboBoxUnitAttr
    QModelIndex unitIndex = srcIndex.sibling(
                srcIndex.row(),
                model()->columnDisplayedAttr(DBATTRXML::ATTR, DBATTRXML::REFUNIT));
    if (unitIndex.data().isNull()) {
        comboBoxUnitAttr->setCurrentIndex(-1);
    }

}

bool AttrWidget::isEdit()
{
    return groupBoxPropAttr->isEnabled();
}

void AttrWidget::edit(bool flag)
{
    if (isEdit()==flag && tableViewAttr->currentIndex().isValid())
        return;

    if (lineEditAttrName->text().isEmpty() && flag==false)
        proxyModel()->removeRow(dataMapper()->currentIndex(),
                               dataMapper()->rootIndex());

    if (!tableViewAttr->currentIndex().isValid()){
        flag = false;
        toolButtonDeleteAttr->setEnabled(false);
        toolButtonEditAttr->setEnabled(false);
        toolButtonAddAttr->setEnabled(true);
    } else {
        toolButtonEditAttr->setDisabled(flag);
        toolButtonDeleteAttr->setDisabled(flag);
        toolButtonAddAttr->setDisabled(flag);
    }

    groupBoxPropAttr->setEnabled(flag);
    groupBoxPropType->setEnabled(flag);
    pushButtonAttrSave->setEnabled(flag);
    pushButtonAttrCancel->setEnabled(flag);
}

void AttrWidget::showParentAttr(bool flag)
{
    AbstractItemWidget::showParent(flag, DBCLASSXML::CLASS, DBCLASSXML::ID,
                                   DBATTRXML::ATTR, DBATTRXML::PARENT);
}

void AttrWidget::changeType(const QString &typeName)
{
    if (DBTYPEXML::STRING==typeName){
        // String
        spinBoxStringLen->setEnabled(true);
        spinBoxAccuracy->setEnabled(false);
        comboBoxLinkAttr->setEnabled(false);
        comboBoxLinkAttr->setDisplayText("");
        comboBoxLinkAttr->setCurrentIndex(-1);
        comboBoxLov->setEnabled(true);
        lineEditDefaultValue->setEnabled(true);
        lineEditLowerBound->setEnabled(true);
        lineEditUpperBound->setEnabled(true);
        comboBoxUnitAttr->setEnabled(false);
        comboBoxUnitAttr->setDisplayText("");
        comboBoxUnitAttr->setCurrentIndex(-1);
        toolButtonUnitAttrClean->setEnabled(false);
        comboBoxNumerator->setEnabled(false);
        toolButtonNumeratorClean->setEnabled(false);
    } else if(DBTYPEXML::REFERENCE==typeName){
        //Reference
        spinBoxStringLen->setEnabled(false);
        spinBoxAccuracy->setEnabled(false);
        spinBoxStringLen->setValue(0);
        comboBoxLinkAttr->setEnabled(true);
        comboBoxLov->setEnabled(false);
        lineEditDefaultValue->setEnabled(false);
        lineEditDefaultValue->setText("");
        lineEditLowerBound->setEnabled(false);
        lineEditLowerBound->setText("");
        lineEditUpperBound->setEnabled(false);
        lineEditUpperBound->setText("");
        comboBoxUnitAttr->setEnabled(false);
        comboBoxUnitAttr->setDisplayText("");
        comboBoxUnitAttr->setCurrentIndex(-1);
        toolButtonUnitAttrClean->setEnabled(false);
        comboBoxNumerator->setEnabled(false);
        toolButtonNumeratorClean->setEnabled(false);
    } else {
        spinBoxStringLen->setEnabled(false);
        spinBoxStringLen->setValue(0);
        spinBoxAccuracy->setEnabled(DBTYPEXML::DECIMAL == typeName);
        comboBoxLinkAttr->setEnabled(false);
        comboBoxLinkAttr->setDisplayText("");
        comboBoxLinkAttr->setCurrentIndex(-1);
        comboBoxLov->setEnabled(true);
        lineEditDefaultValue->setEnabled(true);
        lineEditLowerBound->setEnabled(true);
        lineEditUpperBound->setEnabled(true);

        if( (DBTYPEXML::DECIMAL   == typeName && spinBoxAccuracy->value() == 0)
                || DBTYPEXML::INTEGER   == typeName )
        {
            comboBoxNumerator->setEnabled(true);
            toolButtonNumeratorClean->setEnabled(true);
        } else {
            comboBoxNumerator->setEnabled(false);
            toolButtonNumeratorClean->setEnabled(false);
        }

        // Decimal Double, Integer
        if( DBTYPEXML::DECIMAL   == typeName
         || DBTYPEXML::DOUBLE    == typeName
         || DBTYPEXML::INTEGER   == typeName )
        {
            comboBoxUnitAttr->setEnabled(true);
            toolButtonUnitAttrClean->setEnabled(true);
        } else {
            comboBoxUnitAttr->setEnabled(false);
            comboBoxUnitAttr->setDisplayText("");
            comboBoxUnitAttr->setCurrentIndex(-1);
            toolButtonUnitAttrClean->setEnabled(false);
        }
    }

    TableXMLProxyModel *lovFilterModel = qobject_cast<TableXMLProxyModel *>(comboBoxLov->model());
    lovFilterModel->setFilterRegExp(typeName);
    if (groupBoxPropType->isEnabled())
      comboBoxLov->setCurrentIndex(-1);
}

void AttrWidget::setCurrent(const QModelIndex &index)
{
    AbstractItemWidget::setCurrent(index);
    qint32 indexType = comboBoxTypeAttr->findText(modelData(
                                                   DBATTRXML::ATTR,
                                                   DBATTRXML::TYPE,
                                                   index).toString());
    comboBoxTypeAttr->setCurrentIndex(indexType);
}

}}
