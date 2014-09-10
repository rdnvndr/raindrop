#ifndef TreeXmlHashModel1_H
#define TreeXmlHashModel1_H

#include <QAbstractItemModel>
#include <QtXml/QDomDocument>
#include <QModelIndex>
#include <QVariant>
#include <QStringList>
#include <QIcon>
#include "treexmlmodelglobal.h"
#include "treexmlmodel.h"

class TagXmlItem;

//! Класс модель дерева c хэшем для XML данных
/*! Данный класс предназначен для создания дерева на основе,
    XML данных. Позволяет отображать указанные наследуемые тэги. Узлы дерева
    (записи таблицы) связаны между указателем класса TagXMLItem, а также строить
    хэши (индексы) по полям для быстроко поиска данных.
*/

class XMLMODELLIB TreeXmlHashModel : public TreeXmlModel
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

     //! Конструктор модели
    TreeXmlHashModel(QDomNode document, QObject *parent = 0);
    //! Деструктор модели
    virtual ~TreeXmlHashModel();

    //! Возращает хранимые данные
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole ) const;

    //! Устанавливает значение для указанной записи
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    //! Вставка строки
    bool insertRows (int row, int count, const QModelIndex & parent);

    //! Вставка строки
    QModelIndex insertLastRows (int row, int count,
                                const QModelIndex & parent, QString tag = "element");

    //! Удаление строки
    bool removeRows (int row, int count, const QModelIndex & parent);

    //! Перемещение элемента
    bool moveIndex(const QModelIndex &srcIndex,
                           const QModelIndex &destIndex, bool recursively = false);

    //! Добавления поля для хешеирования
    void addHashAttr(const QString &tag, const QString &value,
                      UniqueAttr unique = TreeXmlHashModel::NoUnique);

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
                                    const QString &linkAttr = QString()) const;

    //! Получить поле на которое ссылается указанное поле
    TagWithAttr toRelation(const QString &tag, const QString &attr);

    //! Возращает ссылку на индекс
    QModelIndex indexLink(const QModelIndex &index) const;

    //! Возращает uuid поле
    QString uuidAttr(const QString &tag) const;

private:

    //! Создание хэша для указанного индекса
    void makeHashing(TagXmlItem *item, bool remove = false);

    //! Создание хэшей начиная с указанног индекса
    void makeHashingOne(TagXmlItem *item, bool remove);

    //! Создание хэша при редактировании индекса
    bool makeHashingData(const QModelIndex &index, QString &dataValue);

    //! Заполнение UUID при вставке строки
    void insertUuid(const QModelIndex &index, QString tag);

    //! Список атрибутов для хэшеированя [тэг][атрибут][уникальность]
    QHash<QString, QHash<QString,UniqueAttr> > m_hashAttr;

    //! Список хэшей [тэг][атрибут][значение атрибута]
    QHash<QString, QHash<QString,QMultiHash<QString,TagXmlItem*> > > m_hashValue;

    //! Список зависимых полей [тэг][атрибут][ссылочныйТэг] = ссылочныйАтрибут
    QHash<QString, QHash<QString, QMap<QString, QString> > > m_linkAttr;
};

#endif
