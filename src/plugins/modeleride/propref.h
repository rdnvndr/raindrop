#ifndef PROPREF_H
#define PROPREF_H

#include "ui_propref.h"
#include <treexmlmodel/treexmlhashmodel.h>
#include <treexmlmodel/tablexmlproxymodel.h>
#include <QDataWidgetMapper>
#include <QStringListModel>

//! Диалог редактирования справочника
/*! Диалог предназначен для редактирования справочника
*/

class PropRef : public QWidget, private Ui::PropRef
{
    Q_OBJECT

public:
     //! Конструктор диалога редактирования справочника
    explicit PropRef(QWidget *parent = 0);

    //! Деструктор диалога редактирования справочника
    virtual ~PropRef();

    //! Установка модели структуры классов
    void setModel(TreeXmlHashModel *model);

    //! Получение модели структуры классов
    TreeXmlHashModel* model();

public slots:
    //! Установка текущего справочника
    void setCurrentRef(const QModelIndex &index);

    //! Установка наименования вкладки/подокна
    void setTabName(const QModelIndex &index);

    //! Закрытие вкладки/подокна
    void closeTab(const QModelIndex &index);

    //! Перевод справочника в режим редактирования
    void edit(bool flag);

private:
    //! Получение данных модели
    QVariant modelData(const QString &tag, const QString &attr, const QModelIndex &index);

    //! Модель структуры классов
    TreeXmlHashModel* m_model;
};

#endif // PROPREF_H
