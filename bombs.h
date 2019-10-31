#ifndef BOMBS_H
#define BOMBS_H
#include <QtCore>
#include<QGraphicsItem>
#include<QGraphicsPixmapItem>


class Bombs
{
public:
    Bombs();
    int x;
    int y;
    quint8 time;
    quint8 id_create;
    QGraphicsItem* it_bax;
    QGraphicsPixmapItem* it_pix_bax;
    int status_bax=0;
    bool active_boom=1;
};

#endif // BOMBS_H
