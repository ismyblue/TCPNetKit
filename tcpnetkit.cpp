#include "tcpnetkit.h"
#include "ui_tcpnetkit.h"
#include <QHostInfo>
#include <QMessageBox>
#include <QAction>
#include <QDebug>
#include <QTableWidgetItem>
#include <QTimer>

TCPNetKit::TCPNetKit(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TCPNetKit)
{
    ui->setupUi(this);
    initDialog();

}

// 初始化对话框
void TCPNetKit::initDialog()
{
    // 设置对话框图标
    this->setWindowIcon(QIcon(QString(":/images/logo.ico")));

    // TODO: Add extra initialization here
    //1.初始化变量,初始化服务端套接字和客户端套接字
    tcpServer = new TcpServer(this);
    tcpClient = new TcpClient(this);

    // tcpServer的信号槽
    connect(tcpServer, &TcpServer::clientConnect, this, &TCPNetKit::onClientConnect);
    connect(tcpServer, &TcpServer::clientDisconnect, this, &TCPNetKit::onClientDisconnect);
    connect(tcpServer, &TcpServer::receiveString, this, &TCPNetKit::onServerReceiveString);
    // tcpClient的信号槽
    connect(tcpClient, &TcpClient::receiveString, this, &TCPNetKit::onClientReceiveString);
    connect(tcpClient, &TcpClient::stateChanged, this, &TCPNetKit::onClientStateChanged);



    //2.获取本机的IP和计算机名
    localName = getLocalHostName();
    localIP = getIPAddress(localName);
    // 展示到ui
    ui->lineEdit_IPLocal->setText(localIP);
    ui->lineEdit_IPRemote->setText(localIP);

    //3.设置服务端端口和客户端端口
    localPort = 8889;
    remotePort = 8889;
    // 展示到ui
    ui->lineEdit_PortLocal->setText(QString::number(localPort));
    ui->lineEdit_PortRemote->setText(QString::number(remotePort));

    //4.初始化表头
    // 设置表头高度
    ui->tableWidget_ListConnections->horizontalHeader()->setFixedHeight(20);
    // 取消序号
    ui->tableWidget_ListConnections->verticalHeader()->setHidden(true);

    //5.状态变量: 客户端是否连接
    isClientConnected = false;

}

// 删除某一个定时器
void TCPNetKit::removeOneTimer(QString tcpClient_key)
{
    // 如果存在定时器，那么删除此定时器
    if(timerMap.contains(tcpClient_key))
    {
        // 从定时器中删除
        QTimer *timer = timerMap.value(tcpClient_key);
        // 删除信号槽连接
        timer->disconnect();
        // 释放指针所指内存
        delete timer;
        // 定时器列表移出元素
        timerMap.remove(tcpClient_key);
    }
}

// 清空所有定时器
void TCPNetKit::clearAllTimer()
{
    // 清空定时器
    QMap<QString, QTimer*>::iterator iter;
    for(iter = timerMap.begin();iter != timerMap.end();)
    {
        QString tcpClient_key = iter.key();
        QTimer *timer = iter.value();
        iter++;
        // 删除信号槽连接
        timer->disconnect();
        // 释放指针所指内存
        delete timer;
        // 定时器列表移出元素
        timerMap.remove(tcpClient_key);
    }
}

// 析构函数，释放指针
TCPNetKit::~TCPNetKit()
{
    delete ui;
    delete tcpServer;
    delete tcpClient;
}

// 开始服务按钮 clicked信号响应槽
void TCPNetKit::on_pushButton_StartServer_clicked()
{
    // 获取本地服务端ip
    localIP = ui->lineEdit_IPLocal->text();
    QHostAddress test;
    // 如果ip地址不合法
    if(!test.setAddress(localIP))
    {
        QMessageBox::warning(this, tr("Error"), tr("Please input correct ip address!"));
        return ;
    }

    // 获取本地服务端口
    bool ok;
    localPort = ui->lineEdit_PortLocal->text().toInt(&ok);
    // 转换失败或者端口不在范围内
    if(!ok || localPort < 0 || localPort > 65535)
    {
        QMessageBox::warning(this, tr("Error"), tr("Please input correct port!"));
        return ;
    }

    // 如果服务器没有启动
    if(!tcpServer->isListening())
    {
        // 创建并开启服务器套接字
        // todo...
        // 如果监听失败
        if(!tcpServer->startServer(localIP, localPort))
        {
            QMessageBox::warning(this, tr("Fail"), tr("Server listen failed"));
            return ;
        }

        // 修改按钮文本
        ui->pushButton_StartServer->setText(tr("关闭服务"));

    }
    // 如果已经创建了服务器
    else
    {
        // 关闭服务器套接字
        // todo....        
        tcpServer->stopServer();
        // 清空定时器
        clearAllTimer();

        // 修改按钮文本
        ui->pushButton_StartServer->setText(tr("开始服务"));
        // 清空传感器列表
        for(int i = ui->tableWidget_ListConnections->rowCount() - 1;i >=0;i--)
            ui->tableWidget_ListConnections->removeRow(i);

    }

}


// client连接按钮 clicked信号相应槽
void TCPNetKit::on_pushButton_Connect_clicked()
{
    qDebug()<<"connect click" << isClientConnected;
    // 如果客户端未连接
    if(!isClientConnected)
    {
        // 获取远程服务端ip
        remoteIP = ui->lineEdit_IPRemote->text();
        QHostAddress test;
        // 如果ip地址不合法
        if(!test.setAddress(remoteIP))
        {
            QMessageBox::warning(this, tr("Error"), tr("Please input correct ip address!"));
            return ;
        }

        // 获取远程服务端口
        bool ok;
        remotePort = ui->lineEdit_PortRemote->text().toInt(&ok);
        // 转换失败或者端口不在范围内
        if(!ok || remotePort < 0 || remotePort > 65535)
        {
            QMessageBox::warning(this, tr("Error"), tr("Please input correct port!"));
            return ;
        }

        // 连接服务端
        // todo...
        tcpClient->connectServer(remoteIP, remotePort);
        // 如果连接失败
        if(false)
        {
            QMessageBox::warning(this, tr("Fail"), tr("Connect failed!"));
            return ;
        }
        // 如果启动接收数据功能失败
        if(false)
        {
            QMessageBox::warning(this, tr("Fail"), tr("receiving data failed!"));
            // 关闭客户端套接字
            // todo...
            tcpClient->disconnectServer();
            return ;
        }
        qDebug()<<"finished connectSerer";

    }
    else
    {
        // 客户端套接字断开连接        
        // todo....
        tcpClient->disconnectServer();
qDebug()<<"finished disconnectServer";
        // 由onStateChanged槽函数修改按钮文本
//        // 设置连接按钮文本为“连接”
//        ui->pushButton_Connect->setText(tr("连接"));
    }
}


// server发送数据按钮 clicked信号响应槽, 发送到消息是循环发送
void TCPNetKit::on_pushButton_ServerSend_clicked()
{
    // 定时向传感器发送消息

    // 获取当前传感器列表选中的行
    int currentRow = ui->tableWidget_ListConnections->currentRow();
    // 如果没有选中传感器，那么提示，然后返回
    if(currentRow < 0 || currentRow >= ui->tableWidget_ListConnections->rowCount())
    {
        QMessageBox::warning(this, tr("Error"), tr("Please select a sensor!"));
        return ;
    }

//    // 获取传感器id
//    int sensorid = ui->tableWidget_ListConnections->item(currentRow, 0)->text().toInt(&ok);
    // 获取客户端ip
    QString tcpClientIP = ui->tableWidget_ListConnections->item(currentRow, 1)->text();
    // 获取客户端port
    int tcpClientPort = ui->tableWidget_ListConnections->item(currentRow, 2)->text().toInt();
    // 获取要发送的信息
    QString message = ui->textEdit_ServerSend->toPlainText();
    if(message == "")
    {
        QMessageBox::information(this, tr("Error"), tr("Empty message!"));
        return ;
    }

    // 计算传感器标识
    QString tcpClient_key = QString("%1:%2").arg(tcpClientIP).arg(tcpClientPort);

    // 删除之前的定时器
    removeOneTimer(tcpClient_key);

    // 新建定时器，发送消息
    QTimer *timer = new QTimer();
    timerMap.insert(tcpClient_key, timer);

    // 连接信号槽
    connect(timer, &QTimer::timeout, this,
            [=]()mutable { onSendMessageToClient(message, tcpClientIP, tcpClientPort); });
    // 启动定时器，定时发送消息
    timer->start(1000);

    // 清空消息框
    ui->textEdit_ServerSend->clear();

}

// 发送消息到客户端
void TCPNetKit::onSendMessageToClient(QString message, QString tcpClientIP, int tcpClientPort)
{
    // todo...
    tcpServer->send(message, tcpClientIP, tcpClientPort);

    // 更新服务器日志
//    onServerReceiveString(message, tcpClientIP, tcpClientPort);
    ui->textEdit_ServerLog->append(QString("向%1:%2发送:%3").arg(tcpClientIP).arg(tcpClientPort).arg(message));

}

// 取消发送消息到传感器 clicked信号响应槽
void TCPNetKit::on_pushButton_CancerServerSend_clicked()
{
    // 获取当前传感器列表选中的行
    int currentRow = ui->tableWidget_ListConnections->currentRow();
    // 如果没有选中传感器，那么提示，然后返回
    if(currentRow < 0 || currentRow >= ui->tableWidget_ListConnections->rowCount())
    {
        QMessageBox::warning(this, tr("Error"), tr("Please select a sensor!"));
        return ;
    }

    // 获取传感器id
//    int sensorid = ui->tableWidget_ListConnections->item(currentRow, 0)->text().toInt(&ok);
    // 获取客户端ip
    QString tcpClientIP = ui->tableWidget_ListConnections->item(currentRow, 1)->text();
    // 获取客户端port
    int tcpClientPort = ui->tableWidget_ListConnections->item(currentRow, 2)->text().toInt();
    // 计算传感器标识
    QString tcpClient_key = QString("%1:%2").arg(tcpClientIP).arg(tcpClientPort);

    // 如果存在定时器，那么删除此定时器
    removeOneTimer(tcpClient_key);

}


// 断开传感器连接按钮 clicked信号响应槽
void TCPNetKit::on_pushButton_ServerDisconnect_clicked()
{
    // 获取当前传感器列表选中的行
    int currentRow = ui->tableWidget_ListConnections->currentRow();
    // 如果没有选中传感器，那么提示，然后返回
    if(currentRow < 0 || currentRow >= ui->tableWidget_ListConnections->rowCount())
    {
        QMessageBox::warning(this, tr("Error"), tr("Please select a sensor!"));
        return ;
    }

    // 获取传感器id
//    int sensorid = ui->tableWidget_ListConnections->item(currentRow, 0)->text().toInt(&ok);
    // 获取客户端ip
    QString tcpClientIP = ui->tableWidget_ListConnections->item(currentRow, 1)->text();
    // 获取客户端port
    int tcpClientPort = ui->tableWidget_ListConnections->item(currentRow, 2)->text().toInt();
    // 计算传感器标识
    QString tcpClient_key = QString("%1:%2").arg(tcpClientIP).arg(tcpClientPort);


    // 断开这个传感器的连接
    // todo...
    tcpServer->disconnectClient(tcpClientIP, tcpClientPort);


    // 更新服务器日志
    ui->textEdit_ServerLog->append(QString("手工断开%1的连接").arg(tcpClient_key));

}


// client发送数据按钮 clicked信号响应槽
void TCPNetKit::on_pushButton_ClientSend_clicked()
{
    // 在连接了服务器的情况下
    if(isClientConnected)
    {
        // 获取消息框消息
        QString message = ui->textEdit_Send->toPlainText();
        if(message != "")
        {
            // 客户端套接字发送消息
            // todo...            
            tcpClient->send(message);
            // 更新客户端日志
            ui->textEdit_ClientLog->append(QString("向%1发送:%2").arg(remoteIP).arg(message));
        }
    }
}

// 新连接一个客户端
void TCPNetKit::onClientConnect(QString tcpClientIP, int tcpClientPort)
{
    int rowCount = ui->tableWidget_ListConnections->rowCount();
    ui->tableWidget_ListConnections->insertRow(rowCount);
    QTableWidgetItem *ip = new QTableWidgetItem();
    QTableWidgetItem *port = new QTableWidgetItem();
    ip->setText(tcpClientIP);
    port->setText(QString::number(tcpClientPort));
    ui->tableWidget_ListConnections->setItem(rowCount, 1, ip);
    ui->tableWidget_ListConnections->setItem(rowCount, 2, port);
    ui->tableWidget_ListConnections->setRowHeight(rowCount, 20);

}

// 断开一个客户端
void TCPNetKit::onClientDisconnect(QString tcpClientIP, int tcpClientPort)
{
    // 列表移除
    for(int i=0;i < ui->tableWidget_ListConnections->rowCount();i++)
    {
        // 判断IP port,确定行
        if(ui->tableWidget_ListConnections->item(i, 1)->text() == tcpClientIP
                && ui->tableWidget_ListConnections->item(i, 2)->text().toInt() == tcpClientPort){
            ui->tableWidget_ListConnections->removeRow(i);
        }
    }

    // 移除定时器，取消定时发送消息
    QString tcpClient_key = QString("%1:%2").arg(tcpClientIP).arg(tcpClientPort);
    removeOneTimer(tcpClient_key);

}

// 客户端收到消息
void TCPNetKit::onClientReceiveString(QString message)
{
    ui->textEdit_ClientLog->append(message);
}

// 响应服务端收到消息
void TCPNetKit::onServerReceiveString(QString message, QString tcpClientIP, int tcpClientPort)
{
    ui->textEdit_ServerLog->append(QString("收到%1:%2信息:%3").arg(tcpClientIP).arg(tcpClientPort).arg(message));
}

// 响应客户端状态改变
void TCPNetKit::onClientStateChanged(QAbstractSocket::SocketState socketState)
{
    // 套接字连接成功
    if(QAbstractSocket::ConnectedState == socketState)
    {
        ui->pushButton_Connect->setText(QString("断开连接"));
        // 更新客户端日志
        QString status = remoteIP + "连接";
        ui->textEdit_ClientLog->append(status);
        isClientConnected = true;
    }
    else if(QAbstractSocket::UnconnectedState == socketState)
    {
        qDebug()<<" 客户端响应断开";
        ui->pushButton_Connect->setText(QString("连接"));
        isClientConnected = false;
    }
    else if(QAbstractSocket::ConnectingState == socketState)
    {
        ui->pushButton_Connect->setText(QString("连接中..."));
    }

}


// 获取本机ip
QString TCPNetKit::getIPAddress(const QString &hostname)
{
    // 获取本机hostinfo实例
    QHostInfo hostinfo = QHostInfo::fromName(hostname);
    // 获取主机地址列表
    QList<QHostAddress> list = hostinfo.addresses();
    // 取出第一个ip
    QString ipaddress = "";
    for(int i=0;i < list.length();i++)
    {
        QHostAddress hostAddress = list.at(i);
        if(hostAddress.protocol() == QAbstractSocket::IPv4Protocol)
            ipaddress = hostAddress.toString();
    }
    return ipaddress;
}

// 获取本机计算机名
QString TCPNetKit::getLocalHostName()
{
    return QHostInfo::localHostName();
}


