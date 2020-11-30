#include "tcpserverhandler.h"
#include <QHostAddress>

TcpServerHandler::TcpServerHandler(QObject *parent) : QTcpSocket(parent)
{    
    connect(this, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    // 响应套接字状态变化
    connect(this, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onStateChanged(QAbstractSocket::SocketState)));
}

// 发送QString消息
void TcpServerHandler::send(QString message)
{
    this->write(message.toLatin1(), message.length());
}

// 发送QByteArray消息
void TcpServerHandler::send(QByteArray byteArray)
{
    this->write(byteArray);
}

// 断开与客户端的连接
void TcpServerHandler::disconnectClient()
{    
    this->disconnectFromHost();
}

// 响应套接字状态变换
void TcpServerHandler::onStateChanged(QAbstractSocket::SocketState socketState)
{
    if(socketState == QAbstractSocket::UnconnectedState)
    {
        // 客户端主动断开连接
        emit clientDisconnect(this->peerAddress().toString(), this->peerPort());
    }
}

// 私有槽，用来响应readyRead信号，处理消息，发出receiveMessage信号和receiveByteArray信号
void TcpServerHandler::onReadyRead()
{
    // 当有字节可以读取的时候
    while(this->bytesAvailable())
    {
        QByteArray datagram;
        datagram.resize(this->bytesAvailable());
        this->read(datagram.data(), datagram.size());
        QString msg = datagram.data();

        // 收到数据，发射信号
        // 信号，收到某客户端的消息 QByteArray格式, ip, port
        emit receiveByteArray(datagram, this->peerAddress().toString(), this->peerPort());
        // 信号，收到某客户端的消息 QString格式, ip, port
        emit receiveString(msg, this->peerAddress().toString(), this->peerPort());

    }
}
