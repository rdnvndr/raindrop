#ifndef MODELERIDE_H
#define MODELERIDE_H

#include <plugin/iplugin.h>
#include <treexmlmodel/treexmlmodel.h>
#include <mainwindow/mainwindow.h>
#include "classtreeview.h"
#include "treefilterproxymodel.h"
#include <QtGui>

class ModelerIDEPlug: public IPlugin
{
    Q_OBJECT
    Q_INTERFACES(IPlugin)

public:

    ModelerIDEPlug(IPlugin *parent = 0);
    virtual ~ModelerIDEPlug();
    bool initialize();
    bool release();
    TreeXMLModel* model();
    void createClassModel(QDomDocument document);
private:
    QString fileName;
    TreeXMLModel* dbStructModel;
    TreeFilterProxyModel* classFilterModel;
    ClassTreeView* treeClassView;
    QAction* actionNewModel;
    QAction* actionSaveModel;
    QAction* actionSaveAsModel;
    QAction* actionOpenModel;
    QAction* actionPublishModel;
    QAction* actionCloseModel;
    QAction* actionAddClass;
    QAction* actionRemoveClass;
    QAction* actionShowAttr;
    QAction* actionShowComp;
    QAction* actionSeparator;
    QMenu*   contextMenu;

    QString className(const QModelIndex &index);

public slots:
    void dblClickTree(QModelIndex index);
    void showPropClass(QModelIndex indexSource);
    void showPropComposition(QModelIndex indexSource);
    void newClassModel();
    void openClassModel();
    void saveClassModel();
    void saveAsClassModel();
    void publishClassModel();
    void closeClassModel();
    void addClass();
    void removeClass();
    void showContextMenu(const QPoint &point);
    void setShownAttr(bool shown);
    void setShownComp(bool shown);

};

#endif
