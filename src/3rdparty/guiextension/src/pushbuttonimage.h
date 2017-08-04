#ifndef PUSHBUTTONIMAGE_H
#define PUSHBUTTONIMAGE_H

#include "guiextensionglobal.h"
#include <QPushButton>

namespace RTPTechGroup {
namespace Widgets {

//! Класс PushButtonImage является кнопкой-изображением
/*! Данный класс предназначен для отображение рисунка в
 *  виде кнопки, загрузки с диска, сохранения на диск
 */

class GUIEXTENSIONLIB PushButtonImage : public QPushButton
{
    Q_OBJECT
public:
    //! Конструктор
    explicit PushButtonImage(QWidget *parent = 0);

    //! Запись рисунка
    bool setData(const QByteArray &data);

    //! Получение рисунка
    QByteArray data() const;
signals:

public slots:

    //! Открытие рисунка с диска
    bool openImage();

    //! Сохранение рисунка на диск
    bool saveImage();

    //! Очистка рисунка
    void cleanImage();

private:
    //! Переменная для хранения рисунка
    QByteArray m_data;
};

}}

#endif

