#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>

// TCP服务端类，用来启动TCP监听服务
class TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);

signals:

};

#endif // TCPSERVER_H
