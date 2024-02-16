#include "threadmanager.h"

ThreadManager::ThreadManager(QObject *parent)
    : QObject{parent}
{
    currSize = 0;
    maxSize = 16;
    currThread = new QThread();
    timer = new QTimer();
    timer->start(10);
    for(int i = 0; i < 100; i++){
        QThread *thread = new QThread(this);
        threadPool.enqueue(thread);
        thread->start();
    }
    qDebug() << "threadsize:" << threadPool.size();

}

void ThreadManager::useExistingOrCreateThread(QVector<Worker*> workers)
{

    for (int i = 0; i < workers.size(); i++)
    {
        if(threadPool.isEmpty()){

            for(int i = 0; i < 10; i++){
                QThread *thread = new QThread(this);
                threadPool.enqueue(thread);
                thread->start();
            }


        }
        if (currSize >= maxSize || !currThread)
        {
            QThread *thread = threadPool.dequeue();

            currThread = thread;

            currSize = 0;
        }
        workers[i]->moveToThread(currThread);
        allWorkers.append(workers[i]);
        // qInfo() << "useExistingOrCreateThread: " << QThread::currentThread();
        QObject::connect(timer, &QTimer::timeout, workers[i], &Worker::compute, Qt::BlockingQueuedConnection);
        QObject::connect(workers[i], &Worker::signalSetPos, this, &ThreadManager::updatePosition, Qt::QueuedConnection);

        currSize++;
        // emit a signal that a worker has ran on thqat specific thread

        if (!currThread->isRunning())
        {
            currThread->start();
        }
    }
}


void ThreadManager::updatePosition(Worker *worker, qreal reflectionX, qreal reflectionY, qreal dx, qreal dy, bool collide)
{

    if (collide) {
        worker->ball->setPos(reflectionX, reflectionY);
        qInfo() << reflectionX << "," << reflectionY << "," << this;
    }
    worker->ball->setPos(worker->ball->mapToParent(dx, dy));
}
