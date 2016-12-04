#include "actionprop.h"
#include <QFileDialog>

namespace RTPTechGroup {
namespace MainWindow {

ActionProp::ActionProp(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    connect(pushButtonOpen, &QPushButton::clicked,
            pushButtonIcon, &RTPTechGroup::Widgets::PushButtonImage::openImage);
    connect(pushButtonDelete, &QPushButton::clicked,
            pushButtonIcon, &RTPTechGroup::Widgets::PushButtonImage::cleanImage);
}

}
}
