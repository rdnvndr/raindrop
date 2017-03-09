#ifndef TREEFILTERPROXYMODEL_H
#define TREEFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QStringList>
#include <QSet>

namespace RTPTechGroup {
namespace ModelerIde {

//! Фильрующая древовидная прокси модель
/*! Прокси модель дерева. предназначена для сортировки и фильтрации
 */

class TreeFilterProxyModel : public QSortFilterProxyModel
{
public:
    //! Конструктор прокси модели
    TreeFilterProxyModel();

    //! Добавление тэгов для отображения
    void addVisibleTag(const QString &tag);

    //! Удаление тэгов для отображения
    void removeVisibleTag(const QString &tag);

    //! Список тэгов для отображения
    QSet<QString> visibleTags();

    //! Возращает хранимые данные
    QVariant data(const QModelIndex &index,
                  qint32 role = Qt::DisplayRole ) const;

    //! Возращает список типов, которые могут быть упакованы
    QStringList mimeTypes() const;

    //! Возращает поддерживаемые моделью операции Drop
    Qt::DropActions supportedDropActions() const;

    //! Возращает поддерживаемые моделью операции Drag
    Qt::DropActions supportedDragActions() const;

    //! Обработчик окрнчания перетаскивания данных путем Drag and Drop
    bool dropMimeData(const QMimeData *data, Qt::DropAction action,
                      qint32 row, qint32 column, const QModelIndex &parent);

    //! Установка колонок для отображения
    void setDisplayColumn(qint32 first, qint32 second = -1);

    //! Получение формата отображения экранного имени
    QString formatDisplayColumn() const;

    //! Установка формата отображения экранного имени
    void setFormatDisplayColumn(const QString &formatDisplayColumn);

protected:
    //! Фильтр строчек
    bool filterAcceptsRow(qint32 source_row,
                          const QModelIndex &source_parent) const;

    //! Фильтр строчек по дереву
    bool filterAcceptsRowItself(qint32 source_row,
                                const QModelIndex &source_parent) const;

    //! Поиск детей удолетворящих фильтру
    bool hasAcceptedChildren(qint32 source_row,
                             const QModelIndex &source_parent) const;

    //! Сортировка. Порядок: Класс, Состав, Атрибут, Алфавит
    bool lessThan(const QModelIndex &left,
                  const QModelIndex &right) const;

private:
    //! Список тэгов для фильтрации
    QSet<QString> m_filterTags;

    //! Формат отображения экранного имени
    QString m_formatDisplayColumn;

    //! Колонки для отображения
    qint32 m_displayFirstColumn;
    qint32 m_displaySecondColumn;
};

}}

#endif // TREEFILTERPROXYMODEL_H
