#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <tilingframe/tilingwidget.h>
using namespace RTPTechGroup::Widgets;
#include <QMainWindow>

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();
private slots:
    void on_action_2_triggered();
    void on_action_1_triggered();
    void on_action_triggered();

private:
    TilingFrame *frame;
};

#endif // MAINWINDOW_H
