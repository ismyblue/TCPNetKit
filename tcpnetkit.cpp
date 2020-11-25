#include "tcpnetkit.h"
#include "ui_tcpnetkit.h"
#include <QMessageBox>
#include <QAction>
#include <QDebug>
#include <QTableWidgetItem>

TCPNetKit::TCPNetKit(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TCPNetKit)
{
    ui->setupUi(this);
    initUi();

}

TCPNetKit::~TCPNetKit()
{
    delete ui;
}

void TCPNetKit::initUi()
{
    this->setWindowIcon(QIcon(QString(":/images/logo.ico")));
    // 设置表头高度
    ui->tableWidget_ListConnections->horizontalHeader()->setFixedHeight(20);
    // 取消序号
    ui->tableWidget_ListConnections->verticalHeader()->setHidden(true);

    ui->tableWidget_ListConnections->setRowCount(10);
    //设置对应的图标、文件名称、最后更新时间、对应的类型、文件大小
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
}

// 开始服务按钮 clicked信号响应槽
void TCPNetKit::on_pushButton_StartServer_clicked()
{

    QMessageBox::information(this, QString("information!"), QString("HelloWorld!"));
}

// 断开连接按钮 clicked信号响应槽
void TCPNetKit::on_pushButton_ServerDisconnect_clicked()
{

}

// 取消发送按钮 clicked信号响应槽
void TCPNetKit::on_pushButton_CancerServerSend_clicked()
{

}

// server发送数据按钮 clicked信号响应槽
void TCPNetKit::on_pushButton_ServerSend_clicked()
{

}

// client连接按钮 clicked信号相应槽
void TCPNetKit::on_pushButton_Connect_clicked()
{

}

// client发送数据按钮 clicked信号响应槽
void TCPNetKit::on_pushButton_ClientSend_clicked()
{
    QString clientData = ui->textEdit_Send->toPlainText();
    ui->textEdit_ClientLog->append(clientData);
    ui->textEdit_ServerLog->append(clientData);
}
