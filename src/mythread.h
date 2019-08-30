#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QThread>

class myThread: public QThread
{
public:
    myThread();
    void run();
signals:

};



#endif // MYTHREAD_H
