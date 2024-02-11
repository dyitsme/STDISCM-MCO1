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
    for (int i = 0; i < workers.size(); i++)
    {
        if (currSize >= maxSize)
        {
            QThread* newThread = new QThread();

            currThread = newThread;
            workers[i]->moveToThread(currThread);

            currSize = 0;
        }
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
