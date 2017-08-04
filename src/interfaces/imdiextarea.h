#ifndef IMDIEXTAREA_H
#define IMDIEXTAREA_H

#include <QMdiArea>
#include <QTabBar>
#include <QMdiSubWindow>


//! Класс области подокон
class IMdiExtArea
{

public:
    //! Добавление подокна
    /*! Предназначено для добавления подокна с выбранным
        визуальным компонентом
        \param widget Визуальный компонент
    */
    virtual QMdiSubWindow *addSubWindow(QWidget *widget, Qt::WindowFlags flags = 0) = 0;

    //! Установка режима отображения окна
    /*! Предназначено для установки режима отображения подокна.
        Можно установить закладочный или оконный.
        \param mode Режим отображения подокна
    */
    virtual void setViewMode(QMdiArea::ViewMode mode) = 0;


    //! Активация подокна по имени компонента
    /*! Предназначено для активации окна по имени добавленого компонента.
        Для работы данного методы перед добавлением подокна необходимо установить
        имя объекта.
       \sa addSubWindow()
    */
    virtual QMdiSubWindow *setActiveSubWindow(QString widgetName) = 0;

    //! Активация подокна
    /*! Предназначено для активации указанного подокна
    */
    virtual void setActiveSubWindow(QMdiSubWindow *window) = 0;

    //! Получение подокна
    /*! Предназначено для получения подокна
    */
    virtual QMdiSubWindow *subWindow(QString widgetName) = 0;

public slots:
    //! Слот закрытия активного подокна
    virtual void closeActiveSubWindow() = 0;

    //! Слот закрытия всех подокон
    virtual void closeAllSubWindows() = 0;

    //! Слот для расположения подокон каскадом
    virtual void cascadeSubWindows() = 0;

    //! Слот для расположения подокон плиткой
    virtual void tileSubWindows() = 0;
};

#endif // IMDIEXTAREA_H
