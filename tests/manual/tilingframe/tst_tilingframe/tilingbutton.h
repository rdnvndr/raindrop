#ifndef TILINGBUTTON_H
#define TILINGBUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QStackedLayout>
#include <QComboBox>

#include <tilingframe/tilingwidget.h>

using namespace RTPTechGroup::Widgets;

class TilingButton : public TilingWidget
{
    Q_OBJECT

public:
    explicit TilingButton(QWidget *parent = 0);
    TilingWidget *clone();

    void setPlainText(int index, const QString &text);

signals:
    void plainTextChanged(int index, const QString &text);

private:
    void textChange();

    QStackedLayout *m_stackedLayout;
    QComboBox *m_comboBox;

};

#endif // TILINGBUTTON_H
