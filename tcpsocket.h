#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>

class TCPSocket : public QObject
{
    Q_OBJECT
public:
    explicit TCPSocket(QObject *parent = nullptr);

signals:

};

#endif // TCPSOCKET_H
