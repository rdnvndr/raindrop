#ifndef REGEXPVALIDATOR_H
#define REGEXPVALIDATOR_H

#include <QRegExpValidator>

namespace RTPTechGroup {
namespace ModelerIde {

//! Класс для проверки значения при помощи регулярного выражения
/*! Класс предназначен для проверки значения при помощи регулярного выражения
*/
class RegExpValidator : public QRegExpValidator
{
    Q_OBJECT
public:
    //! Конструктор класса
    explicit RegExpValidator(QObject * parent = 0);

    //! Конструктор класса
    explicit RegExpValidator(const QRegExp & rx, QObject * parent = 0);

public slots:
    //! Проверка значения
    QValidator::State validate(QString &input, int &pos) const;

signals:
    //! Сигнал об изменении состояния
    void stateChanged(QValidator::State state) const;

};

}}

#endif // REGEXPVALIDATOR_H
