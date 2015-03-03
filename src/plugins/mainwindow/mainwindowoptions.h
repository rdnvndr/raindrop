#ifndef MAINWINDOWOPTIONS_H
#define MAINWINDOWOPTIONS_H

#include <QMenuBar>
#include <QStandardItemModel>
#include <QMainWindow>
#include "ui_mainwindowoptions.h"
#include "actiongroupmodel.h"
#include "toolbarmodel.h"

namespace RTPTechGroup {
namespace MainWindow {

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

    //! Установка размера иконки
    void setIconSize(const QSize & iconSize);

    //! Получение размера иконки
    QSize iconSize();

    //! Установка стиля иконки для панелей инструментов
    void  setIconStyle(Qt::ToolButtonStyle iconStyle);

    //! Получение стиля иконки для панелей инструментов
    Qt::ToolButtonStyle iconStyle();

private slots:

    //! Создание новой панели инструментов
    void insertToolBar();

    //! Удаление панели инструментов
    void removeToolBar();

    //! Переименование панели инструментов
    void renameToolBar();

    //! Смена горячих клавишей
    void changeHotKey(QKeySequence keySequence);

    //! Смена строки в дереве команд
    void commandCurrentChange(QModelIndex current, QModelIndex old);

    //! Сброс смены горячей клавиши
    void resetHotKey();

    //! Удаление горячей клавиши
    void deleteHotKey();

    //! Обработка выбора размера иконки "Другой"
    void selectOtherSize(bool check);

private:
    //! Хранит модель команд
    ActionGroupModel *m_actionGroupModel;
    
    //! Хранит модель панелей инструментов
    ToolBarModel *m_toolBarModel;
};

}
}

#endif // MAINWINDOWOPTIONS_H
