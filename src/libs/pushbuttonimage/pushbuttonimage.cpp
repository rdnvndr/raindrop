#include "pushbuttonimage.h"
#include <QFile>
#include <QFileDialog>

namespace RTPTechGroup {
namespace Widgets {

PushButtonImage::PushButtonImage(QWidget *parent) :
    QPushButton(parent)
{
    connect(this,SIGNAL(clicked()),this,SLOT(openImage()));
}

bool PushButtonImage::setData(const QByteArray &data)
{
    if (data.count() > 0) {
        m_data = data;
        QPixmap pixmap;
        pixmap.loadFromData(data);
        QIcon icon(pixmap);
        this->setIcon(icon);
        return true;
    }
    this->setIcon(QIcon());
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
    if (fileName != "") {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray data = file.readAll();
            file.close();
            if (setData(data))
                return true;
        }
    }
    return false;
}

bool PushButtonImage::saveImage()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Сохранение"),
                                                    "", tr("Рисунок (*.jpg *.jpeg *.png *.svg)"));
    if (fileName != "") {
        QFile file(fileName);
        if ( file.open(QIODevice::WriteOnly) ) {
            if (file.write(m_data) != -1) {
                file.close();
                return true;
            }
            file.close();
        }
    }
    return false;
}

void PushButtonImage::cleanImage()
{
    m_data.clear();
    setIcon(QIcon());
}

}}
