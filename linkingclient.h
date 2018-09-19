#ifndef LINKINGCLIENT_H
#define LINKINGCLIENT_H

#include <QtNetwork>

class linkingclient:public QObject
{
    Q_OBJECT

public:
    linkingclient(QHostAddress,int);
    ~linkingclient();
protected:

public slots:
    void recvMessage();
signals:
    void set(int);
    void sc();
public slots:
    void sendmes(int a);
    void ready();

private:
    QTcpSocket  *readWriteSocket;
    int port;
    QHostAddress ip;
};
#endif // LINKINGCLIENT_H
