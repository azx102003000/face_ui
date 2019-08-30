#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>

#include <QMainWindow>
#include <iostream>
#include <QTimer>
#include <QFileDialog>
#include <QMessageBox>
#include <String>
#include <QThread>
#include <QDebug>
//#include <QMetaType>


using namespace cv;
using namespace std;

namespace Ui {
class MainWindow;
}
//thread 线程
class myThread :public QThread
{
    Q_OBJECT
public:
    explicit myThread();
    ~myThread();
    void run();
    void quit_thread(void);
signals:
    void send_msg(Mat src);
    void send_faceROI_msg(Mat src);
private:
    VideoCapture *vc;

};

#if 0
class face_thread :public QThread
{
    Q_OBJECT
public:
    explicit face_thread();
    ~face_thread();
    void run();
    void quit_face_thread(void);
signals:
    void send_face_msg(Mat src);
    void send_faceROI_msg(Mat src);
private slots:
    void thread_msg(Mat src);
private:
    Mat gray;

};
#endif

//主进程类
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_open_btn_clicked();
    void on_save_btn_clicked();
    void thread_msg(Mat src);
    void thread_face(Mat src);
private:
    Ui::MainWindow *ui;
    VideoCapture *vc;
    String face;
    CascadeClassifier face_cc;
    Mat srcimg,gray,faceROI;
    QImage disimg2,disimg;
    vector <Rect>rect;
    myThread *thread;
    int time_flag;

};

#endif // MAINWINDOW_H
