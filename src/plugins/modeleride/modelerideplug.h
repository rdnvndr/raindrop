#ifndef MODELERIDE_H
#define MODELERIDE_H

#include <QObject>
#include <plugin/iplugin.h>
#include <treexmlmodel/treexmlhashmodel.h>
#include <dockwidget/dockwidget.h>
#include "classtreeview.h"
#include "treefilterproxymodel.h"
#include "classmodel.h"

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
    QString name() {return tr(APP_NAME);};

    //! Получение иконки плагина
    QIcon icon() {return QIcon(":/modeleride");}

    //! Описание продукта
    QString product() {return tr(APP_PRODUCT);};

    //! Авторские права
    QString copyright() {return tr(APP_COPYRIGHT);};

    //! Описание плагина
    QString descript() {return tr(APP_DESCRIPTION);};

    //! Категория в которой состоит плагин
    QString category() {return tr("");};

    //! Версия плагина
    QString version() {return APP_VERSION;};

    //! Производитель плагина
    QString vendor() {return tr(APP_COMPANY);};

// ModelerIDEPlug
    //! Конструктор плагина
    explicit ModelerIDEPlug(QObject *parent = 0);

    //! Деструктор плагина
    virtual ~ModelerIDEPlug();

    //! Получение Модели данных плагина
    TreeXmlHashModel* model();

    //! Создание класса
    void createClassModel(QDomDocument document);

    //! Получение имени по индексу
    QString dataName(const QModelIndex &index);

    //! Проверка на возможность удаления элементов
    bool isRemove(const QModelIndex &srcIndex);

    //! Получение индетификатора по индексу
    QString dataId(const QModelIndex &index);

private:
    //! Имя файла структуры
    QString m_fileName;

    //! Модель дерева классов
    ClassModel* m_model;

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

    //! Стыковый компонент для модели метаданных
    DockWidget *dockWidget;

public slots:
    //! Слот обработки двойного щелчка по дереву классов
    void dblClickTree(const QModelIndex &index);


    //! Слот вызова окна редактирования класса
    void showPropClass(const QModelIndex &indexSource);

    //! Слот вызова окна редактирования фильтра
    void showPropFilter(const QModelIndex &indexSource);

    //! Слот вызова окна редактирования сущности единицы измерения
    void showPropEntity(const QModelIndex &indexSource);

    //! Слот вызова окна редактирования группы сущности единицы измерения
    void showPropEntityGroup(const QModelIndex &indexSource);

    //! Слот вызова окна редактирования списка значений
    void showPropLov(const QModelIndex &indexSource);

    //! Слот вызова окна редактирования группы справочника
    void showPropRefGroup(const QModelIndex &indexSource);

    //! Слот вызова окна редактирования справочника
    void showPropRef(const QModelIndex &indexSource);


    //! Закрытие окна по индексу
    void closePropWindow(const QModelIndex& index);


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

    //! Добавление элемента модели класса
    void add();

    //! Удаление элемента модели класса
    void remove();

    //! Устанавливает активность QAction "Сохранить модель"
    void actionSaveEnable();

};

#endif
