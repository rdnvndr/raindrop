#ifndef PROPLOV_H
#define PROPLOV_H

#include "ui_proplov.h"
#include <treexmlmodel/treexmlhashmodel.h>

class PropLov : public QWidget, private Ui::PropLov
{
    Q_OBJECT

public:
    //! Конструктор диалога редактирования списка значений
    explicit PropLov(QWidget *parent = 0);

    //! Деструктор диалога редактирования списка значений
    virtual ~PropLov();

    //! Установка модели структуры классов
    void setModel(TreeXmlHashModel *model);

    //! Получение модели структуры классов
    TreeXmlHashModel* model();

public slots:
    //! Установка текущего списка значений
    void setCurrentLov(const QModelIndex &index);

    //! Установка наименования вкладки/подокна
    void setTabName(const QModelIndex &index);

    //! Закрытие вкладки/подокна
    void closeTab(const QModelIndex &index);

    //! Перевод списка значений в режим редактирования
    void edit(bool flag);

private:
    //! Получение данных модели
    QVariant modelData(const QString &tag, const QString &attr, const QModelIndex &index);

    //! Модель структуры классов
    TreeXmlHashModel* m_model;
};

#endif // PROPLOV_H
