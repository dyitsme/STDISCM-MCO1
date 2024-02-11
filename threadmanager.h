#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include "ball.h"

class ThreadManager : public QObject
{
    Q_OBJECT
public:
    explicit ThreadManager(QObject *parent = nullptr);
    void useExistingOrCreateThread(QVector<Ball*> workers);

protected:



private:
    int maxSize;
    int currSize;
    QVector<QThread> threadArray;
    QThread *currThread;
    QTimer *timer;

};

#endif // THREADMANAGER_H
