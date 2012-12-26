#ifndef TREECOMBOBOX_H
#define TREECOMBOBOX_H
#include "treecomboboxglobal.h"
#include <QComboBox>
#include <QTreeView>

//! ComboxBox с выпадающим списком в виде QTreeView
/*! Класс предназначен для выбора значения ComboBox
    из древовидной модели.
    \n
    Пример: \n
    \code
    TreeComboBox *comboBoxLinkAttr;
    comboBoxLinkAttr = new TreeComboBox();
    comboBoxLinkAttr->setModel(classFilterModel);
    \endcode
*/

class TREECOMBOBOXLIB TreeComboBox : public QComboBox
{
public:
    //! Конструктор класса
    TreeComboBox(QWidget* parent=0);

    //! Деструктор класса
    virtual ~TreeComboBox();

    //! Устанавливает модель для списка
    void setModel(QAbstractItemModel * model);

    //! Устанавливает текущий индекс модели списка
    void setCurrentModelIndex (const QModelIndex & index);

    //! Возращает текущий индекс модели списка
    QModelIndex currentModelIndex ();

    virtual void paintEvent(QPaintEvent *);

    //! Утанавливает текст по умолчанию
    void setDisplayText(QString text);

    //! Получение текста по умолчанию
    QString displayText() const;

    void setIndexColumn(int column);

private:
    //! Фильтр событий
    bool eventFilter(QObject*object,QEvent*event);

    //! Показывает выпадающий список в виде дерева
    void showPopup();

    //! Скрывает выпадающий список в виде дерева
    void hidePopup();

    //! Флаг пропуска показа спсика
    bool skipNextHide;

    //! Древовидный список
    QTreeView* treeView;

    //! Текст по умолчанию
    QString m_displayText;

    //! Колонка со значением
    int m_indexColumn ;
};

#endif // TREECOMBOBOX_H
