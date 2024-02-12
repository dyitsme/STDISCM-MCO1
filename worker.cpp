#include "worker.h"

Worker::Worker(Ball *inputBall, QObject *parent)
    : QObject{parent}
{
    ball = inputBall;
}


qreal dx, dy;
void Worker::compute()
{
    // QThread::currentThread()->msleep(500);

    // checkCollision();
    startingPosX = ball->startingPosX;
    startingPosX = ball->startingPosY;

    dx += ball->speed*qCos(qDegreesToRadians(ball->angle));
    dy += ball->speed*qSin(qDegreesToRadians(ball->angle));

    // qInfo() << QThread::currentThread();
    qInfo() << "startingdx: " << startingPosX + dx << QThread::currentThread();
    qInfo() << "startingdy: " << startingPosY + dy << QThread::currentThread();

    // QThread::currentThread()->msleep(500);

    // ball->setPos(startingPosX + dx, startingPosY + dy);
    emit signalSetPos(this, startingPosX, startingPosY, dx, dy);
}
