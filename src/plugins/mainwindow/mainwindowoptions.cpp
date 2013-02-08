#include "mainwindowoptions.h"
#include <QDebug>

MainWindowOptions::MainWindowOptions(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    connect(listCategory,SIGNAL(activated(QModelIndex)),
            listCommands,SLOT(setRootIndex(QModelIndex)));
    connect(listCategory,SIGNAL(clicked(QModelIndex)),
            listCommands,SLOT(setRootIndex(QModelIndex)));
}

MainWindowOptions::~MainWindowOptions()
{
    delete m_model;
}

void MainWindowOptions::createActionsModel(QMultiHash <QString, QAction *> *actions)
{
    m_model = new ActionGroupModel(actions);
    listCategory->setModel(m_model);
    listCommands->setModel(m_model);

    listCategory->setCurrentIndex(m_model->index(0,0));
    listCommands->setRootIndex(m_model->index(0,0));
}
