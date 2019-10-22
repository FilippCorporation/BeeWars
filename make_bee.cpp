#include "make_bee.h"

Make_bee::Make_bee():data {0}, x(*reinterpret_cast<quint16*>(data + 1)), y(*reinterpret_cast<quint16*>(data + 3)),sost(0)
{

}
Make_bee::Make_bee(const Make_bee& a):x(*reinterpret_cast<quint16*>(data + 1)), y(*reinterpret_cast<quint16*>(data + 3)),sost(a.sost),time_to_hide(a.time_to_hide),time_to_alive(a.time_to_alive),fall(a.fall){
    memcpy(data, a.data, bugSerializedSize);

}

void Make_bee::setId(quint8 id){
    data[0] = (data[0] & 0xC0) | id;
}

quint8 Make_bee::id() const{
    return data[0] & 0x3Fu;
}

void Make_bee::setState(BugState state){
    data[0] = (data[0] & 0x3Fu) | (static_cast<quint8>(state));
}

Make_bee::BugState Make_bee::state() const{
    return static_cast<BugState>(data[0] & 0xC0);
}

int Make_bee::get_sost()
{
   return sost;

}

void Make_bee::copyFrom(const Make_bee &bee)
{
    std::memcpy(data, bee.data, bugSerializedSize);
    sost=bee.sost;
    time_to_hide = bee.time_to_hide;
    time_to_alive = bee.time_to_alive;
        fall=bee.fall;
}

void Make_bee::copyFrom(const Make_bee &bee, const quint16 & new_x, const quint16 & new_y,const quint8 & new_sost){
    data[0] = bee.data[0];
    x = new_x;
    y = new_y;
    sost = new_sost;
    time_to_hide = bee.time_to_hide;
    time_to_alive = bee.time_to_alive;
    fall=bee.fall;
}

