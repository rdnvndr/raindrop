#ifndef DIALOGCONNECT_H
#define	DIALOGCONNECT_H
#include "ui_dialogconnect.h"
#include <QtSql>

//! Диалоговое окно подключения к БД
class DialogConnect: public QDialog, public Ui::DialogConnect{

		Q_OBJECT
	public:
        //! Коструктор диалогового окна подключения к БД
        DialogConnect(QWidget* pwgt = 0);
	public slots:
        //! Слот для отображения дополнительных параметров подключения к БД
        void onClickButtonProp();
};

#endif
