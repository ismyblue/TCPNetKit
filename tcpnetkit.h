#ifndef TCPNETKIT_H
#define TCPNETKIT_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class TCPNetKit; }
QT_END_NAMESPACE

class TCPNetKit : public QWidget
{
    Q_OBJECT

public:
    TCPNetKit(QWidget *parent = nullptr);
    ~TCPNetKit();

private slots:
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

};
#endif // TCPNETKIT_H
