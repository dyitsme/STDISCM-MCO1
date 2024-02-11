#include "threadmanager.h"

ThreadManager::ThreadManager(QObject *parent)
    : QObject{parent}
{
    currSize = 0;
    maxSize = 2;
    currThread = new QThread();
}

void ThreadManager::useExistingOrCreateThread(QVector<Ball*> workers)
{
    // timer = new QTimer(this);


    // QObject::connect(&timer, &QTimer::timeout, [&]() {
    //     // This code will execute when the timer times out
    //     qDebug() << "Timer timed out!";
    // });

    for (int i = 0; i < workers.size(); i++)
    {
        if (currSize >= maxSize)
        {
            QThread* newThread = new QThread();
            currThread = newThread;
            currSize = 0;
        }
        workers[i]->moveToThread(currThread);
        // qInfo() << "Worker: " << workers[i];

        QObject::connect(currThread, &QThread::started, workers[i], &Ball::run);
        currSize++;
        // emit a signal that a worker has ran on that specific thread

        if (!currThread->isRunning())
        {
            currThread->start();
        }
    }
}
