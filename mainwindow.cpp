#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDoubleValidator>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->screen->setText("0");
    //range, 8 decimals of precision);
    ui->screen->setValidator(
                new QDoubleValidator(-9999999999999999.9, 9999999999999999.9, 8));

    //numpad
    QPushButton *numButtons[11];

    for(int i=0;i<10;i++)
    {
        QString butName="button" + QString::number(i);
        numButtons[i]=MainWindow::findChild<QPushButton*>(butName);
        connect(numButtons[i],SIGNAL(released()),this,SLOT(numPressed()));
    }
    numButtons[10]=MainWindow::findChild<QPushButton*>("dotButton");
    connect(numButtons[10],SIGNAL(released()),this,SLOT(numPressed()));

    //basic operators
    QPushButton *plus=MainWindow::findChild<QPushButton*>("plusButton");
    connect(plus,SIGNAL(released()),this,SLOT(mathButtonPressed()));
    QPushButton *minus=MainWindow::findChild<QPushButton*>("minusButton");
    connect(minus,SIGNAL(released()),this,SLOT(mathButtonPressed()));
    QPushButton *multiplicate=MainWindow::findChild<QPushButton*>("multiplicateButton");
    connect(multiplicate,SIGNAL(released()),this,SLOT(mathButtonPressed()));
    QPushButton *division=MainWindow::findChild<QPushButton*>("divisionButton");
    connect(division,SIGNAL(released()),this,SLOT(mathButtonPressed()));
    //equals
    QPushButton *equals=MainWindow::findChild<QPushButton*>("equalsButton");
    connect(equals,SIGNAL(released()),this,SLOT(equalsPressed()));

    //unary functions
    QPushButton *sqrt=MainWindow::findChild<QPushButton*>("squareRoot");
    connect(sqrt,SIGNAL(released()),this,SLOT(mathButtonPressed()));
    QPushButton *pow2=MainWindow::findChild<QPushButton*>("pow2");
    connect(pow2,SIGNAL(released()),this,SLOT(mathButtonPressed()));
    QPushButton *fractionButton=MainWindow::findChild<QPushButton*>("fractionButton");
    connect(fractionButton,SIGNAL(released()),this,SLOT(mathButtonPressed()));
    QPushButton *piButton=MainWindow::findChild<QPushButton*>("pi");
    connect(piButton,SIGNAL(released()),this,SLOT(mathButtonPressed()));
    QPushButton *sign=MainWindow::findChild<QPushButton*>("signButton");
    connect(sign,SIGNAL(released()),this,SLOT(mathButtonPressed()));

    //controls
    QPushButton *clear=MainWindow::findChild<QPushButton*>("clear");
    connect(clear,SIGNAL(released()),this,SLOT(controlsPressed()));
    QPushButton *clearEverything=MainWindow::findChild<QPushButton*>("clearEverything");
    connect(clearEverything,SIGNAL(released()),this,SLOT(controlsPressed()));
    QPushButton *backspace=MainWindow::findChild<QPushButton*>("backspace");
    connect(backspace,SIGNAL(released()),this,SLOT(controlsPressed()));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::controlsPressed()
{
    QPushButton *button = (QPushButton*) sender();
    QString buttonValue = button->text();
    QString screenValue = ui->screen->text();
    QString inputValue = ui->calcText->toPlainText();


   if(buttonValue=="C")
   {
        screenValue.clear();
   }
   else if(buttonValue=="CE")
   {
        screenValue.clear();
        inputValue.clear();
   }
   else if(buttonValue=="⌫")
   {
        if(screenValue.isEmpty())
        {
            inputValue.chop(1);
        }
        else
        {
            screenValue.chop(1);
        }
   }
   ui->screen->setText(screenValue);
   ui->calcText->setText(inputValue);
}


void MainWindow::numPressed()
{
    QPushButton *button = (QPushButton*) sender();
    QString buttonValue = button->text();
    QString screenValue = ui->screen->text();

    bool dot=false;

    if(button->text()==",")
    {
        dot=true;
    }

    for(int i =0;i<screenValue.size();i++)
    {
        if(screenValue[i]==',')
        {
            if(dot){return;}
            screenValue[i]='.';
            dot=true;
        }
    }

    if((screenValue.toDouble()==0.0)&&(buttonValue!=','&&!dot))
    {
        ui->screen->setText(buttonValue);
    }
    else
    {
        screenValue=ui->screen->text();
        ui->screen->setText(screenValue+buttonValue);
    }

}

void MainWindow::sqrt()
{
    QString screenValue = ui->screen->text();
    QString calcTextValue = ui->calcText->toPlainText();
    ui->calcText->setText(calcTextValue+"√");
}

void MainWindow::pow2()
{
    QString screenValue = ui->screen->text();
    QString calcTextValue = ui->calcText->toPlainText();
    ui->calcText->setText(calcTextValue+screenValue+"^2");
    ui->screen->setText("");
}

void MainWindow::fraction()
{
     QString screenValue = ui->screen->text();
     for (int i=0;i<screenValue.size();i++)
     {
        if(screenValue[i]==",")
        {
            screenValue[i]='.';
        }
     }

     if(screenValue.toDouble()!=0)
     {
         screenValue=QString::number(1/screenValue.toDouble());
         for (int i=0;i<screenValue.size();i++)
         {
            if(screenValue[i]==".")
            {
                screenValue[i]=',';
            }
         }
         ui->screen->setText(screenValue);
     }

}

void MainWindow::mathButtonPressed()
{
    QPushButton *button = (QPushButton*) sender();
    QString input=ui->calcText->toPlainText();

    for (int i=0;i<input.size();i++)
    {
        if(input[i]=="=")
        {
            ui->calcText->setText("");
            break;
        }
    }
    if(button->text()=="√x")
    {
        sqrt();
    }
    else if(button->text()=="x^2")
    {
        pow2();
    }
    else if(button->text()=="1/x")
    {
        fraction();
    }
    else if(button->text()=="π")
    {
        ui->screen->setText("3.14159265359");
    }
    else if(button->text()=="+/-")
    {
        QString screen=ui->screen->text();
        if(screen[0]=="-")
        {
            screen.remove(0,1);
        }
        else
        {
            screen="-"+screen;
        }
        ui->screen->setText(screen);
    }
    else
    {
        QString screenValue = ui->screen->text();
        QString calcTextValue = ui->calcText->toPlainText();
        ui->calcText->setText(calcTextValue+screenValue+button->text());
        ui->screen->setText("0");
    }

}

void MainWindow::equalsPressed()
{
    QString screenValue = ui->screen->text();
    QString calcTextValue = ui->calcText->toPlainText();
    ui->calcText->setText(calcTextValue+screenValue);
    processor pr(calcTextValue+screenValue);
    pr.calculate();
    if(pr.getErrorMsg().isEmpty()==true)
    {
        ui->calcText->setText(ui->calcText->toPlainText()+'=');
        ui->screen->setText(pr.getResult());
    }
    else
    {
        ui->calcText->setText(pr.getErrorMsg());
        ui->screen->setText("");
    }
}

void MainWindow::keyPressEvent(QKeyEvent *k)
{
    switch (k->key())
    {
        case Qt::Key_Q:                   // quit
            QApplication::exit();
            break;
        case Qt::Key_Return:
        case Qt::Key_Enter:
        case Qt::Key_Equal:
            equalsPressed();
            break;
        case Qt::Key_Plus:
            MainWindow::findChild<QPushButton*>("plusButton")->animateClick();
            break;
        case Qt::Key_Minus:
            MainWindow::findChild<QPushButton*>("minusButton")->animateClick();
            break;
        case Qt::Key_Asterisk:
            MainWindow::findChild<QPushButton*>("multiplicateButton")->animateClick();
            break;
        case Qt::Key_Slash:
        case Qt::Key_Backslash:
            MainWindow::findChild<QPushButton*>("divisionButton")->animateClick();
            break;
        case Qt::Key_AsciiCircum:
            MainWindow::findChild<QPushButton*>("pow2")->animateClick();
            break;
        case Qt::Key_Delete:
            MainWindow::findChild<QPushButton*>("clearEverything")->animateClick();
            break;
        case Qt::Key_Backspace:
            MainWindow::findChild<QPushButton*>("backspace")->animateClick();
            break;
        case Qt::Key_Comma:
        case Qt::Key_Period:
            MainWindow::findChild<QPushButton*>("dotButton")->animateClick();
            break;
        case Qt::Key_1:
            MainWindow::findChild<QPushButton*>("button1")->animateClick();
            break;
        case Qt::Key_2:
            MainWindow::findChild<QPushButton*>("button2")->animateClick();
            break;
        case Qt::Key_3:
             MainWindow::findChild<QPushButton*>("button3")->animateClick();
            break;
        case Qt::Key_4:
            MainWindow::findChild<QPushButton*>("button4")->animateClick();
            break;
        case Qt::Key_5:
            MainWindow::findChild<QPushButton*>("button5")->animateClick();
            break;
        case Qt::Key_6:
            MainWindow::findChild<QPushButton*>("button6")->animateClick();
            break;
        case Qt::Key_7:
            MainWindow::findChild<QPushButton*>("button7")->animateClick();
            break;
        case Qt::Key_8:
            MainWindow::findChild<QPushButton*>("button8")->animateClick();;
                break;
        case Qt::Key_9:
            MainWindow::findChild<QPushButton*>("button9")->animateClick();
            break;
        case Qt::Key_0:
            MainWindow::findChild<QPushButton*>("button0")->animateClick();;
            break;
    }

}
