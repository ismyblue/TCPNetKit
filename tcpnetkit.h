#ifndef TCPNETKIT_H
#define TCPNETKIT_H

#include <QWidget>
#include <QMap>
#include "tcpserver.h"
#include "tcpclient.h"
#include "sensorhelper.h"
#include "httpclient.h"

QT_BEGIN_NAMESPACE
namespace Ui { class TCPNetKit; }
QT_END_NAMESPACE

class TCPNetKit : public QWidget
{
    Q_OBJECT

public:
    TCPNetKit(QWidget *parent = nullptr);
    ~TCPNetKit();

    // 字符串转ASCII函数
    QString strToAscii(QString str);
    // 发送消息到客户端
    void onSendMessageToClient(QString message, QString tcpClientIP, int tcpClientPort);

private:
    Ui::TCPNetKit *ui;
    void initDialog();// 初始化对话框

    // 服务器定时发送消息的定时器,key:传感器标识<ip_sensorid>，
    QMap<QString, QTimer*> timerMap;
    // 删除某一个定时器
    void removeOneTimer(QString tcpClient_key);
    // 清空所有定时器
    void clearAllTimer();

    // 服务器套接字
    TcpServer *tcpServer;
    // 客户端套接字
    TcpClient *tcpClient;

    // 本机计算机ip
    QString localIP;
    // 本机计算机名
    QString localName;
    // 本机监听端口
    int localPort;
    // 远程服务器IP
    QString remoteIP;
    // 远程服务器端口
    int remotePort;

    // 客户端连接状态
    bool isClientConnected;

    // 获取本机ip
    QString getIPAddress(const QString &hostname);
    // 获取本机计算机名
    QString getLocalHostName();


private slots:
    // UI槽函数
    // 启动服务器
    void on_pushButton_StartServer_clicked();
    // 连接服务器
    void on_pushButton_Connect_clicked();
    // 服务器断开客户端连接
    void on_pushButton_ServerDisconnect_clicked();
    // 取消服务器消息发送
    void on_pushButton_CancerServerSend_clicked();
    // 服务端发送数据
    void on_pushButton_ServerSend_clicked();
    // 客户端发送数据
    void on_pushButton_ClientSend_clicked();

    // 新增一个客户端
    void onClientConnect(QString tcpClientIP, int tcpClientPort);
    // 断开一个客户端
    void onClientDisconnect(QString tcpClientIP, int tcpClientPort);

    // 客户端收到消息
    void onClientReceiveString(QString message);
    // 服务端收到消息
    void onServerReceiveString(QString message, QString tcpClientIP, int tcpClientPort);

    // 响应客户端状态改变
    void onClientStateChanged(QAbstractSocket::SocketState socketState);



};
#endif // TCPNETKIT_H
