#include "tcpserver.h"
#include <QHostAddress>

TcpServer::TcpServer(QObject *parent) : QTcpServer(parent)
{
    localIP = "0.0.0.0";
    localPort = 8888;
    isListen = false;
}

// 获取本地IP
QString TcpServer::getLocalIP()
{
    return localIP;
}

// 设置本地IP
void TcpServer::setLocalIP(QString localIP)
{
    QHostAddress test;
    if(test.setAddress(localIP))
    {
        this->localIP = localIP;
    }
}

// 获取本地端口
int TcpServer::getLocalPort()
{
    return this->localPort;
}

// 设置本地端口
void TcpServer::setLocalPort(int localPort)
{
    if(0 < localPort && localPort <= 65535)
    {
        this->localPort = localPort;
    }
}

// 启动服务器
void TcpServer::startServer()
{
    QHostAddress hostAddress;
    hostAddress.setAddress(localIP);
    this->listen(hostAddress, localPort);
}

// 启动服务器
void TcpServer::startServer(QString localIP, int localPort)
{
    QHostAddress hostAddress;
    if(hostAddress.setAddress(localIP))
    {
        this->localIP = localIP;
    }
    if(0 < localPort && localPort <= 65535)
    {
        this->localPort = localPort;
    }
    this->listen(hostAddress, this->localPort);

}

// 发送数据到特定的客户端
void TcpServer::send(QString message, QString tcpClientIP)
{

}

void TcpServer::send(QByteArray byteArray, QString tcpClientIP)
{

}

void TcpServer::disconnectClient(QString tcpClientIP)
{

}

void TcpServer::slotsReadyRead()
{

}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{

}


