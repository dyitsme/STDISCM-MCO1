#include "threadmanager.h"

ThreadManager::ThreadManager(QObject *parent)
    : QObject{parent}
{
    currSize = 0;
    maxSize = 16;
    currThread = new QThread();
    timer = new QTimer();
    timer->start(10);


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

        // QObject::connect(currThread, &QThread::started, workers[i], &Worker::compute);
        //QObject::disconnect(timer, &QTimer::timeout, workers[i], &Worker::compute);
        QObject::connect(timer, &QTimer::timeout, workers[i], &Worker::compute, Qt::QueuedConnection);
        QObject::connect(workers[i], &Worker::signalSetPos, this, &ThreadManager::updatePosition, Qt::QueuedConnection);


        currSize++;
        // emit a signal that a worker has ran on that specific thread

        if (!currThread->isRunning())
        {
            currThread->start();
        }
    }
}


int i = 0;
void ThreadManager::updatePosition(Worker *worker, qreal reflectionX, qreal reflectionY, qreal dx, qreal dy, bool collide)
{

    // i++;
    // qInfo() << "This thread: " << QThread::currentThread();
    // if(collide){
    //     qInfo() << "collide: " << collide;
    //     worker->ball->setPos(reflectionX, reflectionY);
    // }
    if (collide) {
        // Update the position only if there was a collision
        //qInfo() << "reflectionX: " << reflectionX;
        qInfo() << "RANDOM";
        worker->ball->setPos(reflectionX, reflectionY);
        qInfo() << reflectionX << "," << reflectionY << "," << this;
    }
    worker->ball->setPos(worker->ball->mapToParent(dx, dy));
}
