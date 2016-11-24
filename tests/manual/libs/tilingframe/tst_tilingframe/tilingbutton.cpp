#include "tilingbutton.h"

#include <QVBoxLayout>
#include <QObject>
#include <QTime>

TilingButton::TilingButton(QWidget *parent) : TilingWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QPushButton *button1 = new QPushButton(this);
    button1->setText("Разделить по горизонтали " + QTime::currentTime().toString());
    layout->addWidget(button1);
    connect(button1,SIGNAL(clicked()),this, SLOT(splitHorizontal()));

    QPushButton *button2 = new QPushButton(this);
    button2->setText("Разделить по вертикали");
    layout->addWidget(button2);
    connect(button2,SIGNAL(clicked()),this, SLOT(splitVertical()));

    QPushButton *button3 = new QPushButton(this);
    button3->setText("Удалить разделение");
    layout->addWidget(button3);
    connect(button3,SIGNAL(clicked()),this, SLOT(unsplit()));

    QPushButton *button4 = new QPushButton(this);
    button4->setText("Удалить все разделения");
    layout->addWidget(button4);
    connect(button4,SIGNAL(clicked()),this, SLOT(unsplitAll()));

    this->setLayout(layout);
}

TilingWidget *TilingButton::clone()
{
    TilingButton *t = new TilingButton();
    return t;
}
