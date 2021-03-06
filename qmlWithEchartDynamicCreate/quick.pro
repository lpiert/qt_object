ADS_OUT_ROOT = $${OUT_PWD}/../..

QT += core gui widgets quick webengine

TARGET = quick
DESTDIR = $${ADS_OUT_ROOT}/lib
TEMPLATE = app
CONFIG += c++14
CONFIG += debug_and_release
DEFINES += QT_DEPRECATED_WARNINGS

adsBuildStatic {
    DEFINES += ADS_STATIC
}

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    Chart.cpp \
    MainWindow.cpp \
    WebControl.cpp
RESOURCES += qml.qrc
QML_IMPORT_PATH =
QML_DESIGNER_IMPORT_PATH =
LIBS += -L$${ADS_OUT_ROOT}/lib

INCLUDEPATH += ../../src
DEPENDPATH += ../../src


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Chart.h \
    MainWindow.h \
    WebControl.h
