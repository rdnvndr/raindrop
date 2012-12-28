#include "classwidget.h"
#include "dbxmlstruct.h"
#include "xmldelegate.h"
#include <QStringListModel>

ClassWidget::ClassWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    m_mapper = new QDataWidgetMapper();
    m_mapper->setItemDelegate(new XmlDelegate(this));
    m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    m_typeClassModel = new QStringListModel();
    m_typeClassModel->setStringList(DBXMLCLASSTYPE);

    lineEditClassParent->setReadOnly(true);

    connect(toolButtonAddClass,SIGNAL(clicked()),this,SLOT(add()));
    connect(toolButtonDelClass,SIGNAL(clicked()),this,SLOT(remove()));
    connect(pushButtonPropSave,SIGNAL(clicked()),this,SLOT(submit()));
    connect(pushButtonPropCancel,SIGNAL(clicked()),this,SLOT(revert()));
    connect(toolButtonEditClass,SIGNAL(clicked()),this,SLOT(edit()));
    m_oldIndex = -1;
}

ClassWidget::~ClassWidget()
{
    delete m_typeClassModel;
    delete m_mapper;
}

void ClassWidget::setModel(TreeXMLModel *model)
{
    m_model = model;
    connect(m_model,SIGNAL(rowsRemoved(QModelIndex,int,int)),
            this,SLOT(rowsRemoved(QModelIndex,int,int)));
    m_mapper->setModel(m_model);

    comboBoxClassType->setModel(m_typeClassModel);

    m_mapper->addMapping(lineEditClassName,
                         model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                   DBCLASSXML::NAME));
    m_mapper->addMapping(lineEditClassDesc,
                         model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                   DBCLASSXML::DESCRIPTION));

    m_mapper->addMapping(comboBoxClassType,
                         model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                   DBCLASSXML::TYPE));

    m_mapper->addMapping(lineEditClassParent,
                         model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                   DBCLASSXML::PARENT));

    m_mapper->addMapping(checkBoxAbsClass,
                         model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                   DBCLASSXML::ISABSTARCT));
    m_mapper->addMapping(checkBoxActiveClass,
                         model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                   DBCLASSXML::ISACTIVE));
    m_mapper->addMapping(plainTextEditShowAttr,
                         model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                   DBCLASSXML::TEMPLATE));
    m_mapper->addMapping(pushButtonIcon,
                         model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                    DBCLASSXML::ICON));
}

void ClassWidget::add()
{
    m_oldIndex = m_mapper->currentIndex();
    QModelIndex srcIndex =  m_model->index(m_mapper->currentIndex(),
                                           0,m_mapper->rootIndex()).parent();
    m_model->setInsTagName(DBCLASSXML::CLASS);
    if (m_model->insertRow(0,srcIndex)) {
        QModelIndex srcCurrentIndex = m_model->lastInsertRow();
        setCurrent(srcCurrentIndex);
        edit(true);
    }
}

bool ClassWidget::isRemove(const QModelIndex &srcIndex)
{
    QString tag = srcIndex.data(Qt::UserRole).toString();
    QStringList tags;
    tags << tag;
    if (m_model->rowCount(srcIndex,tags)) {
        QMessageBox::warning(NULL,tr("Предупреждение"),
                             tr("Удаление не возможно.\nСначало необходимо удалить классы-потомки."));
        return false;
    }

    QString fieldId = m_model->uuidAttr(tag);
    if (fieldId.isEmpty())
        return true;

    QString guid =  srcIndex.sibling(srcIndex.row(),
                                     m_model->columnDisplayedAttr(
                                         tag,fieldId))
            .data().toString();

    foreach (TreeXMLModel::TagWithAttr tagWithAttr,
             m_model->fromRelation(tag))
    {
        int number = 0;

        QModelIndex linkIndex = m_model->indexHashAttr(
                    tagWithAttr.tag,
                    tagWithAttr.attr,
                    guid,
                    number
                    );

        while (linkIndex.isValid()) {
            QModelIndex linkParent = linkIndex.parent();
            if (linkParent.sibling(linkIndex.parent().row(),0)!= srcIndex){
                QMessageBox::warning(NULL,tr("Предупреждение"),
                                     tr("Удаление не возможно.\nСначало необходимо удалить зависимые объекты"));
                return false;
            }
            number++;
            linkIndex = m_model->indexHashAttr(
                        tagWithAttr.tag,
                        tagWithAttr.attr,
                        guid,
                        number
                        );
        }
    }
    return true;
}

void ClassWidget::remove()
{
    QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());

    if (!isRemove(srcIndex))
        return;

    m_mapper->revert();
    setCurrent(srcIndex.parent());

    m_model->removeRow(srcIndex.row(),srcIndex.parent());
    emit dataRemoved(srcIndex);
}


void ClassWidget::setCurrent(const QModelIndex &index)
{
    if (m_mapper->rootIndex() == index.parent() &&
            index.row() == m_mapper->currentIndex())
        return;

    m_mapper->setRootIndex(index.parent());
    m_mapper->setCurrentModelIndex(index);
    edit(false);
    int indexType = comboBoxClassType->findText(modelData(DBCLASSXML::CLASS,
                                                          DBCLASSXML::TYPE,
                                                          index).toString());
    comboBoxClassType->setCurrentIndex(indexType);
    emit currentIndexChanged(index);
}

void ClassWidget::edit(bool flag)
{
    if (groupBoxClass->isEnabled()==flag)
        return;

    if (lineEditClassName->text().isEmpty()){
        toolButtonAddClass->setDisabled(true);
        flag = true;
    }else
        toolButtonAddClass->setEnabled(true);

    groupBoxClass->setEnabled(flag);
    pushButtonPropSave->setEnabled(flag);
    pushButtonPropCancel->setEnabled(flag);
    toolButtonEditClass->setDisabled(flag);
}

void ClassWidget::submit()
{
    QModelIndex existIndex = m_model->indexHashAttr(DBCLASSXML::CLASS,
                                                     DBCLASSXML::NAME,
                                                     lineEditClassName->text());
    QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());

    if (existIndex.isValid()){
        if (existIndex.sibling(existIndex.row(),0)!=srcIndex){
            QMessageBox::warning(this,tr("Предупреждение"),
                                 tr("Класс с таким именем уже существует"));
            return;
        }
    }

    m_mapper->submit();
    removeEmpty();
    edit(false);
    emit dataChanged(srcIndex);
}

void ClassWidget::revert()
{
    m_mapper->revert();
    removeEmpty();
    edit(false);
}

void ClassWidget::rowsRemoved(const QModelIndex &index, int start, int end)
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

QVariant ClassWidget::modelData(const QString &tag, const QString &attr, const QModelIndex &index)
{
    return index.sibling(index.row(), m_model->columnDisplayedAttr(
                             tag,attr)).data();
}

void ClassWidget::removeEmpty()
{
    if (lineEditClassName->text().isEmpty()){
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
