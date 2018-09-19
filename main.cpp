#include "draughts.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    draughts w;
    w.show();

    return a.exec();
}
