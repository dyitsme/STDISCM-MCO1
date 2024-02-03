#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QtCore>
#include <QtGui>
#include <QTimer>
#include <QElapsedTimer>
#include <QLabel>
#include "gamescene.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent *event) override;

protected:

private slots:
    void displayFPS();
    void on_btnAddBall_clicked();

    void on_btnAddWall_clicked();

private:
    Ui::MainWindow *ui;

    GameScene *scene;
    QTimer *timer;
    QElapsedTimer *elapsedTimer;
    QTimer *fpsTimer;
    QLabel *fpsLabel;
    int frameCount;

};
#endif // MAINWINDOW_H
