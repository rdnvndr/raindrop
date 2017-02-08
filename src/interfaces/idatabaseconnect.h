#ifndef IDATABASECONNECT_H
#define IDATABASECONNECT_H

//! Соединение с базой данных
class IDatabaseConnect
{
public:
    explicit IDatabaseConnect() {}
    virtual ~IDatabaseConnect() {}
};

#define IDatabaseConnect_iid "com.RTPTechGroup.Raindrop.IDatabaseConnect"
Q_DECLARE_INTERFACE(IDatabaseConnect, IDatabaseConnect_iid)

#endif // IDATABASECONNECT_H
