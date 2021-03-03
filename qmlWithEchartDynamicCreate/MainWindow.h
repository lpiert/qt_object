#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QUrl>

class MainWindow : public QMainWindow
{
    Q_OBJECT
signals:
    void changed();
private:
protected:
    virtual void closeEvent(QCloseEvent *event) override
    {
        QMainWindow::closeEvent(event);
    }
    virtual void resizeEvent(QResizeEvent *event) override
    {
        QMainWindow::resizeEvent(event);
        emit changed();
    }
public:
    explicit MainWindow(QWidget* parent = nullptr);
};


#endif // MAINWINDOW_H
