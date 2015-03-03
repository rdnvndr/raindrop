#include "refwidget.h"
#include <QMdiSubWindow>
#include <metadatamodel/dbxmlstruct.h>
#include "xmldelegate.h"

using namespace RTPTechGroup::MetaDataModel;

namespace RTPTechGroup {
namespace ModelerIde {

RefWidget::RefWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    m_mapper = new QDataWidgetMapper();
    m_mapper->setItemDelegate(new XmlDelegate(this));
    m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    m_oldIndex = QModelIndex();
}

RefWidget::~RefWidget()
{
    delete m_mapper;
}

void RefWidget::setModel(TreeXmlHashModel *model)
{
    m_model = model;

    connect(m_model,SIGNAL(rowsRemoved(QModelIndex,int,int)),
            this,SLOT(rowsRemoved(QModelIndex,int,int)));

    m_mapper->setModel(m_model);

    m_mapper->addMapping(lineEditName,
                         model->columnDisplayedAttr(DBREFXML::REF,
                                                    DBREFXML::NAME));
    m_mapper->addMapping(lineEditDesc,
                         model->columnDisplayedAttr(DBREFXML::REF,
                                                    DBREFXML::DESCRIPTION));
}

bool RefWidget::isRemove(const QModelIndex &srcIndex)
{
    const TreeXmlHashModel *model = dynamic_cast<const TreeXmlHashModel *>(srcIndex.model());
    if (!model)
        return false;

    bool success = true;
    QString msg;

    QStringList tags;
    tags << DBLINKTOCLASSXML::LINKTOCLASS << DBLINKTOFILTERXML::LINKTOFILTER;
    if (model->hasChildren(srcIndex,tags)) {
        msg += tr("Необходимо удалить элементы справочника.\n\n");
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

bool RefWidget::isEmpty()
{
    return lineEditName->text().isEmpty();
}

void RefWidget::add()
{
    m_oldIndex = m_model->index(m_mapper->currentIndex(),
                                0,m_mapper->rootIndex());
    QModelIndex srcIndex =  m_oldIndex.parent();

    QModelIndex srcCurrentIndex =
            m_model->insertLastRows(0,1,srcIndex,DBREFXML::REF);
    if (srcCurrentIndex.isValid()) {
        setCurrent(srcCurrentIndex);
        edit(true);
    }
}

void RefWidget::remove()
{
    QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());

    if (!isRemove(srcIndex))
        return;

    m_mapper->revert();
    setCurrent(srcIndex.parent());

    m_model->removeRow(srcIndex.row(),srcIndex.parent());
    emit dataRemoved(srcIndex);
}

bool RefWidget::removeEmpty()
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
        }
        return true;
    }
    return false;
}

void RefWidget::setCurrent(const QModelIndex &index)
{
    m_mapper->setRootIndex(index.parent());
    emit currentIndexChanged(index);
    m_mapper->setCurrentModelIndex(index);

    edit(false);
}

void RefWidget::edit(bool flag)
{
    if (isEmpty()) flag = true;
    groupBox->setEnabled(flag);

    emit edited(flag);
}

void RefWidget::submit()
{
    QModelIndex existIndex = m_model->indexHashAttr(DBREFXML::REF,
                                                    DBREFXML::NAME,
                                                    lineEditName->text());
    QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());

    if (existIndex.isValid()){
        if (existIndex.sibling(existIndex.row(),0)!=srcIndex){
            QMessageBox::warning(this,tr("Предупреждение"),
                                 tr("Справочник с таким именем уже существует"));
            return;
        }
    }

    m_mapper->submit();
    edit(false);
    if (!removeEmpty())
        emit dataChanged(srcIndex);
}

void RefWidget::revert()
{
    m_mapper->revert();
    QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());
    setCurrent(srcIndex);
    edit(false);
    removeEmpty();
}

void RefWidget::rowsRemoved(const QModelIndex &index, int start, int end)
{
    Q_UNUSED(start)
    Q_UNUSED(end)

    if (index == m_mapper->rootIndex() && m_mapper->currentIndex()==-1)
        emit dataRemoved(QModelIndex());
}

QVariant RefWidget::modelData(const QString &tag, const QString &attr, const QModelIndex &index, int role)
{
    return index.sibling(index.row(), m_model->columnDisplayedAttr(
                             tag,attr)).data(role);
}

}}
