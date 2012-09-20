#ifndef MDIEXTAREA_H
#define MDIEXTAREA_H

#include <QMdiArea>
#include <QTabBar>
#include <QMdiSubWindow>
#include "mdiextareaglobal.h"

//! Класс области подокон
/*! Класс области подокон предназначен для создания
    подокон и для работы с ними
    \code
        mdiArea = new MdiExtArea(MainWindow);
        mdiArea->setObjectName(QString::fromUtf8("mdiArea"));
        MainWindow->setCentralWidget(mdiArea);
        QMdiSubWindow *subWindow = new QMdiSubWindow;
        QWidget widget = new QWidget();
        widget->setObjectName('MyWidgetName');
        subWindow->setWidget(widget);
        mdiArea->addSubWindow(subWindow);
     \endcode
*/

class MDIEXTAREALIB MdiExtArea : public QMdiArea
{
    Q_OBJECT
public:
    //! Конструктор класса области подокон
    MdiExtArea(QWidget *parent = 0);

    //! Добавление подокна
    /*! Предназначено для добавления подокна с выбранным
        визуальным компонентом
        \param widget Визуальный компонент
    */
    QMdiSubWindow *addSubWindow(QWidget *widget, Qt::WindowFlags flags = 0);

    //! Установка режима отображения окна
    /*! Предназначено для установки режима отображения подокна.
        Можно установить закладочный или оконный.
        \param mode Режим отображения подокна
    */
    void setViewMode(ViewMode mode);


    //! Активация подокна по имени компонента
    /*! Предназначено для активации окна по имени добавленого компонента.
        Для работы данного методы перед добавлением подокна необходимо установить
        имя объекта.
       \sa addSubWindow()
    */
    QMdiSubWindow *setActiveSubWindow(QString widgetName);

    //! Активация подокна
    /*! Предназначено для активации указанного подокна
    */
    void setActiveSubWindow(QMdiSubWindow * window);

    //! Получение подокна
    /*! Предназначено для получения подокна
    */
    QMdiSubWindow *subWindow(QString widgetName);
signals:

public slots:
    //! Слот закрытия активного подокна
    void closeActiveSubWindow();

    //! Слот закрытия всех подокон
    void closeAllSubWindows();

    //! Слот для расположения подокон каскадом
    void cascadeSubWindows();

    //! Слот для расположения подокон плиткой
    void tileSubWindows();
};

#endif // MDIEXTAREA_H
