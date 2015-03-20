#ifndef XMLDELEGATE_H
#define XMLDELEGATE_H

#include <QStyledItemDelegate>

#include <treexmlmodel/treexmlhashmodel.h>

using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

//! Делегат XML модели
/*! Данный делегат предназначен для ввода данных в XML модель
    TreeXMLModel
 */

class XmlDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    //! Конструктор делегата
    explicit XmlDelegate(QObject *parent = 0);

    //! Запись данных в модель
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    //! Предоставление редактора
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    //! Делегат должен предоставить функцию копирования данных модели в редактор.
    void setEditorData(QWidget *editor, const QModelIndex &index) const;

    //! Перерисовка делегата
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

    //! Получения центра для прямоугольника виджета
    QRect getCenteredComboBoxRect(const QStyleOptionViewItem &option) const;

//    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
//                              const QModelIndex &index) const;
private:
    //! Получение хэш модели
    TreeXmlHashModel *sourceModel(QAbstractItemModel *model) const;

    //! Получение индекса прокси модели
    QModelIndex mapToSource(QModelIndex index) const;

    //! Получение индекса хэш модели
    QModelIndex mapFromSource(QAbstractItemModel *toModel, QModelIndex index) const;
};

}}

#endif // XMLDELEGATE_H
