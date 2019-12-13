#pragma once

#include <QtWidgets/QMainWindow>
class QSplitter;
class SaveLayout : public QMainWindow
{
    Q_OBJECT

public:
    SaveLayout(QWidget *parent = Q_NULLPTR);
    virtual void closeEvent(QCloseEvent* event) override;
private:
    QSplitter* m_spliter;
};
