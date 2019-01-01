#include "dmainwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DMainWidget w;
    w.show();
    return a.exec();
}
