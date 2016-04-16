#include "conditiondelegate.h"
#include <QComboBox>
#include <QStringList>
#include <treexmlmodel/treexmlhashmodel.h>
#include <QAbstractItemView>
#include <metadatamodel/dbxmlstruct.h>
#include "conditionproxymodel.h"

using namespace RTPTechGroup::MetaDataModel;
using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

ConditionDelegate::ConditionDelegate(QObject *parent) :
    XmlDelegate(parent)
{
    m_attrModel = new TableXMLProxyModel();
    QStringList tags;
    tags << DBATTRXML::ATTR << DBCOMPXML::COMP;
    m_attrModel->setAttributeTags(tags);
    m_attrModel->setDynamicSortFilter(true);
}

ConditionDelegate::~ConditionDelegate()
{
    delete m_attrModel;
}

QWidget *ConditionDelegate::createEditor(QWidget *parent, const
                                         QStyleOptionViewItem &option,
                                         const QModelIndex &index) const
{
    Q_UNUSED(option)

    if (index.data(TreeXmlModel::TagRole) == DBCONDITIONXML::COND) {
        if (index.column() == 0) {
            ConditionProxyModel *proxyModel = qobject_cast<ConditionProxyModel *>(
                        const_cast<QAbstractItemModel *>(index.model()));
            if (proxyModel) {
                TreeXmlHashModel *xmlModel = qobject_cast<TreeXmlHashModel *>(
                            proxyModel->sourceModel());
                if (xmlModel) {

                    int column = xmlModel->columnDisplayedAttr(DBATTRXML::ATTR,
                                                               DBATTRXML::NAME);
                    m_attrModel->setColumnCount(xmlModel->columnCount(firstIndex()));
                    QComboBox *comboBoxAttr = new QComboBox(parent);
                    comboBoxAttr->setModel(m_attrModel);
                    m_attrModel->setSourceModel(xmlModel);
                    m_attrModel->setFilterIndex(m_firstIndex);
                    comboBoxAttr->setRootModelIndex(
                                m_attrModel->mapFromSource(m_firstIndex));
                    comboBoxAttr->setModelColumn(column);
                    return comboBoxAttr;
                }
            }
        }

        if (index.column() == 1) {
            QComboBox *comboBoxAttr = new QComboBox(parent);
            QStringList operatorList;
            operatorList << "=" << "!=" << "<=" << ">=" << "<" << ">" << "like";
            comboBoxAttr->addItems(operatorList);
            return comboBoxAttr;
        }

        if (index.column() == 2) {
            ConditionProxyModel *proxyModel = qobject_cast<ConditionProxyModel *>(
                        const_cast<QAbstractItemModel *>(index.model()));
            if (proxyModel) {
                TreeXmlHashModel *xmlModel = qobject_cast<TreeXmlHashModel *>(
                            proxyModel->sourceModel());
                if (xmlModel) {
                    int column = xmlModel->columnDisplayedAttr(DBATTRXML::ATTR,
                                                               DBATTRXML::NAME);
                    m_attrModel->setColumnCount(xmlModel->columnCount(firstIndex()));
                    QComboBox *comboBoxAttr = new QComboBox(parent);
                    comboBoxAttr->setModel(m_attrModel);
                    m_attrModel->setSourceModel(xmlModel);
                    m_attrModel->setFilterIndex(m_secondIndex);
                    comboBoxAttr->setRootModelIndex(
                                m_attrModel->mapFromSource(m_secondIndex));
                    comboBoxAttr->setModelColumn(column);
                    return comboBoxAttr;
                }
            }
        }
    }
    if (index.row() < index.model()->rowCount(index.parent())-1
            && (index.data(TreeXmlModel::TagRole) == DBCONDITIONXML::COND
            || index.data(TreeXmlModel::TagRole) == DBFILTERBLOCKXML::BLOCK))
    {
        if (index.column() == 3) {
            QComboBox *comboBoxAttr = new QComboBox(parent);
            QStringList operatorList;
            operatorList << "И" << "ИЛИ";
            comboBoxAttr->addItems(operatorList);
            return comboBoxAttr;
        }
    }

    return NULL;
}

void ConditionDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                     const QModelIndex &index) const
{
    QComboBox *comboBox = dynamic_cast<QComboBox*>(editor);
    if (comboBox && (index.column() == 0 || index.column() == 2)) {
        ConditionProxyModel *proxyModel = qobject_cast<ConditionProxyModel *>(
                    const_cast<QAbstractItemModel *>(index.model()));
        if (proxyModel) {
            TreeXmlHashModel *xmlModel = qobject_cast<TreeXmlHashModel *>(
                        proxyModel->sourceModel());
            if (xmlModel) {
                QModelIndex currentIndex = comboBox->view()->currentIndex();
                if (currentIndex.isValid()) {
                    QString tag = currentIndex.data(TreeXmlModel::TagRole).toString();
                    int column = (tag == DBATTRXML::ATTR)
                            ? xmlModel->columnDisplayedAttr(DBATTRXML::ATTR,
                                                            DBATTRXML::ID)
                            : xmlModel->columnDisplayedAttr(DBCOMPXML::COMP,
                                                            DBCOMPXML::ID);

                    model->setData(index,
                                   m_attrModel->data(
                                       currentIndex.sibling(
                                           currentIndex.row(),
                                           column
                                           )),
                                   Qt::EditRole);
                }
                return;
            }
        }
    }
    XmlDelegate::setModelData(editor, model, index);
    return;
}

void ConditionDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{

    QComboBox *comboBox = dynamic_cast<QComboBox*>(editor);
    if (comboBox && (index.column() == 0 || index.column() == 2)) {
        ConditionProxyModel *proxyModel = qobject_cast<ConditionProxyModel *>(
                    const_cast<QAbstractItemModel *>(index.model()));
        if (proxyModel) {
            TreeXmlHashModel *xmlModel = qobject_cast<TreeXmlHashModel *>(
                        proxyModel->sourceModel());
            if (xmlModel) {

                QModelIndex currentIndex = m_attrModel->mapFromSource(
                            xmlModel->indexHashAttr(
                                DBATTRXML::ATTR,
                                DBATTRXML::ID,
                                index.data(Qt::EditRole)));

                comboBox->setCurrentIndex(currentIndex.row());
                return;
            }
        }
    }
    XmlDelegate::setEditorData(editor, index);
    return;
}

void ConditionDelegate::setFirstIndex(const QModelIndex &index)
{
    m_firstIndex = index;
}

QModelIndex ConditionDelegate::firstIndex() const
{
    return m_firstIndex;
}

void ConditionDelegate::setSecondIndex(const QModelIndex &index)
{
    m_secondIndex = index;
}

QModelIndex ConditionDelegate::secondIndex() const
{
    return m_secondIndex;
}

}}
