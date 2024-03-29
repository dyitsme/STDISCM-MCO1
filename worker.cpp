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
    // QCoreApplication::processEvents();
    startingPosX = ball->startingPosX;
    startingPosY = ball->startingPosY;

    dx = ball->speed*qCos(qDegreesToRadians(ball->angle));
    dy = ball->speed*qSin(qDegreesToRadians(ball->angle));

    // qInfo() << "Worker Thread: " << QThread::currentThread();



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
    angle = fmod(angle + 360.0, 360.0);

    return angle;
}

std::tuple<qreal, qreal, bool> Worker::DoCollision(const QList<QLineF>& walls) {
    bool collisionOccurred = false;

    for (const QLineF& wallLine : walls) {
        // Calculate normal vector of the wall
        qreal wallDx = wallLine.p2().x() - wallLine.p1().x();
        qreal wallDy = wallLine.p2().y() - wallLine.p1().y();
        // Normal vector (perpendicular to the wall)
        QPointF normal(-wallDy, wallDx);
        qreal normalLength = qSqrt(normal.x()*normal.x() + normal.y()*normal.y());
        // Normalize the normal vector
        normal /= normalLength;

        // Ball's direction vector
        qreal ballDx = qCos(qDegreesToRadians(ball->angle));
        qreal ballDy = qSin(qDegreesToRadians(ball->angle));

        // Dot product between ball's direction and wall's normal vector
        qreal dot = ballDx*normal.x() + ballDy*normal.y();

        // Reflect ball's direction vector across the wall's normal vector
        qreal reflectDx = ballDx - 2 * dot * normal.x();
        qreal reflectDy = ballDy - 2 * dot * normal.y();

        // Update ball's angle based on reflected direction
        ball->angle = qRadiansToDegrees(qAtan2(reflectDy, reflectDx));
        collisionOccurred = true;

        // Normalize the angle
        if (ball->angle < 0) ball->angle += 360;
        else if (ball->angle >= 360) ball->angle -= 360;
    }

    // Assuming no displacement if no collision occurred
    qreal reflectionX = ball->x();
    qreal reflectionY = ball->y();

    if (collisionOccurred) {
        // Adjust the ball's position based on its new angle to prevent sticking to the wall
        qreal epsilon = 1.0; // Adjust based on ball speed and desired effect
        reflectionX += epsilon * qCos(qDegreesToRadians(ball->angle));
        reflectionY += epsilon * qSin(qDegreesToRadians(ball->angle));
    }

    return std::make_tuple(reflectionX, reflectionY, collisionOccurred);
}

