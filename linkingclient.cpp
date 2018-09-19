#include "linkingclient.h"

linkingclient::linkingclient(QHostAddress _ip,int _port)
{
    ip=_ip;
    port=_port;
    this->readWriteSocket = new QTcpSocket;
    connect(readWriteSocket,SIGNAL(connected()),this,SLOT(ready()));
    this->readWriteSocket->connectToHost(ip,port);
    qDebug()<<"newclient"<<ip<<port;
}

linkingclient::~linkingclient(){
    delete readWriteSocket;
}

void linkingclient::recvMessage(){
    QString info;
    info += this->readWriteSocket->readAll();
    int a=info.toInt();
    qDebug()<<"r"<<a;
    emit set(a);
}

void linkingclient::sendmes(int a){
    QByteArray *array =new QByteArray;
    array->clear();
    array->append(QString::number(a));
    this->readWriteSocket->write(array->data());
    qDebug()<<"s"<<a;
}

void linkingclient::ready(){
    connect(this->readWriteSocket,SIGNAL(readyRead()),this,SLOT(recvMessage()));

    qDebug()<<"newcon";
    emit sc();
}
