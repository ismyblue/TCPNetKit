#include "tcpnetkit.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TCPNetKit w;
    w.show();
    return a.exec();
}
