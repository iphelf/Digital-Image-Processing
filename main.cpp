#include "dip.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DIP w;
    w.show();
    return a.exec();
}
