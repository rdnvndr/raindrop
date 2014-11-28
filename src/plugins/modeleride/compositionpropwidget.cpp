#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QToolTip>
#include "compositionpropwidget.h"
#include "xmldelegate.h"
#include "dbxmlstruct.h"
#include "regexpvalidator.h"

CompositionPropWidget::CompositionPropWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    RegExpValidator *validator =
            new RegExpValidator(QRegExp("^[A-Za-z]{1}[A-Za-z0-9_]{0,26}|^[A-Za-z]{0}"));
    lineEditName->setValidator(validator);
    connect(validator,SIGNAL(stateChanged(QValidator::State)),
            this,SLOT(validateCompositionName(QValidator::State)));

    m_mapper = new QDataWidgetMapper();
    m_mapper->setItemDelegate(new XmlDelegate(this));
    m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    lineEditSrcClass->setReadOnly(true);

    connect(toolButtonAdd,SIGNAL(clicked()),this,SLOT(add()));
    connect(toolButtonDel,SIGNAL(clicked()),this,SLOT(remove()));
    connect(pushButtonPropSave,SIGNAL(clicked()),this,SLOT(submit()));
    connect(pushButtonPropCancel,SIGNAL(clicked()),this,SLOT(revert()));
    connect(toolButtonEdit,SIGNAL(clicked()),this,SLOT(edit()));
    m_oldIndex = QModelIndex();
}

CompositionPropWidget::~CompositionPropWidget()
{
    delete lineEditName->validator();
    delete m_mapper;
}

void CompositionPropWidget::setModel(TreeXmlHashModel *model)
{
    m_model = model;
    connect(m_model,SIGNAL(rowsRemoved(QModelIndex,int,int)),
            this,SLOT(rowsRemoved(QModelIndex,int,int)));

    m_mapper->setModel(m_model);

    QSortFilterProxyModel* classFilterModel = new QSortFilterProxyModel(this);
    classFilterModel->setFilterKeyColumn(0);
    classFilterModel->setFilterRole(TreeXmlModel::TagRole);
    classFilterModel->setFilterRegExp(DBCLASSXML::CLASS + "|" +
                                      DBMODELXML::MODEL + "|" +
                                      DBCLASSLISTXML::CLASSLIST);
    classFilterModel->setSourceModel(m_model);
    classFilterModel->setDynamicSortFilter(true);
    classFilterModel->sort(0);
    comboBoxDestClass->setModel(classFilterModel);
    comboBoxDestClass->setRootModelIndex(classFilterModel->index(0,0).child(0,0));
    comboBoxDestClass->setIndexColumn(m_model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                                   DBCLASSXML::ID));
    comboBoxDestClass->setCurrentModelIndex(
                classFilterModel->index(0,0).child(0,0).child(0,0));

    m_mapper->addMapping(lineEditName,
                         model->columnDisplayedAttr(DBCOMPXML::COMP,
                                                   DBCOMPXML::NAME));
    m_mapper->addMapping(lineEditAlias,
                         model->columnDisplayedAttr(DBCOMPXML::COMP,
                                                   DBCOMPXML::ALIAS));

    m_mapper->addMapping(lineEditDirectDesc,
                         model->columnDisplayedAttr(DBCOMPXML::COMP,
                                                   DBCOMPXML::DIRECTDESCRIPTION));

    m_mapper->addMapping(lineEditInverseDesc,
                         model->columnDisplayedAttr(DBCOMPXML::COMP,
                                                   DBCOMPXML::INVERSEDESCRIPTION));
    m_mapper->addMapping(lineEditSrcClass,
                         model->columnDisplayedAttr(DBCOMPXML::COMP,
                                                   DBCOMPXML::PARENT));

    m_mapper->addMapping(comboBoxDestClass,
                         model->columnDisplayedAttr(DBCOMPXML::COMP,
                                                   DBCOMPXML::CLASS));

    m_mapper->addMapping(checkBoxDestIsView,
                         model->columnDisplayedAttr(DBCOMPXML::COMP,
                                                   DBCOMPXML::ISVIEW));

    m_mapper->addMapping(checkBoxIsComp,
                         model->columnDisplayedAttr(DBCOMPXML::COMP,
                                                   DBCOMPXML::ISCOMP));
}

void CompositionPropWidget::add()
{
    m_oldIndex = m_model->index(m_mapper->currentIndex(),
                                0,m_mapper->rootIndex());
    QModelIndex srcIndex = m_oldIndex.parent();

    QModelIndex srcCurrentIndex =
            m_model->insertLastRows(0,1,srcIndex,DBCOMPXML::COMP);
    if (srcCurrentIndex.isValid()){
        setCurrent(srcCurrentIndex);
        edit(true);
    }
}

void CompositionPropWidget::remove()
{
    QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());
    setCurrent(QModelIndex());
    m_mapper->revert();


    emit dataRemoved(srcIndex);
    m_model->removeRow(srcIndex.row(),srcIndex.parent());
}


void CompositionPropWidget::setCurrent(const QModelIndex &index)
{
    if (m_mapper->rootIndex() == index.parent() &&
            index.row() == m_mapper->currentIndex())
        return;

    m_mapper->setRootIndex(index.parent());
    m_mapper->setCurrentModelIndex(index);
    edit(false);

    emit currentIndexChanged(index);
}

bool CompositionPropWidget::isEdit()
{
    return groupBoxProp->isEnabled();
}

void CompositionPropWidget::edit(bool flag)
{
    if (isEdit()==flag)
        return;

    if (lineEditName->text().isEmpty()){
        toolButtonAdd->setDisabled(true);
        flag = true;
    }else
        toolButtonAdd->setEnabled(true);

    groupBoxProp->setEnabled(flag);
    pushButtonPropSave->setEnabled(flag);
    pushButtonPropCancel->setEnabled(flag);
    toolButtonEdit->setDisabled(flag);
}

void CompositionPropWidget::submit()
{
    QModelIndex rootIndex = m_mapper->rootIndex();
    QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());
    int compNameColumn = m_model->columnDisplayedAttr(DBCOMPXML::COMP, DBCOMPXML::NAME);

    int row=0;
    QModelIndex childIndex = m_model->index(row, compNameColumn, rootIndex);
    while (childIndex.isValid())
    {
        if (childIndex.data(TreeXmlModel::TagRole) == DBCOMPXML::COMP)
            if (lineEditName->text() == childIndex.data() &&
                    srcIndex != childIndex.sibling(row,0)) {
                QMessageBox::warning(this,tr("Предуреждение"),
                                     tr("Состав с таким имененм уже существует"));
                return;
            }
        childIndex = m_model->index(++row, compNameColumn, rootIndex);
    }

    m_mapper->submit();
    removeEmpty();
    edit(false);

    emit dataChanged(srcIndex);
}

void CompositionPropWidget::revert()
{
    m_mapper->revert();
    removeEmpty();
    edit(false);
}

void CompositionPropWidget::rowsRemoved(const QModelIndex &index, int start, int end)
{   
    Q_UNUSED (start)
    Q_UNUSED (end)

    if (index == m_mapper->rootIndex() && m_mapper->currentIndex()==-1)
        emit dataRemoved(QModelIndex());
}

void CompositionPropWidget::validateCompositionName(QValidator::State state) const
{
    if(state != QValidator::Acceptable)
        QToolTip::showText(lineEditName->mapToGlobal(QPoint(0,5)),
                           tr("Имя состава должно содержать только латинские\n"
                              "символы и цифры длиной не более 27 символов"));
    else
        QToolTip::hideText();
}


QVariant CompositionPropWidget::modelData(const QString &tag, const QString &attr, const QModelIndex &index)
{
    return index.sibling(index.row(), m_model->columnDisplayedAttr(
                             tag,attr)).data();
}

void CompositionPropWidget::removeEmpty()
{
    if (lineEditName->text().isEmpty()){
        if (m_oldIndex.isValid()){
            QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());
            m_mapper->revert();
            setCurrent(m_oldIndex);
            m_model->removeRow(srcIndex.row(),srcIndex.parent());
            m_oldIndex = QModelIndex();
        }else {
            remove();
            return;
        }
    }
}
