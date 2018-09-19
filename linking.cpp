#include "linking.h"

linking::linking(int p)
{
    port=p;
    this->listenSocket =new QTcpServer;
    this->listenSocket->listen(QHostAddress::Any,port);
    connect(this->listenSocket,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
    qDebug()<<"newserver"<<port;
}

linking::~linking(){
    delete listenSocket;
    delete readWriteSocket;
}

void linking::acceptConnection(){
    this->readWriteSocket =this->listenSocket->nextPendingConnection();
    connect(this->readWriteSocket,SIGNAL(readyRead()),this,SLOT(recvMessage()));
    emit sc();
    qDebug()<<"successconnect";
}
void linking::recvMessage(){
    QString info;
    info += this->readWriteSocket->readAll();
    int a=info.toInt();
    qDebug()<<"r"<<a;
    emit set(a);
}

void linking::sendmes(int a){
    QByteArray *array =new QByteArray;
    array->clear();
    array->append(QString::number(a));
    qDebug()<<"s"<<a;
    this->readWriteSocket->write(array->data());
}
