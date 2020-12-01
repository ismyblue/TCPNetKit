#include "tcpclient.h"
#include <QHostInfo>
#include <QHostAddress>
#include <QMessageBox>
#include <QException>

TcpClient::TcpClient(QObject *parent) : QTcpSocket(parent)
{
    // 设置默认ip和端口
    remoteIP = "0.0.0.0";
    remotePort = 8888;
    // 设置服务器连接状态
    isConnected = false;
    // 连接信号槽
    connect(this, &TcpClient::readyRead, this, &TcpClient::onReadyRead);
    connect(this, &TcpClient::stateChanged, this, &TcpClient::onClientStateChanged);
}

// 获取远程IP
QString TcpClient::getRemoteIP()
{
    return remoteIP;
}

// 设置远程IP
void TcpClient::setRemoteIP(QString remoteIP)
{
    QHostAddress test;
    // 只设置合法ip
    if(test.setAddress(remoteIP))
    {
        this->remoteIP = remoteIP;
    }
}

// 获取远程端口
int TcpClient::getRemotePort()
{
    return remotePort;
}

// 设置远程端口
void TcpClient::setRemotePort(int remotePort)
{
    if(0 < remotePort && remotePort <= 65535)
    {
        this->remotePort = remotePort;
    }

}

// 连接远程服务器
void TcpClient::connectServer()
{
    // 如果还没连接
    if(!isConnected)
    {
        QHostAddress serverAddress;
        serverAddress.setAddress(remoteIP);
        // 尝试连接
        this->connectToHost(serverAddress, remotePort);        
    }
}

// 连接远程服务器，指定ip和端口
void TcpClient::connectServer(QString remoteIP, int remotePort)
{
    // 如果还没连接
    if(!isConnected)
    {
        QHostAddress serverAddress;
        if(serverAddress.setAddress(remoteIP))
        {
            this->remoteIP = remoteIP;
        }
        if(0 < remotePort && remotePort <= 65535)
        {
            this->remotePort = remotePort;
        }
        // 连接服务器
        this->connectToHost(serverAddress, remotePort);
        // 修改连接状态
        isConnected = true;
    }
}

// 发送消息
void TcpClient::send(QString message)
{
    this->write(message.toLatin1(), message.length());
}

// 发送消息
void TcpClient::send(QByteArray byteArray)
{
    this->write(byteArray);
}

// 与服务器断开连接
void TcpClient::disconnectServer()
{
    // 如果已经连接
    if(isConnected)
    {
        // 尝试断开连接
        this->disconnectFromHost();        
    }

}

// 私有槽，响应准备读取消息信号，处理数据之后发送receiveString和receiveByteArray信号
void TcpClient::onReadyRead()
{
    while(this->bytesAvailable())
    {
        QByteArray datagram;
        datagram.resize(this->bytesAvailable());
        this->read(datagram.data(), datagram.size());
        QString msg = datagram.data();

        // 收到数据，发射信号
        emit receiveByteArray(datagram);
        emit receiveString(msg);
    }

}

// 响应状态改变
void TcpClient::onClientStateChanged(QAbstractSocket::SocketState socketState)
{
    // 套接字连接成功
    if(QAbstractSocket::ConnectedState == socketState)
    {
       isConnected = true;
    }
    else if(QAbstractSocket::UnconnectedState == socketState)
    {
       isConnected = false;
    }

}
