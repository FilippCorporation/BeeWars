#ifndef BUFERBEE_H
#define BUFERBEE_H

#include <QObject>
#include <QList>
#include"gobee.h"
class BuferBee
{
public:
    BuferBee();
    QList<GoBee>bee;
    quint8 time;
    int kol;
};

#endif // BUFERBEE_H
