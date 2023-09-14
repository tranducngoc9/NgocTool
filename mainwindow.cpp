#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
    qDebug()<< "Main thread: "<<QThread::currentThread();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    ui->buttonBox->addButton("Clear", QDialogButtonBox::ButtonRole::ActionRole); // Them nut clear vao trong combobox
    ui->buttonBox->addButton("Remove", QDialogButtonBox::ButtonRole::ActionRole);
    ui->buttonBox->addButton("Edit", QDialogButtonBox::ButtonRole::ActionRole);
}

void MainWindow::load()
{

}

void MainWindow::save()
{

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
        isRunningThread1 = true;
        while(!isPress){
            int x =  QCursor::pos().x();
            int y = QCursor::pos().y();
            ui->labelCoordinates->setText( QString("(%1,%2)").arg(QString::number(x),QString::number(y) ));
            Sleep(100);
        }
        isRunningThread1 = false;
        qDebug()<< "End thread 1: "<<QThread::currentThread();
    });
    if(isRunningThread1 == false)thread1->start();


}



void MainWindow::on_quitGetPosition_clicked()
{
    isPress = true;
}


void MainWindow::on_addItem_clicked()
{
    QListWidgetItem * item = new QListWidgetItem(ui->editCoordinates->text(), ui->listWidget);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemFlag::ItemIsEditable | Qt::ItemFlag::ItemIsTristate | Qt::ItemFlag::ItemIsUserCheckable | Qt::ItemIsSelectable);
    ui->listWidget->addItem(item);
}


void MainWindow::on_btnRun_clicked()
{


    QThread * thread2 = QThread::create([=](){
        qDebug()<< "Runing thread 2: "<<QThread::currentThread();
        isRunningThread2 = true;


        int flag = 1;
        while(flag){
            //Enter right shift to break loop
            if (GetKeyState(VK_RSHIFT) & 0x8000)
            {
                flag = 0;
                qDebug() << "Enter key pressed. Breaking the loop.";
            }

            int itemCount = ui->listWidget->count(); // Lấy số lượng mục trong QListWidget
            int startX = 0; // Tọa độ X ban đầu
            int startY = 0; // Tọa độ Y ban đầu
            for (int i = 0; i < itemCount; ++i) {
                QListWidgetItem *item = ui->listWidget->item(i); // Lấy mục thứ i
                QStringList coordinates = item->text().split(",");
                if( coordinates.size() == 2 ){
                    int targetX = coordinates[0].toInt();
                    int targetY = coordinates[1].toInt();
                    qDebug() << "x:" << targetX << "y:" << targetY;
                    int numSteps = 100; // Số bước di chuyển
                    QCursor cursor;
                    for (int i = 0; i <= numSteps; ++i) {
                        int currentX = startX + (targetX - startX) * i / numSteps;
                        int currentY = startY + (targetY - startY) * i / numSteps;
                        cursor.setPos(currentX, currentY);
                        Sleep(10); // Thời gian ngừng giữa các bước (milliseconds)
                    }
                    startX = targetX;
                    startY = targetY;
                }
            }
        }
        isRunningThread2 = false;
        qDebug()<< "End thread 2: "<<QThread::currentThread();
    });
    if(isRunningThread2 == false){
        thread2->start();
    }
}


void MainWindow::on_buttonBox_clicked(QAbstractButton *button)
{
    //    qDebug()<< button->text();
    if(button->text().contains("OK")){
        save();
        //        accept();
    }
    if(button->text().contains("Cancel")){
        //        reject();
    }
    if(button->text().contains("Clear")){
        ui->listWidget->clear();
        return;
    }
    if(button->text().contains("Remove")){
        QList<QListWidgetItem*>  items = ui->listWidget->selectedItems();
        foreach (QListWidgetItem * item, items) {
            ui->listWidget->removeItemWidget(item);
            delete item;
        }
    }
    if(button->text().contains("Edit")){
        QList<QListWidgetItem*>  items = ui->listWidget->selectedItems();
        foreach (QListWidgetItem * item, items) {
            item->setText("ngoc");
        }
    }
}

