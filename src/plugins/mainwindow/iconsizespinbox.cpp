#include "iconsizespinbox.h"

namespace RTPTechGroup {
namespace MainWindow {

IconSizeSpinBox::IconSizeSpinBox(QWidget *parent) :
    QSpinBox(parent)
{
}

int IconSizeSpinBox::valueFromText(const QString &text) const
{
    QRegExp regExp(tr("(\\d+)(\\s*[xx]\\s*\\d+)?"));

    if (regExp.exactMatch(text)) {
        return regExp.cap(1).toInt();
    } else {
        return 0;
    }

}

QString IconSizeSpinBox::textFromValue(int val) const
{
    return tr("%1 x %1").arg(val);
}

}
}
