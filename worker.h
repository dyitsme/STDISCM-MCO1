#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include "ball.h"

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(Ball *inputBall, QObject *parent = nullptr);

    void compute();

    qreal startingPosX = 0;
    qreal startingPosY = 0;
    qreal angle = 0;
    qreal speed = 0;

signals:

private:
    Ball *ball;
};

#endif // WORKER_H
