#ifndef IMAINWINDOW_H
#define IMAINWINDOW_H

#include <QMdiSubWindow>
#include <QMainWindow>
#include <QDockWidget>
#include <imdiextarea.h>

//! Класс главного окна
class IMainWindow
{

public:

    //! Получение подокна
    /*! Предназначено для получения подокна
    */
    virtual QMdiSubWindow *subWindow(QString objName) = 0;

    //! Получение списка подокон
    /*! Предназначено для получения списка подокон
    */
    virtual QList<QMdiSubWindow *> subWindowList() const = 0;

    //! Добавление стыковго компонента
    /*! Предназначено для добавления стыкового компонента
    */
    virtual void addDockWidget(Qt::DockWidgetArea area, QDockWidget *dockwidget) = 0;

public slots:

    //! Добавления QAction для использования в главном окне
    virtual void addAction(QString category, QAction *action) = 0;

    //! Удаление QAction из главного окна
    virtual void removeAction(QAction *action) = 0;

    //! Запись настроек меню
    virtual void writeBarSettings() = 0;

    //! Чтение настроек меню
    virtual void readBarSettings() = 0;

    //! Чтение и применение настроек главного окна
    virtual void readSettings() = 0;

    //! Запись настроек главного окна
    virtual void writeSettings() = 0;

    //! Слот обработки события закрытия главного окна
    virtual void closeEvent(QCloseEvent *event) = 0;

    //! Слот обновления меню управления окнами
    /*! Слот предназаначен для изменения состояния
        пунктов меню при изменении состояния приложения
    */
    virtual void updateMenus() = 0;

    //! Слот установки оконного режима
    /*! Слот предназначен переключение приложения в закладочный
        или оконный режим
    */
    virtual void setWindowModeEnable(bool mode) = 0;

    //! Слот добавления подчиненного окна
    /*! Слот предназаначен для добавления подчиненного окна(закладки)
        в главное окно. Возвращает добавленное подокно QMdiSubWindow
    */
    virtual QMdiSubWindow *addSubWindow(QWidget *widget) = 0;

    //! Слот установки активного окна по имени
    /*! Слот предназначен для установки активного подокна в главном окне
        по его имени
     */
    virtual QMdiSubWindow *setActiveSubWindow(QString objName) = 0;

    //! Получение области подокон
    virtual IMdiExtArea *getMdiArea() = 0;
};

#define IMainWindow_iid "com.RTPTechGroup.Raindrop.IMainWindow"
Q_DECLARE_INTERFACE(IMainWindow, IMainWindow_iid)

#endif // IMAINWINDOW_H
