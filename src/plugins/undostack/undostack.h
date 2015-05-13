#ifndef UNDOSTACK_H
#define UNDOSTACK_H

#include <QObject>
#include <QAction>
#include <QUndoGroup>
#include <plugin/iplugin.h>
#include <iundogroup.h>
#include "undostackglobal.h"

namespace RTPTechGroup {
namespace UndoStack {

//! Плагин стека отмена/повтора команд
/*! Плагин предназначен для организации стека отмена или повтора
 *  команд
*/

class  UNDOSTACKLIB UndoStack:
        public IUndoGroup,
        public IPlugin
{
    Q_OBJECT
    Q_INTERFACES(IPlugin IUndoGroup)

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

// IUndoGroup
    //! Удаление стека отмены из группы
    void removeStack(QUndoStack * stack);

public slots:

    //! Добавление QWidget для QUndoStack
    void addWidgetForStack(QUndoStack *stack, QWidget *widget);

    //! Удаление QWidget для QUndoStack
    void removeWidgetForStack(QWidget *widget);

private slots:
    //! Обработка смены фокуса визуального элемента
    void focusChanged(QWidget *old, QWidget *now);

private:
    //! Команда "Отменить"
    QAction *actionUndo;

    //! Команда "Повторить"
    QAction *actionRedo;

    //! Список соотвествия QWidget и QUndoStack
    QHash<QWidget *, QUndoStack *> m_undoStackList;
};

}}

#endif

