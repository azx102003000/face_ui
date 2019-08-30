#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("face_ui");
    thread=new myThread();//线程
    connect(thread,SIGNAL(send_msg(Mat)),this,SLOT(thread_msg(Mat)),Qt::DirectConnection);
    connect(thread,SIGNAL(send_faceROI_msg(Mat)),this,SLOT(thread_face(Mat)),Qt::DirectConnection);
//    face1="E:\\opencv\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml";
//    if(!face_cc.load(face1))//确定找到xml文件
//    {
//        cout<<"*.xml open fail"<<endl;
//    }
}
myThread::myThread(){}//线程构造函数
myThread::~myThread()//线程析构函数
{
    qDebug()<<"out thread";
    if(vc->isOpened())
    {
        vc->release();
    }
    delete vc;
}


/************************************线程执行程序*******************************************/

void myThread::run()
{

    vc= new VideoCapture(0);
    Mat srcimg,gray,faceROI;
    CascadeClassifier face_cc;
    vector <Rect> rect;
    if(!vc->isOpened())
    {
        qDebug()<<"thread vc fail!";
        vc->release();
        return;
    }
    String face1="E:\\opencv\\source\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml";
    if(!face_cc.load(face1))//确定找到xml文件
    {
        cout<<"*.xml open fail"<<endl;
    }

    while(1)
    {
        vc->read(srcimg);
        if(srcimg.empty())
        {
            vc->release();
            return;
        }
        cvtColor(srcimg,gray,COLOR_BGR2GRAY);
        equalizeHist(gray,gray);//直方图均衡化，避免图片过亮或暗
        face_cc.detectMultiScale(gray,rect,1.1,2,0,Size(50,50));//人脸检测
        for(int i=0;i<rect.size();i++)
        {
            faceROI=srcimg.clone();//深拷贝
            Mat faceROI2(faceROI,rect[i]);//对脸部裁剪
            emit send_faceROI_msg(faceROI2);//发送人脸的图像
            rectangle(srcimg,Point(rect[i].x,rect[i].y),Point(rect[i].x+rect[i].width,rect[i].y+rect[i].height),Scalar(0,255,0),2);

        }
        putText(srcimg,String("find face ") + to_string(rect.size()), Point(0,50), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 3);//字体显示在图片上
        emit send_msg(srcimg);//发送整个检测画出人脸图片
    }
    vc->release();
}


void myThread::quit_thread()//释放摄像头
{
    if(vc->isOpened())
    {
        vc->release();
    }
}

void MainWindow::thread_msg(Mat src)//主进程接收到线程的信号
{
    if(src.empty())//读错误或者视频结束
    {
        thread->quit_thread();
        thread->terminate();
        return;
    }

//cvtColor(src,gray,COLOR_BGR2GRAY);
//equalizeHist(gray,gray);//直方图均衡化，避免图片过亮或暗
//    face_cc.detectMultiScale(gray,rect,1.1,2,0,Size(50,50));
//    //imshow("gray",gray);

//    for(int i=0;i<rect.size();i++)
//    {
//        rectangle(src,Point(rect[i].x,rect[i].y),Point(rect[i].x+rect[i].width,rect[i].y+rect[i].height),Scalar(0,255,0),2);
//    }
//    putText(src,String("find face ")+to_string(rect.size()),Point(0,50),FONT_HERSHEY_SIMPLEX,1,Scalar(255,255,255),3);//字体显示在图片上

    cvtColor(src,src,COLOR_BGR2RGB);
    disimg=QImage((const unsigned char*)(src.data),src.cols,src.rows,QImage::Format_RGB888);
    ui->show_lable->setPixmap(QPixmap::fromImage(disimg.scaled(ui->show_lable->size(),Qt::KeepAspectRatio)));
}
void MainWindow::thread_face(Mat src)
{
    cv::resize(src,src,Size(75,75));//将扫描到的人脸放入到face_show的lable中大小为75*75
    cvtColor(src,src,COLOR_BGR2RGB);//Mat类转Qimage类需要颜色转换
    //显示在QT的UI上 比起之前加了faceROI2.cols*faceROI2参数，就不会显示出对角折叠显示的错误
    disimg2=QImage((const unsigned char*)(src.data),src.cols,src.rows,src.cols*src.channels(),QImage::Format_RGB888);
    ui->face_lable->setPixmap(QPixmap::fromImage(disimg2.scaled(ui->face_lable->size(),Qt::KeepAspectRatio)));
}

MainWindow::~MainWindow()
{
    if(thread->isRunning())
    {
        //关闭线程
        thread->terminate();
        delete thread;
    }

    delete ui;
}

void MainWindow::on_open_btn_clicked()//开启 OR 关闭
{
        if(thread->isRunning())
        {
            thread->quit_thread();
            thread->terminate();
            ui->open_btn->setText(tr("打开"));
            //关闭显示并回到最初状态
            ui->face_lable->clear();
            ui->show_lable->clear();
            ui->show_lable->setText("show");
            ui->face_lable->setText("face show");
        }
        else
        {
            thread->start();//线程启动
            ui->open_btn->setText(tr("关闭"));
        }


}

#if 0
void MainWindow::time_out()//超时
{
    if(!face_cc.load(face1))//确定找到xml文件
    {
        cout<<"*.xml open fail"<<endl;
        timer->stop();
        return;
    }

    if(!vc->isOpened())
    {
        cout<<"open video fail"<<endl;
        vc->release();
        timer->stop();
        return;
    }
    vc->read(srcimg);
    if(srcimg.empty())//读错误或者视频结束
    {
        timer->stop();
        vc->release();
        return;

    }
    cvtColor(srcimg,gray,COLOR_BGR2GRAY);
    face_cc.detectMultiScale(gray,rect,1.1,2,0,Size(50,50));
    //imshow("gray",gray);
    equalizeHist(gray,gray);//直方图均衡化，避免图片过亮或暗
    imshow("gray2",gray);

    for(int i=0;i<rect.size();i++)
    {

        faceROI=srcimg.clone();//深拷贝
        Mat faceROI2(faceROI,rect[i]);//对脸部裁剪
        cv::resize(faceROI2,faceROI2,Size(75,75));//将扫描到的人脸放入到face_show的lable中大小为75*75
        cvtColor(faceROI2,faceROI2,COLOR_BGR2RGB);//Mat类转Qimage类需要颜色转换
        //显示在QT的UI上 比起之前加了faceROI2.cols*faceROI2参数，就不会显示出对角折叠显示的错误
        disimg2=QImage((const unsigned char*)(faceROI2.data),faceROI2.cols,faceROI2.rows,faceROI2.cols*faceROI2.channels(),QImage::Format_RGB888);
        ui->face_lable->setPixmap(QPixmap::fromImage(disimg2.scaled(ui->face_lable->size(),Qt::KeepAspectRatio)));
        //画出矩形的绿色框框rect变量已经提出来的
        rectangle(srcimg,Point(rect[i].x,rect[i].y),Point(rect[i].x+rect[i].width,rect[i].y+rect[i].height),Scalar(0,255,0),2);
    }
    //显示寻找的人脸数量并显示在show的lable上
    putText(srcimg,String("find face ")+to_string(rect.size()),Point(0,50),FONT_HERSHEY_SIMPLEX,1,Scalar(255,255,255),3);//字体显示在图片上

    cvtColor(srcimg,srcimg,COLOR_BGR2RGB);
    disimg=QImage((const unsigned char*)(srcimg.data),srcimg.cols,srcimg.rows,QImage::Format_RGB888);
    ui->show_lable->setPixmap(QPixmap::fromImage(disimg.scaled(ui->show_lable->size(),Qt::KeepAspectRatio)));

}
#endif

void MainWindow::on_save_btn_clicked()//保存
{
    if(thread->isRunning())
    {
        for(int i=0;i<rect.size();i++)
        {
            QFileDialog filedialog;
            QString fileName=filedialog.getSaveFileName(this,tr("open file"),"",tr("*.jpg *.jpeg *.png"));
            qDebug()<<fileName;
            if(fileName.isEmpty())
                return;

            Mat faceROI2(faceROI,rect[i]);//对脸部裁剪
            cv::resize(faceROI2,faceROI2,Size(75,75));//将扫描到的人脸放入到face_show的lable中大小为75*75
            imwrite(fileName.toStdString(),faceROI2);
        }

    }
    else
    {
        QMessageBox::warning(this,tr("警告"),tr("请按“打开”再点击“保存”"));
    }
}
