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

signals:

};

#endif // TCPSERVERHANDLER_H
