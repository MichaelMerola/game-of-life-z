#include "menu.h"

#include <QApplication>

// HW4
// Names: Michael Merola
//        Gustav Solis
//

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Menu m;

    m.show();

    return a.exec();
}
