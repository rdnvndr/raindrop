#ifndef MAINWINDOWOPTIONS_H
#define MAINWINDOWOPTIONS_H

#include <QMenuBar>
#include <QStandardItemModel>
#include "ui_mainwindowoptions.h"
#include "actiongroupmodel.h"

//! Класс настройки панели инструментов и главного меню
/*! Класс настройки панели инструментов и главного меню предназначен
 *  для настройки панели инструментов и главного меню при помощи
 *  технологии Drag and Drop
*/

class MainWindowOptions : public QDialog, public Ui::MainWindowOptions
{
    Q_OBJECT
public:

    //! Конструктор класса
    explicit MainWindowOptions(QWidget *parent = 0);

    //! Деструктор класса
    virtual ~MainWindowOptions();

    //! Создание модели команд
    void createActionsModel(QMultiHash<QString, QAction *> *actions);

private:
    //! Хранит модель команд
    ActionGroupModel* m_model;
    
};

#endif // MAINWINDOWOPTIONS_H
