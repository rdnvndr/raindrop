#ifndef REGEXPVALUEVALIDATOR_H
#define REGEXPVALUEVALIDATOR_H

#include <QValidator>

namespace RTPTechGroup {
namespace ModelerIde {

//! Класс для проверки ввода регулярного выражения
/*! Класс предназначен для проверки ввода регулярного выражения
*/
class RegExpValueValidator : public QValidator
{
    Q_OBJECT
public:
    //! Конструктор класса
    explicit RegExpValueValidator(QObject *parent = nullptr);

public slots:
    //! Проверка значения
    State   validate(QString &input, qint32 &pos) const;

signals:
    //! Сигнал об изменении состояния
    void stateChanged(QValidator::State state) const;
};

}}

#endif // REGEXPVALUEVALIDATOR_H
