#include "bombs.h"

void Bombs::_loadPixAndSound( const Bombs &a)
{
    if(a.it_pix_bax!=nullptr){

    it_pix_bax=new QGraphicsPixmapItem(a.it_pix_bax->pixmap(),a.it_pix_bax->parentItem());    
    a.it_pix_bax->scene()->addItem(it_pix_bax);
    it_pix_bax->setPos(a.it_pix_bax->pos());
    it_pix_bax->setZValue(a.it_pix_bax->zValue());
    it_pix_bax->setVisible(a.it_pix_bax->isVisible());
    }
    else {
        it_pix_bax = nullptr;
    }
    effect.setSource(a.effect.source());
    effect.setVolume(a.effect.volume());
    effect.setMuted(a.effect.isMuted());
}

Bombs::Bombs():it_pix_bax(nullptr)
{

}

Bombs::Bombs( const Bombs &a):x(a.x),y(a.y),time(a.time),id_create(a.id_create),
     status_bax(a.status_bax),active_boom(a.active_boom),effect(a.effect.parent())
{
    _loadPixAndSound(a);
}

Bombs::~Bombs()
{
    if(it_pix_bax!=nullptr)
        delete it_pix_bax;
}

Bombs &Bombs::operator=( const Bombs &a)
{
    x=a.x;
    y=a.y;
    time=a.time;
    id_create=a.id_create;
    status_bax=a.status_bax;
    active_boom=a.active_boom;
    effect.setParent(a.effect.parent());
    _loadPixAndSound(a);
    return *this;
}
