#include "lovwidget.h"
#include "xmldelegate.h"
#include "dbxmlstruct.h"
#include <QMessageBox>
#include <QToolTip>
#include "regexpvalidator.h"

LovWidget::LovWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    RegExpValidator *validator =
            new RegExpValidator(QRegExp("^[A-Za-z]{1}[A-Za-z0-9]{0,26}|^[A-Za-z]{0}"));
    lineEditLovName->setValidator(validator);
    connect(validator,SIGNAL(stateChanged(QValidator::State)),
            this,SLOT(validateLovName(QValidator::State)));

    m_mapper = new QDataWidgetMapper();
    m_mapper->setItemDelegate(new XmlDelegate(this));
    m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    m_typeAttrModel = new QStringListModel();
    const QStringList attrTypeList = (QStringList()
                                       << DBTYPEXML::BOOLEAN
                                       << DBTYPEXML::BINARY
                                       << DBTYPEXML::CHAR
                                       << DBTYPEXML::DATE
                                       << DBTYPEXML::DECIMAL
                                       << DBTYPEXML::DIMENSION
                                       << DBTYPEXML::DOUBLE
                                       << DBTYPEXML::INTEGER
                                       << DBTYPEXML::STRING
                                       << DBTYPEXML::RANGE
                                       << DBTYPEXML::TIME
                                       << DBTYPEXML::TIMESHTAMP
                                       );
    m_typeAttrModel->setStringList(attrTypeList);
    m_oldIndex = -1;
}

LovWidget::~LovWidget()
{
    delete m_mapper;
}

bool LovWidget::isRemove(const QModelIndex &srcIndex)
{
    const TreeXmlHashModel *model = dynamic_cast<const TreeXmlHashModel *>(srcIndex.model());
    if (!model)
        return false;

    bool success = true;
    QString msg;

    QStringList tags;
    tags << DBLOVVALUEXML::LOVVALUE;
    if (model->rowCount(srcIndex,tags)) {
        msg += tr("Необходимо удалить значение списка.\n\n");
        success = false;
    }
    if (!success) {
        QMessageBox msgBox;
        msgBox.setText(tr("Удаление данного объекта не воможно."));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setDetailedText(msg);
        msgBox.setWindowTitle(tr("Предупреждение"));
        msgBox.exec();
    }
    return success;
}

bool LovWidget::isEmpty()
{
    return lineEditLovName->text().isEmpty();;
}

void LovWidget::add()
{
    m_oldIndex = m_mapper->currentIndex();
    QModelIndex srcIndex =  m_model->index(m_mapper->currentIndex(),
                                           0,m_mapper->rootIndex()).parent();
    QModelIndex srcCurrentIndex =
            m_model->insertLastRows(0,1,srcIndex,DBLOVXML::LOV);
    if (srcCurrentIndex.isValid()) {
        setCurrent(srcCurrentIndex);
        edit(true);
    }
}

void LovWidget::remove()
{
    QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());

    if (!isRemove(srcIndex))
        return;

    m_mapper->revert();
    setCurrent(srcIndex.parent());

    m_model->removeRow(srcIndex.row(),srcIndex.parent());
    emit dataRemoved(srcIndex);
}

void LovWidget::removeEmpty()
{
    if (lineEditLovName->text().isEmpty()){
        if (m_oldIndex>=0){
            m_model->removeRow(m_mapper->currentIndex(),
                               m_mapper->rootIndex());
            setCurrent(m_mapper->rootIndex().child(m_oldIndex,0));
            m_oldIndex = -1;
        }else {
            remove();
            return;
        }
    }
}

void LovWidget::setModel(TreeXmlHashModel *model)
{
    m_model = model;

    connect(m_model,SIGNAL(rowsRemoved(QModelIndex,int,int)),
            this,SLOT(rowsRemoved(QModelIndex,int,int)));
    m_mapper->setModel(m_model);

    m_mapper->addMapping(lineEditLovName,
                         model->columnDisplayedAttr(DBLOVXML::LOV,
                                                   DBLOVXML::NAME));
    m_mapper->addMapping(lineEditLovAlias,
                         model->columnDisplayedAttr(DBLOVXML::LOV,
                                                   DBLOVXML::ALIAS));
    m_mapper->addMapping(comboBoxLovType,
                         model->columnDisplayedAttr(DBLOVXML::LOV,
                                                    DBLOVXML::TYPE));
    comboBoxLovType->setModel(m_typeAttrModel);
}

void LovWidget::setCurrent(const QModelIndex &index)
{
    m_mapper->setRootIndex(index.parent());
    emit currentIndexChanged(index);
    m_mapper->setCurrentModelIndex(index);

    int indexType = comboBoxLovType->findText(modelData(
                                                   DBLOVXML::LOV,
                                                   DBLOVXML::TYPE,
                                                   index).toString());
    comboBoxLovType->setCurrentIndex(indexType);

    edit(false);
}

void LovWidget::edit(bool flag)
{
    if (isEmpty()) flag = true;
    groupBoxLov->setEnabled(flag);

    emit edited(flag);
}

void LovWidget::submit()
{
    QModelIndex existIndex = m_model->indexHashAttr(DBLOVXML::LOV,
                                                    DBLOVXML::NAME,
                                                    lineEditLovName->text());
    QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());

    if (existIndex.isValid()){
        if (existIndex.sibling(existIndex.row(),0)!=srcIndex){
            QMessageBox::warning(this,tr("Предупреждение"),
                                 tr("Список значений с таким именем уже существует"));
            return;
        }
    }

    m_mapper->submit();
    removeEmpty();
    edit(false);
    emit dataChanged(srcIndex);
}

void LovWidget::revert()
{
    m_mapper->revert();
    QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());
    setCurrent(srcIndex);
    removeEmpty();
    edit(false);
}

void LovWidget::rowsRemoved(const QModelIndex &index, int start, int end)
{
    if (index == m_mapper->rootIndex()){
        if (m_oldIndex > end)
            m_oldIndex = m_oldIndex - end-start-1;
        else if (m_oldIndex >= start && m_oldIndex <= end){
            m_oldIndex = -1;
        }
    }

    if (index == m_mapper->rootIndex() && m_mapper->currentIndex()==-1 && m_oldIndex <0)
        emit dataRemoved(QModelIndex());
}

void LovWidget::validateLovName(QValidator::State state) const
{
    if(state != QValidator::Acceptable)
        QToolTip::showText(lineEditLovName->mapToGlobal(QPoint(0,5)),
                           tr("Имя списка значений должно содержать только латинские\n"
                              "символы и цифры длиной не более 27 символов"));
    else
        QToolTip::hideText();
}

QVariant LovWidget::modelData(const QString &tag, const QString &attr, const QModelIndex &index, int role)
{
    return index.sibling(index.row(), m_model->columnDisplayedAttr(
                             tag,attr)).data(role);
}
