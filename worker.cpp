#include "worker.h"
#include <tuple>
Worker::Worker(Ball *inputBall, QObject *parent)
    : QObject{parent}
{
    ball = inputBall;
}


qreal dx, dy;
void Worker::compute()
{
    // QThread::currentThread()->msleep(500);

    auto [reflectionX, reflectionY, collide] = checkCollision();

    startingPosX = ball->startingPosX;
    startingPosY = ball->startingPosY;

    dx = ball->speed*qCos(qDegreesToRadians(ball->angle));
    dy = ball->speed*qSin(qDegreesToRadians(ball->angle));

    // qInfo() << dx << "," << dy << "," << this;
    //qInfo() << ball->x() << "," << ball->y() << "," << this;

    // qInfo() << "dx =" << dx << "*cos(" << ball->angle << ")" << QThread::currentThread();
    // qInfo() << "dy =" << dy << "*sin("<< ball->angle << ")" << QThread::currentThread();

    // qInfo() << "startingdx: " << startingPosX + dx << QThread::currentThread();
    // qInfo() << "startingdy: " << startingPosY + dy << QThread::currentThread();

    // QThread::currentThread()->msleep(500);
    // qInfo() << "Worker: " << this;
    emit signalSetPos(this, reflectionX, reflectionY, dx, dy, collide);
}


std::tuple<qreal, qreal, bool> Worker::checkCollision()
{
    // QList<QGraphicsItem*> colliding_items = collidingItems();
    // for (QGraphicsItem* item : colliding_items) {
    //     Wall* wall = dynamic_cast<Wall*>(item);
    //     if (typeid(*item) == typeid(Wall)) {
    //         qDebug() << "Hit";
    //         qreal wallAngle = calculateWallAngle(wall);
    //         DoCollision(wallAngle);
    //         return; // Assume one collision at a time for simplicity
    //     }
    // }

    QList<QGraphicsItem*> colliding_items = ball->collidingItems();
    QList<QLineF> colliding_walls;

    for (QGraphicsItem* item : colliding_items) {
        if (typeid(*item) == typeid(Wall)) {
            Wall* wall = dynamic_cast<Wall*>(item);
            colliding_walls.append(wall->line());
        }
    }

    if (!colliding_walls.isEmpty()) {

        qDebug() << "Hit";
        return DoCollision(colliding_walls);
    }
    return{0,0,false};
}

qreal Worker::calculateWallAngle(Wall* wall)
{
    // Retrieve the line representing the wall
    QLineF wallLine = wall->line();

    // Calculate the angle in degrees
    qreal angle = qRadiansToDegrees(qAtan2(wallLine.dy(), wallLine.dx()));

    // Calculate the angle in degrees
    //qreal angle = qRadiansToDegrees(qAtan2(wallY - ballY, wallX - ballX));

    // qDebug() << angle;
    // Ensure the angle is positive and in the range [0, 360)
    angle = fmod(angle + 360.0, 360.0);

    return angle;
}



std::tuple<qreal, qreal, bool> Worker::DoCollision(const QList<QLineF>& walls)
{
    qreal incomingAngle = angle;
    qreal totalReflectionAngle = 0.0;
    int wallCount = 0;
    \

        for (const QLineF& wallLine : walls) {
        qreal wallAngle = qRadiansToDegrees(qAtan2(wallLine.dy(), wallLine.dx()));
        qreal reflectionAngle = 2 * wallAngle - incomingAngle;
        qDebug() << reflectionAngle;
        totalReflectionAngle += reflectionAngle;
        wallCount++;
    }

    // qDebug() << wallCount;

    // Calculate the average reflection angle
    qreal averageReflectionAngle = totalReflectionAngle / wallCount;

    if (wallCount == 2){
        averageReflectionAngle -= 180;
    }
    // Set the ball's angle to the new average reflection angle
    ball->angle = averageReflectionAngle;

    // qDebug() << angle;
    // Move the ball slightly away from the walls to avoid repeated collisions
    qreal epsilon = 1.0;
    qreal reflectionX = ball->x() + epsilon * qCos(qDegreesToRadians(averageReflectionAngle));
    qreal reflectionY = ball->y() + epsilon * qSin(qDegreesToRadians(averageReflectionAngle));

    //ball->setPos(reflectionX, reflectionY);
    return std::make_tuple(reflectionX, reflectionY, true);

}
