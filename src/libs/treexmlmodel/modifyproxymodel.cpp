#include "modifyproxymodel.h"
#include "treexmlmodel.h"
#include <QFont>

struct PrivateModelIndex
{
    int r, c;
    void *p;
    const QAbstractItemModel *m;
};

ModifyProxyModel::ModifyProxyModel(QObject* parent) :
    QAbstractItemModel (parent)
{
    m_hiddenRow = true;
    m_editable  = false;
}

bool ModifyProxyModel::submitAll()
{
    // Вставка строк
    QHashIterator<QPersistentModelIndex,
            QList<QPersistentModelIndex *> > iterParentList(m_insertedRow);
    while (iterParentList.hasNext())
    {
        iterParentList.next();
        QPersistentModelIndex parent = iterParentList.key();
        if (isInsertRow(parent))
            continue;

        insertSourceRows(parent);
    }

    // Модификация строк
    QHashIterator<QPersistentModelIndex,
            QHash<int, QVariant> > iterIndex(m_updatedRow);
    while (iterIndex.hasNext())
    {
        iterIndex.next();
        QModelIndex index = iterIndex.key();
        QHashIterator<int, QVariant> iterRole(iterIndex.value());
        while (iterRole.hasNext()) {
            iterRole.next();
            QVariant value = iterRole.value();
            int role = iterRole.key();
            sourceModel()->setData(mapToSource(index),value,role);
        }
        m_updatedRow[index].clear();
    }
    m_updatedRow.clear();

    // Удаление строк
    QSetIterator<QPersistentModelIndex> iterRemoveIndex(m_removedRow);
    while (iterRemoveIndex.hasNext())
    {
        QModelIndex index = iterRemoveIndex.next();
        beginRemoveRows(mapFromSource(index.parent()),index.row(),index.row());
        sourceModel()->removeRow(index.row(),index.parent());
        endRemoveRows();
    }
    m_removedRow.clear();
    return true;
}

void ModifyProxyModel::revertAll()
{
    // Отмена вставки строк
    QHashIterator<QPersistentModelIndex,
            QList<QPersistentModelIndex *> > iterParentList(m_insertedRow);
    while (iterParentList.hasNext())
    {
        iterParentList.next();
        QPersistentModelIndex parent = iterParentList.key();
        if (parent.isValid()) {
            int count = m_insertedRow[parent].count();
            removeRows(rowCount(parent)-count,count,parent);
        }
        m_insertedRow[parent].clear();
        m_insertedRow.remove(parent);
    }

    // Отмена модификации строк
    QHashIterator<QPersistentModelIndex,
            QHash<int, QVariant> > iterIndex(m_updatedRow);
    while (iterIndex.hasNext())
    {
        iterIndex.next();
        QModelIndex index = iterIndex.key();
        m_updatedRow[index].clear();
        emit dataChanged(index, index);
    }
    m_updatedRow.clear();

    // Отмена удаления строк
    if (!m_removedRow.isEmpty()) {
        emit layoutChanged();
        m_removedRow.clear();
    }
}

void ModifyProxyModel::sourceDataChanged(const QModelIndex &left,
                                         const QModelIndex &right)
{
    for(int column = left.column(); column <= right.column(); column++)
        for(int row = left.row(); row <= right.row(); row++) {
            void *p = left.sibling(row, column).internalPointer();
            QPersistentModelIndex removeIndex(createIndex(row,column,p));
            if (m_updatedRow.contains(removeIndex)) {
                m_updatedRow[removeIndex].clear();
                m_updatedRow.remove(removeIndex);
            }
        }

    emit dataChanged(mapFromSource(left), mapFromSource(right));
}

void ModifyProxyModel::sourceRowsRemoved(const QModelIndex &parent,
                                         int start, int end)
{
    Q_UNUSED(parent)
    Q_UNUSED(start)
    Q_UNUSED(end)
}

void ModifyProxyModel::sourceRowsAboutToBeRemoved(const QModelIndex &parent,
                                                  int start, int end)
{
    for (int i = start; i < end+1; i++) {
        QModelIndex index = sourceModel()->index(i,0,parent);
        for (int j = 0; j < sourceModel()->columnCount(parent);j++) {
            QPersistentModelIndex removeIndex(mapFromSource(index.sibling(i,j)));
            if (m_updatedRow.contains(removeIndex)) {
                m_updatedRow[removeIndex].clear();
                m_updatedRow.remove(removeIndex);
            }
        }
        if (m_removedRow.contains(QPersistentModelIndex(index)))
            m_removedRow.remove(QPersistentModelIndex(index));
    }
}

void ModifyProxyModel::sourceRowsInserted(const QModelIndex &parent, int start, int end)
{
    beginInsertRows(mapFromSource(parent), start, end);
    endInsertRows();
}

bool ModifyProxyModel::insertSourceRows(const QPersistentModelIndex &parent,
                                        const QPersistentModelIndex &sourceParent)
{
    int count = m_insertedRow[parent].count();
    QPersistentModelIndex srcParent = (sourceParent.isValid())?
                sourceParent : QPersistentModelIndex(mapToSource(parent));

    TreeXMLModel *xmlModel = qobject_cast<TreeXMLModel *>(sourceModel());
    for (int i = 0; i < count; i++) {
        bool isInserted = false;
        int row = sourceModel()->rowCount(srcParent);

        QPersistentModelIndex indexProxy(
                    index((srcParent == mapToSource(parent))? row + i: row,
                          0,
                          parent));

        int lastRow = row;
        // Обработка TreeXMLModel
        if (xmlModel) {
            if (m_updatedRow.contains(indexProxy))
                if (m_updatedRow[indexProxy].contains(Qt::UserRole)) {
                    xmlModel->setInsTagName(m_updatedRow[indexProxy][Qt::UserRole].toString());
                    isInserted = xmlModel->insertRow(lastRow,srcParent);
                    lastRow = xmlModel->lastInsertRow().row();
                }
        } else
            isInserted = sourceModel()->insertRow(lastRow, srcParent);

        QPersistentModelIndex indexSource;
        if (isInserted) {
            indexSource = QPersistentModelIndex(sourceModel()->index(
                                                    lastRow, 0, srcParent));
            // Модификация потомков
            if (m_insertedRow.contains(indexProxy))
                insertSourceRows(indexProxy,indexSource);
        }

        // Запись значений
        for (int column = 0; column < columnCount(parent); column++) {
            QModelIndex index = indexProxy.sibling(indexProxy.row(),column);
            if (m_updatedRow.contains(index)) {
                if (isInserted) {
                    QHashIterator<int, QVariant> iterRole(m_updatedRow[index]);
                    while (iterRole.hasNext()) {
                        iterRole.next();
                        QVariant value = iterRole.value();
                        int role = iterRole.key();
                        sourceModel()->setData(
                                    indexSource.sibling(indexSource.row(),column),
                                    value,role);
                    }
                }
                m_updatedRow[index].clear();
                m_updatedRow.remove(QPersistentModelIndex(index));
            }
        }

    }
    removeRows(rowCount(parent)-count,count,parent);

    return true;
}

int ModifyProxyModel::columnCount(const QModelIndex &parent) const
{
    if(!sourceModel())
        return 0;

    // Количество столбцов для вставленных строк
    if (isInsertRow(parent))
        return columnCount();

    return sourceModel()->columnCount(mapToSource(parent));
}

int ModifyProxyModel::rowCount(const QModelIndex &parent) const
{
    if(!sourceModel())
        return 0;

    // Количество вставленных строк
    int insertRowCount = 0;
    QPersistentModelIndex rowIndex(parent.sibling(parent.row(),0));
    if (m_insertedRow.contains(rowIndex))
        insertRowCount = m_insertedRow[rowIndex].count();
    if (isInsertRow(parent))
        return  insertRowCount;

    // Поиск удаленных строк в конце
    int parentRowCount = sourceModel()->rowCount(mapToSource(parent));
    int removeRowCount = 0;
    for (int row = parentRowCount - 1; row >=0; row--) {
        QModelIndex index =  sourceModel()->index(row,0,mapToSource(parent));
        if (m_removedRow.contains(QPersistentModelIndex(index))) {
            removeRowCount++;
        } else
            break;
    }
    return parentRowCount + insertRowCount - removeRowCount;
}

QModelIndex ModifyProxyModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    // Получение вставленной строки
    QPersistentModelIndex rowIndex(parent.sibling(parent.row(),0));
    if (m_insertedRow.contains(rowIndex)) {
        int count = m_insertedRow[rowIndex].count();
        if (row >= rowCount(parent)-count) {
            return createIndex(row,column,
                               m_insertedRow[rowIndex].at(
                                   row - rowCount(parent) + count)
                               );
        }
    }

    // Скрытие удаленных строк
    QPersistentModelIndex removeIndex(m_sourceModel->index(
                                          row, column, mapToSource(parent)));
    if (m_hiddenRow && m_removedRow.contains(removeIndex))
        return QModelIndex();

    return mapFromSource(removeIndex);
}

QModelIndex ModifyProxyModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    // Получение вставленной строки
    QPersistentModelIndex *rowIndex =
            static_cast<QPersistentModelIndex *>(index.internalPointer());
    if (rowIndex)
        if (m_insertedRow.contains(*rowIndex))
            return *rowIndex;

    if (!mapToSource(index).isValid())
        return QModelIndex();
    return mapFromSource(mapToSource(index).parent());
}

QModelIndex ModifyProxyModel::mapFromSource(const QModelIndex &index) const
{
    if(index.isValid()) {
        QModelIndex sourceIndex = createIndex(index.row(), index.column(),
                                              index.internalPointer());
        if (sourceIndex.isValid())
            return sourceIndex;
    }

    return QModelIndex();
}

QModelIndex ModifyProxyModel::mapToSource(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    QModelIndex sourceIndex;
    PrivateModelIndex* hack = reinterpret_cast<PrivateModelIndex*>(&sourceIndex);

    hack->r = index.row();
    hack->c = index.column();
    hack->p = index.internalPointer();
    hack->m = sourceModel();

    if (!sourceIndex.isValid())
        return QModelIndex();

    return sourceIndex;
}

QVariant ModifyProxyModel::data(const QModelIndex &proxyIndex, int role) const
{
    QPersistentModelIndex dataIndex(proxyIndex);

    // Устанавливает зачеркивание удаленной строки
    if (role == Qt::FontRole) {
        QFont font = sourceModel()->data(
                    mapToSource(proxyIndex),role).value<QFont>();
        QPersistentModelIndex removeIndex(
                    mapToSource(proxyIndex.sibling(proxyIndex.row(),0)));
        if (m_removedRow.contains(removeIndex))
            font.setStrikeOut(true);
        return font;
    }

    // Получение измененных данных
    if (m_updatedRow.contains(dataIndex))  {
        int updateRole = (role==Qt::DisplayRole) ? Qt::EditRole : role;
        if (m_updatedRow[dataIndex].contains(updateRole))
            return m_updatedRow[dataIndex][updateRole];
    }

    // Пустое значение для вставленных строк
    if (isInsertRow(proxyIndex))
        return QVariant();

    return sourceModel()->data(mapToSource(proxyIndex),role);
}

bool ModifyProxyModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    QPersistentModelIndex dataIndex(index);
    int updateRole = (role==Qt::DisplayRole) ? Qt::EditRole : role;
    m_updatedRow[dataIndex][updateRole] = value;
    emit dataChanged(index,index);

    return true;
}

bool ModifyProxyModel::hasChildren(const QModelIndex &parent) const
{
    return rowCount(parent);
}

void ModifyProxyModel::setSourceModel(QAbstractItemModel *srcModel)
{
    m_sourceModel = srcModel;

    connect(srcModel, SIGNAL(rowsRemoved(const QModelIndex &, int, int)),
            this, SLOT(sourceRowsRemoved(const QModelIndex &, int, int)));
    connect(srcModel, SIGNAL(rowsAboutToBeRemoved(const QModelIndex &, int, int)),
            this, SLOT(sourceRowsAboutToBeRemoved(const QModelIndex &, int, int)));
    connect(srcModel, SIGNAL(rowsInserted(const QModelIndex &, int, int)),
            this, SLOT(sourceRowsInserted(const QModelIndex &, int, int)));
    connect(srcModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            this, SLOT(sourceDataChanged(QModelIndex,QModelIndex)));
}

QAbstractItemModel *ModifyProxyModel::sourceModel() const
{
    return m_sourceModel;
}

Qt::ItemFlags ModifyProxyModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index)

    if (m_editable)
        return  Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void ModifyProxyModel::setEditable(bool flag)
{
    m_editable = flag;
}

bool ModifyProxyModel::isEditable()
{
    return m_editable;
}

QVariant ModifyProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{    
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole){
        if (m_header[section].isNull())
            return sourceModel()->headerData(section,orientation,role);
        else
            return  m_header[section];
    }
    return  QVariant();

}

bool ModifyProxyModel::setHeaderData(int section, Qt::Orientation orientation,
                                     const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    m_header[section] = value.toString();
    emit headerDataChanged(orientation, section, section);

    return true;
}

bool ModifyProxyModel::removeRows(int row, int count, const QModelIndex &parent)
{

    int beginRowInsert = row + count;

    // Удаление вставленных строк
    QPersistentModelIndex rowIndex(parent.sibling(parent.row(),0));
    if (m_insertedRow.contains(rowIndex)) {

        int countRowInCache =  m_insertedRow[rowIndex].count();
        int beginRowInCache =  row - rowCount(parent) + countRowInCache;

        int removeRowCountInCache =
                (beginRowInCache >= 0) ? count : count + beginRowInCache;

        if (beginRowInCache < 0)
            beginRowInCache = 0;

        if (removeRowCountInCache > 0) {

            beginRowInsert = beginRowInCache + rowCount(parent)
                    - countRowInCache;
            beginRemoveRows(parent, beginRowInsert,
                            beginRowInsert+removeRowCountInCache-1);

            for (int i = beginRowInsert; i < beginRowInsert + removeRowCountInCache; i++)
                for (int j = 0; j < sourceModel()->columnCount(parent);j++) {
                    QPersistentModelIndex removeIndex(parent.child(i,j));
                    if (m_updatedRow.contains(removeIndex)) {
                        m_updatedRow[removeIndex].clear();
                        m_updatedRow.remove(removeIndex);
                    }
                }

            for (int i = beginRowInCache + removeRowCountInCache-1;
                 i >= beginRowInCache; i--)
            {
                delete m_insertedRow[rowIndex].at(i);
                m_insertedRow[rowIndex].removeAt(i);

            }
            if (m_insertedRow[rowIndex].isEmpty())
                m_insertedRow.remove(rowIndex);


            endRemoveRows();
        }
    }

    // Удаление строк в исходной модели
    for (int i = row; i< beginRowInsert; i++) {
        QModelIndex insertIndex = index(i,0, parent);
        m_removedRow.insert(QPersistentModelIndex(mapToSource(insertIndex)));
    }
    emit layoutChanged();

    return true;
}

bool ModifyProxyModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(row)
    QPersistentModelIndex rowIndex(parent.sibling(parent.row(),0));

    int position = rowCount(rowIndex);

    beginInsertRows(parent,position,position+count-1);
    for (int i = position; i < position+count; i++) {
        QPersistentModelIndex *index = new QPersistentModelIndex(rowIndex);
        m_insertedRow[rowIndex].append(index);
    }
    endInsertRows();

    if (parent.isValid())
        m_lastInsRow = parent.child(position+count-1,0);
    else
        m_lastInsRow = index(position+count-1,0,parent);
    return true;
}

QModelIndex ModifyProxyModel::lastInsertRow()
{
    return m_lastInsRow;
}

bool ModifyProxyModel::isInsertRow(const QModelIndex &index) const
{
    if (!index.isValid())
        return false;

    QPersistentModelIndex *rowIndex =
            static_cast<QPersistentModelIndex *>(
                index.sibling(index.row(),0).internalPointer());
    if (rowIndex) {
        if (m_insertedRow.contains(*rowIndex))
            return  true;
    }
    return false;
}

void ModifyProxyModel::setHiddenRow(bool enabled)
{
    m_hiddenRow = enabled;
}


bool ModifyProxyModel::isHiddenRow()
{
    return m_hiddenRow;
}
