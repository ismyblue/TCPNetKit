#include "httpclient.h"

HttpClient::HttpClient(QObject *parent) : QNetworkAccessManager(parent)
{
    request.setRawHeader("Accept", "*,*/*");;
    request.setRawHeader("Accept", "*,*/*");
    request.setRawHeader("Accept-Language", "zh-cn");
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("Accept-Encoding", "gzip, deflate");

    // http 收到response信号槽
    connect(this, SIGNAL(finished(QNetworkReply *)), this, SLOT(onFinished(QNetworkReply *)));
}

// 析构函数，释放指针
HttpClient::~HttpClient()
{

}

// 设置http请求头
void HttpClient::setRawHeader(const QByteArray &headerName, const QByteArray &value)
{
    request.setRawHeader(headerName, value);
}

// http get方法
void HttpClient::httpGet(QString url)
{
    request.setUrl(QUrl(url));
    this->get(request);
}

// http post方法
void HttpClient::httpPost(QString url, const QByteArray &form_data)
{
    request.setUrl(QUrl(url));
    this->post(request, form_data);
}


// 响应请求完成信号
void HttpClient::onFinished(QNetworkReply *reply)
{
    QByteArray all = reply->readAll();
    emit httpResponse(QString().fromUtf8(all));
    emit httpResponse(all);

    // 释放reply
    reply->deleteLater();
}

