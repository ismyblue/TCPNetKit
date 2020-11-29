#ifndef TCPSERVERHANDLER_H
#define TCPSERVERHANDLER_H

#include <QObject>
#include <QTcpSocket>

// TCP服务处理类，用来单独处于与某一个TcpClient的通信
class TcpServerHandler : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpServerHandler(QObject *parent = nullptr);

    // 发送QString消息到客户端
    void send(QString message);
    // 发送QByteArray消息到客户端
    void send(QByteArray byteArray);
    // 断开与客户端的连接
    void disconnectClient();    


signals:
    // 信号，客户端主动断开
    void clientDisconnect(QString tcpClientIp, int tcpClientPort);

    // 信号，收到某客户端的消息 QString格式
    void receiveString(QString message, QString tcpClientIP, int tcpClientPort);
    // 信号，收到某客户端的消息 QByteArray格式
    void receiveByteArray(QByteArray byteArray, QString tcpClientIP, int tcpClientPort);

private slots:
    // 私有槽，用来响应readyRead信号，处理消息，发出receiveMessage信号和receiveByteArray信号
    void onReadyRead();
    // 套接字状态改变， 响应客户端主动断开
    void onStateChanged(QAbstractSocket::SocketState socketState);

};

#endif // TCPSERVERHANDLER_H
