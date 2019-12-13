#include "SaveLayout.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SaveLayout w;
    w.show();
    return a.exec();
}
