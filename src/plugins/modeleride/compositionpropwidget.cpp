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
            new RegExpValidator(QRegExp("^[A-Za-z]{1}[A-Za-z0-9]{0,26}|^[A-Za-z]{0}"));
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
    m_oldIndex = -1;
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
    classFilterModel->setFilterRole(Qt::UserRole);
    classFilterModel->setFilterRegExp(DBCLASSXML::CLASS + "|" +
                                      DBMODELXML::MODEL + "|" +
                                      DBCLASSLISTXML::CLASSLIST);
    classFilterModel->setSourceModel(m_model);
    classFilterModel->setDynamicSortFilter(true);
    classFilterModel->sort(0);
    comboBoxDestClass->setModel(classFilterModel);
    comboBoxDestClass->setRootModelIndex(classFilterModel->index(0,0).child(0,0));
    comboBoxDestClass->setIndexColumn(m_model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                                  DBATTRXML::ID));

    m_mapper->addMapping(lineEditName,
                         model->columnDisplayedAttr(DBCOMPXML::COMP,
                                                   DBCOMPXML::NAME));
    m_mapper->addMapping(lineEditDesc,
                         model->columnDisplayedAttr(DBCOMPXML::COMP,
                                                   DBCOMPXML::DESCRIPTION));

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
}

void CompositionPropWidget::add()
{
    m_oldIndex = m_mapper->currentIndex();
    QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),
                                          0,m_mapper->rootIndex()).parent();

    m_model->setInsTagName(DBCOMPXML::COMP);
    QModelIndex srcCurrentIndex = m_model->insertLastRows(0,1,srcIndex);
    if (srcCurrentIndex.isValid()){
        setCurrent(srcCurrentIndex);
        edit(true);
    }
}

void CompositionPropWidget::remove()
{
    QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());
    m_mapper->revert();
    setCurrent(srcIndex.parent());

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

void CompositionPropWidget::edit(bool flag)
{
    if (groupBoxProp->isEnabled()==flag)
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
    for (int row=0; row < m_model->rowCount(rootIndex); row++){
        QModelIndex childIndex = m_model->index(row, m_model->columnDisplayedAttr(
                                                    DBCOMPXML::COMP,
                                                    DBCOMPXML::NAME),
                                                rootIndex);
        if (childIndex.data(Qt::UserRole) == DBCOMPXML::COMP)
            if (lineEditName->text() == childIndex.data() &&
                    srcIndex != childIndex.sibling(row,0)) {
                QMessageBox::warning(this,tr("Предуреждение"),
                                     tr("Состав с таким имененм уже существует"));
                return;
            }
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
    if (index == m_mapper->rootIndex()){
        if (m_oldIndex > end)
            m_oldIndex = m_oldIndex - end-start-1;
        else if (m_oldIndex >= start && m_oldIndex <= end){
            m_oldIndex = -1;
        }
    }

    if (index == m_mapper->rootIndex() && m_mapper->currentIndex()==-1 && m_oldIndex <0)
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
        if (m_oldIndex>=0){
            m_model->removeRow(m_mapper->currentIndex(),
                               m_mapper->rootIndex());
            setCurrent(m_mapper->rootIndex().child(m_oldIndex,0));
            m_oldIndex = -1;
        }else {
            remove();
            return;
        }
    }
}
