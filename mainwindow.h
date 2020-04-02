#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include "processor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void numPressed();
    void mathButtonPressed();
    void equalsPressed();
    void controlsPressed();
protected:
    void keyPressEvent(QKeyEvent *k);
private:
    void sqrt();
    void pow2();
    void fraction();
    Ui::MainWindow *ui;
    double result;
    QString input;
};
#endif // MAINWINDOW_H
