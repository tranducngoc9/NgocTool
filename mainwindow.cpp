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
            //Get Enter press
            if (GetAsyncKeyState(VK_RETURN) & 0x8000)
            {
                ui->editCoordinates->setText(QString("%1,%2").arg(QString::number(x),QString::number(y) ));
            }
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
    ui->labelCoordinates->setText("(None,None)");
}


void MainWindow::on_addItem_clicked()
{
    QListWidgetItem * item = new QListWidgetItem(ui->editCoordinates->text(), ui->listWidget);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemFlag::ItemIsEditable | Qt::ItemFlag::ItemIsTristate | Qt::ItemFlag::ItemIsUserCheckable | Qt::ItemIsSelectable);

    if (ui->editCoordinates->text() != ""){
        ui->listWidget->addItem(item);
    }
    else{
        delete  item;
    }
}


void MainWindow::on_btnRun_clicked()
{


    QThread * thread2 = QThread::create([=](){
        qDebug()<< "Runing thread 2: "<<QThread::currentThread();
        isRunningThread2 = true;



        int flag = 1;
        int startX = QCursor::pos().x(); // Tọa độ X ban đầu
        int startY = QCursor::pos().y(); // Tọa độ Y ban đầu


        while(flag){

            int itemCount = ui->listWidget->count(); // Lấy số lượng mục trong QListWidget

            if( ui->listWidget->count() < 2 ){
                break;
            }

            for (int i = 0; i < itemCount && flag != 0 ; ++i) {

                QListWidgetItem *item = ui->listWidget->item(i); // Lấy mục thứ i
                QStringList coordinates = item->text().split(",");
                if( coordinates.size() == 2 ){
                    ui->noitify->setText("Press ESC key to stop !!");
                    int targetX = coordinates[0].toInt();
                    int targetY = coordinates[1].toInt();
                    int numSteps = 100; // Số bước di chuyển
                    QCursor cursor;
                    for (int i = 0; i <= numSteps; ++i) {
                        int currentX = startX + (targetX - startX) * i / numSteps;
                        int currentY = startY + (targetY - startY) * i / numSteps;
                        cursor.setPos(currentX, currentY);
                        Sleep(10); // Thời gian ngừng giữa các bước (milliseconds)
                        //Enter right shift to break loop
                        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
                        {
                            flag = 0;
                            qDebug() << "Enter ESC key pressed. Breaking the loop.";
                            ui->noitify->setText("");
                            break;
                        }
                    }

                    // Simulate a left mouse button press at the specified coordinates
                    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                    // Simulate a left mouse button release at the specified coordinates
                    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

                    startX = targetX;
                    startY = targetY;
                }
                else{
                    flag = 0;
                    ui->noitify->setText("Invalid coordinates !!");
                    break;
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
        on_btnRun_clicked();
    }
    if(button->text().contains("Cancel")){
        ui->noitify->setText("");
        ui->listWidget->clear();
        return;
    }
    if(button->text().contains("Clear")){
        ui->noitify->setText("");
        ui->listWidget->clear();
        return;
    }
    if(button->text().contains("Remove")){
        ui->noitify->setText("");
        QList<QListWidgetItem*>  items = ui->listWidget->selectedItems();
        foreach (QListWidgetItem * item, items) {
            ui->listWidget->removeItemWidget(item);
            delete item;
        }
    }
}

