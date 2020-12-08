#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "SlidePush.h"
#include <QFrame>
#include <QDebug>
#include <QLabel>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_slide_clicked()
{
    if(m_target)
    {
      m_target->close();
      m_target = nullptr;
      return;
    }
    // start pos
    QPoint start = ui->slide->geometry().topRight();

    // label test
    QLabel* label = new QLabel("here",this->centralWidget());
    label->setGeometry(QRect(start,QSize(50,20)));
    label->setStyleSheet("background-color:red;");
    qDebug() <<"label geometry:" << label->geometry();
    label->show();
    // target
    if(!m_target) m_target =  new QFrame(this->centralWidget());
    m_target->setStyleSheet("background-color:red;");
    m_target->setGeometry(QRect(start,QSize(100,100)));
    qDebug() <<"target geometry:" << m_target->geometry();

    SlidePush* slide = new SlidePush(this->centralWidget());
    slide->setTargetObject(m_target,SlidePush::RunMode::fromBottom);
    slide->slidePop();
}
