#ifndef DBCONNECT_H
#define DBCONNECT_H

#include <QObject>
#include <QAction>
#include <QSqlDatabase>
#include <plugin/iplugin.h>
#include "dbconnectglobal.h"

//! Плагин соединения с базой данных
/*! Плагин предназначен для создания соединения с БД.
*/

class  DBCONNECTLIB DbConnect:
        public QObject,
        public IPlugin
{
    Q_OBJECT
    Q_INTERFACES(IPlugin)
    
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "com.RTPTechGroup.Raindrop.DbConnect" FILE "dbconnect.json")
#endif
    
public:
    
    //! Конструктор плагина
    explicit DbConnect(QObject *parent = 0);

    //! Деструктор плагина
    virtual ~DbConnect();

    // IPlugin
    
    //! Получение экземпляра
    QObject *instance() { return this; }
    
    //! Получение имени плагина
    QString name() {return tr("Подключение к БД");};
    
    //! Получение иконки плагина
    QIcon icon() {return QIcon(":/dbconnect");}
    
    //! Описание плагина
    QString descript() {return tr("");};
    
    //! Категория в которой состоит плагин
    QString category() {return tr("");};

    //! Версия плагина
    QString version() {return tr("1.0");};
    
    //! Производитель плагина
    QString vendor() {return tr("RTPTechGroup");};

public slots:
    //! Слот для подключения к БД
    void dbConnect();

private:
    //! Команда отображения окна соединения
    QAction *actionDbConnect;
};

#endif

