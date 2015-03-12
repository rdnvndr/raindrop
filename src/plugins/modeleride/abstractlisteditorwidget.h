#ifndef ABSTRACTLISTEDITORWIDGET_H
#define ABSTRACTLISTEDITORWIDGET_H

#include <QWidget>
#include <QAbstractProxyModel>
#include <QTableView>

#include <treexmlmodel/treexmlhashmodel.h>
#include <treexmlmodel/modifyproxymodel.h>

using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {


class AbstractListEditorWidget : public QWidget
{
    Q_OBJECT
public:
    //! Конструктор класса
    explicit AbstractListEditorWidget(QWidget *parent = 0);

    //! Деструктор класса
    ~AbstractListEditorWidget();

    //! Установка модели для редактирования
    void setModel(TreeXmlHashModel *model);

    //! Возращает прокси модели
    QAbstractProxyModel* proxyModel();

    //! Проверка на возможность удаления
    static bool isRemove(const QModelIndex &proxyIndex);

    //! Установка таблицы
    virtual void setTableView(QTableView *tableView);

    //! Возращает таблицу
    virtual QTableView *tableView();

signals:
    //! Сигнал об изменении корневого индекса в прокси
    void proxyIndexChanged(const QModelIndex &index);

public slots:
    //! Добавление значений списка
    bool add(const QString &tag);

    //! Удаление значений списка
    void remove();

    //! Применение изменений значений списка
    void submit();

    //! Перевод в режим редактирования
    void edit(bool flag = true);

    //! Отмена изменений значений списка
    void revert();

    //! Установка родителя значений списка
    void setRootIndex(const QModelIndex &index);

private:

    //! Прокси модель для значений списка
    ModifyProxyModel* m_proxyModel;

    //! Модель структуры классов
    TreeXmlHashModel* m_model;

    //! Таблица данных
    QTableView *m_tableView;
};

}}

#endif // ABSTRACTLISTEDITORWIDGET_H
