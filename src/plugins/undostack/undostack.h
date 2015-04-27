#ifndef UNDOSTACK_H
#define UNDOSTACK_H

#include <QObject>
#include <QAction>
#include <plugin/iplugin.h>
#include "undostackglobal.h"

namespace RTPTechGroup {
namespace Widgets {

//! Плагин стека отмена/повтора команд
/*! Плагин предназначен для организации стека отмена или повтора
 *  команд
*/

class  UNDOSTACKLIB UndoStack:
        public QObject,
        public IPlugin
{
    Q_OBJECT
    Q_INTERFACES(IPlugin)

#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "com.RTPTechGroup.Raindrop.UndoStack" FILE "undostack.json")
#endif

public:

    //! Конструктор плагина
    explicit UndoStack(QObject *parent = 0);

// IPlugin

    //! Получение имени плагина
    QString name() {return APP_NAME;};

    //! Получение иконки плагина
    QIcon icon() {return QIcon(":/undostack");}

    //! Описание продукта
    QString product() {return APP_PRODUCT;};

    //! Авторские права
    QString copyright() {return APP_COPYRIGHT;};

    //! Описание плагина
    QString descript() {return tr(APP_DESCRIPTION);};

    //! Категория в которой состоит плагин
    QString category() {return tr("");};

    //! Версия плагина
    QString version() {return APP_VERSION;};

    //! Производитель плагина
    QString vendor() {return tr(APP_COMPANY);};

private:
    //! Команда "Отменить"
    QAction *actionUndo;

    //! Команда "Повторить"
    QAction *actionRedo;
};

}}

#endif

