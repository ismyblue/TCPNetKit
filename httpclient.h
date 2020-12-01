#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkCookie>
#include <QThreadPool>
#include <QList>

// HTTP客户端类，用来请求web api
class HttpClient : public QNetworkAccessManager
{
    Q_OBJECT
public:
    explicit HttpClient(QObject *parent = nullptr);
    ~HttpClient();

    // 设置http请求头
    void setRawHeader(const QByteArray &headerName, const QByteArray &value);

    // http get方法
    void httpGet(QString url);

    // http post方法
    void httpPost(QString url, const QByteArray &from_data);

private:
    bool isEnableSSL;
    bool isAutoDelete;
    QNetworkRequest request;


signals:
    // http 响应信号,提供QString的http response4
    void httpResponse(QString);
    // http 响应信号，提供QByteArray的http response
    void httpResponse(QByteArray);


private slots:
    // 响应http请求完成的信号
    void onFinished(QNetworkReply *reply);
};

#endif // HTTPCLIENT_H
