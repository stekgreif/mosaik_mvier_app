#ifndef PTHREAD_H
#define PTHREAD_H

#include <QObject>

class PPThread : public QObject
{
    Q_OBJECT
    public:
        explicit PPThread(QObject *parent = 0);
        virtual ~PPThread();

    protected:
        virtual void run() = 0;

    signals:

    public slots:
        void start();
        void stop();
        void requestStop();


    private:
        static void *__run(void *data);
        pthread_t thread;
        pthread_attr_t thread_attr;
        bool running;

    protected:
        bool terminateRequest;
};

#endif // PTHREAD_H
