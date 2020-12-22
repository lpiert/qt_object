#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QUrl>
#include "DockManager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
signals:
    void changed();
private slots:
     void onTriggered(bool);
private:
    ads::CDockManager* m_DockManager = nullptr;
    int m_index= 0;
    QAction *m_action;
protected:
    virtual void closeEvent(QCloseEvent *event) override
    {
        QMainWindow::closeEvent(event);
        if (m_DockManager)
        {
            // 保存布局
            QSettings settings("user/layout.ini", QSettings::Format::IniFormat);
            m_DockManager->savePerspectives(settings);
            m_DockManager->deleteLater();
        }
    }
    virtual void resizeEvent(QResizeEvent *event) override
    {
        QMainWindow::resizeEvent(event);
        emit changed();
    }
public:
    explicit MainWindow(QWidget* parent = nullptr);
    inline void setDockManager(ads::CDockManager* DockManager)
    {
        m_DockManager = DockManager;
        if(m_DockManager)
        {
            // 加载布局
            QSettings load_setting("user/layout.ini", QSettings::Format::IniFormat);
            m_DockManager->loadPerspectives(load_setting);
            load_setting.endGroup();
        }
    }
};


#endif // MAINWINDOW_H
