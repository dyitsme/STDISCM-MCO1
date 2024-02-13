#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QDebug>
#include "worker.h"

class ThreadManager : public QObject
{
    Q_OBJECT
public:
    explicit ThreadManager(QObject *parent = nullptr);
    void useExistingOrCreateThread(QVector<Worker*> workers);
    void startTimer();
public slots:
    void updatePosition(Worker *worker, qreal reflectionX, qreal reflectionY, qreal dx, qreal dy, bool collide);

private:
    int maxSize;
    int currSize;
    QThread *currThread;
    QTimer *timer;
    QVector<Worker*> allWorkers;
    QVector <QThread*> threads;

};

#endif // THREADMANAGER_H
