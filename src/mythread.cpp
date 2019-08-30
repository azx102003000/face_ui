#include "mythread.h"
#include <QDebug>
myThread::myThread()
{

}
void myThread::run()
{
    qDebug()<<"hello world";
    while(1)
    {
        for(static int i=0;i<65535;i++)
        {
            qDebug()<<i;
        }

    }
}
