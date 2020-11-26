#ifndef TCPNETKIT_H
#define TCPNETKIT_H

#include <QWidget>
#include "tcpsocket.h"
#include "httpsocket.h"

QT_BEGIN_NAMESPACE
namespace Ui { class TCPNetKit; }
QT_END_NAMESPACE

class TCPNetKit : public QWidget
{
    Q_OBJECT

public:
    TCPNetKit(QWidget *parent = nullptr);
    ~TCPNetKit();

    // 自定义槽函数
    // 服务器请求函数
    void onServerRequest();
    // 服务器响应函数
    void onServerReply(int index);
    // 数据处理函数
    void onDataHandle(QString data);
    // 上传数据函数
    void onUploadData(QString device, QString data);
    // 字符串转ASCII函数
    QString strToAscii(QString str);


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




private:
    Ui::TCPNetKit *ui;
    void initUi();// 初始化图形界面

    // 服务器套接字
    TCPSocket *socketServer;
    // 客户端套接字
    TCPSocket *socketClient;

    // 添加信息
    void addMessage(QString msg, int which);
    // 更新列表
    void updatelist(QString ip, int No, int operation);

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

    // 当前选中的传感器
    int currentSelect;

    // 服务器创建状态
    bool isServerCreated;
    // 客户端连接状态
    bool isClientConnected;

    // 获取本机ip
    bool getIPAddress(const QString &hostname, QString &ipAddress);
    // 获取本机计算机名
    bool getLocalHostName(QString &hostname);





};
#endif // TCPNETKIT_H
