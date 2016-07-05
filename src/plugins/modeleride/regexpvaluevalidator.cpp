#include "regexpvaluevalidator.h"

#include <QRegExpValidator>

namespace RTPTechGroup {
namespace ModelerIde {

RegExpValueValidator::RegExpValueValidator(QObject *parent)
    :QValidator (parent)
{

}

QValidator::State RegExpValueValidator::validate(QString &input, int &pos) const
{
    Q_UNUSED(pos)

    QRegExp regExp = QRegExp(input);

    QValidator::State state = (regExp.isValid())
            ? QValidator::Acceptable : QValidator::Intermediate;
    emit stateChanged(state);

    return state;
}

}}
