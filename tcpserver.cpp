#include "tcpserver.h"

TcpServer::TcpServer(QObject *parent) : QObject(parent)
{
    localIP = "0.0.0.0";
    localPort = 8888;
    isListen = false;
}


