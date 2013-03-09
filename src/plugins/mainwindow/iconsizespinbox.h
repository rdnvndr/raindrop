#ifndef ICONSIZESPINBOX_H
#define ICONSIZESPINBOX_H

#include <QSpinBox>

//! Виджет ввода размера иконки

class IconSizeSpinBox : public QSpinBox
{
    Q_OBJECT
public:
    explicit IconSizeSpinBox(QWidget *parent = 0);

    //! Получение значения из текста
    int valueFromText(const QString &text) const;

    //! Получение текста по значению
    QString textFromValue(int val) const;
};

#endif // ICONSIZESPINBOX_H
