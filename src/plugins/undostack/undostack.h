#ifndef UNDOSTACK_H
#define UNDOSTACK_H

#include <QObject>
#include <QUndoGroup>
#include <QAction>

#include <plugin/iplugin.h>
#include <iundogroup.h>

#include "undostackglobal.h"

namespace RTPTechGroup {
namespace StackEditing {

//! Плагин стека отмена/повтора команд
/*! Плагин предназначен для организации стека отмена или повтора команд
*/

class  UNDOSTACKLIB UndoStack:
        public QObject,
        public IUndoGroup,
        public IPlugin
{
    Q_OBJECT
    Q_INTERFACES(IPlugin IUndoGroup)

#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID IUndoGroup_iid FILE "undostack.json")
#endif

public:

    //! Конструктор плагина
    explicit UndoStack(QObject *parent = 0);

    //! Деструктор плагина
    virtual ~UndoStack();

// IPlugin
    //! Получение имени плагина
    QString name() {return APP_NAME;}

    //! Получение иконки плагина
    QIcon icon() {return QIcon(":/undostack");}

    //! Описание продукта
    QString product() {return APP_PRODUCT;}

    //! Авторские права
    QString copyright() {return APP_COPYRIGHT;}

    //! Описание плагина
    QString descript() {return tr(APP_DESCRIPTION);}

    //! Категория в которой состоит плагин
    QString category() {return tr("Stack Editing");}

    //! Версия плагина
    QString version() {return APP_VERSION;}

    //! Производитель плагина
    QString vendor() {return tr(APP_COMPANY);}

// IUndoGroup
    //! Добавление стека отмены
    virtual void addStack(QUndoStack *stack);

    //! Удаление стека отмены
    virtual void removeStack(QUndoStack *stack);

    //! Добавление QWidget для QUndoStack
    virtual void addWidgetForStack(QUndoStack *stack, QWidget *widget);

    //! Удаление QWidget для QUndoStack
    virtual void removeWidgetForStack(QWidget *widget);

private slots:
    //! Обработка смены фокуса визуального элемента
    void focusChanged(QWidget *old, QWidget *now);

    //! Удаление стека отмены
    void removeStack(QObject *obj);

private:
    //! Установка активного стека отмены для QWidget
    void setActiveStackForWidget(QWidget *widget);

    //! Команда "Отменить"
    QAction *m_actionUndo;

    //! Команда "Повторить"
    QAction *m_actionRedo;

    //! Список соотвествия QWidget и QUndoStack
    QMultiMap<QUndoStack *, QWidget *> m_undoStackList;

    //! Группа стека отмены
    QUndoGroup *m_undoGroup;

};

}}

#endif
