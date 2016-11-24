#ifndef TILINGFRAME_H
#define TILINGFRAME_H

#include "tilingframeglobal.h"

#include <QWidget>
#include <QSplitter>
#include <QStackedLayout>

#include "tilingwidget.h"

namespace RTPTechGroup {
namespace Widgets {

class TilingWidget;

//! Фреймовое представленение рабочей области
class TILINGFRAMELIB TilingFrame: public QWidget
{
    Q_OBJECT

public:
    //! Конструктор класса
    explicit TilingFrame(TilingWidget *widget = 0);

    //! Деструктор класса
    ~TilingFrame();


    //! Проверка содержит ли фрейм виджет
    bool isWidget() const;

    //! Получение виджета фрейма
    TilingWidget *widget() const;

    //! Получение виджета с отвязкой от текущего фрейма
    TilingWidget *takeWidget();


    //! Проверка содержит ли фрейм сплитер
    bool isSplitter() const;

    //! Получение сплитера фрейма
    QSplitter *splitter() const;

    //! Получение сплитера с отвязкой от текущего фрейма
    QSplitter *takeSplitter();

    //! Получение родительского фрейма со сплитером
    TilingFrame *findParentSplitter() const;

    //! Получение родительского фрейма
    TilingFrame *parentFrame() const;

public slots:
    //! Разделение фрейма
    void split(Qt::Orientation orientation);

    //! Разделение фрейма по горизонтали
    void splitHorizontal();

    //! Разделение фрейма по вертикали
    void splitVertical();

    //! Схлопывание фрейма
    void unsplit(TilingFrame *unsplitFrame = NULL);

    //! Схлопывание всех фреймов
    void unsplitAll();

signals:
    //! Сигнал о разделении или схлопывании фреймов
    void splitStateChanged();

private:
    //! Сплитер фреймов
    QSplitter *m_splitter;

    //! Хранилище виджета
    QStackedLayout *m_layout;

    //! Виджет фрейма
    TilingWidget *m_tilingWidget;

    //! Родительский тайловый фрейм
    TilingFrame *m_parentFrame;
};

}}

#endif
