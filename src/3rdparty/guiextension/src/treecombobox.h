#ifndef TREECOMBOBOX_H
#define TREECOMBOBOX_H
#include "guiextensionglobal.h"
#include <QComboBox>
#include <QTreeView>

namespace RTPTechGroup {
namespace Widgets {

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

class GUIEXTENSIONLIB TreeComboBox : public QComboBox
{
public:
    //! Конструктор класса
    explicit TreeComboBox(QWidget *parent=0);

    //! Деструктор класса
    virtual ~TreeComboBox();

    //! Устанавливает модель для списка
    void setModel(QAbstractItemModel *model);

    //! Устанавливает текущий индекс модели списка
    void setCurrentModelIndex (const QModelIndex & index);

    //! Возращает текущий индекс модели списка
    QModelIndex currentModelIndex ();

    //! Событие для перерисовки
    void paintEvent(QPaintEvent *);

    //! Утанавливает текст по умолчанию
    void setDisplayText(const QString &text);

    //! Получение текста по умолчанию
    QString displayText() const;

    //! Утанавливает иконку по умолчанию
    void setDisplayIcon(const QIcon &icon);

    //! Получение иконки по умолчанию
    QIcon displayIcon() const;

    //! Установка колонки для индекса
    void setIndexColumn(qint32 column);

    //! Утанавливать режим Показа иконки
    void setShowingIcon(bool showing);

    //! Получить режим показа иконки
    bool showingIcon();

    //! Устанавливает корневой индекс
    void setRootModelIndex(const QModelIndex & index);

    //! Получение корневого индекса
    QModelIndex rootModelIndex();

private:
    //! Фильтр событий
    bool eventFilter(QObject*object,QEvent*event);

    //! Показывает выпадающий список в виде дерева
    void showPopup();

    //! Скрывает выпадающий список в виде дерева
    void hidePopup();

    //! Флаг пропуска показа спиcка
    bool skipNextHide;

    //! Древовидный список
    QTreeView *treeView;

    //! Текст по умолчанию
    QString m_displayText;

    //! Иконка по умолчанию
    QIcon   m_displayIcon;

    //! Режим показа иконки
    bool m_showingIcon;

    //! Колонка со значением
    qint32 m_indexColumn ;

    //! Корневой индекс
    QModelIndex m_rootIndex;
};

}}

#endif // TREECOMBOBOX_H
