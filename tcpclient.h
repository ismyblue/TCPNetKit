#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>

// TCP客户端类，作为TCP客户端与服务端通信
class TcpClient : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = nullptr);
    // 获取远程IP
    QString getRemoteIP();
    // 设置远程IP
    void setRemoteIP(QString remoteIP);
    // 获取远程端口
    int getRemotePort();
    // 设置远程端口
    void setRemotePort(int remotePort);

    // 连接远程服务器
    void connectServer();
    // 连接远程服务器
    void connectServer(QString remoteIP, int remotePort);
    // 发送QString消息
    void send(QString message);
    // 发送QByteArray消息
    void send(QByteArray byteArray);
    // 取消连接
    void disconnectServer();

signals:    
    // 信号：接收到数据,抛出QString格式数据
    void receiveString(QString);
    // 信号：接收到数据，抛出QString格式数据
    void receiveByteArray(QByteArray);

private slots:
    // 私有槽，响应准备读取消息信号，处理数据之后发送receiveString和receiveByteArray信号
    void slotsReadyRead();

private:
    // 服务器ip
    QString remoteIP;
    // 服务器端口
    int remotePort;
    // 连接状态
    bool isConnected;



};

#endif // TCPCLIENT_H
