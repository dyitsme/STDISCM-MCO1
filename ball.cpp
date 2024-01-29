#include "ball.h"

Ball::Ball(qreal sp)
{
    speed = sp;

    // random start position to be removed
    startingPosX = QRandomGenerator::global()->bounded(100);
    startingPosY = QRandomGenerator::global()->bounded(100);

}

QRectF Ball::boundingRect() const
{
    return QRect(0, 0, 10, 10);
}

void Ball::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(Qt::red);
    painter->drawEllipse(0, 0, 10, 10);
}

void Ball::advance(int step)
{
    if (!step) return;

    QPointF location = this->pos();

    setPos(mapToParent(0, (speed)));
}

QPainterPath Ball::shape() const
{
    QPainterPath path;
    path.addEllipse(boundingRect());
    return path;
}


