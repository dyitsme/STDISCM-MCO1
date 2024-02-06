#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ball.h"
#include "wall.h"
#include "gamescene.h"
#include "QGraphicsLineItem"
#include <QPen>
#include <QColor>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new GameScene(this);
    ui->graphicsView->setScene(scene);

    // Create a label to display FPS
    fpsLabel = ui->counterfpslabel;
    fpsLabel->setText("0");

    // Setup a timer to update FPS



    // moves the balls and computes for the fps of the scene, is dependent on the timer
    timer = new QTimer(scene);
    QObject::connect(timer, &QTimer::timeout, scene, &GameScene::advance);
    QObject::connect(timer, &QTimer::timeout, scene, &GameScene::computeFPS);
    timer->start(10);

    fpsTimer = new QTimer(this);
    QObject::connect(fpsTimer, &QTimer::timeout, this, &MainWindow::displayFPS);
    fpsTimer->start(500); // Update every half-second


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

void MainWindow::displayFPS()
{
    double fps = scene->getFPS();
    fpsLabel->setText(QString::number(fps, 'f', 1));
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    // Increment frame count
    ++frameCount;
}

int numBalls;
qreal speed, angle, startPosX, startPosY;

void MainWindow::on_btnAddBall_clicked()
{
    // variables, starting position X and Y, number of balls, ball speed, ball direction
    startPosX = ui->txtBallPosX->text().toInt();
    startPosY = ui->txtBallPosY->text().toInt();
    //numBalls = ui->txtNumBalls->text().toInt();
    speed = ui->txtBallSpeed->text().toDouble();
    angle = ui->txtBallAngle->text().toDouble();
    numBalls = 1;

    for (int i = 0; i < numBalls; ++i) {
        Ball *ball = new Ball(startPosX, startPosY, speed, angle);
        scene->addItem(ball);
    }
    // clearing of input fields

    ui->txtBallPosX->setText("");
    ui->txtBallPosY->setText("");
    //ui->txtNumBalls->setText("");
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
    QLineEdit *lineEdit;
    QPushButton *button;


    // Based on the index, we adjust the grid layout
    switch (index) {
    case 0: // First setting for individual ball
        label = new QLabel("Ball settings");
        ui->ballGrid->addWidget(label, 0, 0);

        label = new QLabel("Starting position (X)");
        lineEdit = new QLineEdit();
        ui->ballGrid->addWidget(label, 1, 0);
        ui->ballGrid->addWidget(lineEdit, 1, 1);
        lineEdit->setObjectName("txtBallPosX");

        label = new QLabel("Starting position (Y)");
        lineEdit = new QLineEdit();
        ui->ballGrid->addWidget(label, 2, 0);
        ui->ballGrid->addWidget(lineEdit, 2, 1);
        lineEdit->setObjectName("txtBallPosY");


        label = new QLabel("Ball velocity (p/s)");
        lineEdit = new QLineEdit();
        ui->ballGrid->addWidget(label, 3, 0);
        ui->ballGrid->addWidget(lineEdit, 3, 1);
        lineEdit->setObjectName("txtBallSpeed");

        label = new QLabel("Ball direction (Θ)");
        lineEdit = new QLineEdit();
        ui->ballGrid->addWidget(label, 4, 0);
        ui->ballGrid->addWidget(lineEdit, 4, 1);
        lineEdit->setObjectName("txtBallAngle");

        button = new QPushButton("Add Ball");
        ui->ballGrid->addWidget(button, 5, 0, 1, 2); // Span two columns


        button->setObjectName("btnAddBall");
        connect(button, &QPushButton::clicked, this, &MainWindow::on_btnAddBall_clicked);
        break;

    case 1: // Second setting
        label = new QLabel("Ball settings");
        ui->ballGrid->addWidget(label, 0, 0);

        label = new QLabel("Number of balls (n)");
        lineEdit = new QLineEdit();
        ui->ballGrid->addWidget(label, 1, 0);
        ui->ballGrid->addWidget(lineEdit, 1, 1);
        lineEdit->setObjectName("txtNumBalls");

        label = new QLabel("Starting Position (X)");
        lineEdit = new QLineEdit();
        ui->ballGrid->addWidget(label, 2, 0);
        ui->ballGrid->addWidget(lineEdit, 2, 1);
        lineEdit->setObjectName("txtBallPosX");

        label = new QLabel("Starting Position (Y)");
        lineEdit = new QLineEdit();
        ui->ballGrid->addWidget(label, 3, 0);
        ui->ballGrid->addWidget(lineEdit, 3, 1);
        lineEdit->setObjectName("txtBallPosY");

        label = new QLabel("Ending Position (X)");
        lineEdit = new QLineEdit();
        ui->ballGrid->addWidget(label, 4, 0);
        ui->ballGrid->addWidget(lineEdit, 4, 1);
        lineEdit->setObjectName("txtBallEndPosX");

        label = new QLabel("Ending Position (Y)");
        lineEdit = new QLineEdit();
        ui->ballGrid->addWidget(label, 5, 0);
        ui->ballGrid->addWidget(lineEdit, 5, 1);
        lineEdit->setObjectName("txtBallEndPosY");

        button = new QPushButton("Add Ball");
        ui->ballGrid->addWidget(button, 6, 0, 1, 2); // Span two columns
        lineEdit->setObjectName("btnAddBall");
        connect(button, &QPushButton::clicked, this, &MainWindow::on_btnAddBall_clicked);
        break;

    case 2: // third setting
        label = new QLabel("Ball settings");
        ui->ballGrid->addWidget(label, 0, 0);

        label = new QLabel("Number of balls (n)");
        lineEdit = new QLineEdit();
        ui->ballGrid->addWidget(label, 1, 0);
        ui->ballGrid->addWidget(lineEdit, 1, 1);

        label = new QLabel("Starting Position (X)");
        lineEdit = new QLineEdit();
        ui->ballGrid->addWidget(label, 2, 0);
        ui->ballGrid->addWidget(lineEdit, 2, 1);

        label = new QLabel("Starting Position (Y)");
        lineEdit = new QLineEdit();
        ui->ballGrid->addWidget(label, 3, 0);
        ui->ballGrid->addWidget(lineEdit, 3, 1);

        label = new QLabel("Starting Angle (Θ)");
        lineEdit = new QLineEdit();
        ui->ballGrid->addWidget(label, 4, 0);
        ui->ballGrid->addWidget(lineEdit, 4, 1);

        label = new QLabel("Ending Angle (Θ)");
        lineEdit = new QLineEdit();
        ui->ballGrid->addWidget(label, 5, 0);
        ui->ballGrid->addWidget(lineEdit, 5, 1);

        button = new QPushButton("Add Ball");
        ui->ballGrid->addWidget(button, 6, 0, 1, 2); // Span two columns
        connect(button, &QPushButton::clicked, this, &MainWindow::on_btnAddBall_clicked);
        break;

    case 3: // fourth setting
        label = new QLabel("Ball settings");
        ui->ballGrid->addWidget(label, 0, 0);

        label = new QLabel("Number of balls (n)");
        lineEdit = new QLineEdit();
        ui->ballGrid->addWidget(label, 1, 0);
        ui->ballGrid->addWidget(lineEdit, 1, 1);

        label = new QLabel("Starting Position (X)");
        lineEdit = new QLineEdit();
        ui->ballGrid->addWidget(label, 2, 0);
        ui->ballGrid->addWidget(lineEdit, 2, 1);

        label = new QLabel("Starting Position (Y)");
        lineEdit = new QLineEdit();
        ui->ballGrid->addWidget(label, 3, 0);
        ui->ballGrid->addWidget(lineEdit, 3, 1);

        label = new QLabel("Starting velocity (p/s)");
        lineEdit = new QLineEdit();
        ui->ballGrid->addWidget(label, 4, 0);
        ui->ballGrid->addWidget(lineEdit, 4, 1);

        label = new QLabel("Ending velocity (p/s)");
        lineEdit = new QLineEdit();
        ui->ballGrid->addWidget(label, 5, 0);
        ui->ballGrid->addWidget(lineEdit, 5, 1);

        button = new QPushButton("Add Ball");
        ui->ballGrid->addWidget(button, 6, 0, 1, 2); // Span two columns
        connect(button, &QPushButton::clicked, this, &MainWindow::on_btnAddBall_clicked);
        break;
    }
}

