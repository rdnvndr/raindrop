#include "tilingbutton.h"

#include <QVBoxLayout>
#include <QObject>
#include <QTime>
#include <QToolButton>
#include <QIcon>
#include <QPlainTextEdit>
#include <QStackedLayout>

TilingButton::TilingButton(QWidget *parent) : TilingWidget(parent)
{
    QHBoxLayout *hlayout = new QHBoxLayout();
    hlayout->setMargin(0);
    hlayout->setSpacing(0);

    m_comboBox = new QComboBox(this);
    m_comboBox->setMinimumWidth(150);
    hlayout->addWidget(m_comboBox);

    hlayout->addStretch();

    QToolButton *button1 = new QToolButton(this);
    button1->setAutoRaise(true);
    button1->setIcon(QIcon(":/horizontal"));
    button1->setText(tr("Разделить по горизонтали"));
    button1->setToolTip(tr("Разделить по горизонтали"));
    hlayout->addWidget(button1);
    connect(button1, &QAbstractButton::clicked,
            this, &TilingWidget::splitHorizontal);

    QToolButton *button2 = new QToolButton(this);
    button2->setAutoRaise(true);
    button2->setIcon(QIcon(":/vertical"));
    button2->setText(tr("Разделить по вертикали"));
    button2->setToolTip(tr("Разделить по вертикали"));
    hlayout->addWidget(button2);
    connect(button2, &QAbstractButton::clicked,
            this, &TilingWidget::splitVertical);

    QToolButton *button3 = new QToolButton(this);
    button3->setAutoRaise(true);
    button3->setIcon(QIcon(":/closeleft"));
    button3->setText(tr("Удалить разделение"));
    button3->setToolTip(tr("Удалить разделение"));
    hlayout->addWidget(button3);
    connect(button3, &QAbstractButton::clicked,
            this, &TilingWidget::unsplit);

    QToolButton *button4 = new QToolButton(this);
    button4->setAutoRaise(true);
    button4->setIcon(QIcon(":/closetop"));
    button4->setText(tr("Удалить все разделения"));
    button4->setToolTip(tr("Удалить все разделения"));
    hlayout->addWidget(button4);
    connect(button4, &QAbstractButton::clicked,
            this, &TilingWidget::unsplitAll);

    QVBoxLayout *vlayout = new QVBoxLayout(this);
    vlayout->setMargin(0);
    vlayout->setSpacing(0);
    vlayout->addLayout(hlayout);
    m_stackedLayout = new QStackedLayout;
    vlayout->addLayout(m_stackedLayout);

    for (int i = 1;i <= 3; i++) {
        QPlainTextEdit *plainTextEdit = new QPlainTextEdit(this);
        connect(plainTextEdit, &QPlainTextEdit::textChanged,
                this, &TilingButton::textChange);
        m_stackedLayout->addWidget(plainTextEdit);
        m_comboBox->addItem(QString("Page %1").arg(i));
    }

    connect(m_comboBox,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::activated),
            m_stackedLayout,
            static_cast<void(QStackedLayout::*)(int)>(
                &QStackedLayout::setCurrentIndex)
            );

    this->setLayout(vlayout);
}

TilingWidget *TilingButton::clone()
{
    TilingButton *tilingButton = new TilingButton();
    tilingButton->m_comboBox->setCurrentIndex(m_comboBox->currentIndex());
    tilingButton->m_stackedLayout->setCurrentIndex(m_stackedLayout->currentIndex());

    connect(this, &TilingButton::plainTextChanged,
            tilingButton, &TilingButton::setPlainText);
    connect(tilingButton, &TilingButton::plainTextChanged,
            this, &TilingButton::setPlainText);

    for (int i = m_stackedLayout->count() - 1; i >= 0; i--) {
        QPlainTextEdit *srcPlainTextEdit = qobject_cast<QPlainTextEdit *>(
                    m_stackedLayout->widget(i));
        QPlainTextEdit *dstPlainTextEdit = qobject_cast<QPlainTextEdit *>(
                    tilingButton->m_stackedLayout->widget(i));
        dstPlainTextEdit->setPlainText(srcPlainTextEdit->toPlainText());
    }

    return tilingButton;
}

void TilingButton::textChange()
{
    QPlainTextEdit *plainTextEdit
            = qobject_cast<QPlainTextEdit *>(QObject::sender());
    int index = m_stackedLayout->indexOf(plainTextEdit);
    emit plainTextChanged(index, plainTextEdit->toPlainText());
}

void TilingButton::setPlainText(int index, const QString &text)
{
    QPlainTextEdit *plainTextEdit
            = qobject_cast<QPlainTextEdit *>(m_stackedLayout->widget(index));
    if (plainTextEdit->toPlainText() != text)
        plainTextEdit->setPlainText(text);
}
