#ifndef MDISUBWINDOW_H
#define MDISUBWINDOW_H

#include <QMdiSubWindow>

namespace RTPTechGroup {
namespace MainWindow {

//! Класс подокна области MdiAreaExt
/*! Предназначен для создания подокна в области  MdiAreaExt и обхода
 *  ошибки позднего удаления widget() при закрытии окна методом close().\n
 *  Для исправления ошибки переопределен метод: \n
 *      void closeEvent(QCloseEvent *event)
 */

class MdiSubWindow : public QMdiSubWindow
{
    Q_OBJECT

public:
    //! Конструктор класса
    explicit MdiSubWindow(QWidget *parent = 0, Qt::WindowFlags flags = Qt::WindowFlags());

    //! Деструктор класса
    virtual ~MdiSubWindow();

protected:
    //! Событие закрытия окна
    void closeEvent(QCloseEvent *event);
};

}
}

#endif // MDISUBWINDOW_H
