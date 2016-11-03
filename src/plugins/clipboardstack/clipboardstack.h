#ifndef CLIPBOARDSTACK_H
#define CLIPBOARDSTACK_H

#include <QObject>
#include <QAction>

#include <plugin/iplugin.h>
#include <iclipboardstack.h>
#include <iclipboarditem.h>

#include "clipboardstackglobal.h"

namespace RTPTechGroup {
namespace StackEditing {

//! Плагин стека буфера обмена
/*! Плагин предназначен для организации стека буфера обмена
*/

class  CLIPBOARDSTACKLIB ClipboardStack:
        public QObject,
        public IClipboardStack,
        public IPlugin
{
    Q_OBJECT
    Q_INTERFACES(IPlugin IClipboardStack)

#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID IClipboardStack_iid FILE "clipboardstack.json")
#endif

public:

    //! Конструктор плагина
    explicit ClipboardStack(QObject *parent = 0);

    //! Деструктор плагина
    virtual ~ClipboardStack();

// IPlugin
    //! Получение имени плагина
    QString name() {return APP_NAME;}

    //! Получение иконки плагина
    QIcon icon() {return QIcon(":/clipboardstack");}

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

// IClipboardStack
    //! Добавление компонента для буфера обмена
    void addClipboardItem(IClipboardItem *item);

    //! Удаление компонента для буфера обмена
    void removeClipboardItem(IClipboardItem *item);

private slots:
    //! Обработка смены фокуса визуального элемента
    void focusChanged(QWidget *old, QWidget *now);

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
    //! Установка активного элемента стека буфера обмена для QWidget
    void setActiveItemForWidget(QWidget *widget);

    //! Команда "Вырезать"
    QAction *m_actionCut;

    //! Команда "Копировать"
    QAction *m_actionCopy;

    //! Команда "Вставить"
    QAction *m_actionPaste;

    //! Команда "Выделить все"
    QAction *m_actionSelectAll;

    //! Список команд стека буфера обмена
    QList<QObject *> m_clipboardItemList;

    //! Текущий элемент стека буфера обмена
    IClipboardItem *m_currentClipboardItem;
};

}}

#endif
