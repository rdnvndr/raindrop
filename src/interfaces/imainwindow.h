#ifndef IMAINWINDOW_H
#define IMAINWINDOW_H

#include <QMdiSubWindow>
#include <QMainWindow>
#include <mdiextarea/mdiextarea.h>

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

    //! Добавления QAction для использования в главном окне
    virtual void addAction(QString category, QAction *action) = 0;

public slots:

    //! Запись настроек меню
    virtual void writeMenuSettings() = 0;

    //! Чтение настроек меню
    virtual void readMenuSettings() = 0;

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
        в главное окно. Возращает добавленное подокно QMdiSubWindow
    */
    virtual QMdiSubWindow* addSubWindow(QWidget* widget) = 0;

    //! Слот установки активного окна по имени
    /*! Слот предназначен для установки активного подокна в главном окне
        по его имени
     */
    virtual QMdiSubWindow *setActiveSubWindow(QString objName) = 0;

    //! Получение меню File
    virtual QMenu* getMenuFile() = 0;

    //! Получение меню Edit
    virtual QMenu* getMenuEdit() = 0;

    //! Получение меню Settings
    virtual QMenu* getMenuSettings() = 0;

    //! Получение меню Help
    virtual QMenu* getMenuHelp() = 0;

    //! Получение панели инструментов
    virtual QToolBar*   getToolBarMain() = 0;

    //! Получение области подокон
    virtual MdiExtArea* getMdiArea() = 0;
};

Q_DECLARE_INTERFACE(IMainWindow,"com.RTPTechGroup.Raindrop.IMainWindow/1.0")

#endif // IMAINWINDOW_H
