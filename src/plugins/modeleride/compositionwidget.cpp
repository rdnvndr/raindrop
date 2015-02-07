#include "compositionwidget.h"
#include <metadatamodel/dbxmlstruct.h>
#include "xmldelegate.h"
#include <QHeaderView>
#include <QMessageBox>

CompositionWidget::CompositionWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    m_compositionModel = new TableXMLProxyModel();
    QStringList tags;
    tags << DBCOMPXML::COMP;
    m_compositionModel->setAttributeTags(tags);

    m_mapperComp = new QDataWidgetMapper();
    m_mapperComp->setItemDelegate(new XmlDelegate(this));
    m_mapperComp->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    tableViewComp->setItemDelegate(new XmlDelegate(this));
    tableViewComp->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(tableViewComp,SIGNAL(clicked(QModelIndex)),
            this,SLOT(setCurrent(QModelIndex)));

    connect(toolButtonAdd,SIGNAL(clicked()),this,SLOT(add()));
    connect(toolButtonDelete,SIGNAL(clicked()),this,SLOT(remove()));

    connect(pushButtonSave,SIGNAL(clicked()),this,SLOT(submit()));
    connect(pushButtonCancel,SIGNAL(clicked()),this,SLOT(revert()));

    connect(toolButtonEdit,SIGNAL(clicked()),this,SLOT(edit()));
    connect(checkBoxInInherited,SIGNAL(clicked(bool)),this,SLOT(showParent(bool)));

    connect(toolButtonDown,SIGNAL(clicked()),this,SLOT(down()));
    connect(toolButtonUp,SIGNAL(clicked()),this,SLOT(up()));
}

CompositionWidget::~CompositionWidget()
{
    delete m_mapperComp;
    delete m_compositionModel;
}

void CompositionWidget::setModel(TreeXmlHashModel *model)
{
    m_model = model;

    m_compositionModel->setSourceModel(m_model);
    m_compositionModel->setHeaderData(0, Qt::Horizontal, tr("Состав"));
    m_compositionModel->setHeaderData(1, Qt::Horizontal, tr("Псевдоним"));
    m_compositionModel->setHeaderData(2, Qt::Horizontal, tr("Исходный класс"));
    m_compositionModel->setHeaderData(3, Qt::Horizontal, tr("Входящий класс"));
    m_compositionModel->setHeaderData(4, Qt::Horizontal, tr("Представление"));
    m_compositionModel->setHeaderData(5, Qt::Horizontal, tr("Композиция"));
    m_compositionModel->setHeaderData(6, Qt::Horizontal, tr("Описание включения"));
    m_compositionModel->setHeaderData(7, Qt::Horizontal, tr("Описание вхождения"));
    m_compositionModel->setHeaderData(8, Qt::Horizontal, tr("Индетификатор"));
    m_compositionModel->setColumnCount(9);
    m_compositionModel->setDynamicSortFilter(true);

    tableViewComp->setModel(m_compositionModel);
    tableViewComp->setColumnHidden(8,true);

    QSortFilterProxyModel* classFilterModel = new QSortFilterProxyModel(this);
    classFilterModel->setFilterKeyColumn(0);
    classFilterModel->setFilterRole(TreeXmlModel::TagRole);
    classFilterModel->setFilterRegExp(DBCLASSXML::CLASS + "|" +
                                      DBMODELXML::MODEL + "|" +
                                      DBCLASSLISTXML::CLASSLIST);
    classFilterModel->setSourceModel(m_model);
    classFilterModel->setDynamicSortFilter(true);
    classFilterModel->sort(0);

    comboBoxDestClass->setModel(classFilterModel);
    comboBoxDestClass->setRootModelIndex(classFilterModel->index(0,0).child(0,0));
    comboBoxDestClass->setIndexColumn(m_model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                                   DBCLASSXML::ID));

    comboBoxLinkClass->setModel(classFilterModel);
    comboBoxLinkClass->setRootModelIndex(classFilterModel->index(0,0).child(0,0));
    comboBoxLinkClass->setIndexColumn(m_model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                                   DBCLASSXML::ID));

    m_mapperComp->setModel(m_compositionModel);
//    m_mapperComp->addMapping(lineEditName,
//                             m_model->columnDisplayedAttr(DBCOMPXML::COMP,
//                                                          DBCOMPXML::NAME));
    m_mapperComp->addMapping(lineEditAlias,
                             m_model->columnDisplayedAttr(DBCOMPXML::COMP,
                                                          DBCOMPXML::ALIAS));
    m_mapperComp->addMapping(checkBoxDestIsView,
                             m_model->columnDisplayedAttr(DBCOMPXML::COMP,
                                                          DBCOMPXML::ISVIEW));
    m_mapperComp->addMapping(checkBoxIsComp,
                             m_model->columnDisplayedAttr(DBCOMPXML::COMP,
                                                          DBCOMPXML::ISCOMP));
    m_mapperComp->addMapping(lineEditDirectDesc,
                             m_model->columnDisplayedAttr(DBCOMPXML::COMP,
                                                          DBCOMPXML::DIRECTDESCRIPTION));
    m_mapperComp->addMapping(lineEditInverseDesc,
                             m_model->columnDisplayedAttr(DBCOMPXML::COMP,
                                                          DBCOMPXML::INVERSEDESCRIPTION));
    m_mapperComp->addMapping(lineEditSrcClass,
                             m_model->columnDisplayedAttr(DBCOMPXML::COMP,
                                                          DBCOMPXML::PARENT));
    m_mapperComp->addMapping(comboBoxDestClass,
                             m_model->columnDisplayedAttr(DBCOMPXML::COMP,
                                                          DBCOMPXML::CLASS));
    m_mapperComp->addMapping(comboBoxLinkClass,
                             m_model->columnDisplayedAttr(DBCOMPXML::COMP,
                                                          DBCOMPXML::LINKCLASS));
}


void CompositionWidget::add()
{
    QModelIndex srcIndex = m_compositionModel->mapToSource(tableViewComp->rootIndex());
    QModelIndex srcCurrentIndex =
            m_model->insertLastRows(0,1,srcIndex,DBCOMPXML::COMP);
    if (srcCurrentIndex.isValid()){
        this->setCurrent(m_compositionModel->mapFromSource(srcCurrentIndex));
        comboBoxDestClass->setCurrentIndex(-1);
        comboBoxDestClass->setDisplayText("");
        comboBoxLinkClass->setCurrentIndex(-1);
        comboBoxLinkClass->setDisplayText("");
        edit(true);
    }
}

void CompositionWidget::remove()
{   
    QModelIndex srcIndex = m_compositionModel->mapToSource(tableViewComp->rootIndex());
    QModelIndex curIndex = m_compositionModel->mapToSource(tableViewComp->currentIndex());
    if (srcIndex.isValid() && curIndex.isValid()){
        emit dataRemoved(srcIndex);
        m_model->removeRow(curIndex.row(),srcIndex);
        this->setCurrent(tableViewComp->currentIndex());
    } else
        QMessageBox::warning(NULL,tr("Предупреждение"),
                             tr("Невозможно удалить состав, поскольку ничего не выбрано."));
}

void CompositionWidget::edit(bool flag)
{
    QModelIndex currentIndex = tableViewComp->currentIndex();

    if (flag==false){
        QModelIndex mapIndex = m_compositionModel->index(
                    m_mapperComp->currentIndex(),
                    m_model->columnDisplayedAttr(DBCOMPXML::COMP,DBCOMPXML::LINKCLASS),
                    m_mapperComp->rootIndex());
        if (mapIndex.data().toString().isEmpty())
            m_compositionModel->removeRow(m_mapperComp->currentIndex(),
                                          m_mapperComp->rootIndex());
    }

    if (!currentIndex.isValid()){
        flag = false;
        toolButtonDelete->setEnabled(false);
        toolButtonEdit->setEnabled(false);
        toolButtonAdd->setEnabled(true);
    } else {
        if (isEdit()==flag)
            return;
        toolButtonEdit->setDisabled(flag);
        toolButtonDelete->setDisabled(flag);
        toolButtonAdd->setDisabled(flag);
    }

    groupBoxProp->setEnabled(flag);
    pushButtonSave->setEnabled(flag);
    pushButtonCancel->setEnabled(flag);

}


void CompositionWidget::setCurrent(const QModelIndex &index)
{
    edit(false);
    if (!index.isValid())
        return;

    tableViewComp->setCurrentIndex(index);
    m_mapperComp->setCurrentModelIndex(index);

    emit currentIndexChanged(index);
}

void CompositionWidget::submit()
{
    QModelIndex rootIndex = tableViewComp->rootIndex();
    QModelIndex srcIndex = m_mapperComp->rootIndex().child(m_mapperComp->currentIndex(),0);
    int nameColumn = m_model->columnDisplayedAttr(DBCOMPXML::COMP, DBCOMPXML::LINKCLASS);

    int row = 0;
    QModelIndex childIndex = m_compositionModel->index(row, nameColumn, rootIndex);
    while (childIndex.isValid())
    {
        if (comboBoxLinkClass->currentText() == childIndex.data()
                && childIndex.sibling(row,0)!=srcIndex) {
            QMessageBox::warning(this,tr("Предуреждение"),
                                 tr("Состав с таким именем уже существует"));
            return;
        }
        childIndex = m_compositionModel->index(++row, nameColumn, rootIndex);
    }
    m_mapperComp->submit();
    edit(false);

    emit dataChanged(srcIndex);
}

bool CompositionWidget::isEdit()
{
     return groupBoxProp->isEnabled();
}

void CompositionWidget::up()
{
    QPersistentModelIndex index = tableViewComp->currentIndex();
    QPersistentModelIndex srcIndex  = m_compositionModel->mapToSource(index);
    QPersistentModelIndex srcParent = srcIndex.parent();
    int row = m_compositionModel->mapToSource(index.sibling(index.row()-1,0)).row();
    if (m_model->moveIndex(srcIndex,srcParent,row)) {
        if (row >= 0)
            index = tableViewComp->currentIndex().sibling(index.row()-2,0);
        else
            index = index.sibling(0,0);
        m_model->removeRow(srcIndex.row(),srcIndex.parent());
        setCurrent(index);
    }
}

void CompositionWidget::down()
{
    QPersistentModelIndex index = tableViewComp->currentIndex();
    QPersistentModelIndex srcIndex  = m_compositionModel->mapToSource(index);
    QPersistentModelIndex srcParent = srcIndex.parent();
    QModelIndex indexNew = index.sibling(index.row()+2,0);
    int row = (indexNew.isValid()) ? m_compositionModel->mapToSource(indexNew).row()
                                   : m_model->rowCount(srcParent,
                                                       m_model->tagsFilter(),
                                                       QStringList());
    if (m_model->moveIndex(srcIndex,srcParent,row)) {
        index = tableViewComp->currentIndex().sibling(index.row()+2,0);
        m_model->removeRow(srcIndex.row(),srcIndex.parent());
        setCurrent(index);
    }
}

void CompositionWidget::revert()
{
    m_mapperComp->revert();
    this->setCurrent(tableViewComp->currentIndex());
    edit(false);
}

QVariant CompositionWidget::modelData(const QString &tag, const QString &attr, const QModelIndex &index)
{
    return index.sibling(index.row(), m_model->columnDisplayedAttr(
                             tag,attr)).data();
}

void CompositionWidget::showParent(bool flag)
{
    m_compositionModel->setFilterRole(Qt::EditRole);

    if (flag==true){
        m_compositionModel->setFilterRegExp("");
    } else {
        QModelIndex index = tableViewComp->rootIndex();
        QString className = modelData(DBCLASSXML::CLASS,
                                      DBCLASSXML::ID,
                                      index).toString();
        className.replace("{","\\{");
        className.replace("}","\\}");
        if (className.isEmpty()){
            m_compositionModel->setFilterRegExp("\\S*");
        }else
            m_compositionModel->setFilterRegExp(className);
    }
    m_compositionModel->setFilterKeyColumn(m_model->columnDisplayedAttr(DBCOMPXML::COMP,
                                                                DBCOMPXML::PARENT));
}

void CompositionWidget::setRootIndex(QModelIndex index)
{
    QModelIndex rootIndex = m_compositionModel->mapToSource(tableViewComp->rootIndex());
    if (rootIndex == index)
        return;

    m_compositionModel->setFilterIndex(index);
    m_compositionModel->setSourceModel(m_model);

    tableViewComp->setRootIndex(m_compositionModel->mapFromSource(index));
    tableViewComp->setCurrentIndex(tableViewComp->rootIndex().child(0,0));

    m_mapperComp->setRootIndex(m_compositionModel->mapFromSource(index));

    this->setCurrent(tableViewComp->rootIndex().child(0,0));
}
