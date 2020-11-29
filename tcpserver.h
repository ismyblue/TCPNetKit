#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QMap>
#include <QTcpServer>
#include "tcpserverhandler.h"

// TCP服务端类，用来启动TCP监听服务
class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);
    // 获取本地IP
    QString getLocalIP();
    // 设置本地IP
    void setLocalIP(QString LocalIP);
    // 获取本地端口
    int getLocalPort();
    // 设置本地端口
    void setLocalPort(int LocalPort);

    // 开启本地服务器
    void startServer();
    // 开启本地服务器
    void startServer(QString LocalIP, int LocalPort);
    // 停止服务器
    void stopServer();

    // 发送QString消息给所有客户端
    void send(QString message);
    // 发送QByteArray消息给所有客户端
    void send(QByteArray byteArray);
    // 发送QString消息给某个客户端
    void send(QString message, QString tcpClientIP, int tcpClientPort);
    // 发送QByteArray消息给某个客户端
    void send(QByteArray byteArray, QString tcpClientIP, int tcpClientPort);
    // 取消某个个客户端的连接
    void disconnectClient(QString tcpClientIP, int tcpClientPort);


signals:
    // 信号，某客户端连接信号
    void clientConnect(QString tcpClientIP, int tcpClientPort);
    // 信号，某客户端断开连接信号
    void clientDisconnect(QString tcpClientIP, int tcpClientPort);
    // 信号，收到某客户端的消息 QString格式
    void receiveString(QString message, QString tcpClientIP, int tcpClientPort);
    // 信号，收到某客户端的消息 QByteArray格式
    void receiveByteArray(QByteArray byteArray, QString tcpClientIP, int tcpClientPort);

private:
    // 监听ip
    QString localIP;
    // 监听端口
    int localPort;

    // “服务器处理套接字”字典, 字典中的每个套接字用来单独与客户端套接字通信
    QMap<QString, TcpServerHandler*> handlerMap;

protected:
    // 重载incommintConnection， 生成handler，用来单独处理与每一个TcpClient的通信
    void incomingConnection(qintptr socketDescriptor);
};

#endif // TCPSERVER_H
