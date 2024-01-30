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
#include <QLabel>

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

private slots:
    void updateFPS();
    void on_btnAddBall_clicked();

    void on_btnAddWall_clicked();

private:
    Ui::MainWindow *ui;

    QGraphicsScene *scene;
    QTimer *timer;
    QTimer *fpsTimer;
    QLabel *fpsLabel;
    int frameCount;

};
#endif // MAINWINDOW_H
