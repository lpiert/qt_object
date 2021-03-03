#include "MainWindow.h"
#include <QQmlApplicationEngine>
#include <QAction>
#include <QMenuBar>
#include <QQuickView>
#include <QDebug>
#include <QQmlContext>
#include "Chart.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    showMaximized();

    QQmlApplicationEngine* engine = new QQmlApplicationEngine();
    QQmlContext* context = engine->rootContext();
    QJsonObject opt = Chart::instance()->getOpition("line");
    context->setContextProperty("_option", opt);
    engine->load(QUrl("qrc:/index.qml"));
    auto lists = engine->rootObjects();
    if (!lists.size()) return;
    QWidget *container = QWidget::createWindowContainer(qobject_cast<QWindow*>(lists[0]));
    setCentralWidget(container);
}

