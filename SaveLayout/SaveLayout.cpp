#include "SaveLayout.h"
#include <QVBoxLayout>
#include <QSplitter>
#include <QSettings>
#include <QLabel>
#include <QFile>

SaveLayout::SaveLayout(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(500, 400);
    m_spliter = new QSplitter(Qt::Vertical, this);
    m_spliter->setStyleSheet(
        tr("QSplitter QSplitterHandle{background-color: rgb(255, 255, 255);}"));
    m_spliter->setHandleWidth(2);
    setCentralWidget(m_spliter);
    
    auto laout_up = new QVBoxLayout;
    auto up_w = new QWidget(this);
    up_w->setStyleSheet("QWidget{background-color: rgb(255, 0, 0);}");
    up_w->setMinimumHeight(150);
    up_w->setLayout(laout_up);

    auto laout_mid = new QVBoxLayout;
    auto mid_w = new QWidget(this);
    mid_w->setStyleSheet("QWidget{background-color: rgb(0, 0, 255);}");
    mid_w->setMinimumHeight(50);
    mid_w->setLayout(laout_mid);
    
    auto laout_down = new QVBoxLayout;
    auto down_w = new QWidget(this);
    down_w->setStyleSheet("QWidget{background-color: rgb(0, 255, 0);}");
    down_w->setMinimumHeight(100);
    down_w->setLayout(laout_down);
    

    m_spliter->addWidget(up_w);
    m_spliter->setStretchFactor(0, 1);
    m_spliter->addWidget(mid_w);
    m_spliter->setStretchFactor(1, 1);
    m_spliter->addWidget(down_w);  
    m_spliter->setStretchFactor(2, 1);

    // 恢复布局
    QSettings settings("MyCompany", "MyAppSplite");
    QString v = settings.fileName();
    QByteArray saved_geometry = settings.value("geometry").toByteArray();
    m_spliter->restoreGeometry(saved_geometry);
    QByteArray saved_state = settings.value("windowState").toByteArray();
    m_spliter->restoreState(saved_state);
}


void SaveLayout::closeEvent(QCloseEvent* event)
{
    // 保存布局
    QSettings settings("MyCompany", "MyAppSplite");
    settings.setValue("geometry", m_spliter->saveGeometry());
    settings.setValue("windowState", m_spliter->saveState());
    QString v = settings.fileName();
    QMainWindow::closeEvent(event);
}