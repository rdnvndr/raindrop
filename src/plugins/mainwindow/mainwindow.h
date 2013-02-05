#ifndef MAINWINDOW_H
#define	MAINWINDOW_H

#include "mainwindowglobal.h"
#include "ui_mainwindow.h"
#include <imainwindow.h>
#include <plugin/iplugin.h>
#include <mdiextarea/mdiextarea.h>
#include <QMainWindow>
#include <QMenu>

//! Плагин главного окна
/*! Плагин предназначен для создания главного окна
    Весь графический интерфейс приложения должен строится
    на базе данного окна. Поддерживает закладочный и оконный
    интерфейс.\n
    Пример:
    \code
        PluginManager* pluginManager = PluginManager::instance();
        IMainWindow* mainWindow = qobject_cast<IMainWindow*>(
                  pluginManager->getPlugin("IMainWindow")->instance());
        mainwindow->addSubWindow(new QTextEdit(NULL));
    \endcode
*/

class MAINWINDOWLIB MainWindow:
        public QMainWindow,
        public Ui::MainWindow,
        public IMainWindow,
        public IPlugin

{
    Q_OBJECT
    Q_INTERFACES(IPlugin IMainWindow)

#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "com.RTPTechGroup.Raindrop.MainWindow" FILE "mainwindow.json")
#endif

public:
    struct MenuItem {
        QString name;              //!< Имя объекта
        QString text;              //!< Название
        QString type;              //!< Тип
        MenuItem *parentItem;          //!< Родитель
        QList<MenuItem *> childIItems; //!< Потомки
        QAction *action;           //!< Созданный объект
    };

    //! Конструктор плагина главного окна
    explicit MainWindow(QMainWindow* pwgt = 0);

    //! Деструктор плагина главного окна
    virtual ~MainWindow();

    //! Чтение и применение настроек плагина главного окна
    void readSettings();

    //! Запись настроек плагина главного окна
    void writeSettings();

    //! Добавления QAction для использования в главном окне
    void addAction(QString category, QAction *action);

// IPlugin

    //! Получение экземпляра
    QObject *instance() { return this; }

    //! Получение имени плагина
    QString name() {return tr("Главное окно");};

    //! Получение иконки плагина
    QIcon icon() {return QIcon();}

    //! Описание плагина
    QString descript() {return tr("");};

    //! Категория в которой состоит плагин
    QString category() {return tr("");};

    //! Версия плагина
    QString version() {return tr("");};

    //! Производитель плагина
    QString vendor() {return tr("");};

// IMainWindow

    //! Получение подокна
    /*! Предназначено для получения подокна
    */
    QMdiSubWindow *subWindow(QString objName);

    //! Получение списка подокон
    /*! Предназначено для получения списка подокон
    */
    QList<QMdiSubWindow *> subWindowList() const;

public slots:

    //! Слот обработки события закрытия главного окна
    void closeEvent(QCloseEvent *event);

    //! Слот обновления меню управления окнами
    /*! Слот предназаначен для изменения состояния
        пунктов меню при изменении состояния приложения
    */
    void updateMenus();

    //! Слот установки оконного режима
    /*! Слот предназначен переключение приложения в закладочный
        или оконный режим
    */
    void setWindowModeEnable(bool mode);

    //! Слот добавления подчиненного окна
    /*! Слот предназаначен для добавления подчиненного окна(закладки)
        в главное окно. Возращает добавленное подокно QMdiSubWindow
    */
    QMdiSubWindow* addSubWindow(QWidget* widget);

    //! Слот установки активного окна по имени
    /*! Слот предназначен для установки активного подокна в главном окне
        по его имени
     */
    QMdiSubWindow *setActiveSubWindow(QString objName);

    //! Получение меню File
    QMenu* getMenuFile();

    //! Получение меню Edit
    QMenu* getMenuEdit();

    //! Получение меню Settings
    QMenu* getMenuSettings();

    //! Получение меню Help
    QMenu* getMenuHelp();

    //! Получение панели инструментов
    QToolBar*   getToolBarMain();

    //! Получение области подокон
    MdiExtArea* getMdiArea();

private:
    //! Запись настроек меню
    void writeMenuSettings(QWidget *menu, int level = 0);

    //! Чтение настроек меню
    void readMenuSettings();

    //! Индекс для записи настроек меню
    int menuArrayIndex;

    //! Список команд по группам
    QMultiHash <QString, QAction *> m_actions;

    //! Список команд в структуре
    QHash <QString, MenuItem *> m_actionItem;

    MenuItem *m_item;
};

#endif
