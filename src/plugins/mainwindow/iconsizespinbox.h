#ifndef ICONSIZESPINBOX_H
#define ICONSIZESPINBOX_H

#include <QSpinBox>

namespace RTPTechGroup {
namespace MainWindow {

//! Виджет ввода размера иконки

class IconSizeSpinBox : public QSpinBox
{
    Q_OBJECT
public:
    explicit IconSizeSpinBox(QWidget *parent = 0);

    //! Получение значения из текста
    qint32 valueFromText(const QString &text) const;

    //! Получение текста по значению
    QString textFromValue(qint32 val) const;
};

}
}

#endif // ICONSIZESPINBOX_H
