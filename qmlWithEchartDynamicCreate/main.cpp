#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtWebEngine>
#include <QApplication>
#include "WebControl.h"
#include "MainWindow.h"

// 获取单例
WebControl* getMySingleton(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return WebControl::instance();
}


// 注册单例到QML
void qmlRegisterMySingleton()
{
    qmlRegisterSingletonType<WebControl>(
                "WebControl", 1, 0, "WebControl", &getMySingleton);
}


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QtWebEngine::initialize();
    qmlRegisterMySingleton();
    QApplication app(argc, argv);
    MainWindow w;
    w.show();

    return app.exec();
}
