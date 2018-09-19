#ifndef LINKING_H
#define LINKING_H

#include <QtNetwork>

class linking :public QObject
{
    Q_OBJECT

public:
    linking(int);
    ~linking();
protected:

public slots:
    void acceptConnection();
    void recvMessage();
signals:
    void set(int);
    void sc();
public slots:
    void sendmes(int a);//1001白赢,1002黑，1000和，1003求和,1004拒绝和棋

private:
    QTcpServer  *listenSocket;
    QTcpSocket  *readWriteSocket;
    int port;
};

#endif // LINKING_H
