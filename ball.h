#ifndef BALL_H
#define BALL_H

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QRandomGenerator>
#include <QtMath>
#include <wall.h>
#include <QTime>
#include <QTimer>
#include <QThread>

class Ball : public QGraphicsObject
{
public:
    Ball(qreal startPosX, qreal startPosY, qreal speed, qreal angle);
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    qreal getStartPosX();
    qreal getStartPosY();
    qreal getAngle();
    qreal getSpeed();

    qreal startingPosX = 0;
    qreal startingPosY = 0;
    qreal angle = 0;
    qreal speed = 0;

public slots:
    void run();
    void compute();

protected slots:
    void advance(int step) override;
    void checkCollision();
    qreal calculateWallAngle(Wall *wall);
    void DoCollision(const QList<QLineF>& walls);

private:
    // qreal startingPosX = 0;
    // qreal startingPosY = 0;
    // qreal angle = 0;
    // qreal speed = 0;

    QTime lastTime;
    QTimer *timer;
    int frames;

};

#endif // BALL_H
