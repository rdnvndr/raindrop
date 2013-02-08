#ifndef MAINWINDOWOPTIONS_H
#define MAINWINDOWOPTIONS_H

#include <QMenuBar>
#include <QStandardItemModel>
#include "ui_mainwindowoptions.h"
#include "actiongroupmodel.h"

class MainWindowOptions : public QWidget, public Ui::MainWindowOptions
{
    Q_OBJECT
public:
    explicit MainWindowOptions(QWidget *parent = 0);
    virtual ~MainWindowOptions();

    //! Создает модель команд
    void createActionsModel(QMultiHash<QString, QAction *> *actions);

private:
    ActionGroupModel* m_model;
    
};

#endif // MAINWINDOWOPTIONS_H
