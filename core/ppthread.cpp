#include "ppthread.h"
#include <pthread.h>
#include <QtDebug>

PPThread::PPThread(QObject *parent) :
    QObject(parent),
    thread(0),
    thread_attr(),
    running(false),
    terminateRequest(false)
{
}

PPThread::~PPThread()
{
    stop();
}


void *PPThread::__run(void *ptr)
{
    static_cast<PPThread *>(ptr)->run();
    static_cast<PPThread *>(ptr)->stop();
    return 0;
}



void PPThread::start()
{
    if (!running)
    {
        pthread_attr_init(&thread_attr);

        if (pthread_create(&thread, &thread_attr, PPThread::__run, this))
        {
            qDebug() << "ERROR in pthread_create() " << endl;
            return;
        }

        running = true;
        pthread_detach(thread);
    }
}



void PPThread::requestStop()
{
    terminateRequest = true;
}




void PPThread::stop()
{
    if (running)
    {
        pthread_cancel(thread);
        pthread_join(thread, 0);
        running = false;
    }
}
