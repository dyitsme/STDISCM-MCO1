#include "threadmanager.h"

ThreadManager::ThreadManager(QObject *parent)
    : QObject{parent}
{
    currSize = 0;
    maxSize = 2;
    currThread = new QThread();
}

void ThreadManager::useExistingOrCreateThread(QVector<Worker*> workers)
{

    for (int i = 0; i < workers.size(); i++)
    {
        if (currSize >= maxSize)
        {
            QThread* newThread = new QThread();
            currThread = newThread;

            currSize = 0;
        }
        workers[i]->moveToThread(currThread);
        allWorkers.append(workers[i]);
        // qInfo() << "useExistingOrCreateThread: " << QThread::currentThread();

        QObject::connect(currThread, &QThread::started, workers[i], &Worker::compute);
        currSize++;
        // emit a signal that a worker has ran on that specific thread

        if (!currThread->isRunning())
        {
            currThread->start();
        }
    }
    }

void ThreadManager::startTimer()
{
    timer = new QTimer(this);

    // connect(timer, timeout, main, main thread:start)
    // connect(main, main started, threads, thread start)
    // connect(thread, thread start

    QObject::connect(timer, &QTimer::timeout, [&]() {
        // This code will execute when the timer times out
        // qDebug() << "Timer timed out!";

        for (int i = 0; i < allWorkers.size(); i++)
        {
            allWorkers[i]->compute();
        }
    });
    timer->start(10);

}

void ThreadManager::startParentThread()
{

}
