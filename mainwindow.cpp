#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ball.h"
#include "wall.h"
#include "gamescene.h"
#include "QGraphicsLineItem"
#include <QPen>
#include <QColor>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    initializeInput();
    scene = new GameScene(this);
    ui->graphicsView->setScene(scene);

    // Create a label to display FPS
    fpsLabel = ui->counterfpslabel;
    fpsLabel->setText("0");



    timer = new QTimer(scene);
    QObject::connect(timer, &QTimer::timeout, scene, &GameScene::computeFPS);
    timer->start(10);

    fpsTimer = new QTimer(this);
    QObject::connect(fpsTimer, &QTimer::timeout, this, &MainWindow::displayFPS);
    fpsTimer->start(500); // Update every half-second



    QTransform transform;
    transform.scale(1, -1); // Flip the Y-axis
    // transform.scale(0.5, -0.5);
    transform.translate(0, -ui->graphicsView->height()); // Shift the scene downwards
    ui->graphicsView->setTransform(transform);




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

    threadManager = new ThreadManager();
    // threadManager->startTimer();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::displayFPS()
{
    double fps = scene->getFPS();
    fpsLabel->setText(QString::number(fps, 'f', 1));
}



int numBalls;
qreal speed, angle, startPosX, startPosY, endPosX, endPosY, endAngle, endVel;
void MainWindow::on_btnAddBall_clicked()
{
    // variables, starting position X and Y, number of balls, ball speed, ball direction
    int currentIndex = ui->comboBox->currentIndex();

    // to be deleted for testing purposes only *************************
    QThread::currentThread()->setObjectName("Main Thread");
    qInfo() << QThread::currentThread();
    // *************************************************************
    switch(currentIndex){
        case 0:
        {
            qDebug() << "index:" << currentIndex;
            startPosX = txtBallPosX->text().toDouble();
            qDebug() << "BallX:" << startPosX;
            startPosY = txtBallPosY->text().toDouble();
            qDebug() << "BallY:" << startPosY;
            //numBalls = ui->txtNumBalls->text().toInt();
            speed = txtBallStartSpeed->text().toDouble();
            qDebug() << "speed:" << speed;
            angle = txtBallStartAngle->text().toDouble();
            qDebug() << "angle:" << angle;
            numBalls = 1;
            QVector<Ball*> balls;
            QVector<Worker*> workers;
            for (int i = 0; i < numBalls; ++i) {
                Ball *ball = new Ball(startPosX, startPosY, speed, angle);
                Worker *worker = new Worker(ball);
                scene->addItem(ball);
                workers.append(worker);
            }

            threadManager->useExistingOrCreateThread(workers);

            txtBallPosX->setText("");
            txtBallPosY->setText("");
            txtBallStartSpeed->setText("");
            txtBallStartAngle->setText("");
            break;
        }
        case 1: // spawn balls within range of position
        {
            qDebug() << "index:" << currentIndex;
            startPosX = txtBallPosX->text().toDouble();
            qDebug() << "StartX:" << startPosX;
            startPosY = txtBallPosY->text().toDouble();
            qDebug() << "BallY:" << startPosY;

            endPosX = txtBallEndPosX->text().toDouble();
            qDebug() << "endX:" << endPosX;
            endPosY = txtBallEndPosY->text().toDouble();
            qDebug() << "endY:" << endPosY;


            speed = txtBallStartSpeed->text().toDouble();
            qDebug() << "speed:" << speed;
            angle = txtBallStartAngle->text().toDouble();
            qDebug() << "angle:" << angle;
            numBalls = txtNumBalls->text().toInt();
            //calculate the range

            qreal deltaX = (endPosX - startPosX) / (numBalls + 1);
            qreal deltaY = (endPosY - startPosY) / (numBalls + 1);


            // QVector<Ball*> balls;
            QVector<Ball*> balls;
            QVector<Worker*> workers;

            for (int i = 0; i < numBalls; ++i) {
                qreal currentPosX = startPosX + i * deltaX;
                qreal currentPosY = startPosY + i * deltaY;

                Ball *ball = new Ball(currentPosX, currentPosY, speed, angle);
                 Worker *worker = new Worker(ball);
                // balls.append(ball);
                scene->addItem(ball);
                workers.append(worker);
            }



            threadManager->useExistingOrCreateThread(workers);

            txtNumBalls->setText("");
            txtBallPosX->setText("");
            txtBallPosY->setText("");
            txtBallEndPosX->setText("");
            txtBallEndPosY->setText("");
            txtBallStartSpeed->setText("");
            txtBallStartAngle->setText("");
            break;
        }
        case 2: // spawn balls within range of angles
        {
            qDebug() << "index:" << currentIndex;
            startPosX = txtBallPosX->text().toDouble();
            qDebug() << "StartX:" << startPosX;
            startPosY = txtBallPosY->text().toDouble();
            qDebug() << "BallY:" << startPosY;


            speed = txtBallStartSpeed->text().toDouble();
            qDebug() << "speed:" << speed;
            angle = txtBallStartAngle->text().toDouble();
            qDebug() << "angle:" << angle;
            endAngle = txtBallEndAngle->text().toDouble();
            qDebug() << "endangle:" << endAngle;

            numBalls = txtNumBalls->text().toInt();

            //calculate the range

            QVector<Ball*> balls;
            QVector<Worker*> workers;

            qreal deltaAngle = (endAngle - angle) / (numBalls + 1);
            for (int i = 0; i < numBalls; ++i) {
                qreal newAngle = angle + i * deltaAngle;
                qInfo() << "New angle " << newAngle;
                Ball *ball = new Ball(startPosX, startPosY, speed, newAngle);
                Worker *worker = new Worker(ball);

                scene->addItem(ball);
                workers.append(worker);
                // timer start
                // timer end
            }


            threadManager->useExistingOrCreateThread(workers);

            txtNumBalls->setText("");
            txtBallPosX->setText("");
            txtBallPosY->setText("");
            txtBallStartSpeed->setText("");
            txtBallStartAngle->setText("");
            txtBallEndAngle->setText("");
            break;
        }
        case 3: // spawn balls within range of velocities
        {
            qDebug() << "index:" << currentIndex;
            startPosX = txtBallPosX->text().toDouble();
            qDebug() << "StartX:" << startPosX;
            startPosY = txtBallPosY->text().toDouble();
            qDebug() << "BallY:" << startPosY;


            speed = txtBallStartSpeed->text().toDouble();
            qDebug() << "speed:" << speed;
            endVel = txtBallEndSpeed->text().toDouble();
            qDebug() << "endspeed:" << endVel;
            angle = txtBallStartAngle->text().toDouble();
            qDebug() << "angle:" << angle;


            numBalls = txtNumBalls->text().toInt();
            // calculate range
            QVector<Ball*> balls;
            QVector<Worker*> workers;
            qreal deltaVel = (endVel - speed) / (numBalls + 1);
            for (int i = 0; i < numBalls; ++i) {
                qreal newVel = speed + i * deltaVel;
                Ball *ball = new Ball(startPosX, startPosY, newVel, angle);
                Worker *worker = new Worker(ball);
                scene->addItem(ball);
                workers.append(worker);
            }

            threadManager->useExistingOrCreateThread(workers);

            txtNumBalls->setText("");
            txtBallPosX->setText("");
            txtBallPosY->setText("");
            txtBallStartSpeed->setText("");
            txtBallStartAngle->setText("");
            txtBallEndSpeed->setText("");
            break;
        }

    }



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


void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    QLayoutItem *item;

    while ((item = ui->ballGrid->takeAt(0)) != nullptr) {
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
    QLabel *label;

    QPushButton *button;


    // Based on the index, we adjust the grid layout
    switch (index) {
    case 0: // First setting for individual ball
        label = new QLabel("Ball settings");
        ui->ballGrid->addWidget(label, 0, 0);

        label = new QLabel("Starting position (X)");
        txtBallPosX = new QLineEdit();
         txtBallPosX->setObjectName("txtBallPosX");
        ui->ballGrid->addWidget(label, 1, 0);
        ui->ballGrid->addWidget(txtBallPosX, 1, 1);


        label = new QLabel("Starting position (Y)");
       txtBallPosY = new QLineEdit();
        txtBallPosY->setObjectName("txtBallPosY");
        ui->ballGrid->addWidget(label, 2, 0);
        ui->ballGrid->addWidget(txtBallPosY, 2, 1);



        label = new QLabel("Ball velocity (p/s)");
        txtBallStartSpeed = new QLineEdit();
        txtBallStartSpeed->setObjectName("txtBallStartSpeed");
        ui->ballGrid->addWidget(label, 3, 0);
        ui->ballGrid->addWidget(txtBallStartSpeed, 3, 1);


        label = new QLabel("Ball direction (Θ)");
        txtBallStartAngle = new QLineEdit();
        txtBallStartAngle->setObjectName("txtBallStartAngle");
        ui->ballGrid->addWidget(label, 4, 0);
        ui->ballGrid->addWidget(txtBallStartAngle, 4, 1);


        button = new QPushButton("Add Ball");
        ui->ballGrid->addWidget(button, 5, 0, 1, 2); // Span two columns


        button->setObjectName("btnAddBall");
        connect(button, &QPushButton::clicked, this, &MainWindow::on_btnAddBall_clicked);
        break;

    case 1: // Second setting
        label = new QLabel("Ball settings");
        ui->ballGrid->addWidget(label, 0, 0);

        label = new QLabel("Number of balls (n)");
        txtNumBalls = new QLineEdit();
        txtNumBalls->setObjectName("txtNumBalls");
        ui->ballGrid->addWidget(label, 1, 0);
        ui->ballGrid->addWidget(txtNumBalls, 1, 1);


        label = new QLabel("Starting Position (X)");
        txtBallPosX = new QLineEdit();
        txtBallPosX->setObjectName("txtBallPosX");
        ui->ballGrid->addWidget(label, 2, 0);
        ui->ballGrid->addWidget(txtBallPosX, 2, 1);


        label = new QLabel("Starting Position (Y)");
        txtBallPosY = new QLineEdit();
        txtBallPosY->setObjectName("txtBallPosY");
        ui->ballGrid->addWidget(label, 3, 0);
        ui->ballGrid->addWidget(txtBallPosY, 3, 1);


        label = new QLabel("Ending Position (X)");
        txtBallEndPosX = new QLineEdit();
        txtBallEndPosX->setObjectName("txtBallEndPosX");
        ui->ballGrid->addWidget(label, 4, 0);
        ui->ballGrid->addWidget(txtBallEndPosX, 4, 1);


        label = new QLabel("Ending Position (Y)");
        txtBallEndPosY = new QLineEdit();
        txtBallEndPosY->setObjectName("txtBallEndPosY");
        ui->ballGrid->addWidget(label, 5, 0);
        ui->ballGrid->addWidget(txtBallEndPosY, 5, 1);

        label = new QLabel("Angle (Θ)");
        txtBallStartAngle = new QLineEdit();
        txtBallStartAngle->setObjectName("txtBallStartAngle");
        ui->ballGrid->addWidget(label, 6, 0);
        ui->ballGrid->addWidget(txtBallStartAngle , 6, 1);

        label = new QLabel("Velocity (p/s)");
        txtBallStartSpeed = new QLineEdit();
        txtBallStartSpeed->setObjectName("txtBallStartSpeed");
        ui->ballGrid->addWidget(label, 7, 0);
        ui->ballGrid->addWidget(txtBallStartSpeed, 7, 1);


        button = new QPushButton("Add Ball");
        ui->ballGrid->addWidget(button, 8, 0, 1, 2); // Span two columns
        button->setObjectName("btnAddBall");
        connect(button, &QPushButton::clicked, this, &MainWindow::on_btnAddBall_clicked);
        break;

    case 2: // third setting
        label = new QLabel("Ball settings");
        ui->ballGrid->addWidget(label, 0, 0);

        label = new QLabel("Number of balls (n)");
        txtNumBalls = new QLineEdit();
        txtNumBalls->setObjectName("txtNumBalls");
        ui->ballGrid->addWidget(label, 1, 0);
        ui->ballGrid->addWidget(txtNumBalls, 1, 1);


        label = new QLabel("Starting Position (X)");
        txtBallPosX = new QLineEdit();
        txtBallPosX->setObjectName("txtBallPosX");
        ui->ballGrid->addWidget(label, 2, 0);
        ui->ballGrid->addWidget(txtBallPosX, 2, 1);


        label = new QLabel("Starting Position (Y)");
        txtBallPosY = new QLineEdit();
        txtBallPosY->setObjectName("txtBallPosY");
        ui->ballGrid->addWidget(label, 3, 0);
        ui->ballGrid->addWidget(txtBallPosY, 3, 1);


        label = new QLabel("Starting Angle (Θ)");
        txtBallStartAngle = new QLineEdit();
        txtBallStartAngle->setObjectName("txtBallStartAngle");
        ui->ballGrid->addWidget(label, 4, 0);
        ui->ballGrid->addWidget(txtBallStartAngle , 4, 1);


        label = new QLabel("Ending Angle (Θ)");
        txtBallEndAngle = new QLineEdit();
        txtBallEndAngle->setObjectName("txtBallEndAngle");
        ui->ballGrid->addWidget(label, 5, 0);
        ui->ballGrid->addWidget(txtBallEndAngle, 5, 1);

        label = new QLabel("Velocity (p/s)");
        txtBallStartSpeed = new QLineEdit();
        txtBallStartSpeed->setObjectName("txtBallStartSpeed");
        ui->ballGrid->addWidget(label, 6, 0);
        ui->ballGrid->addWidget(txtBallStartSpeed, 6, 1);


        button = new QPushButton("Add Ball");
        ui->ballGrid->addWidget(button, 7, 0, 1, 2); // Span two columns
        connect(button, &QPushButton::clicked, this, &MainWindow::on_btnAddBall_clicked);
        break;

    case 3: // fourth setting
        label = new QLabel("Ball settings");
        ui->ballGrid->addWidget(label, 0, 0);

        label = new QLabel("Number of balls (n)");
        txtNumBalls = new QLineEdit();
        ui->ballGrid->addWidget(label, 1, 0);
        ui->ballGrid->addWidget(txtNumBalls, 1, 1);
        txtNumBalls->setObjectName("txtNumBalls");

        label = new QLabel("Starting Position (X)");
        txtBallPosX = new QLineEdit();
        ui->ballGrid->addWidget(label, 2, 0);
        ui->ballGrid->addWidget(txtBallPosX, 2, 1);
        txtBallPosX->setObjectName("txtBallPosX");

        label = new QLabel("Starting Position (Y)");
        txtBallPosY = new QLineEdit();
        ui->ballGrid->addWidget(label, 3, 0);
        ui->ballGrid->addWidget(txtBallPosY, 3, 1);
        txtBallPosY->setObjectName("txtBallPosY");

        label = new QLabel("Starting velocity (p/s)");
        txtBallStartSpeed = new QLineEdit();
        ui->ballGrid->addWidget(label, 4, 0);
        ui->ballGrid->addWidget(txtBallStartSpeed, 4, 1);
        txtBallStartSpeed->setObjectName("txtBallStartSpeed");

        label = new QLabel("Ending velocity (p/s)");
        txtBallEndSpeed = new QLineEdit();
        ui->ballGrid->addWidget(label, 5, 0);
        ui->ballGrid->addWidget(txtBallEndSpeed, 5, 1);
        txtBallEndSpeed->setObjectName("txtBallEndSpeed");

        label = new QLabel("Angle (Θ)");
        txtBallStartAngle = new QLineEdit();
        txtBallStartAngle->setObjectName("txtBallStartAngle");
        ui->ballGrid->addWidget(label, 6, 0);
        ui->ballGrid->addWidget(txtBallStartAngle, 6, 1);
        button = new QPushButton("Add Ball");
        ui->ballGrid->addWidget(button, 7, 0, 1, 2); // Span two columns
        connect(button, &QPushButton::clicked, this, &MainWindow::on_btnAddBall_clicked);
        break;
    }
}

void MainWindow::initializeInput()
{
    QLayoutItem *item;
    QLabel *label;
    QPushButton *button;

    while ((item = ui->ballGrid->takeAt(0)) != nullptr) {
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
    label = new QLabel("Ball settings");
    ui->ballGrid->addWidget(label, 0, 0);

    label = new QLabel("Starting position (X)");
    txtBallPosX = new QLineEdit();
    txtBallPosX->setObjectName("txtBallPosX");
    ui->ballGrid->addWidget(label, 1, 0);
    ui->ballGrid->addWidget(txtBallPosX, 1, 1);


    label = new QLabel("Starting position (Y)");
    txtBallPosY = new QLineEdit();
    txtBallPosY->setObjectName("txtBallPosY");
    ui->ballGrid->addWidget(label, 2, 0);
    ui->ballGrid->addWidget(txtBallPosY, 2, 1);



    label = new QLabel("Ball velocity (p/s)");
    txtBallStartSpeed = new QLineEdit();
    txtBallStartSpeed->setObjectName("txtBallStartSpeed");
    ui->ballGrid->addWidget(label, 3, 0);
    ui->ballGrid->addWidget(txtBallStartSpeed, 3, 1);


    label = new QLabel("Ball direction (Θ)");
    txtBallStartAngle = new QLineEdit();
    txtBallStartAngle->setObjectName("txtBallStartAngle");
    ui->ballGrid->addWidget(label, 4, 0);
    ui->ballGrid->addWidget(txtBallStartAngle, 4, 1);


    button = new QPushButton("Add Ball");
    ui->ballGrid->addWidget(button, 5, 0, 1, 2); // Span two columns


    button->setObjectName("btnAddBall");
    connect(button, &QPushButton::clicked, this, &MainWindow::on_btnAddBall_clicked);
}
