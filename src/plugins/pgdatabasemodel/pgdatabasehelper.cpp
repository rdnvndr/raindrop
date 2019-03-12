#include "pgdatabasehelper.h"

namespace RTPTechGroup {
namespace DatabaseModel {

QString clsTable(const QString &className) {
    return QString("\"CLS" + className + "\"");
}

QString lovTable(const QString &lovName) {
    return QString("\"LOV" + lovName + "\"");
}

QString tblField(const QString &attrName) {
    return QString("\"" + attrName + "\"");
}

QString fldBool() {
    return QString("BOOL");
}

QString fldBin(){
    return QString("VARCHAR(4000)");
}

QString fldChar(){
    return QString("CHAR");
}

QString fldDate() {
    return QString();
}

QString fldDcml() {
    return QString();
}

QString fldDbl() {
    return QString("DOUBLE PRECISION");
}

QString fldInt() {
    return QString("INTEGER");
}

QString fldStr(int count, bool variable) {
    return QString( (variable) ? "VARCHAR(%1)" : "CHAR(%1)").arg(count);
}

QString fldTime() {
    return QString();
}

QString fldStmp() {
    return QString();
}

QString fldGuid() {
    return QString("CHAR(36)");
}

QString vlAccessMode(IDatabaseClass::AccessMode mode)
{
    switch (mode) {
    case IDatabaseClass::System:
        return QString("System");
    case IDatabaseClass::Standart:
        return QString("Standart");
    case IDatabaseClass::User:
        return QString("User");
    }

    return QString("User");
}

QString vlClassType(IDatabaseClass::ClassType clsType)
{
    switch (clsType) {
    case IDatabaseClass::Abstract:
        return QString("Abstract");
    case IDatabaseClass::Context:
        return QString("Context");
    case IDatabaseClass::Embedded:
        return QString("Embedded");
    case IDatabaseClass::Normal:
        return QString("Normal");
    }

    return QString("Normal");
}

QString vlUuidString(const QUuid &uuid)
{
    return uuid.toString(QUuid::WithoutBraces);
}

}}
