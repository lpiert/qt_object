#include "MainWindow.h"
#include <QQmlApplicationEngine>
#include <QAction>
#include <QMenuBar>
#include <QQuickView>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    m_DockManager = new ads::CDockManager(this);
    setCentralWidget(m_DockManager);
#if 0
    connect(m_DockManager, &ads::CDockManager::focusedDockWidgetChanged,
                     [] (ads::CDockWidget* old, ads::CDockWidget* now) {
        static int Count = 0;
        qDebug() << Count++ << " CDockManager::focusedDockWidgetChanged old: "
                 << (old ? old->objectName() : "-") << " now: "
                 << now->objectName() << " visible: " << now->isVisible();
        now->widget()->setFocus();
    });
#endif
    m_action = new QAction("New", this);
    menuBar()->addAction(m_action);
    connect(m_action, &QAction::triggered, this,&MainWindow::onTriggered);
    showMaximized();
}

void MainWindow::onTriggered(bool)
{
    auto dw = new ads::CDockWidget(QStringLiteral("test %1").arg(m_index++), this);
#if 0
    QQuickView* view = new QQuickView();
    view->show();
    view->setResizeMode (QQuickView::SizeViewToRootObject);
    view->setSource (QUrl("qrc:/main.qml"));
    QWidget *container = QWidget::createWindowContainer(view);
#else
    QQmlApplicationEngine* engine = new QQmlApplicationEngine();
    engine->load(QUrl("qrc:/main.qml"));
    QWindow *qmlWindow = qobject_cast<QWindow*>(engine->rootObjects().at(0));
    QWidget *container = QWidget::createWindowContainer(qmlWindow);
    dw->setWidget(container);
#endif
    auto area = m_DockManager->addDockWidgetTab(ads::CenterDockWidgetArea, dw);
    qDebug() << "dock widget created!" << dw << area;
}
