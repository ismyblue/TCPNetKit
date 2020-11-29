#include <QHostAddress>
#include "tcpserver.h"
#include "tcpserverhandler.h"

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

// 停止服务器
void TcpServer::stopServer()
{
    // 关闭所有handler, 并删除
    QMap<QString, TcpServerHandler*>::iterator i;
    for(i = handlerMap.begin();i != handlerMap.end();)
    {
        // 获得key
        QString handler_key = i.key();
        // 获得value
        TcpServerHandler *handler = i.value();
        i++;
        // 关闭客户端连接
        handler->disconnectClient();
        // 释放内存
        delete handler;
        // 从map中删除
        handlerMap.remove(handler_key);
    }
    // 停止服务端套接字监听
    this->close();
}

// 发送String消息到所有客户端
void TcpServer::send(QString message)
{
    // 遍历所有hanlder,分别发送
    QMap<QString, TcpServerHandler*>::iterator iter;
    for(iter = handlerMap.begin();iter != handlerMap.end();iter++)
    {
        TcpServerHandler *handler = iter.value();
        // 发送消息
        handler->send(message);
    }
}

// 发送ByteArray消息到所有客户端
void TcpServer::send(QByteArray byteArray)
{
    // 遍历所有hanlder,分别发送
    QMap<QString, TcpServerHandler*>::iterator iter;
    for(iter = handlerMap.begin();iter != handlerMap.end();iter++)
    {
        TcpServerHandler *handler = iter.value();
        // 发送消息
        handler->send(byteArray);
    }
}

// 发送QString消息给某个客户端
void TcpServer::send(QString message, QString tcpClientIP, int tcpClientPort)
{
    // 特定的客户端标识， ip:port, 如192.168.0.1:5678
    QString handler_key = QString("%1:%2").arg(tcpClientIP).arg(tcpClientPort);
    // 获取handler
    if(handlerMap.contains(handler_key))
    {
        TcpServerHandler *handler = handlerMap.value(handler_key);
        // 发送消息
        handler->send(message);
    }
}

// 发送QByteArray消息给谋个客户端
void TcpServer::send(QByteArray byteArray, QString tcpClientIP, int tcpClientPort)
{
    // 特定的客户端标识， ip:port, 如192.168.0.1:5678
    QString handler_key = QString("%1:%2").arg(tcpClientIP).arg(tcpClientPort);
    // 获取handler
    if(handlerMap.contains(handler_key))
    {
        TcpServerHandler *handler = handlerMap.value(handler_key);
        // 发送消息
        handler->send(byteArray);
    }

}

// 取消某个个客户端的连接
void TcpServer::disconnectClient(QString tcpClientIP, int tcpClientPort)
{
    // 特定的客户端标识， ip:port, 如192.168.0.1:5678
    QString handler_key = QString("%1:%2").arg(tcpClientIP).arg(tcpClientPort);
    // 删除handler
    if(handlerMap.contains(handler_key))
    {
        TcpServerHandler *handler = handlerMap.value(handler_key);
        // 断开客户端连接
        handler->disconnectClient();
        // 删除信号槽连接
        handler->disconnect();
        // 释放资源
        delete handler;
        // 删除handler
        handlerMap.remove(handler_key);
    }
}


// 重载incommintConnection， 生成handler，用来单独处理与每一个TcpClient的通信
void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    // 新建一个handler用来处理这个新进来的TcpClient通信
    TcpServerHandler *handler = new TcpServerHandler(this);
    handler->setSocketDescriptor(socketDescriptor);
    // 获取tcpClient的ip和端口
    QString tcpClientIP = handler->peerAddress().toString();
    int tcpClientPort = handler->peerPort();
    // 生成客户端标识
    QString handler_key = QString("%1:%2").arg(tcpClientIP).arg(tcpClientPort);
    // 加入handlerMap，handler池
    if(!handlerMap.contains(handler_key))
    {
        handlerMap.insert(handler_key, handler);
        // 信号连接信号，发送收到消息信号，
        //void receiveMessage(QString message, QString tcpClientIP, int tcpClientPort);
        //void receiveByteArray(QByteArray byteArray, QString tcpClientIP, int tcpClientPort);
        connect(handler, SIGNAL(receiveString(QString, QString, int)), this, SIGNAL(receiveString(QString, QString, int)));
        connect(handler, SIGNAL(receiveByteArray(QByteArray, QString, int)), this, SIGNAL(receiveByteArray(QByteArray, QString, int)));
    }


}


