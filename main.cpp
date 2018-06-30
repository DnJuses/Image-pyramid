#include "pyramid.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    pyramid w;
    w.setWindowTitle("Image pyramid");
    w.show();

    return a.exec();
}
