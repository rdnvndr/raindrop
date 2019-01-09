#include "abstractpropeditor.h"

#include <QMdiSubWindow>

using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

AbstractPropEditor::AbstractPropEditor(QWidget *parent)
    : QWidget(parent)
{
    m_model = nullptr;
}

AbstractPropEditor::~AbstractPropEditor()
{

}

void AbstractPropEditor::setModel(TreeXmlHashModel *model)
{
    m_model = model;
}

TreeXmlHashModel *AbstractPropEditor::model()
{
    return m_model;
}

void AbstractPropEditor::closeTab(const QModelIndex &index)
{
    QString subWindowName = this->objectName();
    QString id = this->dataId(index);
    if (!id.isEmpty())
        if (subWindowName.contains(id, Qt::CaseInsensitive)) {
            QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *> (this->parent());
            if (subWindow) subWindow->close();
        }
}

void AbstractPropEditor::edit()
{
    edit(true);
}

QVariant AbstractPropEditor::modelData(const QString &tag, const QString &attr, const QModelIndex &index)
{
    return index.sibling(index.row(), m_model->columnDisplayedAttr(
                             tag,attr)).data();
}

}}

