#ifndef SQLEDITOR_H
#define SQLEDITOR_H

#include <QObject>
#include <QAction>
#include <plugin/iplugin.h>
#include "sqleditorglobal.h"

namespace RTPTechGroup {
namespace SqlEditor {

//! Плагин выполнения SQL запросов
/*! Плагин предназначен для создания в редакторе SQL запросов к БД и
 *  их выполнения.
*/
class  SQLEDITORLIB SqlEditor:
        public QObject,
        public IPlugin
{
    Q_OBJECT

    Q_INTERFACES(IPlugin)
    Q_PLUGIN_METADATA(IID IPlugin_iid FILE "sqleditor.json")
    
public:
    
    //! Конструктор плагина
    explicit SqlEditor(QObject *parent = 0);

    //! Деструктор плагина
    virtual ~SqlEditor();
    
// IPlugin

    //! Получение имени плагина
    QString name() {return APP_NAME;}
    
    //! Получение иконки плагина
    QIcon icon() {return QIcon(":/sqleditor");}
    
    //! Описание продукта
    QString product() {return tr(APP_PRODUCT);}

    //! Авторские права
    QString copyright() {return tr(APP_COPYRIGHT);}

    //! Описание плагина
    QString descript() {return tr(APP_DESCRIPTION);}

    //! Категория в которой состоит плагин
    QString category() {return tr("");}

    //! Версия плагина
    QString version() {return APP_VERSION;}

    //! Производитель плагина
    QString vendor() {return tr(APP_COMPANY);}

public slots:
    //! Отображение SQL редактора
    void showSqlEditor();

private:
    //! Команда вызова SQL редактора
    QAction *m_actionSqlEditor;
};

}}

#endif

