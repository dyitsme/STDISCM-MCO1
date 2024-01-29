#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ball.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int numBalls;

void MainWindow::on_btnAddBall_clicked()
{
    // variables, starting position X and Y, number of balls, ball speed, ball direction
    numBalls = ui->txtNumBalls->text().toInt();
    Ball *ball = new Ball();
    scene->addItem(ball);
}


