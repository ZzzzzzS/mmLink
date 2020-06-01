#include "mmLink_RadarSimulation.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mmLink_RadarSimulation w;
    w.show();

    return a.exec();
}
