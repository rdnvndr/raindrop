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
    ~ModelerIDEPlug();
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
    QMenu*   contextMenu;

    QString className(const QModelIndex &index);

public slots:
    void showPropClass(QModelIndex index);
    void showPropComposition(QModelIndex index);
    void newClassModel();
    void openClassModel();
    void saveClassModel();
    void saveAsClassModel();
    void publishClassModel();
    void closeClassModel();
    void addClass();
    void removeClass();
    void showContextMenu(const QPoint &point);

};

#endif
