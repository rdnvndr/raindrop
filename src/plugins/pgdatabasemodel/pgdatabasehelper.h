#ifndef PGDATABASEHELPER_H
#define PGDATABASEHELPER_H

#include <QtCore/QString>

#include <databasemodel/idatabaseclass.h>

namespace RTPTechGroup {
namespace DatabaseModel {

//! Возвращает наименование режимов доступа
QString vlAccessMode(IDatabaseClass::AccessMode mode);

//! Возвращает наименование типов класса
QString vlClassType(IDatabaseClass::ClassType clsType);

//! Возвращает строку идентификатор
QString vlUuidString(const QUuid &uuid);

//! Возвращает таблицу для класса
QString clsTable(const QString &className);

//! Возвращает таблицу для списка значений
QString lovTable(const QString &lovName);

//! Возвращает поле таблицы для атрибута
QString tblField(const QString &attrName);

//! Возвращает тип БД для логического типа
QString fldBool();

//! Возвращает тип БД для бинарного типа
QString fldBin();

//! Возвращает тип БД для типа символ
QString fldChar();

//! Возвращает тип БД для типа дата
QString fldDate();

//! Возвращает тип БД для вещественного типа фиксированной точности
QString fldDcml();

//! Возвращает тип БД для вещественного типа плавающей точкой
QString fldDbl();

//! Возвращает тип БД для целого типа
QString fldInt();

//! Возвращает тип БД для типа строки постоянной и переменной длины
QString fldStr(int count, bool variable = true);

//! Возвращает тип БД для типа время
QString fldTime();

//! Возвращает тип БД для типа дата и время
QString fldStmp();

//! Возвращает тип БД для типа глобальный идентификатор
QString fldGuid();

}}

#endif // PGDATABASEHELPER_H
