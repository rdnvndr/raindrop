#ifndef MAINWINDOWOPTIONS_H
#define MAINWINDOWOPTIONS_H

#include <QMenuBar>
#include <QStandardItemModel>
#include <QMainWindow>
#include "ui_mainwindowoptions.h"
#include "actiongroupmodel.h"
#include "toolbarmodel.h"

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

    //! Создание модели панелей инструментов
    void createToolBarModel(QMainWindow *mainWindow);

private slots:

    //! Создание новой панели инструментов
    void insertToolBar();

    //! Удаление панели инструментов
    void removeToolBar();

    //! Переименование панели инструментов
    void renameToolBar();

private:
    //! Хранит модель команд
    ActionGroupModel *m_actionGroupModel;
    
    //! Хранит модель панелей инструментов
    ToolBarModel *m_toolBarModel;
};

#endif // MAINWINDOWOPTIONS_H
