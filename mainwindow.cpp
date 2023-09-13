#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QListWidget>
#include <QListWidgetItem>
//#include <QTextEdit>
#include <QLineEdit>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug()<< "Main thread: "<<QThread::currentThread();
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_actionOpen_triggered()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("Open Directory"));
    qDebug() << path;
    ui->numFile->setText(QString::number( QDir(path).count()) );
}





void MainWindow::on_mouseControl_clicked()
{
    int startX = 0; // Tọa độ X ban đầu
    int startY = 0; // Tọa độ Y ban đầu
    int targetX = 500; // Tọa độ X mục tiêu
    int targetY = 300; // Tọa độ Y mục tiêu
    int numSteps = 100; // Số bước di chuyển

    for (int i = 0; i <= numSteps; ++i) {
        int currentX = startX + (targetX - startX) * i / numSteps;
        int currentY = startY + (targetY - startY) * i / numSteps;

        SetCursorPos(currentX, currentY);
        Sleep(10); // Thời gian ngừng giữa các bước (milliseconds)
    }
}


void MainWindow::on_getPosMouse_clicked()
{
    isPress = false;
    QThread * thread1 = QThread::create([=](){
        qDebug()<< "Runing thread 1: "<<QThread::currentThread();
        while(!isPress){
            int x =  QCursor::pos().x();
            int y = QCursor::pos().y();
            ui->labelCoordinates->setText( QString("(%1,%2)").arg(QString::number(x),QString::number(y) ));
            Sleep(100);
        }
        qDebug()<< "End thread 1: "<<QThread::currentThread();
    });

    thread1->start();

}



void MainWindow::on_quitGetPosition_clicked()
{
    isPress = true;
}


void MainWindow::on_addItem_clicked()
{
    ui->listWidget->setFlow(QListView::LeftToRight);
    ui->listWidget->setGridSize(QSize(110, 90));
    ui->listWidget->setResizeMode(QListView::Adjust);
    ui->listWidget->setViewMode(QListView::ListMode);
    ui->listWidget->setWrapping(true);
        auto    item = new QListWidgetItem("", ui->listWidget);
        auto    text = new QLineEdit("");
        text->setMinimumSize(100, 80);
        ui->listWidget->setItemWidget(item, text);
}


void MainWindow::on_btnRun_clicked()
{
    qDebug()<< ui->listWidget->takeItem(0)->data(2);
}

