#ifndef CLASSMODELXML_H
#define CLASSMODELXML_H

#include "metadatamodelglobal.h"
#include <treexmlmodel/treexmlhashmodel.h>

namespace  RTPTechGroup {
namespace  MetaDataModel {

//! Класс модели дерева классов
/*! Данный класс предназначен для создания дерева метаданных в XML формате
    на основе TreeXmlHashModel. Позволяет отображать указанные наследуемые тэги.
*/

class METADATAMODELLIB ClassModelXml : public RTPTechGroup::XmlModel::TreeXmlHashModel
{
    Q_OBJECT
public:
    //! Конструктор класса
    explicit ClassModelXml(QDomNode document = QDomNode(), QObject *parent = 0);

    //! Деструктор класса
    virtual ~ClassModelXml();

    //! Вставка строки
    QModelIndex insertLastRows (qint32 row, qint32 count,
                                const QModelIndex & parent, QString tag = "element");

    //! Удаление строки
    bool removeRows (qint32 row, qint32 count, const QModelIndex & parent);

    //! Проверка на возможность удаления элементов
    bool isRemove(const QModelIndex &srcIndex);

    //! Возвращает последнюю ошибку
    QString lastError();

private:
    //! Добавления тегов для работы
    void initTagFilters();

    //! Добавление видимых атрибутов
    void initDisplayedAttrs();

    //! Добавление тэгов доступных для вставки
    void initInsertTags();

    //! Добавление хэшей для атрибутов и контроля их уникальности
    void initHashAttrs();

    //! Добавление связей между атрибутами
    void initRelations();

    //! Инициализация модели классов
    void initModel();

private:
    //! Текст последней ошибки
    QString m_errorText;
};

}}

#endif // CLASSMODEL_H
