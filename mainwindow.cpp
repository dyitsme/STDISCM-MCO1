#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ball.h"
#include "QGraphicsLineItem"
#include <QPen>
#include <QColor>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), scene, SLOT(advance()));
    timer->start(10);

    // Set the transformation to flip the Y-axis
    QTransform transform;
    transform.scale(1, -1); // Flip the Y-axis
    transform.translate(0, -ui->graphicsView->height()); // Shift the scene downwards
    ui->graphicsView->setTransform(transform);
    QGraphicsLineItem *invisibleLine = new QGraphicsLineItem(0, 0, 1280, 720);

    // Set the pen to a transparent color
    QPen pen(Qt::transparent);
    invisibleLine->setPen(pen);

    scene->addItem(invisibleLine);

    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int numBalls;
qreal speed, angle, startPosX, startPosY;

void MainWindow::on_btnAddBall_clicked()
{
    // variables, starting position X and Y, number of balls, ball speed, ball direction
    startPosX = ui->txtBallPosX->text().toInt();
    startPosY = ui->txtBallPosX->text().toInt();
    numBalls = ui->txtNumBalls->text().toInt();
    speed = ui->txtBallSpeed->text().toDouble();
    angle = ui->txtBallAngle->text().toDouble();

    for (int i = 0; i < numBalls; ++i) {
        Ball *ball = new Ball(startPosX, startPosY, speed, angle);
        scene->addItem(ball);
    }
    // clearing of input fields

    ui->txtBallPosX->setText("");
    ui->txtBallPosY->setText("");
    ui->txtNumBalls->setText("");
    ui->txtBallSpeed->setText("");
    ui->txtBallAngle->setText("");

}


int wallX1, wallX2 ,wallY1, wallY2, wallSlope;
void MainWindow::on_btnAddWall_clicked() {
    wallX1 = ui->wallX1->text().toInt();
    wallY1 = ui->wallY1->text().toInt();
    wallX2 = ui->wallX2->text().toInt();
    wallY2 = ui->wallY2->text().toInt();
    wallSlope = ui->wallSlope->text().toInt();

    // Inverting the Y-coordinates to match QGraphicsView's coordinate system

    // wallY1 = 720 - wallY1;
    // wallY2 = 720 - wallY2;

    // Create a line representing the wall and add it to the scene
    QGraphicsLineItem *wall = new QGraphicsLineItem(wallX1, wallY1, wallX2, wallY2);
    scene->addItem(wall);
    // Clearing the input fields
    ui->wallX1->setText("");
    ui->wallY1->setText("");
    ui->wallX2->setText("");
    ui->wallY2->setText("");
    ui->wallSlope->setText("");
}

