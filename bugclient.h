#ifndef BUGCLIENT_H
#define BUGCLIENT_H

#include <QObject>
#include <QUrl>
#include <QUdpSocket>
#include <QByteArray>
#include <QNetworkDatagram>
#include <QTimer>
#include <QDebug>

class BugClient : public QObject
{
    Q_OBJECT
public:
//    BugClient(int port, QString ip, QObject *parent = nullptr);
    BugClient(QHostAddress address /*= QHostAddress::LocalHost*/, quint16 port /*= 49152*/, QObject *parent = Q_NULLPTR);

    QUdpSocket udpSocket;
    QHostAddress address;
    quint16 sendPort;
    quint16 bindPort;
    QByteArray sendMsg;
    QTimer sendTimer;
    QNetworkDatagram dm;
protected:
    static const int defaultSendInterval = 150;
    static const int msgSize = 2048;

signals:
       void mySignal(int myParam);
protected slots:
    void socket_connected();
    void socket_disconnected();
    void readPendingDatagrams();
    void socket_error(QAbstractSocket::SocketError err);

    void sendTimer_timeout();


    //осталось от websocket
//public:
//    explicit BugClient(int port, QString ip, QObject *parent = nullptr);
//QWebSocket* webSocket;
//QByteArray message;
//QString _message;
//virtual ~BugClient();

//private Q_SLOTS:
//    void processMessage(QString message);
//    void processMessage(const QByteArray & message);
//    //MainWindow w;

//public slots:

//private slots:
//void Connect_();
//void Disconnect_();
//void Error_();
};

#endif // BUGCLIENT_H


