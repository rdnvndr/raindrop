#include "mainwindowoptions.h"
#include "toolbarprop.h"
#include "hotkeydelegate.h"
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
    connect(lineEditHotKey,SIGNAL(changeHotKey(QKeySequence)),
            this, SLOT(changeHotKey(QKeySequence)));
    connect(pushButtonKeyClear,SIGNAL(clicked()),this,SLOT(resetHotKey()));
    connect(pushButtonKeyDelete,SIGNAL(clicked()),this,SLOT(deleteHotKey()));
    treeViewHotKey->setItemDelegate(new HotKeyDelegate());
    connect(radioButtonOther,SIGNAL(toggled(bool)),
            this, SLOT(selectOtherSize(bool)));
    spinBoxIconSize->setDisabled(true);
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
    treeViewHotKey->setModel(m_actionGroupModel);
    treeViewHotKey->setColumnWidth(0,250);
    treeViewHotKey->setColumnHidden(2,true);
    treeViewHotKey->setColumnHidden(3,true);
    treeViewHotKey->setColumnHidden(4,true);
    treeViewHotKey->expandAll();

    listCategory->setCurrentIndex(m_actionGroupModel->index(0,0));
    listCommands->setRootIndex(m_actionGroupModel->index(0,0));

    connect(treeViewHotKey->selectionModel(),SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this,SLOT(hotKeyCurrentChange(QModelIndex,QModelIndex)));

    connect(listCommands->selectionModel(),SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this,SLOT(listCommandsCurrentChange(QModelIndex,QModelIndex)));

    connect(listCategory->selectionModel(),SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this,SLOT(listCategoryCurrentChange(QModelIndex,QModelIndex)));
}

void MainWindowOptions::createToolBarModel(QMainWindow *mainWindow)
{
    m_toolBarModel = new ToolBarModel(mainWindow);
    listToolBars->setModel(m_toolBarModel);
    listToolBars->setCurrentIndex(m_toolBarModel->index(0,0));
}

void MainWindowOptions::setIconSize(const QSize &iconSize)
{
    if (iconSize.height() == 16 && iconSize.width() == 16)
        radioButtonSmall->setChecked(true);
    else if (iconSize.height() == 24 && iconSize.width() == 24)
        radioButtonMedium->setChecked(true);
    else if (iconSize.height() == 32 && iconSize.width() == 32)
        radioButtonLarge->setChecked(true);
    else if (iconSize.height() == 20 && iconSize.width() == 20)
        radioButtonSystem->setChecked(true);
    else
        radioButtonOther->setChecked(true);

    spinBoxIconSize->setValue(iconSize.height());
}

QSize MainWindowOptions::iconSize()
{
    if (radioButtonSmall->isChecked())
        return QSize(16,16);
    else if (radioButtonMedium->isChecked())
        return QSize(24,24);
    else if (radioButtonLarge->isChecked())
        return QSize(32,32);
    else if (radioButtonOther->isChecked())
        return QSize(spinBoxIconSize->value(),spinBoxIconSize->value());

    return QSize(20,20);
}

void MainWindowOptions::setIconStyle(Qt::ToolButtonStyle iconStyle)
{
    if (iconStyle == Qt::ToolButtonIconOnly)
        radioButtonIconOnly->setChecked(true);
    else if (iconStyle == Qt::ToolButtonTextOnly)
        radioButtonTextOnly->setChecked(true);
    else if (iconStyle == Qt::ToolButtonTextBesideIcon)
        radioButtonTextBeside->setChecked(true);
    else if (iconStyle == Qt::ToolButtonTextUnderIcon)
        radioButtonTextUnder->setChecked(true);
    else if (iconStyle == Qt::ToolButtonFollowStyle)
        radioButtonSytemStyle->setChecked(true);
}

Qt::ToolButtonStyle MainWindowOptions::iconStyle()
{
    if (radioButtonIconOnly->isChecked())
        return Qt::ToolButtonIconOnly;
    else if (radioButtonTextOnly->isChecked())
        return Qt::ToolButtonTextOnly;
    else if (radioButtonTextBeside->isChecked())
        return Qt::ToolButtonTextBesideIcon;
    else if (radioButtonTextUnder->isChecked())
        return Qt::ToolButtonTextUnderIcon;

    return Qt::ToolButtonFollowStyle;
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

void MainWindowOptions::changeHotKey(QKeySequence keySequence)
{
    QModelIndex currentIndex = treeViewHotKey->currentIndex();
    if (currentIndex.isValid() && currentIndex.parent().isValid()) {
        m_actionGroupModel->setData(currentIndex.sibling(currentIndex.row(),1),
                                    keySequence);
    }
}

void MainWindowOptions::hotKeyCurrentChange(QModelIndex current, QModelIndex old)
{
    Q_UNUSED(old)
    QVariant key = m_actionGroupModel->data(current.sibling(current.row(),1));
    lineEditHotKey->setKeySequence(key.value<QKeySequence>());
}

void MainWindowOptions::listCategoryCurrentChange(QModelIndex current, QModelIndex old)
{
    Q_UNUSED(old)
    listCommands->setCurrentIndex(current.child(0,0));
}

void MainWindowOptions::listCommandsCurrentChange(QModelIndex current, QModelIndex old)
{
    Q_UNUSED(old)
    QVariant whatsThis = m_actionGroupModel->data(current.sibling(current.row(),2));
    labelDescription->setText(whatsThis.toString());
}

void MainWindowOptions::resetHotKey()
{
    lineEditHotKey->resetKeySequence();
    QModelIndex currentIndex = treeViewHotKey->currentIndex();
    if (currentIndex.isValid() && currentIndex.parent().isValid()) {
        m_actionGroupModel->setData(currentIndex.sibling(currentIndex.row(),1),
                                    lineEditHotKey->keySequence());
    }
}

void MainWindowOptions::deleteHotKey()
{
    lineEditHotKey->setKeySequence(QKeySequence());
    QModelIndex currentIndex = treeViewHotKey->currentIndex();
    if (currentIndex.isValid() && currentIndex.parent().isValid()) {
        m_actionGroupModel->setData(currentIndex.sibling(currentIndex.row(),1),
                                    lineEditHotKey->keySequence());
    }
}

void MainWindowOptions::selectOtherSize(bool check)
{
    spinBoxIconSize->setEnabled(check);
}
