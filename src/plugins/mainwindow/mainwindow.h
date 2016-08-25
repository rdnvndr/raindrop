#ifndef MAINWINDOW_H
#define	MAINWINDOW_H

#include "mainwindowglobal.h"
#include "ui_mainwindow.h"
#include "mainwindowoptions.h"
#include <imainwindow.h>
#include <plugin/iplugin.h>
#include <mdiextarea/mdiextarea.h>
#include <QMainWindow>
#include <QMenu>

namespace RTPTechGroup {
namespace MainWindow {

//! Плагин главного окна
/*! Плагин предназначен для создания главного окна
    Весь графический интерфейс приложения должен строится
    на базе данного окна. Поддерживает закладочный и оконный
    интерфейс.\n
    Пример:
    \code
        PluginManager *pluginManager = PluginManager::instance();
        IMainWindow *iMainWindow = qobject_cast<IMainWindow*>(
                pluginManager->interfaceObject("IMainWindow"));
        iMainWindow->addSubWindow(new QTextEdit(NULL));
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
        QString name;                 //!< Имя объекта
        QString text;                 //!< Название
        QString type;                 //!< Тип
        QByteArray icon;              //!< Иконка
        MenuItem *parentItem;         //!< Родитель
        QList<MenuItem *> childItems; //!< Потомки
        QAction *action;              //!< Созданный объект
    };
    typedef QMultiHash <QString, MenuItem *> MenuItemHash;

    //! Конструктор плагина главного окна
    explicit MainWindow(QMainWindow *pwgt = 0);

    //! Деструктор плагина главного окна
    virtual ~MainWindow();

// IPlugin

    //! Получение имени плагина
    QString name() {return APP_NAME;};

    //! Получение иконки плагина
    QIcon icon() {return QIcon();}

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

// IMainWindow

    //! Получение подокна
    /*! Предназначено для получения подокна
    */
    QMdiSubWindow *subWindow(QString objName);

    //! Получение списка подокон
    /*! Предназначено для получения списка подокон
    */
    QList<QMdiSubWindow *> subWindowList() const;

    //! Добавление стыковго компонента
    /*! Предназначено для добавления стыкового компонента
    */
    void addDockWidget(Qt::DockWidgetArea area, QDockWidget *dockwidget);

    //! Возращает всплывающее
    /*! Предназначено для отображения всплывающего меню с списком панелей
        инструментов и главного меню которые можно выбрать для отображения при
        помощи переключателей.
    */
    QMenu *createPopupMenu();

public slots:

    //! Добавления QAction для использования в главном окне
    void addAction(QString category, QAction *action);

    //! Удаление QAction из главного окна
    void removeAction(QAction *action);

    //! Запись настроек меню и панелей инструментов
    void writeBarSettings();

    //! Чтение настроек меню и панелей инструментов
    void readBarSettings();

    //! Чтение и применение настроек плагина главного окна
    void readSettings();

    //! Запись настроек плагина главного окна
    void writeSettings();

    //! Слот обработки события закрытия главного окна
    void closeEvent(QCloseEvent *event);

    //! Слот обновления меню управления окнами
    /*! Слот предназаначен для изменения состояния
        пунктов меню при изменении состояния приложения
    */
    void updateMenus();

    //! Обновляет главное меню окна и панели инструментов по структуре
    void refreshAllBar(bool readingBarSettings = true);

    //! Слот установки оконного режима
    /*! Слот предназначен переключение приложения в закладочный
        или оконный режим
    */
    void setWindowModeEnable(bool mode);

    //! Слот добавления подчиненного окна
    /*! Слот предназаначен для добавления подчиненного окна(закладки)
        в главное окно. Возращает добавленное подокно QMdiSubWindow
    */
    QMdiSubWindow *addSubWindow(QWidget *widget);

    //! Слот установки активного окна по имени
    /*! Слот предназначен для установки активного подокна в главном окне
        по его имени
     */
    QMdiSubWindow *setActiveSubWindow(QString objName);

    //! Получение области подокон
   RTPTechGroup::Widgets::MdiExtArea *getMdiArea();

    //! Вызов окна "О Qt..."
    void aboutQt();

    //! Вызов окна "О программе..."
    void about();

    //! Перейти в режим "Что это?"
    void showWhatsThis();

    //! Вызов настройки главного окна
    void showOptionsDialog();

private slots:

    //! Удаление QAction из главного окна
    void removeAction(QObject *obj);

    //! Вызов сохранение настроек меню
    void saveOptionsDialog();

    //! Вызов отмены сохранения настроек меню
    void cancelOptionsDialog();

    //! Изменение состояние окна настройки меню
    void optionsDialogStateChanged(Qt::WindowStates oldState,
                                   Qt::WindowStates newState);

    //! Выполнение действий после загрузки плагина
    void endLoadingPlugins();

private:

    //! Создание пунктов меню
    QAction *createBranchAction(MenuItem *menuItem);

    //! Удаление указанного пункта меню
    void deleteBranchAction(MenuItem *menuItem);

    //! Удаление ветки меню начиная с указанного узла
    void removeBranchAction(MenuItem *menuItem);

    //! Очистка меню начиная с указанного узла
    void cleanBranchAction(MenuItem *menuItem);

    //! Запись настроек меню определенного уровня
    void writeMenu(QWidget *menu, qint32 level = 0);

    //! Установка режима редактирования меню
    void setEditedMenu(QWidget *widget, bool edited);

    //! Установка режима редактирования для всех меню
    void setEditedAllMenu(bool edited);

    //! Команда закрытия окна
    QAction *m_actionWindowClose;

    //! Команда закрытия всех окон
    QAction *m_actionWindowCloseAll;

    //! Команда расположения окон каскадом
    QAction *m_actionWindowCascade;

    //! Команда расположения окон плиткой
    QAction *m_actionWindowTile;

    //! Команда выбора следующего окна
    QAction *m_actionWindowNext;

    //! Команда выбора предыдущего окна
    QAction *m_actionWindowPrev;

    //! Команда выбора типа отображения окон
    QAction *m_actionWindowGui;

    //! Команда вызова настроек меню и панелей инструментов
    QAction *m_actionGuiOptions;

    //! Команда выхода
    QAction *m_actionExit;

    //! Команда вызова окна "О Qt..."
    QAction *m_actionAboutQt;

    //! Команда вызова окна "О программе..."
    QAction *m_actionAbout;

    //! Команда вызова "Что это?"
    QAction *m_actionWhatsThis;

    //! Индекс для записи настроек меню
    qint32 m_menuArrayIndex;

    //! Список команд по группам
    QMultiHash <QString, QAction *> m_actions;

    //! Список команд в структуре
    QList <MenuItemHash> m_actionItem;

    //! Ссылка на структуру меню
    QList <MenuItem *> m_item;

    //! Окно настройки главного меню окна
    MainWindowOptions *m_optionsDialog;

    //! Cписок горячих клавиш команд
    QHash <QString, QKeySequence> m_hotkey;
};

}
}

#endif
