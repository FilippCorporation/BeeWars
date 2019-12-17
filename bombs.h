#ifndef BOMBS_H
#define BOMBS_H
#include <QtCore>
#include<QGraphicsItem>
#include <QGraphicsScene>
#include<QGraphicsPixmapItem>
//#include<QSound>
#include<QSoundEffect>
#include <QUrl>
class Bombs
{
protected:
    void _loadPixAndSound( const Bombs &a);
public:
    Bombs();
    Bombs( const Bombs &a);
    int x;
    int y;
    quint8 time;
    quint8 id_create;
//    QGraphicsItem* it_bax;
    QGraphicsPixmapItem* it_pix_bax;
    int status_bax=0;
    bool active_boom=1;
    QSoundEffect effect;
    ~Bombs();

    Bombs& operator=( const Bombs& a);
};



#endif // BOMBS_H
