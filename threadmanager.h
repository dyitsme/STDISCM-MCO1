#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <QObject>
#include <QThread>
#include "ball.h"

class ThreadManager : public QObject
{
    Q_OBJECT
public:
    explicit ThreadManager(QObject *parent = nullptr);
    void useExistingOrCreateThread(QVector<Ball*> workers);

private:
    int maxSize;
    int currSize;
    QVector<QThread> threadArray;
    QThread *currThread;
};

#endif // THREADMANAGER_H
