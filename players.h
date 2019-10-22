#ifndef PLAYERS_H
#define PLAYERS_H

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QNetworkDatagram>
#include <QHostAddress>
class Players
{
public:
    Players();
    qint32 s=100000;
    int kol_pat=10;
    int add = 150;
    bool activ_add=false;
    quint8 id_sesia;
    QString name;
    QByteArray name_u8;
    QNetworkDatagram playDm;
    quint32 time_close=0;
    int del_count=0;
    quint32 b_time=0;


};

Q_DECLARE_METATYPE(Players)
Q_DECLARE_METATYPE(QList<Players>)

#endif // PLAYERS_H
