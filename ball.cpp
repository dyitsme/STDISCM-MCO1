#include "ball.h"

Ball::Ball() {}

QRectF Ball::boundingRect() const
{
    return QRect(0, 0, 20, 20);
}

void Ball::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(Qt::red);
    painter->drawEllipse(0, 0, 20, 20);
}

void Ball::advance(int step)
{
    if (!step) return;
}

QPainterPath Ball::shape() const
{
    QPainterPath path;
    path.addRect(-10, -20, 20, 40);
    return path;
}


