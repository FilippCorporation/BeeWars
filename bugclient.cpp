#include "bugclient.h"
#include<QString>
#include<QDebug>
#include "mainwindow.h"
#include <QMainWindow>
#include "ui_mainwindow.h"
BugClient::BugClient(QHostAddress address, quint16 port, QObject *parent) : QObject(parent),
    address(address), sendPort(port), bindPort(port+1) {

    connect(&sendTimer, SIGNAL(timeout()), this, SLOT(sendTimer_timeout()));
    connect(&udpSocket, SIGNAL(connected()), this, SLOT(socket_connected()));
    connect(&udpSocket, SIGNAL(disconnected()), this, SLOT(socket_disconnected()));
    connect(&udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
    connect(&udpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socket_error(QAbstractSocket::SocketError)));
    //Запускать bind если сам себе сервер, иначе connect
    if (address.isLoopback()){
        if (!udpSocket.bind(address, bindPort, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint)){
            qDebug()<<"Cannot bind client udp Socket";
            return;
        }
        else {
            socket_connected();
        }
    }
    else {
        udpSocket.connectToHost(address, sendPort);
    }
}

void BugClient::socket_connected(){
    sendTimer.start(defaultSendInterval);

}

void BugClient::socket_disconnected(){
    sendTimer.stop();
}

void BugClient::readPendingDatagrams(){

    while (udpSocket.hasPendingDatagrams()){
        dm = udpSocket.receiveDatagram();
        QByteArray message(dm.data());

       switch(static_cast<int>(*(message.constData()+1))){
        case 0x02:
            this->sendMsg=message;
            emit mySignal(2);
            break;
        case 0x04:
            this->sendMsg=message;
            emit mySignal(4);
            break;
        case 0x06:
            this->sendMsg=message;
            emit mySignal(6);
            break;
        case 0x08:
            this->sendMsg=message;
            emit mySignal(8);
            break;
       case 0x0a:
           this->sendMsg=message;
           emit mySignal(10);
           break;
       case 0x0c:
           this->sendMsg=message;
           emit mySignal(12);
           break;
       case 0x10:
           this->sendMsg=message;
           emit mySignal(16);
           break;
       case 0x12:
           this->sendMsg=message;
           emit mySignal(18);
           break;

        default:
            break;
        }
    }

}

void BugClient::socket_error(QAbstractSocket::SocketError err){
    sendTimer.stop();
    qDebug()<<"UDP Client: socket error: "<<err;
}

void BugClient::sendTimer_timeout(){
    emit mySignal(1);
    sendTimer.stop();
//    static int pos = 1;
//    static int k = 0;

//    switch (pos) {

//    case 1:{
//        if(k<=3){

//            k=0;
//            pos++;
//        }else{

//        }
//        break;
//    }
//    case 2:{
//        if(k<=3){
//            emit mySignal(2);
//            k=0;
//            pos++;
//        }else{
//        sendTimer.stop();
//        }
//        break;
//    }

//    }

    //udpSocket.writeDatagram(sendMsg, address, sendPort);
}

//BugClient::BugClient(int port,QString ip,QObject *parent) : QObject(parent)
//{
//if(ip==""){
//    ip+="127.0.0.1";
//}
//QString str="ws://"+ip+":"+QString::number(port,10);
//qDebug()<<str;
//QUrl web_url(str);
//webSocket = new QWebSocket(str);
//connect(webSocket,SIGNAL(connected()),this,SLOT(Connect_()),Qt::UniqueConnection);
//connect(webSocket,SIGNAL(disconnected()),this,SLOT(Disconnect_()));
//connect(webSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(Error_()));
//webSocket->open(web_url);


//}

//void BugClient::processMessage(QString message)
//{
//    this->_message=message;
//    emit mySignal(10);
//}

//void BugClient::processMessage(const QByteArray &message)
//{
//    switch (*message+1) {
//    case 0x02:
//        this->message=message;
//        emit mySignal(2);
//        break;
//    case 0x04:
//        this->message=message;
//        emit mySignal(4);
//        break;
//    case 0x06:
//        this->message=message;
//        emit mySignal(6);
//        break;
//    case 0x08:
//        this->message=message;
//        emit mySignal(8);
//        break;
//    default:
//        break;
//    }
////    if(*message.data()==0x01){
////        qDebug() <<"отправил";
////        QByteArray send;
////        send.resize(1);
////        quint8 *ptr = reinterpret_cast<quint8*> (send.data());
////        *ptr = 0x09;++ptr;
////        webSocket->sendBinaryMessage(send);
////    }

//}

//void BugClient::Connect_()
//{
//    connect(webSocket, SIGNAL(textMessageReceived(QString)), this, SLOT(processMessage(QString)));
//    connect(webSocket, SIGNAL(binaryMessageReceived(QByteArray)), this, SLOT(processMessage(QByteArray)));
//      emit mySignal(1);

//}

//void BugClient::Disconnect_()
//{
//   // qDebug()<<1;
//emit mySignal(12);
//}

//void BugClient::Error_()
//{
//     //qDebug()<<2;
//emit mySignal(12);
//}
//BugClient::~BugClient()
//{

//    webSocket->close();


//}
