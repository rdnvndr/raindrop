#include "mainwindow.h"

#include "tilingbutton.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    TilingButton *tw = new TilingButton();
    frame = new TilingFrame(tw);

    this->setCentralWidget(frame);
}

MainWindow::~MainWindow()
{
    delete frame;
}

void MainWindow::on_action_2_triggered()
{
    frame->split(Qt::Vertical);
}

void MainWindow::on_action_1_triggered()
{
    frame->split(Qt::Horizontal);
}

void MainWindow::on_action_triggered()
{
    frame->unsplit();
}
