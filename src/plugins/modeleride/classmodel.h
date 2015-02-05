#ifndef CLASSMODEL_H
#define CLASSMODEL_H

#include <treexmlmodel/treexmlhashmodel.h>

//! Класс модели дерева классов
/*! Данный класс предназначен для создания дерева метаданных в XML формате
    на основе TreeXmlHashModel. Позволяет отображать указанные наследуемые тэги.
*/

class ClassModel : public TreeXmlHashModel
{

public:
    //! Конструктор класса
    explicit ClassModel(QDomNode document = QDomNode(), QObject *parent = 0);

    //! Деструктор класса
    virtual ~ClassModel();

    //! Вставка строки
    QModelIndex insertLastRows (int row, int count,
                                const QModelIndex & parent, QString tag = "element");

    //! Удаление строки
    bool removeRows (int row, int count, const QModelIndex & parent);

private:
    //! Добавления дегов для работы
    void initTagFilters();

    //! Добавление видимых атрибутов
    void initDisplayedAttrs();

    //! Добавление тэгов доступных для вставки
    void initInsertTags();

    //! Добавление жэшей для атрибутов и контроля их уникальности
    void initHashAttrs();

    //! Добавление связей между атрибутами
    void initRelations();

    //! Инициализация модели классов
    void initModel();
};

#endif // CLASSMODEL_H
