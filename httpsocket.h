#ifndef HTTPSOCKET_H
#define HTTPSOCKET_H

#include <QObject>

class HttpSocket : public QObject
{
    Q_OBJECT
public:
    explicit HttpSocket(QObject *parent = nullptr);

signals:

};

#endif // HTTPSOCKET_H
