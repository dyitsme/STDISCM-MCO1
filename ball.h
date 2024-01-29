#ifndef BALL_H
#define BALL_H

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>

class Ball : public QGraphicsItem
{
public:
    Ball();
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
protected:
    void advance(int step) override;

private:
    qreal startingPosX = 0;
    qreal startingPosY = 0;
    qreal angle = 0;
    qreal speed = 0;
};

#endif // BALL_H
