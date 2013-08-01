#ifndef MODELERIDE_H
#define MODELERIDE_H

#include <QObject>
#include <plugin/iplugin.h>
#include <treexmlmodel/treexmlhashmodel.h>
#include "classtreeview.h"
#include "treefilterproxymodel.h"

//! Плагин среды разработки структуры классов
/*! Плагин предназначен для разработки структуры классов
    с сохранением ее XML. Имеется возможность разработанную
    структуру классов опубликовать на сервере БД
*/

class ModelerIDEPlug:
        public QObject,
        public IPlugin
{
    Q_OBJECT
    Q_INTERFACES(IPlugin)

    #if QT_VERSION >= 0x050000
        Q_PLUGIN_METADATA(IID "com.RTPTechGroup.Raindrop.ModelerIDEPlug" FILE "modelerideplug.json")
    #endif

public:

// IPlugin

    //! Получение экземпляра
    QObject *instance() { return this; }

    //! Получение имени плагина
    QString name() {return tr("Редактор модели данных");};

    //! Получение иконки плагина
    QIcon icon() {return QIcon(":/modeleride");}

    //! Описание плагина
    QString descript() {return tr("");};

    //! Категория в которой состоит плагин
    QString category() {return tr("");};

    //! Версия плагина
    QString version() {return tr("");};

    //! Производитель плагина
    QString vendor() {return tr("");};

// ModelerIDEPlug
    //! Конструктор плагина
    explicit ModelerIDEPlug(QObject *parent = 0);

    //! Деструктор плагина
    virtual ~ModelerIDEPlug();

    //! Получение Модели данных плагина
    TreeXmlHashModel* model();

    //! Создание класса
    void createClassModel(QDomDocument document);

    //! Получение имени класса по индексу
    QString className(const QModelIndex &index);

    //! Проверка на возможность удаления класса
    bool isRemoveClass(const QModelIndex &srcIndex);

    //! Получение индетификатора класса по индексу
    QString classId(const QModelIndex &index);
private:
    //! Имя файла структуры
    QString m_fileName;

    //! Модель дерева классов
    TreeXmlHashModel* m_model;

    //! Прокси модель дерева классов. Отвечает за сортировку и фильтрацию дерева классов
    TreeFilterProxyModel* classFilterModel;

    //! Дерево классов
    ClassTreeView* treeClassView;

    //! Команда создания структуры классов
    QAction* actionNewModel;

    //! Команда сохранения структуры классов
    QAction* actionSaveModel;

    //! Команда сохранения структуры классов как...
    QAction* actionSaveAsModel;

    //! Команда открытия структуры классов
    QAction* actionOpenModel;

    //! Команда публикации структуры классов в БД
    QAction* actionPublishModel;

    //! Команда закрытия структуры классов
    QAction* actionCloseModel;

    //! Команда добавления класса
    QAction* actionAddClass;

    //! Команда удаления класса
    QAction* actionRemoveClass;

    //! Команда отображения атрибутов в дереве классов
    QAction* actionShowAttr;

    //! Команда отображения состава в дереве классов
    QAction* actionShowComp;

    //! Команда отображения фильтров в дереве классов
    QAction* actionShowFilter;

    //! Команда разделитель
    QAction* actionSeparator;

    //! Контекстное меню дерева классов
    QMenu*   contextMenu;

public slots:
    //! Слот обработки двойного щелчка по дереву классов
    void dblClickTree(const QModelIndex &index);

    //! Слот вызова окна редактирования класса
    void showPropClass(const QModelIndex &indexSource);

    //! Слот вызова окна редактирования состава
    void showPropComposition(const QModelIndex &indexSource);

    //! Слот вызова окна редактирования фильтра
    void showPropFilter(const QModelIndex &indexSource);

    //! Создание новой структуры классов
    void newClassModel();

    //! Открытие структуры классов
    void openClassModel();

    //! Сохранение структуры классов
    void saveClassModel();

    //! Сохранение структуры классов как ...
    void saveAsClassModel();

    //! Публикация структуры классов на сервере БД
    void publishClassModel(const QModelIndex &index = QModelIndex());

    //! Закрытие структуры классов
    void closeClassModel();

    //! Добавление класса
    void addClass();

    //! Удаление класса
    void removeClass();

    //! Слот вызова отображения контекстного меню дерева классов
    void showContextMenu(const QPoint &point);

    //! Установка отображения атрибутов в дереве классов
    void setShowAttr(bool shown);

    //! Установка отображения состава в дереве классов
    void setShowComp(bool shown);

    //! Установка отображения фильтров в дереве классов
    void setShowFilter(bool shown);

    //! Устанавливает активность QAction "Сохранить модель"
    void actionSaveEnable();
};

#endif
