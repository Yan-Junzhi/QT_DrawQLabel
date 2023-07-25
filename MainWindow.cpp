#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "CreateBinAngle.h"
#include "CDirectionPointer.h"
#include <QDebug>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    qreal angle=ui->lineEdit->text().toFloat();
    ui->widget_2->setLocRotateAngle(angle);
}


void MainWindow::on_pushButton_2_clicked()
{
    ui->widget_2->setKeep(true);
}


void MainWindow::on_pushButton_3_clicked()
{
    qreal angle=ui->lineEdit_2->text().toFloat();
    ui->widget_2->setAngle(angle);
}


void MainWindow::on_pushButton_4_clicked()
{
    qreal angle=ui->lineEdit_3->text().toFloat();
    ui->widget_2->setApRotateAngle(angle);
}

