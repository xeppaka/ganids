#include <QtGui/QApplication>
#include "ganidsmainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GanidsMainWindow w;
    w.show();
    
    return a.exec();
}
