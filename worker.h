#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QThread>
#include <QDebug>

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QRandomGenerator>
#include <QtMath>
#include <wall.h>
#include <QTime>
#include <QTimer>
#include <QThread>
#include <tuple>
#include <QCoreApplication>
#include <QMutex>
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
    Ball *ball;

signals:
    void signalSetPos(Worker *worker, qreal reflectionX, qreal reflectionY, qreal dx, qreal dy, bool collide);

private:
    std::tuple<qreal, qreal, bool> checkCollision();
    qreal calculateWallAngle(Wall *wall);
    std::tuple<qreal, qreal, bool> DoCollision(const QList<QLineF>& walls);
};

#endif // WORKER_H
