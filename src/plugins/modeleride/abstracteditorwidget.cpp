#include "abstracteditorwidget.h"

#include <metadatamodel/classmodelxml.h>
#include "xmldelegate.h"

using namespace RTPTechGroup::MetaDataModel;
using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

AbstractEditorWidget::AbstractEditorWidget(QWidget *parent)
    :QWidget(parent)
{
    m_mapper = new QDataWidgetMapper();
    m_mapper->setItemDelegate(new XmlDelegate(this));
    m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    m_oldIndex = QModelIndex();
}

AbstractEditorWidget::~AbstractEditorWidget()
{
    delete m_mapper;
}

void AbstractEditorWidget::setModel(TreeXmlHashModel *model)
{
    m_model = model;
    connect(m_model,SIGNAL(rowsRemoved(QModelIndex,int,int)),
            this,SLOT(rowsRemoved(QModelIndex,int,int)));
    m_mapper->setModel(m_model);
}

TreeXmlHashModel *AbstractEditorWidget::model()
{
    return m_model;
}

QDataWidgetMapper *AbstractEditorWidget::dataMapper()
{
    return m_mapper;
}

bool AbstractEditorWidget::isRemove(const QModelIndex &srcIndex)
{
    ClassModelXml *model = const_cast<ClassModelXml *>(
                dynamic_cast<const ClassModelXml *>(srcIndex.model()));

    if (!model)
        return false;

    return model->isRemove(srcIndex);
}

bool AbstractEditorWidget::add(const QString &tag)
{
    m_oldIndex = m_model->index(m_mapper->currentIndex(),
                                0,m_mapper->rootIndex());
    QModelIndex srcIndex =  m_oldIndex.parent();

    QModelIndex srcCurrentIndex
            = m_model->insertLastRows(0, 1, srcIndex, tag);
    if (srcCurrentIndex.isValid()) {
        setCurrent(srcCurrentIndex);
        edit(true);

        return true;
    }

    return false;
}

void AbstractEditorWidget::remove()
{
    int row = m_mapper->currentIndex();
    QModelIndex srcParent = m_mapper->rootIndex();
    QModelIndex srcIndex  = m_model->index(row, 0, srcParent);

    if (!isRemove(srcIndex))
        return;

    m_mapper->revert();
    emit dataRemoved(srcIndex);

    m_model->removeRow(row, srcParent);

}

bool AbstractEditorWidget::removeEmpty()
{
    if (isEmpty()){
        if (m_oldIndex.isValid()){
            QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());
            m_mapper->revert();
            setCurrent(m_oldIndex);
            m_model->removeRow(srcIndex.row(),srcIndex.parent());
            m_oldIndex = QModelIndex();
        }else {
            remove();
        }
        return true;
    }
    return false;
}

void AbstractEditorWidget::setCurrent(const QModelIndex &index)
{
    m_mapper->setRootIndex(index.parent());
    emit currentIndexChanged(index);
    m_mapper->setCurrentModelIndex(index);

    edit(false);
}

void AbstractEditorWidget::submit()
{
    QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),0,
                                          m_mapper->rootIndex());
    m_mapper->submit();
    edit(false);
    if (!removeEmpty())
        emit dataChanged(srcIndex);
}

void AbstractEditorWidget::revert()
{
    m_mapper->revert();
    QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());
    setCurrent(srcIndex);
    edit(false);
    removeEmpty();
}

void AbstractEditorWidget::rowsRemoved(const QModelIndex &index, int start, int end)
{
    Q_UNUSED(start)
    Q_UNUSED(end)

    if (index == m_mapper->rootIndex() && m_mapper->currentIndex()==-1 && !m_oldIndex.isValid())
        emit dataRemoved(QModelIndex());
}

QVariant AbstractEditorWidget::modelData(const QString &tag, const QString &attr,
                                         const QModelIndex &index, int role)
{
    return index.sibling(index.row(), m_model->columnDisplayedAttr(
                             tag,attr)).data(role);
}

}}
