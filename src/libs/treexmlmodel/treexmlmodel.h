#ifndef TREEXMLMODEL_H
#define TREEXMLMODEL_H

#include <QAbstractItemModel>
#include <QtXml/QDomDocument>
#include <QModelIndex>
#include <QVariant>
#include <QStringList>
#include <QIcon>
#include "treexmlmodelglobal.h"

class TagXMLItem;

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

    //! Перечисления типов уникальности поля
    enum UniqueAttr {
        NoUnique = 0,         //!< Поле не уникальное
        UniqueRename = 1,     //!< Контроль уникальности путем переименования
        Unique = 2,           //!< Поле уникальное
        Uuid = 3              //!< Поле UUID
    };
    //! Структура для хранения тэга и аттрибута вместе
    struct TagWithAttr {
        QString tag;    //!< Поле с тэгом
        QString attr;   //!< Поле с атрибутом
    };

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

    //! Возращает количество строк указанных тэгов в индексе родителя
    int rowCount(const QModelIndex &parent, const QStringList &tags) const;

    //! Возращает количество столбцов в индексе родителя
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    //! Добавляет тэг для фильтрации
    void addTagFilter(const QString &tag);

    //! Очищает фильтр тэгов
    void clearTagsFilter();

    //! Добавляет тэг, который будет являться атрибутом
    void addAttrTag(const QString &tag);

    //! Очищает список тэгов, которые являются атрибутами
    void clearAttrTags();

    //! Возращает True если строка является атрибутом
    bool isAttr(const QModelIndex &index) const;

    //! Получает класс связку TagXMLItem по индексу модели
    TagXMLItem *toItem(const QModelIndex &index) const;

    //! Получает  индекс модели по классу связке TagXMLItem
    QModelIndex fromItem(TagXMLItem *item) const;

    //! Указывает xml атрибуты тэга для отображения
    void addDisplayedAttr(const QString &tag,const QStringList &value,QIcon icon = QIcon());

    //! Указывает тэги разрешенные для вставки
    void addInsertTags(const QString &tag, const QStringList &value);

    //! Получение индекса поля в тэге
    int columnDisplayedAttr(const QString &tag, const QString &attr) const;

    //! Получение поля в тэге по индексу
    QString displayedAttr(const QString &tag, int column) const;

    //! Удаляет xml атрибуты тэга для отображения
    void removeDisplayedAttr(const QString &tag);

    //! Получение корневой узла дерева
    TagXMLItem* rootItem();

    //! Установить наименование тэга для вставки строки
    void setInsTagName(const QString &tag);

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
    QMimeData *mimeData(const QModelIndexList &indexes);

    //! Возращает список типов, которые могут быть упакованы
    QStringList mimeTypes() const;

    //! Возращает поддерживаемые моделью операции Drop
    Qt::DropActions supportedDropActions() const;

    //! Возращает поддерживаемые моделью операции Drag
    Qt::DropActions supportedDragActions() const;

    //! Возращает True если можно вставить строку
    bool isInsert(const QModelIndex &index) const;

    //! Возращает True если имеются потомки
    bool hasChildren(const QModelIndex &parent) const;

    //! Возращает True если унаследован
    bool isInherited(const QModelIndex &index) const;

    //! Добавления поля для хешеирования
    void addHashAttr(const QString &tag, const QString &value,
                      UniqueAttr unique = TreeXMLModel::NoUnique);

    //! Возращает индекс по значению хешеированного поля
    QModelIndex indexHashAttr(const QString &tag, const QString &attr,
                               const QVariant &value, int number = 0) const;

    //! Обновление хэшей начиная с указанног индекса
    void refreshHashing(const QModelIndex &index = QModelIndex(), bool remove = false);

    //! Обновление хэша указанног индекса
    void refreshHashingOne(const QModelIndex &index, bool remove = false);

    //! Установка ссылки на другое поле
    void addRelation(const QString &tag, const QString &attr,
                     const QString &linkTag, const QString &linkAttr);

    //! Получить поля которые ссылаются на указанное поле
    QList<TagWithAttr> fromRelation(const QString &linkTag,
                                    const QString &linkAttr = QString());

    //! Получить поле на которое ссылается указанное поле
    TagWithAttr toRelation(const QString &tag, const QString &attr);

    //! Возращает ссылку на индекс
    QModelIndex indexLink(const QModelIndex &index) const;

    //! Возращает uuid поле
    QString uuidAttr(const QString &tag) const;
private:

    //! Создание хэша для указанного индекса
    void makeHashing(TagXMLItem *item, bool remove = false);

    //! Создание хэшей начиная с указанног индекса
    void makeHashingOne(TagXMLItem *item, bool remove);

    //! Создание хэша при редактировании индекса
    bool makeHashingData(const QModelIndex &index, QString &dataValue);

    //! Заполнение UUID при вставке строки
    void insertUuid(const QModelIndex &index);

    //! Распаковка данных из потока
    bool unpackData(const QModelIndex &parent, QDataStream &stream, int row, bool move = false);

    //! Упаковка данных в поток
    void packData(const QModelIndex &parent, QDataStream &stream) const;

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
    QHash<QString, QStringList> m_displayedAttr;

    //! Список иконок тэгов для отображения
    QHash<QString, QIcon> m_displayedIcon;

    //! Список названий заголовков
    QMap<int, QString> m_header;

    //! Индекс последней вставленной строки
    QModelIndex m_lastInsRow;

    //! Список тэгов в которые нельзя вставлять строки
    QHash<QString, QStringList> m_insertTags;

    //! Список атрибутов для хэшеированя [тэг][атрибут][уникальность]
    QHash<QString, QHash<QString,UniqueAttr> > m_hashAttr;

    //! Список хэшей [тэг][атрибут][значение атрибута]
    QHash<QString, QHash<QString,QMultiHash<QString,TagXMLItem*> > > m_hashValue;

    //! Список зависимых полей [тэг][атрибут][ссылочныйТэг] = ссылочныйАтрибут
    QHash<QString, QHash<QString, QMap<QString, QString> > > m_linkAttr;
};

#endif
