#ifndef MAKE_BEE_H
#define MAKE_BEE_H

#include <QtCore>
#include <cstdio>
#include <cstring>

class Make_bee
{

public:
    enum BugState {Hidden = 0x0, Alive = 0x40, JustKilled = 0x80, Dead = 0xC0};
    Make_bee();
    Make_bee(const Make_bee& a);
    static const int bugSerializedSize = 5;
    quint8 data[bugSerializedSize];
    quint16 &x;
    quint16 &y;
    quint8 sost;
    quint8 time_to_hide = 150;
    quint8 time_to_alive = 250;
    void setId(quint8 id);
    quint8 id() const;

    int fall=0;
    void setState(BugState state);
    BugState state() const;
    int get_sost();
    void copyFrom(const Make_bee &bee);
    void copyFrom(const Make_bee &bee, const quint16 & new_x, const quint16 & new_y,const quint8 & new_sost);
};



#endif // MAKE_BEE_H
