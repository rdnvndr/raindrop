#include "pluginviewdlg.h"
#include "plugincollection.h"
#include <QDebug>

PluginViewDlg::PluginViewDlg(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
}

PluginViewDlg::~PluginViewDlg()
{
    DELETE_OBJECT(m_dependModel);
    DELETE_OBJECT(m_clientModel);
}

void PluginViewDlg::setModel(PluginModel *model)
{
    tvPluginTree->setModel(model);
    connect(tvPluginTree, SIGNAL(clicked(QModelIndex)), model, SLOT(currentItemChanged(QModelIndex)));
    connect(tvPluginTree, SIGNAL(clicked(QModelIndex)), this, SLOT(currentItemChanged(QModelIndex)));

    QHeaderView *header = tvPluginTree->header();
    header->resizeSection(0, 200);
    header->resizeSection(1, 80);
    header->resizeSection(2, 60);
    header->resizeSection(3, 120);
    tvPluginTree->expandAll();

    m_dependModel = new QStringListModel();
    lvDependence->setModel(m_dependModel);
    m_clientModel = new QStringListModel();
    lvClient->setModel(m_clientModel);
}

void PluginViewDlg::cleanUserInterface()
{
    teDescript->setText(QString(""));
    lbStatusText->setText(QString(""));
    m_dependModel->setStringList(QStringList());
    m_clientModel->setStringList(QStringList());
}

void PluginViewDlg::fillUserInterface(IPlugin *plugin)
{
    if(plugin != 0)
    {
        teDescript->setText(plugin->descript());
        if(plugin->state == IPlugin::Init)
            lbStatusText->setText(tr("Исправен"));
        else if(plugin->state == IPlugin::Error)
            lbStatusText->setText(plugin->errorString());

        m_dependModel->setStringList(plugin->dependence());
        m_clientModel->setStringList(plugin->clients());
    }
}

void PluginViewDlg::currentItemChanged(QModelIndex index)
{
    PluginModelItem *item = static_cast<PluginModelItem*>(index.internalPointer());

    if(item->type == PluginModelItem::Group)
    {
        cleanUserInterface();
    }
    else if(item->type == PluginModelItem::Plugin)
    {
        fillUserInterface(item->plugin());
    }
}

