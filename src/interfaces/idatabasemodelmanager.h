#ifndef IDATABASEMODELMANAGER_H
#define IDATABASEMODELMANAGER_H

#include <idatabasemodelbuilder.h>

//! Менеджер модели базы данных
class IDatabaseModelManager
{

public:
    //! Конструктор класса
    explicit IDatabaseModelManager() {
        m_pool = nullptr;
    }

    //! Деструктор класса
    virtual ~IDatabaseModelManager() {}

// IDatabaseModel
    //! Получение модели базы данных
    virtual IDatabaseModel *createInstance(
            QSqlDatabase db = QSqlDatabase::database()) = 0;

    //! Создаёт поток базы данных
    virtual IDatabaseThread *createDatabaseThread() {
        if (m_pool != nullptr)
            return new IDatabaseThread(m_pool);

        return nullptr;
    }

    //! Записывает информацию о соединении
    void setDatabase(QSqlDatabase db) {
        m_driverName   = db.driverName();
        m_databaseName = db.databaseName();
        m_hostName     = db.hostName();
        m_port         = db.port();
        m_userName     = db.userName();
        m_password     = db.password();
    }

    //! Устанавливает имя драйвера
    virtual QString driverName() const {
        return m_driverName;
    }

    //! Возвращает имя драйвера
    virtual void setDriverName(const QString &driverName) {
        m_driverName = driverName;
    }

    //! Устанавливает имя базы данных
    virtual QString databaseName() const {
        return m_databaseName;
    }

    //! Возвращает имя базы данных
    virtual void setDatabaseName(const QString &databaseName) {
        m_databaseName = databaseName;
    }

    //! Устанавливает имя хоста
    virtual QString hostName() const {
        return m_hostName;
    }

    //! Возвращает имя хоста
    virtual void setHostName(const QString &hostName) {
        m_hostName = hostName;
    }

    //! Устанавливает номер порта
    virtual int port() const {
        return m_port;
    }

    //! Возвращает номер порта
    virtual void setPort(int port) {
        m_port = port;
    }

    //! Устанавливает имя пользователя
    virtual QString userName() const {
        return m_userName;
    }

    //! Возвращает имя пользователя
    virtual void setUserName(const QString &userName) {
        m_userName = userName;
    }

    //! Устанавливает пароль
    virtual QString password() const {
        return m_password;
    }

    //! Возвращает пароль
    virtual void setPassword(const QString &password){
        m_password = password;
    }

private:
    //! Имя драйвера
    QString m_driverName;

    //! Имя базы данных
    QString m_databaseName;

    //! Имя хоста
    QString m_hostName;

    //! Порт
    int     m_port;

    //! Имя пользователя
    QString m_userName;

    //! Пароль
    QString m_password;

    IDatabasePool *m_pool;
};

#define IDatabaseModelManager_iid "com.RTPTechGroup.Raindrop.IDatabaseModelManager"
Q_DECLARE_INTERFACE(IDatabaseModelManager, IDatabaseModelManager_iid)

#endif
