#ifndef DBCONNECT_H
#define DBCONNECT_H

#include <QObject>
#include <QAction>
#include <QSqlDatabase>

#include <plugin/iplugin.h>
#include <idatabaseconnect.h>

#include "dbconnectglobal.h"

namespace RTPTechGroup {
namespace DbConnect {

//! Плагин соединения с базой данных
/*! Плагин предназначен для создания соединения с БД.
*/

class  DBCONNECTLIB DbConnect:
        public QObject,
        public IPlugin,
        public IDatabaseConnect
{
    Q_OBJECT

    Q_INTERFACES(IPlugin IDatabaseConnect)
    Q_PLUGIN_METADATA(IID IDatabaseConnect_iid FILE "dbconnect.json")
    
public:
    
    //! Конструктор плагина
    explicit DbConnect(QObject *parent = 0);

    //! Деструктор плагина
    virtual ~DbConnect();

// IPlugin
    //! Получение имени плагина
    QString name() {return APP_NAME;}
    
    //! Получение иконки плагина
    QIcon icon() {return QIcon(":/dbconnect");}
    
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
    //! Слот для подключения к БД
    void dbConnect();

private:
    //! Команда отображения окна соединения
    QAction *m_actionDbConnect;
};

}}

#endif

