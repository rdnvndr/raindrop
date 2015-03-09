#ifndef ABSTRACTPROPEDITOR_H
#define ABSTRACTPROPEDITOR_H

#include <QWidget>
#include <treexmlmodel/treexmlhashmodel.h>

using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

//! Общее описание диалога редактирования объекта дерева класса

class AbstractPropEditor: public QWidget
{
    Q_OBJECT

public:
    //! Конструктор диалога редактирования
    explicit AbstractPropEditor(QWidget *parent = 0);

    //! Деструктор диалога редактирования
    virtual ~AbstractPropEditor();

    //! Установка модели структуры классов
    virtual void setModel(TreeXmlHashModel *model);

    //! Получение модели структуры классов
    virtual TreeXmlHashModel* model();

    //! Получение данных модели
    virtual QVariant modelData(const QString &tag, const QString &attr,
                       const QModelIndex &index);

public slots:
    //! Установка текущей строки
    virtual void setCurrent(const QModelIndex &index) = 0;

    //! Установка наименования вкладки/подокна
    virtual void setTabName(const QModelIndex &index) = 0;

    //! Закрытие вкладки/подокна
    virtual void closeTab(const QModelIndex &index);

    //! Перевод в режим редактирования
    virtual void edit(bool flag  = true) = 0;

private:
    //! Модель структуры классов
    TreeXmlHashModel* m_model;
};

}}

#endif // ABSTRACTPROPEDITOR_H
