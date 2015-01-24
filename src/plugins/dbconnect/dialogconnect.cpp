#include "dialogconnect.h"

DialogConnect::DialogConnect(QWidget* pwgt) : QDialog(pwgt) {
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
    connect(pushButtonProp, SIGNAL(clicked()), this, SLOT(onClickButtonProp()));
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
