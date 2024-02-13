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


    emit signalSetPos(this, reflectionX, reflectionY, dx, dy, collide);
}


std::tuple<qreal, qreal, bool> Worker::checkCollision()
{


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

std::tuple<qreal, qreal, bool> Worker::DoCollision(const QList<QLineF>& walls) {
    bool collisionOccurred = false;

    for (const QLineF& wallLine : walls) {
        qreal dx = wallLine.p2().x() - wallLine.p1().x();
        qreal dy = wallLine.p2().y() - wallLine.p1().y();

        // Assuming a very small value to account for floating point imprecision
        const qreal epsilon = 0.01;

        if (std::abs(dy) < epsilon) { // Horizontal wall
            ball->angle = 360 - ball->angle;
            collisionOccurred = true;
        } else if (std::abs(dx) < epsilon) { // Vertical wall
            ball->angle = 180 - ball->angle;
            collisionOccurred = true;
        }

        // Normalize the angle
        if (ball->angle < 0) ball->angle += 360;
        else if (ball->angle >= 360) ball->angle -= 360;
    }

    // Assuming no displacement if no collision occurred
    qreal reflectionX = ball->x();
    qreal reflectionY = ball->y();

    if (collisionOccurred) {
        // Adjust the ball's position based on its new angle to prevent sticking to the wall
        qreal epsilon = 5.0; // Adjust based on ball speed and desired effect
        reflectionX += epsilon * qCos(qDegreesToRadians(ball->angle));
        reflectionY += epsilon * qSin(qDegreesToRadians(ball->angle));
    }

    return std::make_tuple(reflectionX, reflectionY, collisionOccurred);
}

