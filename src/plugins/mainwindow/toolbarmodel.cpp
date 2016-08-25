#include "toolbarmodel.h"

namespace RTPTechGroup {
namespace MainWindow {

ToolBarModel::ToolBarModel(QMainWindow *mainWindow, QObject *parent) :
    QAbstractItemModel(parent)
{
    m_mainWindow = mainWindow;

    m_mapper = new QSignalMapper(this);
    m_toolBars = mainWindow->findChildren<ToolBar *> ();
    foreach (ToolBar *toolBar, m_toolBars) {
        connect(toolBar, SIGNAL(visibilityChanged(bool)),
                m_mapper,SLOT(map()));
        m_mapper->setMapping(toolBar,toolBar);
    }
    connect(m_mapper,SIGNAL(mapped(QWidget*)),
            this,SLOT(setToolBarVisible(QWidget*)));

}

ToolBarModel::~ToolBarModel()
{
    m_toolBars.clear();
    delete m_mapper;
}

bool ToolBarModel::setData(const QModelIndex &index, const QVariant &value,
                           qint32 role)
{
    if (role == Qt::EditRole)
        m_toolBars.at(index.row())->setWindowTitle(value.toString());
    else if (role == Qt::CheckStateRole) {
        if (value == Qt::Checked) {
            if (m_toolBars.at(index.row())->isHidden())
                m_toolBars.at(index.row())->setVisible(true);
        } else {
            if (m_toolBars.at(index.row())->isVisible())
                m_toolBars.at(index.row())->setVisible(false);
        }
    } else
        return false;

    emit dataChanged(index,index);
    return true;
}

QVariant ToolBarModel::data(const QModelIndex &index, qint32 role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole){
        return m_toolBars.at(index.row())->windowTitle();
    } else if (role == Qt::CheckStateRole) {
        if (m_toolBars.at(index.row())->isVisible())
            return Qt::Checked;
        else
            return Qt::Unchecked;
    }

    return QVariant();
}

Qt::ItemFlags ToolBarModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return  Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable
            | Qt::ItemIsUserCheckable;
}

QVariant ToolBarModel::headerData(qint32 section, Qt::Orientation orientation, qint32 role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole){
        if (section == 0)
            return  tr("Видимость");
        else
            return  tr("Наименование");
    }
    return QVariant();
}

QModelIndex ToolBarModel::index(qint32 row, qint32 column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return createIndex(row,column);
}

QModelIndex ToolBarModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)
    return QModelIndex();
}

qint32 ToolBarModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_toolBars.count();
}

qint32 ToolBarModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

bool ToolBarModel::insertRows(qint32 row, qint32 count, const QModelIndex &parent)
{
    beginInsertRows(parent,row,row+count-1);
    ToolBar *toolBar = new ToolBar();
    m_mainWindow->addToolBar(toolBar);
    m_toolBars.insert(row,toolBar);
    connect(toolBar, SIGNAL(visibilityChanged(bool)),
            m_mapper,SLOT(map()));
    m_mapper->setMapping(toolBar,toolBar);
    endInsertRows();
    toolBar->setEdited(true);
    return true;
}

void ToolBarModel::setToolBarVisible(QWidget *widget)
{
    ToolBar *toolBar = qobject_cast<ToolBar *>(widget);
    if (toolBar) {
        qint32 row = m_toolBars.indexOf(toolBar);
        if (toolBar->isVisible())
            setData(index(row,0),Qt::Checked,Qt::CheckStateRole);
        else
            setData(index(row,0),Qt::Unchecked,Qt::CheckStateRole);
    }
}

bool ToolBarModel::removeRows(qint32 row, qint32 count, const QModelIndex &parent)
{

    beginRemoveRows(parent,row,row+count-1);
    for (qint32 i=count-1;i>=0;--i) {
        ToolBar *toolBar = m_toolBars.at(row+i);
        m_mainWindow->removeToolBar(toolBar);
        delete toolBar;
        m_toolBars.removeAt(row+i);
    }
    endRemoveRows();
    return true;
}

}
}
