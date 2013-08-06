#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include <treexmlmodel/modifyproxymodel.h>
#include "ui_mainwindow.h"

class QAction;
class QTreeView;
class QWidget;

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ModifyProxyModel *proxyModel;
public slots:
    void updateActions();

private slots:
    void insertChild();
    bool insertColumn(const QModelIndex &parent = QModelIndex());
    void insertRow();
    bool removeColumn(const QModelIndex &parent = QModelIndex());
    void removeRow();

    void insert();
    void remove();
    void submit();
};

#endif
