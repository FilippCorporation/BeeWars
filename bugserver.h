#ifndef BUGSERVER_H
#define BUGSERVER_H

#include <QObject>
#include <QUdpSocket>
#include <QByteArray>
#include <QNetworkDatagram>
#include <QHostAddress>
#include <QDebug>

//#include <QtCore/QObject>
//#include <QtCore/QList>
//#include <QtCore/QByteArray>
//#include <QDebug>
//#include <QWebSocket>

#include <random>
//#include <windows.h>
#include <map>
#include <QVector>
#include <map>
#include <set>
#include <array>
#include <utility>
#include <list>
#include <iostream>
#include <algorithm>
#include<QApplication>
#include<time.h>
#include <QTimer>
#include <QList>
#include<gobee.h>
#include<players.h>
#include <QThread>
#include <QMutex>
#include"make_bee.h"
#include"game_plane.h"
#include"bombs.h"
#include"decal.h"
QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

class BugServer : public QObject
{
    Q_OBJECT
public:
    explicit BugServer(quint16 port, QObject *parent = Q_NULLPTR);
    explicit BugServer(quint16 port,quint16 width, quint16 height,quint16 count, QObject *parent = Q_NULLPTR);
    virtual ~BugServer();

protected:
    QUdpSocket udpSocket;
    quint16 bindPort;

protected slots:
    void readPendingDatagrams();
    void socket_error(QAbstractSocket::SocketError err);

    //private Q_SLOTS:
    //    void onNewConnection();
    //    void processMessage(QString message);
    //    void processMessage(const QByteArray & message);
    //    void socketDisconnected();

private:

    QByteArray message_f02;
    QByteArray message_f04;
    QByteArray message_f06;
    QByteArray message_f08;
    quint32 time=10;


    int hei=480;
    int wid=640;
    int size_b=64;
    int kol = 25;
    const int bee_speed = 3;


    QList<Bombs> bomb;

    quint8 id_players=1;
    int max_players = 10;

    void fillMessage02();
    void fillMessage02_error(QString);

    void send_bees();


    void process_f01(const QByteArray &message, QNetworkDatagram dm);
    void process_f09(const QByteArray &message, QNetworkDatagram dm);

    //-------------новые функции UDP---------------------------------------------------------------------------------

    QList<Players> Player;


    void fillMessage04();
    void fillMessage06();
    void fillMessage08();
    void fillMessage12();


    void fillMessage10(QNetworkDatagram dm, int k);
    void do_bee();

    static const int M = 32;
    static const quint8 planeIdxMask = 31;

    void process_f0b(const QByteArray &message,QNetworkDatagram dm);
    void process_f0d(const QByteArray &message);
    Game_plane plane[M];

    quint8 plane_currIdx;
    const quint8 & incPlaneIdx(quint8 &planeIdx);
    const quint8 & decPlaneIdx(quint8 &planeIdx);

    QTimer timer_do_bee;
    QTimer timer_close_pl;
    int fall = 1;
    int down_fall=5;

    QList<Decal>decali;

    QByteArray message_f0a;
    QByteArray message_f0c;
    QByteArray message_f10;

private slots:
    void doing_bufer();
    void close_pl();
    void remove_cl(int i);
signals:
    void client_list(QList<Players> client);
};

//class BugServerThread : public QThread
//{
//    Q_OBJECT
//public:
//    BugServerThread(QObject *parent = Q_NULLPTR):QThread(parent), bugserver(nullptr){}
//    virtual ~BugServerThread();

//    bool restartServer(quint16 port, quint16 boardWidth, quint16 boardHeight, quint8 bugCount);
//  protected:
//    void run();
//  private:
//    quint16 port;
//    quint16 boardWidth;
//    quint16 boardHeight;
//    quint8 bugCount;
//    QMutex argMutex;
//    QMutex runMutex;

//   BugServer* bugserver;
//    void terminate_r();

//};


#endif // BUGSERVER_H
