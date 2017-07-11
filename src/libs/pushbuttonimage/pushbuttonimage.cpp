#include "pushbuttonimage.h"
#include <QFile>
#include <QFileDialog>

#include "clogging.h"

namespace RTPTechGroup {
namespace Widgets {

PushButtonImage::PushButtonImage(QWidget *parent) :
    QPushButton(parent)
{
    connect(this, &PushButtonImage::clicked, this, &PushButtonImage::openImage);
}

bool PushButtonImage::setData(const QByteArray &data)
{
    if (!data.isEmpty()) {
        m_data = data;
        QPixmap pixmap;
        pixmap.loadFromData(data);
        QIcon icon(pixmap);
        this->setIcon(icon);
        return true;
    }
    this->setIcon(QIcon());
    qCWarning(lcWidgets) << tr("Данные не являются иконкой");
    return false;
}



QByteArray PushButtonImage::data() const
{
    return m_data;
}

bool PushButtonImage::openImage()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открытие"), ".",
                                                    tr("Рисунок (*.jpg *.jpeg *.png *.svg)"));

    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        file.close();
        if (setData(data))
            return true;
    } else {
        qCWarning(lcWidgets) << file.errorString();
    }

    return false;
}

bool PushButtonImage::saveImage()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Сохранение"),
                                                    "", tr("Рисунок (*.jpg *.jpeg *.png *.svg)"));

    QFile file(fileName);
    if ( file.open(QIODevice::WriteOnly) ) {
        if (file.write(m_data) != -1) {
            file.close();
            return true;
        }
        file.close();
    } else {
        qCWarning(lcWidgets) << file.errorString();
    }
    return false;
}

void PushButtonImage::cleanImage()
{
    m_data.clear();
    setIcon(QIcon());
}

}}
