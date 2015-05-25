#ifndef STACKEDITING_H
#define STACKEDITING_H

#include <QObject>
#include <QAction>
#include <QUndoGroup>

#include <plugin/iplugin.h>
#include <iundogroup.h>
#include <iclipboardstack.h>
#include <iclipboarditem.h>

#include "stackeditingglobal.h"

namespace RTPTechGroup {
namespace StackEditing {

//! Плагин стека отмена/повтора команд и буфера обмена
/*! Плагин предназначен для организации стека отмена или повтора
 *  команд и работой с буфером обмена
*/

class  STACKEDITINGLIB StackEditing:
        public QObject,
        public IUndoGroup,
        public IClipboardStack,
        public IPlugin
{
    Q_OBJECT
    Q_INTERFACES(IPlugin IUndoGroup IClipboardStack)

#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "com.RTPTechGroup.Raindrop.StackEditing" FILE "stackediting.json")
#endif

public:

    //! Конструктор плагина
    explicit StackEditing(QObject *parent = 0);

    //! Деструктор плагина
    virtual ~StackEditing();

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

// IClipboardStack
    //! Добавление компонента для буфера обмена
    void addClipboardItem(IClipboardItem *item);

    //! Удаление компонента для буфера обмена
    void removeClipboardItem(IClipboardItem *item);

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

    //! Вырезание в буфер обмена
    void cut();

    //! Копирование в буфер обмена
    void copy();

    //! Вставка из буфер обмена
    void paste();

    //! Выбор всего
    void selectAll();

    //! Вырезание в буфер обмена
    void canCutChange(bool canCut);

    //! Копирование в буфер обмена
    void canCopyChange(bool canCopy);

    //! Вставка из буфер обмена
    void canPasteChange(bool canPaste);

    //! Выбор всего
    void canSelectAllChange(bool canSelectAll);

    //! Обработка изменения состояния буфера обмена
    void clipboardDataChange();

    //! Удаление элемента буфера обмена
    void removeItem(QObject *obj);

private:
    //! Установка активного стека отмены для QWidget
    void setActiveStackForWidget(QWidget *widget);

    //! Установка активного элемента стека буфера обмена для QWidget
    void setActiveItemForWidget(QWidget *widget);

    //! Команда "Отменить"
    QAction *m_actionUndo;

    //! Команда "Повторить"
    QAction *m_actionRedo;

    //! Команда "Вырезать"
    QAction *m_actionCut;

    //! Команда "Копировать"
    QAction *m_actionCopy;

    //! Команда "Вставить"
    QAction *m_actionPaste;

    //! Команда "Выделить все"
    QAction *m_actionSelectAll;


    //! Список соотвествия QWidget и QUndoStack
    QMultiMap<QUndoStack *, QWidget *> m_undoStackList;

    //! Группа стека отмены
    QUndoGroup *m_undoGroup;


    //! Список команд стека буфера обмена
    QList<QObject *> m_clipboardItemList;

    //! Текущий элемент стека буфера обмена
    IClipboardItem *m_currentClipboardItem;
};

}}

#endif

