#include "GameController.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GameController w;
    w.show();
    return a.exec();
}
