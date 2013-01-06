#ifndef MAINWINDOW_H
#define	MAINWINDOW_H

#include "mainwindowglobal.h"
#include  "ui_mainwindow.h"
#include <QtGui>
#include <QMainWindow>

//! Класс главного окна
class MAINWINDOWLIB MainWindow: public QMainWindow, public Ui::MainWindow
{
        Q_OBJECT
public:

    //! Конструктор главного окна
    MainWindow(QMainWindow* pwgt = 0);

    //! Получение подокна
    /*! Предназначено для получения подокна
    */
    virtual QMdiSubWindow *subWindow(QString objName);

    //! Получение списка подокон
    /*! Предназначено для получения списка подокон
    */
    virtual QList<QMdiSubWindow *> subWindowList() const;

public slots:

    //! Слот обработки события закрытия главного окна
    void closeEvent(QCloseEvent *event);

    //! Слот обновления меню управления окнами
    /*! Слот предназаначен для изменения состояния
        пунктов меню при изменении состояния приложения
    */
    virtual void updateMenus();

    //! Слот установки оконного режима
    /*! Слот предназначен переключение приложения в закладочный
        или оконный режим
    */
    virtual void setWindowModeEnable(bool mode);

    //! Слот добавления подчиненного окна
    /*! Слот предназаначен для добавления подчиненного окна(закладки)
        в главное окно. Возращает добавленное подокно QMdiSubWindow
    */
    virtual QMdiSubWindow* addSubWindow(QWidget* widget);

    //! Слот установки активного окна по имени
    /*! Слот предназначен для установки активного подокна в главном окне
        по его имени
     */
    virtual QMdiSubWindow *setActiveSubWindow(QString objName);
};

#endif
