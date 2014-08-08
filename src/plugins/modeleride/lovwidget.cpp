#include "lovwidget.h"
#include "xmldelegate.h"
#include "dbxmlstruct.h"
#include <QMessageBox>

LovWidget::LovWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    m_mapper = new QDataWidgetMapper();
    m_mapper->setItemDelegate(new XmlDelegate(this));
    m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    m_oldIndex = -1;
}

LovWidget::~LovWidget()
{
    delete m_mapper;
}

bool LovWidget::isRemove(const QModelIndex &srcIndex)
{
    const TreeXmlHashModel *model = dynamic_cast<const TreeXmlHashModel *>(srcIndex.model());
    if (!model)
        return false;

    bool success = true;
    QString msg;

    QStringList tags;
    tags << DBLOVVALUEXML::LOVVALUE;
    if (model->rowCount(srcIndex,tags)) {
        msg += tr("Необходимо удалить значение списка.\n\n");
        success = false;
    }
    if (!success) {
        QMessageBox msgBox;
        msgBox.setText(tr("Удаление данного объекта не воможно."));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setDetailedText(msg);
        msgBox.setWindowTitle(tr("Предупреждение"));
        msgBox.exec();
    }
    return success;
}

bool LovWidget::isEmpty()
{
    return lineEditLovName->text().isEmpty();;
}

void LovWidget::add()
{
    m_oldIndex = m_mapper->currentIndex();
    QModelIndex srcIndex =  m_model->index(m_mapper->currentIndex(),
                                           0,m_mapper->rootIndex()).parent();
    m_model->setInsTagName(DBLOVXML::LOV);
    if (m_model->insertRow(0,srcIndex)) {
        QModelIndex srcCurrentIndex = m_model->lastInsertRow();
        setCurrent(srcCurrentIndex);
        edit(true);
    }
}

void LovWidget::remove()
{
    QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());

    if (!isRemove(srcIndex))
        return;

    m_mapper->revert();
    setCurrent(srcIndex.parent());

    m_model->removeRow(srcIndex.row(),srcIndex.parent());
    emit dataRemoved(srcIndex);
}

void LovWidget::removeEmpty()
{
    if (lineEditLovName->text().isEmpty()){
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

void LovWidget::setModel(TreeXmlHashModel *model)
{
    m_model = model;

    connect(m_model,SIGNAL(rowsRemoved(QModelIndex,int,int)),
            this,SLOT(rowsRemoved(QModelIndex,int,int)));
    m_mapper->setModel(m_model);

    m_mapper->addMapping(lineEditLovName,
                         model->columnDisplayedAttr(DBLOVXML::LOV,
                                                   DBLOVXML::NAME));
    m_mapper->addMapping(lineEditLovDesc,
                         model->columnDisplayedAttr(DBLOVXML::LOV,
                                                   DBLOVXML::DESCRIPTION));
    m_mapper->addMapping(lineEditLovType,
                         model->columnDisplayedAttr(DBLOVXML::LOV,
                                                    DBLOVXML::TYPE));
}

void LovWidget::setCurrent(const QModelIndex &index)
{
    m_mapper->setRootIndex(index.parent());
    emit currentIndexChanged(index);
    m_mapper->setCurrentModelIndex(index);

    edit(false);
}

void LovWidget::edit(bool flag)
{
    if (isEmpty()) flag = true;
    groupBoxLov->setEnabled(flag);

    emit edited(flag);
}

void LovWidget::submit()
{
    QModelIndex existIndex = m_model->indexHashAttr(DBLOVXML::LOV,
                                                    DBLOVXML::NAME,
                                                    lineEditLovName->text());
    QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());

    if (existIndex.isValid()){
        if (existIndex.sibling(existIndex.row(),0)!=srcIndex){
            QMessageBox::warning(this,tr("Предупреждение"),
                                 tr("Список значений с таким именем уже существует"));
            return;
        }
    }

    m_mapper->submit();
    removeEmpty();
    edit(false);
    emit dataChanged(srcIndex);
}

void LovWidget::revert()
{
    m_mapper->revert();
    QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());
    setCurrent(srcIndex);
    removeEmpty();
    edit(false);
}

void LovWidget::rowsRemoved(const QModelIndex &index, int start, int end)
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

QVariant LovWidget::modelData(const QString &tag, const QString &attr, const QModelIndex &index, int role)
{
    return index.sibling(index.row(), m_model->columnDisplayedAttr(
                             tag,attr)).data(role);
}
