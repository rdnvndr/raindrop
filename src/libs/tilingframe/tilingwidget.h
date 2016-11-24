#ifndef TILINGWIDGET_H
#define TILINGWIDGET_H

#include "tilingframe.h"

namespace RTPTechGroup {
namespace Widgets {

class TilingFrame;

//! Тайловый виджет
/*! Тайловый виджет предназначен для добавления виджета в тайловый фрейм
 */

class TILINGFRAMELIB TilingWidget: public QWidget
{
    Q_OBJECT
public:
    //! Конструтор класса
    explicit TilingWidget(QWidget *parent = 0);

    //! Деструктор класса
    virtual ~TilingWidget();

    //! Предоставляет копию виджета
    virtual TilingWidget *clone() = 0;

    //! Возращает родительский фрейм виджета
    TilingFrame *tilingFrame() { return m_tilingFrame;}

public slots:
    //! Разделение фрейма
    void split(Qt::Orientation orientation);

    //! Разделение фрейма по горизонтали
    void splitHorizontal();

    //! Разделение фрейма по вертикали
    void splitVertical();

    //! Схлопывание фрейма
    void unsplit();

    //! Схлопывание всех фреймов
    void unsplitAll();

private:
    friend class TilingFrame;

    //! Устанавливает родительский фрейм для виджета
    void setTilingFrame(TilingFrame *tilingFrame) { m_tilingFrame = tilingFrame; }

    //! Фрейм виджета
    TilingFrame *m_tilingFrame;

};

}}

#endif // TILINGWIDGET_H
