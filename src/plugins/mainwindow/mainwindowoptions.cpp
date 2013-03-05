#include "mainwindowoptions.h"
#include "toolbarprop.h"
#include <QDebug>

MainWindowOptions::MainWindowOptions(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    connect(listCategory,SIGNAL(activated(QModelIndex)),
            listCommands,SLOT(setRootIndex(QModelIndex)));
    connect(listCategory,SIGNAL(clicked(QModelIndex)),
            listCommands,SLOT(setRootIndex(QModelIndex)));

    connect(pushButtonNew,SIGNAL(clicked()),this,SLOT(insertToolBar()));
    connect(pushButtonDelete,SIGNAL(clicked()),this,SLOT(removeToolBar()));
    connect(pushButtonRename,SIGNAL(clicked()),this,SLOT(renameToolBar()));
}

MainWindowOptions::~MainWindowOptions()
{
    delete m_actionGroupModel;
    delete m_toolBarModel;
}

void MainWindowOptions::createActionsModel(QMultiHash <QString, QAction *> *actions)
{
    m_actionGroupModel = new ActionGroupModel(actions);
    listCategory->setModel(m_actionGroupModel);
    listCommands->setModel(m_actionGroupModel);

    listCategory->setCurrentIndex(m_actionGroupModel->index(0,0));
    listCommands->setRootIndex(m_actionGroupModel->index(0,0));
}

void MainWindowOptions::createToolBarModel(QMainWindow *mainWindow)
{
    m_toolBarModel = new ToolBarModel(mainWindow);
    listToolBars->setModel(m_toolBarModel);
    listToolBars->setCurrentIndex(m_toolBarModel->index(0,0));
}

void MainWindowOptions::insertToolBar()
{
    ToolBarProp *toolBarProp = new ToolBarProp();
    if (toolBarProp->exec() == QDialog::Accepted) {
        m_toolBarModel->insertRow(0);
        m_toolBarModel->setData(m_toolBarModel->index(0,0),
                                toolBarProp->text());
    }
}

void MainWindowOptions::removeToolBar()
{
    QModelIndex currentIndex = listToolBars->currentIndex();
    if (currentIndex.isValid())
        m_toolBarModel->removeRow(currentIndex.row());
    listToolBars->setCurrentIndex(m_toolBarModel->index(0,0));
}

void MainWindowOptions::renameToolBar()
{
    QModelIndex currentIndex = listToolBars->currentIndex();
    if (currentIndex.isValid()) {
        ToolBarProp *toolBarProp = new ToolBarProp();
        toolBarProp->setText(m_toolBarModel->data(currentIndex).toString());
        if (toolBarProp->exec() == QDialog::Accepted)
            m_toolBarModel->setData(currentIndex,
                                    toolBarProp->text());
    }
}
