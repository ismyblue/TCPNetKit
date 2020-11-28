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
    socketServer = new TCPSocket(this);
    socketClient = new TCPSocket(this);

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
    ui->tableWidget_ListConnections->setRowCount(10);
    int rowcount = ui->tableWidget_ListConnections->rowCount();
    for(int i = 0;i < rowcount;i++)
    {
        QTableWidgetItem *number = new QTableWidgetItem();
        QTableWidgetItem *address = new QTableWidgetItem();
        number->setText(QString("%1").arg(i));
        address->setText(QString("192.168.0.10%1").arg(i));
        ui->tableWidget_ListConnections->setItem(i, 0, number);
        ui->tableWidget_ListConnections->setItem(i, 1, address);
        ui->tableWidget_ListConnections->setRowHeight(i, 20);
    }

    //5.状态变量: 服务端是否创建，客户端是否连接，当前是否选中传感器
    isServerCreated = false;
    isClientConnected = false;

}

// 析构函数，释放指针
TCPNetKit::~TCPNetKit()
{
    delete ui;
    delete socketServer;
    delete socketClient;
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

    // 如果还没创建服务器
    if(!isServerCreated)
    {
        // 创建并开启服务器套接字
        // todo...
        // 如果创建失败
        if(false)
        {
            QMessageBox::warning(this, tr("Fail"), tr("Server create failed"));
            return ;
        }
        // 如果启动失败
        if(false)
        {
            QMessageBox::information(this, tr("Fail"), tr("Server start failed"));
            return ;
        }

        // 修改按钮文本
        ui->pushButton_StartServer->setText(tr("关闭服务"));
        // 重置服务器创建状态为已创建
        isServerCreated = true;
    }
    // 如果已经创建了服务器
    else
    {
        // 关闭服务器套接字
        // todo....
        // socketServer->close();
        // 修改按钮文本
        ui->pushButton_StartServer->setText(tr("开始服务"));
        // 清空传感器列表
        for(int i = ui->tableWidget_ListConnections->rowCount() - 1;i >=0;i--)
            ui->tableWidget_ListConnections->removeRow(i);
        // 重置服务器创建状态为未创建
        isServerCreated = false;
    }

}


// client连接按钮 clicked信号相应槽
void TCPNetKit::on_pushButton_Connect_clicked()
{
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
            return ;
        }

        // 修改客户端连接状态为已连接
        isClientConnected = true;
        // 修改按钮文本
        ui->pushButton_Connect->setText(tr("断开连接"));

        // 更新客户端日志
        QString status = remoteIP + "连接\r\n";
        ui->textEdit_ClientLog->append(status);

    }
    else
    {
        // 客户端套接字断开连接
        //        socketClient->close();
        // todo....
        // 修改客户端连接状态为false
        isClientConnected = false;
        // 设置连接按钮文本为“连接”
        ui->pushButton_Connect->setText(tr("连接"));
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

    bool ok;
    // 获取传感器id
    int sensorid = ui->tableWidget_ListConnections->item(currentRow, 0)->text().toInt(&ok);
    // 获取传感器ip
    QString sensorip = ui->tableWidget_ListConnections->item(currentRow, 1)->text();
    // 获取要发送的信息
    QString message = ui->textEdit_ServerSend->toPlainText();
    if(message == "")
    {
        QMessageBox::information(this, tr("Error"), tr("Empty message!"));
        return ;
    }

    // 计算传感器标识
    QString sensor_key = QString("%1_%2").arg(sensorip).arg(sensorid);

    // 如果存在定时器
    QTimer *timer;
    if(timerMap.contains(sensor_key)){

        // 停止之前的定时器，
        timer = timerMap.value(sensor_key);
        // 删除所有信号槽连接
        timer->disconnect();
        timer->stop();
    }
    // 如果不存在定时器
    else
    {
        // 新建定时器，发送消息
        timer = new QTimer();
        timerMap.insert(sensor_key, timer);
    }
    // 连接信号槽
    connect(timer, &QTimer::timeout, this,
            [=]()mutable { onSendMessageToClient(sensorid, sensorip, message); });
    // 启动定时器，定时发送消息
    timer->start(1000);

    // 清空消息框
    ui->textEdit_ServerSend->clear();

}

// 发送消息到传感器
void TCPNetKit::onSendMessageToClient(int sensorid, QString sensorip, QString message)
{
    // todo...
    // socketServer->send(sensorip, message);

    // 更新服务器日志
    QString sensor_key = QString("%1_%2").arg(sensorip).arg(sensorid);
    ui->textEdit_ServerLog->append(QString("向%1发送:%2").arg(sensor_key).arg(message));

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

    bool ok;
    // 获取传感器id
    int sensorid = ui->tableWidget_ListConnections->item(currentRow, 0)->text().toInt(&ok);
    // 获取传感器ip
    QString sensorip = ui->tableWidget_ListConnections->item(currentRow, 1)->text();
    // 计算传感器标识
    QString sensor_key = QString("%1_%2").arg(sensorip).arg(sensorid);

    // 如果存在定时器，那么删除此定时器
    if(timerMap.contains(sensor_key))
    {
        // 从定时器中删除
        QTimer *timer = timerMap.value(sensor_key);
        // 删除信号槽连接
        timer->disconnect();
        // 释放指针所指内存
        delete timer;
        // 定时器列表移出元素
        timerMap.remove(sensor_key);
    }

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

    bool ok;
    // 获取传感器id
    int sensorid = ui->tableWidget_ListConnections->item(currentRow, 0)->text().toInt(&ok);
    // 获取传感器ip
    QString sensorip = ui->tableWidget_ListConnections->item(currentRow, 1)->text();
    // 计算传感器标识
    QString sensor_key = QString("%1_%2").arg(sensorip).arg(sensorid);

    // 先删除这个向这个传感器发送消息的定时器，然后断开连接
    // 如果存在定时器，那么删除此定时器
    if(timerMap.contains(sensor_key))
    {
        // 从定时器中删除
        QTimer *timer = timerMap.value(sensor_key);
        // 删除信号槽连接
        timer->disconnect();
        // 释放指针所指内存
        delete timer;
        // 定时器列表移出元素
        timerMap.remove(sensor_key);
    }

    // 断开这个传感器的连接
    // todo...
    // socketServer->disconnect(sensor_id, sensor_ip);

    // 从传感器列表删除此传感器
    ui->tableWidget_ListConnections->removeRow(currentRow);

    // 更新服务器日志
    ui->textEdit_ServerLog->append(QString("手工断开%1的连接").arg(sensor_key));




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
            // socketClient->send(message);
            // 更新客户端日志
            ui->textEdit_ClientLog->append(QString("向%1发送:%2").arg(remoteIP).arg(message));
            // 清除消息框消息
//            ui->textEdit_Send->clear();
        }
    }
}


// 服务器请求函数
void TCPNetKit::onServerRequest()
{

}

// 服务器响应函数
void TCPNetKit::onServerReply(int index)
{

}

// 数据处理函数
void TCPNetKit::onDataHandle(QString data)
{

}

// 上传数据函数
void TCPNetKit::onUploadData(QString device, QString data)
{

}

// 字符串转ASCII函数
QString TCPNetKit::strToAscii(QString str)
{
    return "";
}


// 更新列表
void TCPNetKit::updatelist(QString ip, int No, int operation)
{

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


