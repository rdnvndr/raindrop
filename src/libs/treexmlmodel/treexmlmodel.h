#ifndef TREEXMLMODEL_H
#define TREEXMLMODEL_H

#include <QAbstractItemModel>
#include <QtXml/QDomDocument>
#include <QModelIndex>
#include <QVariant>
#include "treexmlmodelglobal.h"
#include <QtCore>
#include <QtGui>

class XMLMODELLIB TagXMLItem;

//! Класс модель дерева для XML данных
/*! Данный класс предназначен для создания дерева на основе,
    XML данных. Позволяет отображать указанные наследуемые тэги. Узлы дерева
    (записи таблицы) связаны между указателем класса TagXMLItem. \n
    Пример использования:
    \code

    QString filePath = QFileDialog::getOpenFileName(NULL, tr("Открытие"),"", tr("Фаил модели (*.xml)"));
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly)) {
        QDomDocument document;
        if (document.setContent(&file)) {
            dbStructModel = new TreeXMLModel(document, this);

            dbStructModel->addTagFilter("Class");
            dbStructModel->addTagFilter("Attribute");

            QStringList propsClass;
            propsClass << "ClassName" << "parent";
            dbStructModel->addDisplayedAttr("Class",propsClass);

            QStringList propsAttr;
            propsAttr << "AttributeName" << "parent";
            dbStructModel->addDisplayedAttr("Attribute",propsAttr);

            dbStructModel->addAttributeTag("Attribute");

            dbStructModel->setHeaderData(0, Qt::Horizontal, tr("Имя атирибута"));
            dbStructModel->setHeaderData(1, Qt::Horizontal, tr("Родитель"));

            QTreeView* treeView = new QTreeView( this );
            treeView->setModel(dbStructModel);
        }
        file.close();
    }
    \endcode

*/
class XMLMODELLIB TreeXMLModel : public QAbstractItemModel
{
    Q_OBJECT

public:
     //! Конструктор модели
    TreeXMLModel(QDomNode document, QObject *parent = 0);
    //! Деструктор модели
    virtual ~TreeXMLModel();

    //! Возращает хранимые данные
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole ) const;

    //! Устанавливает значение для указанной записи
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    //! Возвращает флаг записи
    Qt::ItemFlags flags(const QModelIndex &index) const;

    //! Возращает название заголовка
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

    //! Устанавливает название заголовка
    bool setHeaderData(int section, Qt::Orientation orientation,
                       const QVariant &value, int role = Qt::EditRole);

    //! Возращает индекс модели для строки и колонки
    QModelIndex index(int row, int m_column,
                      const QModelIndex &parent = QModelIndex()) const;

    //! Возращает индекс родителя
    QModelIndex parent(const QModelIndex &child) const;

    //! Возращает количество строк в индексе родителя
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    //! Возращает количество столбцов в индексе родителя
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    //! Добавляет тэг для фильтрации
    void addTagFilter(QString tag);

    //! Очищает фильтр тэгов
    void clearTagsFilter();

    //! Добавляет тэг, который будет являться атрибутом
    void addAttributeTag(QString tag);

    //! Очищает список тэгов, которые являются атрибутами
    void clearAttributeTags();

    //! Возращает True если строка является атрибутом
    bool isAttribute(const QModelIndex &index) const;

    //! Получает класс связку TagXMLItem по индексу модели
    TagXMLItem *getItem(const QModelIndex &index) const;

    //! Указывает xml атрибуты тэга для отображения
    void addDisplayedAttr(QString nameAttr, QStringList value,QIcon icon = QIcon());

    //! Указывает тэги разрешенные для вставки
    void addInsertTags(QString tag,QStringList value);

    //! Получение индекса поля в тэге
    int indexDisplayedAttr(QString nameAttr, QString fieldName);

    //! Удаляет xml атрибуты тэга для отображения
    void removeDisplayedAttr(QString nameAttr);

    //! Получение корневой узла дерева
    TagXMLItem* rootItem();

    //! Установить наименование тэга для вставки строки
    void setInsTagName(QString tagName);

    //! Вставка строки
    bool insertRows (int row, int count, const QModelIndex & parent);

    //! Удаление строки
    bool removeRows (int row, int count, const QModelIndex & parent);

    //! Получение индекса последней вставленной строки
    QModelIndex lastInsertRow();

    //! Обработчик окрнчания перетаскивания данных путем Drag and Drop
    bool dropMimeData(const QMimeData *data, Qt::DropAction action,
                      int row, int column, const QModelIndex &parent);

    //! Возращает объект с упакованными данными по списку индексов
    QMimeData *mimeData(const QModelIndexList &indexes) const;

    //! Возращает список типов, которые могут быть упакованы
    QStringList mimeTypes() const;

    //! Возращает поддерживаемые моделью операции Drop
    Qt::DropActions supportedDropActions() const;

    //! Возращает поддерживаемые моделью операции Drag
    Qt::DropActions supportedDragActions() const;

    //! Возращает True если можно вставить строку
    bool isInsert(const QModelIndex &index) const;

    //! Возращает True если имеются потомки
    virtual bool hasChildren(const QModelIndex &parent) const;

private:
     //! Обновление отредактированных унаследованных строк
    void updateModifyRow(int emptyRowAttr, const QModelIndex &parent);

    //! Обновление вставленных унаследованных строк
    void updateInsertRows(int row, int count, const QModelIndex &parent);

    //! Обновление удаленных унаследованных строк
    void updateRemoveRows(int emptyRowAttr, int count, const QModelIndex &parent);

    //! Корневой узел
    TagXMLItem *m_rootItem;

    //! Количество колонок
    int m_column;

    //! Список тэгов для фильтрации
    QStringList m_filterTags;

    //! Список тэгов которые являются атрибутами
    QStringList m_attrTags;

    //! Наименование тэга для вставки строки
    QString m_insTag;

    //! Список атрибутов тэга для отображения
    QMap<QString, QStringList> m_displayedAttr;

    //! Список иконок тэгов для отображения
    QMap<QString, QIcon> m_displayedIcon;

    //! Список названий заголовков
    QMap<int, QString> m_header;

    //! Индекс последней вставленной строки
    QModelIndex m_lastInsRow;

    //! Список тэгов в которые нельзя вставлять строки
    QMap<QString, QStringList> m_insertTags;
};

#endif
