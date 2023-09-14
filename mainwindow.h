#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCoreApplication>
#include <QEvent>
#include <QKeyEvent>

#include <QMainWindow>
#include <windows.h>
#include <QDir>
#include <QThread>
#include <QFileDialog>
#include <QDebug>
#include <QAbstractButton>
#include <QLineEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void init();
    void load();
    void save();



private slots:

    void on_actionOpen_triggered();

    void on_mouseControl_clicked();

    void on_getPosMouse_clicked();


    void on_quitGetPosition_clicked();

    void on_addItem_clicked();

    void on_btnRun_clicked();

    void on_buttonBox_clicked(QAbstractButton *button);

private:
    bool isPress = false;
    bool isRunningThread1 = false;
    bool isRunningThread2 = false;
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
