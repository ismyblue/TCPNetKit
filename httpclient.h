#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QObject>

// HTTP客户端类，用来请求web api
class HttpClient : public QObject
{
    Q_OBJECT
public:
    explicit HttpClient(QObject *parent = nullptr);

signals:

};

#endif // HTTPCLIENT_H
