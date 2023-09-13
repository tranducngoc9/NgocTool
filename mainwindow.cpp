#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include "QDebug"

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



void MainWindow::on_actionOpen_triggered()
{
    QStringList lsFileList = QFileDialog::getOpenFileNames(this, "Select PNG Files", "", tr("PNG ngoctest Files (*.png)"));


}





void MainWindow::on_mouseControl_clicked()
{
    int targetX = 500; // Tọa độ X mục tiêu
       int targetY = 300; // Tọa độ Y mục tiêu

       // Di chuyển con trỏ chuột đến tọa độ mục tiêu
       SetCursorPos(targetX, targetY);
}

