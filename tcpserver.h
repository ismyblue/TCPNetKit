#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QList>
#include "tcpserverhandler.h"

// TCP服务端类，用来启动TCP监听服务
class TcpServer : public QObject
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
    // 发送QString消息给某个客户端
    void send(QString message, QString tcpClientIP);
    // 发送QByteArray消息给谋个客户端
    void send(QByteArray byteArray, QString tcpClientIP);
    // 取消某个个客户端的连接
    void disconnectClient(QString tcpClientIP);

signals:
    // 信号，收到某客户端的消息 QString格式
    void receiveMessage(QString message, QString tcpClientIP);
    // 信号，收到某客户端的消息 QByteArray格式
    void receiveByteArray(QByteArray byteArray, QString tcpClientIP);

private slots:
    // 私有槽，响应准备读取消息信号，处理数据之后发送receiveString和receiveByteArray信号
    void slotsReadyRead();

private:
    // 监听ip
    QString localIP;
    // 监听端口
    int localPort;
    // 监听状态
    bool isListen;

    // “服务器处理套接字”列表, 列表中的每个套接字用来单独与客户端套接字通信
    QList<TcpServerHandler*> handlerList;

protected:
    // 重载incommintConnection， 生成handler，用来单独处理与每一个TcpClient的通信
    void incomingConnection(qintptr socketDescriptor);
};

#endif // TCPSERVER_H
