#ifndef DOCKWIDGET_H
#define DOCKWIDGET_H

#include <QWidget>
#include <QDockWidget>
#include <QTimer>
#include "guiextensionglobal.h"

namespace RTPTechGroup {
namespace Widgets {


//! Компонент, который который может быть пристыкован
//! к главному окну QMainWindow. По сравнению с QDockWidget
//! обладает свойством скрытия заголовка и автоскрытием заголовка

class GUIEXTENSIONLIB DockWidget : public QDockWidget
{
public:
    //! Конструктор класса
    explicit DockWidget(QWidget *parent = 0);

    //! Деструктор класса
    virtual ~DockWidget();


    //! Событие с фильтрацией
    bool eventFilter(QObject *obj, QEvent *event);

    //! Событие ввода
    void enterEvent(QEvent *event);

    //! Событие покидания курсоры мыши окна
    void leaveEvent(QEvent *event);

    //! Обработка события от мыши по таймеру
    void handleMouseTimeout();

    //! Обработка события изиения типа окна
    void handleToplevelChanged(bool floating);


    //! Устанавливает минимальную ширину компонента
    void setMinimumWidth(qint32 minw);

    //! Устанавливает видимость заголовка
    void setTitleBarVisible(bool visible);


    //! Возращает виден ли заголовк
    bool isTitleBarVisible();

    //! Устанавливает компонент в качестве заголовка
    void setTitleBarWidget(QWidget *widget);

    //! Устанавливает автоматическое скрытие заголовка
    void setAutoHide(bool autoHide);

    //! Возращает установлено ли автоматическое скрытие заголовков
    bool isAutoHide();

private:
    //! Точка старта
    QPoint  m_startPos;
    //! Таймер
    QTimer  m_timer;
    //! Пользовательский заголовок окна
    QWidget *m_titleBar;
    //! Пустой заголовок окна
    QWidget *m_emptyTitleBar;
    //! Флаг автоскрытия заголовка
    bool m_autoHide;
};

}}

#endif // DOCKWIDGET_H
