#ifndef TREEFILTERPROXYMODEL_H
#define TREEFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QStringList>
#include <QSet>

//! Фильрующая древовидная прокси модель
/*! Прокси модель дерева. предназначена для сортировки и фильтрации
 */

class TreeFilterProxyModel : public QSortFilterProxyModel
{
public:
    //! Конструктор прокси модели
    TreeFilterProxyModel();

    //! Добавление скрытых тэгов
    void addHiddenTag(const QString &tag);

    //! Удаление скрытых тэгов
    void removeHiddenTag(const QString &tag);

    //! Список скрытых тэгов
    QSet<QString> hiddenTags();

    //! Обработчик окрнчания перетаскивания данных путем Drag and Drop
    bool dropMimeData(const QMimeData *data, Qt::DropAction action,
                      int row, int column, const QModelIndex &parent);

    //! Возращает объект с упакованными данными по списку индексов
    /*! Возращает объект с упакованными данными по списку индексов\n
        Упаковка данных производится в виде:
        \code
          ^Tag1 prop1 ... propN
          {
             ^Tag2 prop1 ... propN
             ^Tag3 prop1 ... propN
             ^...
          }
          ^...
          ^TagN
        \endcode
    */
    QMimeData *mimeData(const QModelIndexList &indexes) const;

    //! Возращает список типов, которые могут быть упакованы
    QStringList mimeTypes() const;

    //! Возращает поддерживаемые моделью операции Drop
    Qt::DropActions supportedDropActions() const;

    //! Возращает поддерживаемые моделью операции Drag
    Qt::DropActions supportedDragActions() const;

protected:
    //! Фильтр строчек
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

    //! Фильтр строчек по дереву
    bool filterAcceptsRowItself(int source_row, const QModelIndex &source_parent) const;

    //! Поиск детей удолетворящих фильтру
    bool hasAcceptedChildren(int source_row, const QModelIndex &source_parent) const;

    //! Сортировка. Порядок: Класс, Состав, Атрибут, Алфавит
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

private:
    //! Распаковка данных из потока
   bool unpackData(const QModelIndex &parent, QDataStream &stream, int row, bool move = false);

   //! Упаковка данных в поток
   void packData(QModelIndex parent, QDataStream &stream) const;

    //! Список тэгов для фильтрации
    QSet<QString> m_filterTags;
};

#endif // TREEFILTERPROXYMODEL_H
