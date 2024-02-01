#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ball.h"
#include "wall.h"
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

    // Create a label to display FPS
    fpsLabel = ui->counterfpslabel;
    fpsLabel->setText("0");

    // Setup a timer to update FPS
    fpsTimer = new QTimer(this);
    connect(fpsTimer, SIGNAL(timeout()), this, SLOT(updateFPS()));
    fpsTimer->start(500); // Update every half-second

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), scene, SLOT(advance()));
    timer->start(10);


    // elapsedTimer = new QElapsedTimer();
    // elapsedTimer->start();
    // qDebug() << "elapsedTimer" << (double)elapsedTimer->elapsed()/1000.00;

    // Set the transformation to flip the Y-axis
    QTransform transform;
    transform.scale(1, -1); // Flip the Y-axis
    transform.translate(0, -ui->graphicsView->height()); // Shift the scene downwards
    ui->graphicsView->setTransform(transform);


    // Set the pen to a transparent color
    //QPen pen(Qt::transparent);



    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    int b = 1;
    // creating walls on the borders
    Wall *bottomwall = new Wall(0-b, 0-b, 1280+b, 0-b);
    Wall *topwall = new Wall(0-b, 720+b, 1280+b, 720+b);
    Wall *leftwall = new Wall(0-b, 0-b, 0-b, 720+b);
    Wall *rightwall = new Wall(1280+b, 0-b, 1280+b, 720+b);
    scene->addItem(topwall);
    scene->addItem(bottomwall);
    scene->addItem(leftwall);
    scene->addItem(rightwall);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateFPS()
{
    // Calculate FPS based on frame count and time elapsed
    static qint64 lastTime = 0;
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    qint64 elapsedTime = currentTime - lastTime;
    lastTime = currentTime;


    int frames = frameCount;
    frameCount = 0;

    // Calculate FPS
    double fps = (frames * 1000.0) / elapsedTime;

    // Update the FPS label
    fpsLabel->setText("r: " + QString::number(fps, 'f', 1));}

void MainWindow::paintEvent(QPaintEvent *event)
{
    // Increment frame count
    ++frameCount;
    // qDebug() << "frameCount: " << frameCount;

    // if (timer->elapsed() >= 1000)
    // {

    //     double fps = frameCount / ((double)frameTime.elapsed()/1000.0);

    // }


    // Your painting code goes here
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

    // Inverting the Y-coordinates to match QGraphicsView's coordinate system

    // wallY1 = 720 - wallY1;
    // wallY2 = 720 - wallY2;

    // Create a line representing the wall and add it to the scene
    Wall *wall = new Wall(wallX1, wallY1, wallX2, wallY2);
    scene->addItem(wall);
    // Clearing the input fields
    ui->wallX1->setText("");
    ui->wallY1->setText("");
    ui->wallX2->setText("");
    ui->wallY2->setText("");
}

