#include "PacQt.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PacQt w;
    w.show();
    return a.exec();
}
