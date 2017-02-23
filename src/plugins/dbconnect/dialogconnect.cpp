#include "dialogconnect.h"

#include <QSqlDatabase>
#include <QMessageBox>
#include <QCloseEvent>

namespace RTPTechGroup {
namespace DbConnect {

DialogConnect::DialogConnect(QWidget *pwgt) : QDialog(pwgt) {
    setupUi(this);

    textLabelHostname->hide();
    editHostname->hide();
    textLabelBD->hide();
    editDatabase->hide();
    textLabelPort->hide();
    portSpinBox->hide();
    textLabelDrv->hide();
    comboDriver->hide();
    line->hide();

    QStringList drivers = QSqlDatabase::drivers();

    // remove compat names
    drivers.removeAll("QMYSQL3");
    drivers.removeAll("QOCI8");
    drivers.removeAll("QPSQL7");
    drivers.removeAll("QTDS7");
    drivers.removeAll("QSQLITE");
    //drivers.removeAll("QOCI");
    //drivers.removeAll("QTDS");
    if (drivers.indexOf("QODBC")!=-1)
        drivers.insert(0,"MSSQL");
    drivers.removeAll("QODBC3");
    //drivers.removeAll("QODBC");
    //drivers.removeAll("QPSQL");
    //drivers.removeAll("QIBASE");

    comboDriver->addItems(drivers);

    this->adjustSize();
    connect(pushButtonProp, &QPushButton::clicked,
            this, &DialogConnect::onClickButtonProp);

    threadConnect = new ThreadConnect(this);
    connect(threadConnect, &ThreadConnect::finishConnect,
            this, &DialogConnect::finishConnect);
}

DialogConnect::~DialogConnect()
{
    delete threadConnect;
}

void DialogConnect::accept()
{
    startConnect();
}

void DialogConnect::setDriver(const QString &name)
{
    comboDriver->setCurrentIndex(comboDriver->findText(name));
}

void DialogConnect::setDatabaseName(const QString &name)
{
    editDatabase->setText(name);
}

void DialogConnect::setHostName(const QString &host)
{
    editHostname->setText(host);
}

void DialogConnect::setUserName(const QString &name)
{
    editUsername->setText(name);
}

void DialogConnect::setPassword(const QString &password)
{
    editPassword->setText(password);
}

void DialogConnect::setPort(int port)
{
    portSpinBox->setValue(port);
}

QString DialogConnect::driver() const
{
    return comboDriver->currentText();
}

QString DialogConnect::databaseName() const
{
    return editDatabase->text();
}

QString DialogConnect::hostName() const
{
    return editHostname->text();
}

QString DialogConnect::userName() const
{
    return editUsername->text();
}

QString DialogConnect::password() const
{
    return editPassword->text();
}

int DialogConnect::port() const
{
    return portSpinBox->value();
}

void DialogConnect::closeEvent(QCloseEvent *event)
{
    if (!comboDriver->isEnabled())
        event->ignore();
}

void DialogConnect::setLockDialog(bool locked)
{
    comboDriver->setDisabled(locked);
    editDatabase->setDisabled(locked);
    editHostname->setDisabled(locked);
    editUsername->setDisabled(locked);
    editPassword->setDisabled(locked);
    portSpinBox->setDisabled(locked);
    pushButtonOk->setDisabled(locked);
    pushButtonCancel->setDisabled(locked);

}

void DialogConnect::startConnect()
{
    setLockDialog(true);
    if (QSqlDatabase::database().isOpen()) {
        QSqlDatabase::database().close();
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    }

    {
        QSqlDatabase db;

        if (driver() == "MSSQL"){
            db = QSqlDatabase::addDatabase("QODBC");

            db.setDatabaseName(
                        QString("%1;%2;%3").arg("DRIVER={SQL Server}")
                        .arg("DATABASE=" + databaseName())
                        .arg("SERVER=" + hostName()));
            db.setPort(port());
            db.setUserName(userName());
            db.setPassword(password());
        } else if (driver() == "QOCI") {
            db = QSqlDatabase::addDatabase("QOCI");

            db.setDatabaseName(
                        QString("(DESCRIPTION = "
                                "(ADDRESS_LIST = "
                                "(ADDRESS = "
                                "(PROTOCOL = TCP)"
                                "(HOST = %1)"
                                "(PORT = %2)"
                                ")"
                                ")"
                                "(CONNECT_DATA ="
                                "(SERVER = DEDICATED)"
                                "(SID =%3)"
                                ")"
                                ")")
                        .arg(hostName())
                        .arg(port())
                        .arg(databaseName()));
        } else {
            db = QSqlDatabase::addDatabase(driver());
            db.setDatabaseName(databaseName());
            db.setHostName(hostName());
        }

        db.setPort(port());
        db.setUserName(userName());
        db.setPassword(password());

        threadConnect->start();
    }
}

void DialogConnect::finishConnect(QString result)
{
    setLockDialog(false);

    QSqlDatabase db = QSqlDatabase::database();
    if (!result.isEmpty()) {
        QMessageBox::warning(
                    NULL,
                    tr("Не удается открыть базу данных"),
                    tr("Произошла ошибка при создании соединения:\n")
                    + result);
    } else
        QDialog::accept();
}

void DialogConnect::onClickButtonProp(){
    if (textLabelHostname->isHidden()){
        pushButtonProp->setText(tr("Кратко"));
        textLabelHostname->show();
        editHostname->show();
        textLabelBD->show();
        editDatabase->show();
        textLabelPort->show();
        portSpinBox->show();
        textLabelDrv->show();
        comboDriver->show();
        line->show();
    } else {
        pushButtonProp->setText(tr("Подробно"));
        textLabelHostname->hide();
        editHostname->hide();
        textLabelBD->hide();
        editDatabase->hide();
        textLabelPort->hide();
        portSpinBox->hide();
        textLabelDrv->hide();
        comboDriver->hide();
        line->hide();
    }
    this->adjustSize();
}

}}
