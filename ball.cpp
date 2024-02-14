#include "ball.h"
#include "wall.h"
#include "cstdlib"
#include <QDebug>

Ball::Ball(qreal startPosX, qreal startPosY, qreal spd, qreal direction)
{
    startingPosX = startPosX;
    startingPosY = startPosY;
    speed = spd;
    angle = direction;
    lastTime = QTime::currentTime();
}


QRectF Ball::boundingRect() const
{
    return QRect(startingPosX, startingPosY, 10, 10);
}

void Ball::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(Qt::red);
    painter->drawEllipse(startingPosX, startingPosY, 10, 10);

}


qreal mdx, mdy;
void Ball::compute()
{

    mdx += speed*qCos(qDegreesToRadians(angle));
    mdy += speed*qSin(qDegreesToRadians(angle));

    // qInfo() << QThread::currentThread();
    qInfo() << "dx: " << mdx << QThread::currentThread();
    qInfo() << "dy: " << mdy << QThread::currentThread();

    // QThread::currentThread()->msleep(500);
    setPos(mapToParent(mdx, mdy));
}

void Ball::advance(int step)
{
    if (!step) return;

    qreal dx, dy;

    checkCollision();

    dx = speed*qCos(qDegreesToRadians(angle));
    dy = speed*qSin(qDegreesToRadians(angle));

    setPos(mapToParent(dx, dy));

}

void Ball::checkCollision()
{

    QList<QGraphicsItem*> colliding_items = collidingItems();
    QList<QLineF> colliding_walls;

    for (QGraphicsItem* item : colliding_items) {
        if (typeid(*item) == typeid(Wall)) {
            Wall* wall = dynamic_cast<Wall*>(item);
            colliding_walls.append(wall->line());
        }
    }

    if (!colliding_walls.isEmpty()) {
        qDebug() << "Hit";
        DoCollision(colliding_walls);
    }
}

qreal Ball::calculateWallAngle(Wall* wall)
{
    // Retrieve the line representing the wall
    QLineF wallLine = wall->line();

    // Calculate the angle in degrees
    qreal angle = qRadiansToDegrees(qAtan2(wallLine.dy(), wallLine.dx()));

    // Calculate the angle in degrees
    angle = fmod(angle + 360.0, 360.0);

    return angle;
}


void Ball::DoCollision(const QList<QLineF>& walls)
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
    angle = averageReflectionAngle;

    // qDebug() << angle;
    // Move the ball slightly away from the walls to avoid repeated collisions
    qreal epsilon = 1.0;
    qreal reflectionX = x() + epsilon * qCos(qDegreesToRadians(averageReflectionAngle));
    qreal reflectionY = y() + epsilon * qSin(qDegreesToRadians(averageReflectionAngle));

    setPos(reflectionX, reflectionY);

}


QPainterPath Ball::shape() const
{
    QPainterPath path;
    path.addEllipse(boundingRect());
    return path;
}
