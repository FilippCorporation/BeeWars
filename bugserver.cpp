#include "bugserver.h"

QT_USE_NAMESPACE
using namespace std;

BugServer::BugServer(quint16 port,QObject *parent) :  QObject(parent),
    bindPort(port) {
    qRegisterMetaType<QList<Players>>("QList<Players>");

    connect(&udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
    connect(&udpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socket_error(QAbstractSocket::SocketError)));

    do_bee();

    if (!udpSocket.bind(QHostAddress(QHostAddress::Any), bindPort, QUdpSocket::ReuseAddressHint | QUdpSocket::ShareAddress)){
        //qDebug()<<"Cannot bind server udp Socket";
    }

}
BugServer::BugServer(quint16 port,quint16 width, quint16 height,quint16 count, QObject *parent) :  QObject(parent),

                                                                                                    bindPort(port) {
    this->hei=height;
    this->wid=width;
    this->kol=count;
    qRegisterMetaType<QList<Players>>("QList<Players>");

    connect(&udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
    connect(&udpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socket_error(QAbstractSocket::SocketError)));

    do_bee();

    if (!udpSocket.bind(QHostAddress(QHostAddress::Any), bindPort, QUdpSocket::ReuseAddressHint | QUdpSocket::ShareAddress)){
        //qDebug()<<"Cannot bind server udp Socket";
    }

}


void BugServer::readPendingDatagrams(){

    while (udpSocket.hasPendingDatagrams()){
        QNetworkDatagram dm(udpSocket.receiveDatagram());
        bool f = true;
        for(int i=0;i<Player.size();i++){
            if((Player[i].playDm.senderPort()==dm.senderPort())&&(Player[i].playDm.senderAddress()==dm.senderAddress())){f=false;}
        }
        QByteArray message(dm.data());

        if(f){
            if(static_cast<int>(*(message.constData()+1)) != 1)return;
             }

        switch(static_cast<int>(*(message.constData()+1))){
        case 1:{
            process_f01(message, dm);
            qDebug()<<"Кто-то вошел погулять!!!";
            break;
        }
        case 3:{
            fillMessage04();
            fillMessage06();
            break;
        }        
        case 9:{
            process_f09(message, dm);
            break;
        }
        case 11:{
            process_f0b(message,dm);
            break;
        }
        case 13:{
            process_f0d(message);
            qDebug()<<" 13 ---- "<<message;

            fillMessage04();
            fillMessage06();
            break;
        }
        default:break;
//        {
//            qDebug()<<message<<"     Что-то получил?!"<<static_cast<int>(*(message.constData()+1));

//        }

        }
    }



    //udpSocket.writeDatagram(dm.data(), dm.senderAddress(), dm.senderPort());
    //udpSocket.writeDatagram(dm.makeReply(dm.data()));//или этот вариант

}

void BugServer::socket_error(QAbstractSocket::SocketError err){
    //qDebug()<<"UDP Server: socket error: "<<err;
}



void BugServer::fillMessage02(){
    //restartServer(49152, 640, 480, 50);
    message_f02.resize(19);
    quint8 *ptr = reinterpret_cast<quint8*> (message_f02.data());
    *ptr = 0x00; ++ptr;
    *ptr = 0x02; ++ptr;
    *ptr = 0x00; ++ptr;
    *ptr=static_cast< const quint8>(kol);++ptr;
    *reinterpret_cast<quint16*> (ptr) = static_cast<const quint16>(wid); ptr += sizeof(quint16);
    *reinterpret_cast<quint16*> (ptr) = static_cast<const quint16>(hei); ptr += sizeof(quint16);
    *ptr = static_cast<const quint8>(size_b); ++ptr;
    *ptr = static_cast<const quint8>(size_b);++ptr;
    *ptr = static_cast<const quint8>(down_fall);++ptr; //падение жука
    *ptr = static_cast<const quint8>(30);++ptr;
    *ptr = static_cast<const quint8>(35);++ptr;
    *ptr = static_cast<const quint8>(150);++ptr;
    *ptr = static_cast<const quint8>(100);++ptr;
    *ptr = static_cast<const qint8>(5);++ptr;
    *ptr = static_cast<const qint8>(-3);++ptr;
    *ptr = static_cast<const qint8>(-15);++ptr;
    *ptr = static_cast <const quint8>(id_players);++ptr;

}

void BugServer::fillMessage02_error(QString str){
    QByteArray ar=str.toUtf8();
    message_f02.resize(ar.size()+3);
    quint8 *ptr = reinterpret_cast<quint8*> (message_f02.data());
    *ptr = 0x00; ++ptr;
    *ptr = 0x02; ++ptr;
    *ptr = static_cast<const quint8>(ar.size());++ptr;
    memcpy(ptr,ar,ar.size());
    ptr+=ar.size();

}/*
void BugServer::fillMessage02_error(){

    QString str="Ошибка, такое имя игрока уже существует!!!";
    QByteArray ar=str.toUtf8();
    message_f02.resize(ar.size()+3);
    quint8 *ptr = reinterpret_cast<quint8*> (message_f02.data());
    *ptr = 0x00; ++ptr;
    *ptr = 0x02; ++ptr;
    *ptr = static_cast<const quint8>(ar.size());++ptr;
    memcpy(ptr,ar,ar.size());
    ptr+=ar.size();

}*/

void BugServer::fillMessage04()
{

    int leng = 3;
    for(int i=0; i<Player.size();i++){
        leng+=2;
        leng+=Player[i].name_u8.size();
    }
    message_f04.resize(leng);

    quint8 *ptr = reinterpret_cast<quint8*> (message_f04.data());
    *ptr = 0xFF; ++ptr;
    *ptr = 0x04; ++ptr;
    *ptr = static_cast<const quint8>(Player.size()); ++ptr;

    for(int i=0;i<Player.size();i++){

        *ptr = Player[i].id_sesia; ++ptr;
        *ptr =static_cast<quint8>(Player[i].name_u8.size()); ++ptr;
        memcpy(ptr, Player[i].name_u8,Player[i].name_u8.size());
        ptr+=Player[i].name_u8.size();

    }
    for(int i=0;i<Player.size();i++){
        udpSocket.writeDatagram(message_f04, Player[i].playDm.senderAddress(), Player[i].playDm.senderPort());
    }
//qDebug()<<" 04 ----   "<<message_f04<<" --- size = "<<Player.size();

//qDebug()<<"kol_na_servere ---- "<<Player.size();

}


void BugServer::fillMessage06()
{
    if(Player.isEmpty())return;
    int leng = 3+(5*Player.size());
    message_f06.resize(leng);
    message_f06.fill('\0');
    quint8 *ptr = reinterpret_cast<quint8*> (message_f06.data());
    *ptr = 0xFF; ++ptr;
    *ptr = 0x06; ++ptr;
    *ptr =static_cast<quint8>(Player.size()); ++ptr;
    for(int i=0;i<Player.size();i++){

        *ptr=static_cast<const quint8>(Player[i].id_sesia);++ptr;
        *reinterpret_cast<qint32*> (ptr) = Player[i].s;ptr+=3;
        //if(Player[i].kol_pat<=0)Player[i].kol_pat=10;
        *ptr=static_cast<const quint8>(Player[i].kol_pat);++ptr;
        //qDebug()<<"player  "<<i<<"     patron = "<<Player[i].kol_pat;
    }
//    qDebug()<<"06 ---  "<<message_f06<<"    kol|pl -- "<<Player.size();
    for(int i=0;i<Player.size();i++){
        udpSocket.writeDatagram(message_f06, /*addr*/ Player[i].playDm.senderAddress(), /*port*/Player[i].playDm.senderPort());
    }
}

void BugServer::fillMessage08()
{

    int size_mes = ((kol * 5) + 1)*10 + 3;
    int fix_mes=3;
    message_f08.reserve(size_mes);
    message_f08.fill('\0');

    quint8 *ptr = reinterpret_cast<quint8*> (message_f08.data());
    *ptr = static_cast<quint8>(0xFF); ++ptr;
    *ptr = 0x08; ++ptr;

    quint8 start_i=(plane_currIdx-9)&planeIdxMask;
    quint8 *ktr;
    *ptr =static_cast<quint8>(plane[start_i].time); ++ptr;

    //bool stopDbg = false;

    for(quint8 i=start_i, temp_count=0; temp_count < 10 ; ++temp_count,incPlaneIdx(i)){

        ktr = ptr; ++ptr;
        ++fix_mes;
        quint8 vis_kol=0;
        for (quint8 j = 0; j < kol; ++j){
            //stopDbg = plane[i].bee[j].state() == Make_bee::Dead;
            if(plane[i].bee[j].state()!=Make_bee::Hidden){
                std::memcpy(ptr, plane[i].bee[j].data, Make_bee::bugSerializedSize);
                vis_kol++;
                fix_mes+=Make_bee::bugSerializedSize;
                ptr+=Make_bee::bugSerializedSize;
            }
        }


        *ktr = vis_kol;

    }

    /*if (stopDbg){
        quint8 dbgIdx = plane_currIdx;
        for (int i = 0; i < 32; ++i,decPlaneIdx(dbgIdx)){
            qDebug()<<QString("TM: "+QString::number(plane[dbgIdx].time) +
                    " State: "+QString::number(static_cast<int>(plane[dbgIdx].bee[0].state())) +
                    " X Y: "+QString::number(plane[dbgIdx].bee[0].x) +"; " +QString::number(plane[dbgIdx].bee[0].y));
        }
        qDebug()<<"--------------";
    }*/

    message_f08.resize(fix_mes);


}

void BugServer::fillMessage12()
{
    if(decali.size() == 0)return;
    QByteArray message_f12;
    int size_mes = 4 + decali.size()*5;
    message_f12.resize(size_mes);
    message_f12.fill('\0');

    quint8 *ptr = reinterpret_cast<quint8*> (message_f12.data());
    *ptr = static_cast<quint8>(0xFF); ++ptr;
    *ptr = 0x12; ++ptr;
    *reinterpret_cast<qint16*> (ptr) = static_cast<qint16>(decali.size()); ptr += sizeof(quint16);
    for(int i=0;i<decali.size();i++){
        *ptr = decali[i].doing; ++ptr;
        *reinterpret_cast<qint16*> (ptr) = decali[i].x; ptr += 2;
        *reinterpret_cast<qint16*> (ptr) = decali[i].y; ptr += 2;
//        qDebug()<<"DEcali ----- "<<i<<"     ----"<<decali[i].x<<"   "<<decali[i].y<<endl;
    }

    for(int i=0;i<Player.size();i++){
        udpSocket.writeDatagram(message_f12, /*addr*/ Player[i].playDm.senderAddress(), /*port*/Player[i].playDm.senderPort());
    }
    decali.clear();
}

void BugServer::fillMessage10(QNetworkDatagram dm, int k)
{

    QByteArray send_mes;
    switch(k){
    case 0:{
        send_mes = QString("Через минуту вас отключат из-за неактивности!!!").toUtf8();
        break;
    }
    case 1:{
        send_mes = QString("Вас отключили от сервера!!!").toUtf8();
        break;
    }
    case 2:{
        send_mes = QString("У вас нет прав на установку бомбы.").toUtf8();
        break;
    }
    case 3:{
        send_mes = QString("Вас удалила система!!!").toUtf8();
        break;
    }
    default:
        return;
    }

    int leng = 3+send_mes.size();

    message_f10.resize(leng);
    int idx=-1;
    for(int i=0;i<Player.size();++i){
        if((Player[i].playDm.senderPort()==dm.senderPort())&&(Player[i].playDm.senderAddress()==dm.senderAddress())){
            idx=i;
            break;
        }}
    if(idx==-1)return;

    quint8 *ptr = reinterpret_cast<quint8*> (message_f10.data());
    *ptr = Player[idx].id_sesia; ++ptr;
    *ptr = 0x10; ++ptr;
    *ptr = static_cast<const quint8>(send_mes.size()); ++ptr;
    memcpy(ptr,send_mes,send_mes.size());
    ptr+=send_mes.size();

    udpSocket.writeDatagram(message_f10, Player[idx].playDm.senderAddress(), Player[idx].playDm.senderPort());



}

void BugServer::do_bee()
{

    for(int i=0;i<M;i++){
        plane[i].bee = new Make_bee[kol];
    }

    for(quint8 i=0;i<kol;++i){

        plane[0].bee[i].setId(i);
        plane[0].bee[i].y=std::rand()%(hei-size_b);
        //qDebug()<<"y = "<<plane[0].bee[i].y;
        plane[0].bee[i].x=std::rand()%(wid-size_b);
        plane[0].bee[i].sost=std::rand()%9;
        //qDebug()<<"sost ="<<plane[0].bee[i].sost;
        plane[0].time = 0;
    }
    plane_currIdx = 0;


    for(int i=0;i<9;i++){
        for(int j=0;j<kol;j++){
            switch (plane[i].bee[j].get_sost()) {
            case 0:
                plane[i].bee[j].x=std::rand()%(wid-size_b);
                plane[i].bee[j].y=std::rand()%(hei-size_b);
                plane[i].bee[j].sost=std::rand()%9;

                break;
            case 1:
                if(plane[i].bee[j].y>0){
                    plane[i].bee[j].y--;

                }else{
                    plane[i].bee[j].x=std::rand()%(wid-size_b);
                    plane[i].bee[j].y=std::rand()%(hei-size_b);
                    plane[i].bee[j].sost=std::rand()%9;

                }
                break;
            case 2:
                if(plane[i].bee[j].y<hei-size_b){
                    plane[i].bee[j].y++;

                }else{
                    plane[i].bee[j].x=std::rand()%(wid-size_b);
                    plane[i].bee[j].y=std::rand()%(hei-size_b);
                    plane[i].bee[j].sost=std::rand()%9;

                }
                break;
            case 3:
                if(plane[i].bee[j].x > 0){
                    plane[i].bee[j].x--;

                }else{
                    plane[i].bee[j].x=std::rand()%(wid-size_b);
                    plane[i].bee[j].y=std::rand()%(hei-size_b);
                    plane[i].bee[j].sost=std::rand()%9;

                }
                break;
            case 4:

                if(plane[i].bee[j].x<wid-size_b){
                    plane[i].bee[j].x++;

                }else{
                    plane[i].bee[j].x=std::rand()%(wid-size_b);
                    plane[i].bee[j].y=std::rand()%(hei-size_b);
                    plane[i].bee[j].sost=std::rand()%9;

                }
                break;
            case 5:

                if((plane[i].bee[j].y>0)&&(plane[i].bee[j].x<wid-size_b)){
                    plane[i].bee[j].x+=std::rand()%6;
                    plane[i].bee[j].y-=std::rand()%6;

                }else{
                    plane[i].bee[j].x=std::rand()%(wid-size_b);
                    plane[i].bee[j].y=std::rand()%(hei-size_b);
                    plane[i].bee[j].sost=std::rand()%9;

                }
                break;

            case 6:

                if((plane[i].bee[j].y>0)&&(plane[i].bee[j].x>0)){
                    plane[i].bee[j].x--;
                    plane[i].bee[j].y--;

                }else{

                    plane[i].bee[j].x=std::rand()%(wid-size_b);
                    plane[i].bee[j].y=std::rand()%(hei-size_b);
                    plane[i].bee[j].sost=std::rand()%9;
                }
                break;
            case 7:

                if((plane[i].bee[j].y<hei-size_b)&&(plane[i].bee[j].x<wid-size_b)){
                    plane[i].bee[j].x++;
                    plane[i].bee[j].y++;

                }else{
                    plane[i].bee[j].x=std::rand()%(wid-size_b);
                    plane[i].bee[j].y=std::rand()%(hei-size_b);
                    plane[i].bee[j].sost=std::rand()%9;

                }
                break;
            case 8:
                if((plane[i].bee[j].y<hei-size_b)&&(plane[i].bee[j].x>0)){
                    plane[i].bee[j].x--;
                    plane[i].bee[j].y++;

                }else{
                    plane[i].bee[j].x=std::rand()%(wid-size_b);
                    plane[i].bee[j].y=std::rand()%(hei-size_b);
                    plane[i].bee[j].sost=std::rand()%9;

                }
                break;
            default:
                break;
            }

            plane[i+1].bee[j].copyFrom(plane[i].bee[j]);
            plane[i+1].bee[j].setState(Make_bee::Alive);
            plane[i+1].time=plane[i].time+1u;

        }
    }
    plane_currIdx=9;

    connect(&timer_do_bee,SIGNAL(timeout()),this,SLOT(doing_bufer()),Qt::UniqueConnection);
    timer_do_bee.start(50);
    connect(&timer_close_pl,SIGNAL(timeout()),this,SLOT(close_pl()),Qt::UniqueConnection);
    timer_close_pl.start(1000);


}


const quint8 & BugServer::incPlaneIdx(quint8 &planeIdx){
    planeIdx = (planeIdx + static_cast<quint8>(1u)) & planeIdxMask;
    return planeIdx;
}

const quint8 & BugServer::decPlaneIdx(quint8 &planeIdx){
    planeIdx = (planeIdx - static_cast<quint8>(1u)) & planeIdxMask;
    return planeIdx;
}

void BugServer::process_f0b(const QByteArray &message, QNetworkDatagram dm)
{
    if(message.size() == 3)return;
    QByteArray send_mes=message.mid(3,message.size());
    if(send_mes.size() > 251) {send_mes.truncate(251);}

    int idx=-1;
    for(int i=0;i<Player.size();++i){
        if((Player[i].playDm.senderPort()==dm.senderPort())&&(Player[i].playDm.senderAddress()==dm.senderAddress())){
            idx=i;
            break;
        }}
    if(idx==-1)return;

//    qDebug()<<time;
//    qDebug()<<Player[idx].b_time;

    if(int(time - Player[idx].b_time) >0){
        Player[idx].b_time=time;
        Player[idx].del_count=0;
        int leng = 4+send_mes.size();
        message_f0c.resize(leng);

        quint8 *ptr = reinterpret_cast<quint8*> (message_f0c.data());
        *ptr = 0xFF; ++ptr;
        *ptr = 0x0c; ++ptr;
        *ptr = Player[idx].id_sesia; ++ptr;

        *ptr = static_cast<const quint8>(send_mes.size()); ++ptr;
        memcpy(ptr,send_mes,send_mes.size());
        ptr+=send_mes.size();
        for(int i=0;i<Player.size();i++){
            udpSocket.writeDatagram(message_f0c, Player[i].playDm.senderAddress(), Player[i].playDm.senderPort());
        }

    }else{
        QStringList str;
        str.append("Вы сможете отправить сообщение в чат только через 3 секунды.");
        str.append("Ваше  сообщение будет отправлено в чат только через 3 секунды.");
        str.append("Подождите 3 секунды.");
        str.append("ЧЕРЕЗ 3 СЕКУНДЫ.");
        str.append("Прекратите издеваться над сервером!!!");
        str.append("Ещё ОДИН раз и вы будете отключены от сервера!!!");
        str.append("Пока!.. Пока!..");


        switch (Player[idx].del_count) {
        case 0:
            send_mes=(str[Player[idx].del_count]).toUtf8();
            break;
        case 1:
            send_mes=(str[Player[idx].del_count]).toUtf8();
            break;
        case 2:
            send_mes=(str[Player[idx].del_count]).toUtf8();
            break;
        case 3:
            send_mes=(str[Player[idx].del_count]).toUtf8();
            break;
        case 4:
            send_mes=(str[Player[idx].del_count]).toUtf8();
            break;
        case 5:
            send_mes=(str[Player[idx].del_count]).toUtf8();
            break;
        default:
            return;
        }
        Player[idx].del_count++;
        Player[idx].b_time=time+3;
        int leng = 4+send_mes.size();
        message_f0c.resize(leng);

        quint8 *ptr = reinterpret_cast<quint8*> (message_f0c.data());
        *ptr = 0xFF; ++ptr;
        *ptr = 0x0c; ++ptr;
        *ptr = Player[idx].id_sesia; ++ptr;

        *ptr = static_cast<const quint8>(send_mes.size()); ++ptr;
        memcpy(ptr,send_mes,send_mes.size());
        ptr+=send_mes.size();
        udpSocket.writeDatagram(message_f0c, Player[idx].playDm.senderAddress(), Player[idx].playDm.senderPort());

    }



    //qDebug()<<"mess   ---   "<<send_mes<<"   "<<leng-4;



}

void BugServer::process_f0d(const QByteArray &message)
{
    const int id = static_cast<int>(*(message.constData()));
    int idx=-1;
    for(int i=0; i<Player.size();++i){
        if(Player[i].id_sesia == id){
            idx=i;
        }
    }
    if(idx == -1)return;
//    qDebug()<<" rem  --- "<<Player[idx].name;
    Player.removeAt(idx);
}

void BugServer::send_bees()
{
    if(Player.isEmpty()) return;
    fillMessage08();

    //qDebug()<<"First sent TM to client is: "<<static_cast<int>(*reinterpret_cast<const quint8*>(message_f08.constData()+2));
    for(int i=0;i<Player.size();i++){
        udpSocket.writeDatagram(message_f08, /*addr*/ Player[i].playDm.senderAddress(), /*port*/Player[i].playDm.senderPort());
    }

}

void BugServer::doing_bufer()
{

    static int send_time = 0;
    static int send_shcool = 0;
    static int send_decal = 0;
    if(send_time<6){
        send_time++;
    }
    else{
        send_time=0;
        send_bees();

    }
    if(send_decal<8){
        send_decal++;
    }
    else{
        send_decal=0;
        fillMessage12();

    }
    if(send_shcool<100){
        send_shcool++;
    }
    else{
        send_shcool=0;
        fillMessage06();

    }
    quint8 i = plane_currIdx;

    incPlaneIdx(plane_currIdx);

    for(int k=0;k<bomb.size();k++){
        if((!bomb.isEmpty())&&(bomb[k].time == plane[i].time)){
            bool control = false;
            int kill_bee[4] {0};
            for(int j=0;j<kol;j++){
                if((pow((plane[i].bee[j].x-bomb[0].x),2)+pow((plane[i].bee[j].y-bomb[0].y),2)<=pow(55,2))||
                        (pow((plane[i].bee[j].x-bomb[0].x),2)+pow((plane[i].bee[j].y-bomb[0].y+size_b),2)<=pow(55,2))||
                        (pow((plane[i].bee[j].x-bomb[0].x+size_b),2)+pow((plane[i].bee[j].y-bomb[0].y+size_b),2)<=pow(55,2))||
                        (pow((plane[i].bee[j].x-bomb[0].x+size_b),2)+pow((plane[i].bee[j].y-bomb[0].y),2)<=pow(55,2)))
                {
                    control=true;
                    switch(plane[i].bee[j].state()){
                    case Make_bee::Hidden :
                        ++kill_bee[0];
                        break;

                    case Make_bee::Alive : {
                        ++kill_bee[1];
                        plane[i].bee[j].setState(Make_bee::Dead);
                        plane[i].bee[j].time_to_hide = plane[i].time+ static_cast<quint8>(200u);
                        quint8 temp_currIdx = plane_currIdx;
                        incPlaneIdx(temp_currIdx);

                        for(quint8 k = i, t_count=0; k!=temp_currIdx ; ++t_count,incPlaneIdx(k)){

                            plane[k].bee[j].copyFrom(plane[i].bee[j],plane[i].bee[j].x,plane[i].bee[j].y,plane[i].bee[j].sost);

                        }
                        plane[i].bee[j].setState(Make_bee::JustKilled);
                        break;
                    }

                    case Make_bee::JustKilled :
                        ++kill_bee[2];
                        break;

                    case Make_bee::Dead :
                        ++kill_bee[3];
                        break;


                    }
                }
            }

            int idx = -1;
            for(int i=0;i<Player.size();++i){
                if(Player[i].id_sesia == bomb[k].id_create){
                    idx =i;
                    break;
                }
            }
            if(idx == -1)return;
            if(!control){
                Player[idx].s-=100;
            }
            if(kill_bee[1] != 0){
                Player[idx].s +=5*kill_bee[1];
            }
            fillMessage06();
            bomb.removeAt(k);

        }
    }






    for(int j=0;j<kol;j++){

        plane[plane_currIdx].bee[j].copyFrom(plane[i].bee[j]);

        switch(plane[plane_currIdx].bee[j].state()){

        case Make_bee::JustKilled:
            //plane[plane_currIdx].bee[j].fall++;
            plane[plane_currIdx].bee[j].setState(Make_bee::Dead);
            plane[plane_currIdx].bee[j].time_to_hide = plane[i].time+ static_cast<quint8>(100u);
            break;

        case Make_bee::Dead:
        if(fall == 1){
            if(plane[plane_currIdx].bee[j].fall==2){
                const int heightDiff = hei-size_b - plane[i].bee[j].y;
                const quint16 downStep =  static_cast<quint16>((heightDiff > down_fall)? down_fall : heightDiff);
                if(plane[plane_currIdx].bee[j].y<hei-size_b){
                    plane[plane_currIdx].bee[j].y=plane[i].bee[j].y+downStep;

                }/*else {
                    plane[plane_currIdx].bee[j].fall=0;
                }*/}
            else{plane[plane_currIdx].bee[j].fall++;}
        }
//            if(plane[plane_currIdx].bee[j].fall==2){
//                if(plane[plane_currIdx].bee[j].y<hei-size_b){
//                    plane[plane_currIdx].bee[j].y+=10;

//                }else {
//                    plane[plane_currIdx].bee[j].fall=0;
//                }}
//            else{plane[plane_currIdx].bee[j].fall++;}

            if(plane[plane_currIdx].bee[j].time_to_hide == plane[i].time){
                plane[plane_currIdx].bee[j].time_to_alive = plane[i].time + static_cast<quint8>(100u);
                plane[plane_currIdx].bee[j].setState(Make_bee::Hidden);
            }
            break;
        case Make_bee::Hidden:
            if(plane[plane_currIdx].bee[j].time_to_alive == plane[i].time){
                plane[plane_currIdx].bee[j].fall=0;
                plane[plane_currIdx].bee[j].setState(Make_bee::Alive);
            }
            break;
        default:
            break;
        }
        if(plane[plane_currIdx].bee[j].state()==Make_bee::Alive){

            switch (plane[plane_currIdx].bee[j].sost) {
            case 0:
                plane[plane_currIdx].bee[j].x=std::rand()%(wid-size_b);
                plane[plane_currIdx].bee[j].y=std::rand()%(hei-size_b);
                plane[plane_currIdx].bee[j].sost=std::rand()%9;

                break;
            case 1:
                if(plane[plane_currIdx].bee[j].y>0){
                    plane[plane_currIdx].bee[j].y--;

                }else{
                    plane[plane_currIdx].bee[j].x=std::rand()%(wid-size_b);
                    plane[plane_currIdx].bee[j].y=std::rand()%(hei-size_b);
                    plane[plane_currIdx].bee[j].sost=std::rand()%9;

                }
                break;
            case 2:
                if(plane[plane_currIdx].bee[j].y<hei-size_b){
                    plane[plane_currIdx].bee[j].y++;

                }else{
                    plane[plane_currIdx].bee[j].x=std::rand()%(wid-size_b);
                    plane[plane_currIdx].bee[j].y=std::rand()%(hei-size_b);
                    plane[plane_currIdx].bee[j].sost=std::rand()%9;

                }
                break;
            case 3:
                if(plane[plane_currIdx].bee[j].x > 0){
                    plane[plane_currIdx].bee[j].x--;

                }else{
                    plane[plane_currIdx].bee[j].x=std::rand()%(wid-size_b);
                    plane[plane_currIdx].bee[j].y=std::rand()%(hei-size_b);
                    plane[plane_currIdx].bee[j].sost=std::rand()%9;

                }
                break;
            case 4:

                if(plane[plane_currIdx].bee[j].x<wid-size_b){
                    plane[plane_currIdx].bee[j].x++;

                }else{
                    plane[plane_currIdx].bee[j].x=std::rand()%(wid-size_b);
                    plane[plane_currIdx].bee[j].y=std::rand()%(hei-size_b);
                    plane[plane_currIdx].bee[j].sost=std::rand()%9;

                }
                break;
            case 5:

                if((plane[plane_currIdx].bee[j].y>0)&&(plane[i].bee[j].x<wid-size_b)){
                    plane[plane_currIdx].bee[j].x+=std::rand()%6;
                    plane[plane_currIdx].bee[j].y-=std::rand()%6;

                }else{
                    plane[plane_currIdx].bee[j].x=std::rand()%(wid-size_b);
                    plane[plane_currIdx].bee[j].y=std::rand()%(hei-size_b);
                    plane[plane_currIdx].bee[j].sost=std::rand()%9;

                }
                break;

            case 6:

                if((plane[plane_currIdx].bee[j].y>0)&&(plane[i].bee[j].x>0)){
                    plane[plane_currIdx].bee[j].x--;
                    plane[plane_currIdx].bee[j].y--;

                }else{

                    plane[plane_currIdx].bee[j].x=std::rand()%(wid-size_b);
                    plane[plane_currIdx].bee[j].y=std::rand()%(hei-size_b);
                    plane[plane_currIdx].bee[j].sost=std::rand()%9;
                }
                break;
            case 7:

                if((plane[plane_currIdx].bee[j].y<hei-size_b)&&(plane[i].bee[j].x<wid-size_b)){
                    plane[plane_currIdx].bee[j].x++;
                    plane[plane_currIdx].bee[j].y++;

                }else{
                    plane[plane_currIdx].bee[j].x=std::rand()%(wid-size_b);
                    plane[plane_currIdx].bee[j].y=std::rand()%(hei-size_b);
                    plane[plane_currIdx].bee[j].sost=std::rand()%9;

                }
                break;
            case 8:
                if((plane[plane_currIdx].bee[j].y<hei-size_b)&&(plane[i].bee[j].x>0)){
                    plane[plane_currIdx].bee[j].x--;
                    plane[plane_currIdx].bee[j].y++;

                }else{
                    plane[plane_currIdx].bee[j].x=std::rand()%(wid-size_b);
                    plane[plane_currIdx].bee[j].y=std::rand()%(hei-size_b);
                    plane[plane_currIdx].bee[j].sost=std::rand()%9;

                }
                break;
            default:
                break;
            }

        }



        plane[plane_currIdx].time=plane[i].time+1u;


    }
}

void BugServer::close_pl()
{
    emit client_list(Player);
    time++;
    for(int i=0;i<Player.size();i++){
        ++Player[i].time_close;
//        qDebug()<<"tic-tac"<<Player[i].time_close;
        if(Player[i].time_close == 60){
            fillMessage10(Player[i].playDm,0);
        }
        if(Player[i].time_close > 120){
            fillMessage10(Player[i].playDm,1);
          //
            fillMessage04();
            fillMessage06();
            Player[i].time_close=0;
        }

    }


}

void BugServer::remove_cl(int i)
{
//    if(Player.size()<=i)return;
    fillMessage10(Player[i].playDm,3);
    Player.removeAt(i);
    fillMessage04();
    fillMessage06();

}

BugServer::~BugServer()
{
    udpSocket.close();

    //    m_pWebSocketServer->close();

    //    qDeleteAll(m_clients.begin(), m_clients.end());
}

void BugServer::process_f01(const QByteArray &message, QNetworkDatagram dm){

    const int strLength =static_cast<int>(*(message.constData()+2));
    if (message.size() < (3 + strLength)) return;
    const QString newPlayerName (QString::fromUtf8(message.constData() + 3, strLength) );
    if((newPlayerName.toLower()=="admin")||(newPlayerName.toLower()=="админ")
        ||(newPlayerName.toLower()=="администратор")||(newPlayerName.toLower()=="server")
        ||(newPlayerName.toLower()=="сервер")){
        QString str="Ошибка, такое имя игрока уже существует!!!";
        fillMessage02_error(str);
        udpSocket.writeDatagram(message_f02, dm.senderAddress(), dm.senderPort());
        return;
    }
    for(int i=0;i<Player.size();i++){
        if((Player[i].name==newPlayerName)&&(Player[i].playDm.senderPort()!=dm.senderPort())
            &&(Player[i].playDm.senderAddress()!=dm.senderAddress())){
            QString str="Ошибка, такое имя игрока уже существует!!!";
            fillMessage02_error(str);
            udpSocket.writeDatagram(message_f02, dm.senderAddress(), dm.senderPort());
            return;
        }else{
            if(Player[i].name==newPlayerName){
                Player.removeAt(i);
            }
        }
    }

    if(Player.size()>0){
        bool can_go_forward = true;
        if(id_players > max_players)
        {
            id_players = 1;
        }
        int zacikleno = id_players;
        while(true)
        {

            if(id_players > max_players)
            {
                id_players = 1;
            }
//            qDebug()<<"Player.size == "<< Player.size();======================================================================================================
            for(int i=0;i<=Player.size()-1;i++)
            {
//                qDebug()<<"i == "<< i;
                if(Player[i].id_sesia == id_players)
                {
                    can_go_forward = false;
                }
            }
            if(can_go_forward == true)
            {
                break;
            }else{
                id_players++;
                if(id_players > max_players)
                {
                    id_players = 1;
                }
                if(id_players == zacikleno)
                {
                    can_go_forward = false;
                    break;
                }
                can_go_forward = true;
            }
        }
        if(can_go_forward == false)
        {
            QString str="Максимальное количество игроков на сервере";
            fillMessage02_error(str);
            udpSocket.writeDatagram(message_f02, dm.senderAddress(), dm.senderPort());
            return;

        }
    }

    Players pl;
    pl.name = newPlayerName;
    pl.name_u8 = message.mid(3,strLength);
    pl.id_sesia = id_players;
    pl.kol_pat=7;
    pl.playDm=dm;
    fillMessage02();
    Player.append(pl);
    udpSocket.writeDatagram(message_f02, dm.senderAddress(), dm.senderPort());
    fillMessage04();
    fillMessage06();
    id_players++;

}


void BugServer::process_f09(const QByteArray &message, QNetworkDatagram dm){

    if (message.size() < 3) return;
    const quint8 id_pl =*reinterpret_cast<const quint8*>(message.constData());
    const quint8 time = *reinterpret_cast<const quint8*>(message.constData()+2);
    const int type =*reinterpret_cast<const quint8*>(message.constData()+3);
    const int x=*reinterpret_cast<const quint16*>(message.constData()+4);
    const int y=*reinterpret_cast<const quint16*>(message.constData()+6);
    int idx = -1;
    for(int i=0;i<Player.size();++i){
        if(Player[i].id_sesia == id_pl){
            idx =i;
            break;
        }
    }
    Decal dec;
    dec.x = static_cast<qint16>(x);
    dec.y = static_cast<qint16>(y);
    Player[idx].time_close=0;
    if(type == 1){
        if(Player[idx].s<150){
            fillMessage10(dm,2);
            return;
        }else {
            Player[idx].s-=150;
            fillMessage06();
        }

        Bombs boom;
        boom.x=x;
        boom.y=y;
        boom.time=time + static_cast<quint8>(100u);
        boom.id_create = id_pl;
        bomb.append(boom);

        message_f0a.resize(10);
        message_f0a.fill('\0');
        quint8 *ptr = reinterpret_cast<quint8*> (message_f0a.data());
        *ptr = 0xff; ++ptr;
        *ptr = 0x0a; ++ptr;
        *ptr = id_pl; ++ptr;
        *ptr = time; ++ptr;
        *ptr = 1; ++ptr;
        *reinterpret_cast<quint16*> (ptr) = x; ptr+=sizeof(quint16);
        *reinterpret_cast<quint16*> (ptr) = y; ptr+=sizeof(quint16);
        *ptr =time + static_cast<quint8>(100u); ++ptr;
        for(int i=0;i<Player.size();++i){
            udpSocket.writeDatagram(message_f0a, /*addr*/ Player[i].playDm.senderAddress(), /*port*/Player[i].playDm.senderPort());
        }


    }


    if(Player[idx].kol_pat<=1){
        Player[idx].kol_pat=7;
        //qDebug()<<"Not patron ------------------------------------------------------------------";
        fillMessage06();
    }else{
        Player[idx].kol_pat--;
    }
    int kill_bee[4] {0};
    bool control = false;
    //bool stopDbg = false;
    for(quint8 i=plane_currIdx, temp_count=0; temp_count < M ; ++temp_count,decPlaneIdx(i)){


        if((time == plane[i].time)&&(type == 0)){

            for(int j=0;j<kol;j++){
                if((plane[i].bee[j].x<=x)&&(plane[i].bee[j].y<=y)&&(plane[i].bee[j].x+size_b>x)&&(plane[i].bee[j].y+size_b>y)){
                    control=true;
                    switch(plane[i].bee[j].state()){
                    case Make_bee::Hidden :
                        ++kill_bee[0];
                        break;

                    case Make_bee::Alive : {
                        ++kill_bee[1];
                        //stopDbg = true;
                        plane[i].bee[j].setState(Make_bee::Dead);
                        plane[i].bee[j].time_to_hide = plane[i].time+ static_cast<quint8>(200u);
                        quint8 temp_currIdx = plane_currIdx;
                        incPlaneIdx(temp_currIdx);
                        int localFall = 0;

                        for(quint8 k = i, t_count=0, z = i+static_cast<quint8>(1u); k!=temp_currIdx ; ++t_count, incPlaneIdx(k)){

                            plane[k].bee[j].copyFrom(plane[i].bee[j],plane[i].bee[j].x,plane[i].bee[j].y,plane[i].bee[j].sost);
                        if(fall == 1){
                        Q_ASSERT(z!=k);
                            if(/*plane[k].bee[j].fall*/localFall==2){
                                plane[k].bee[j].fall = 2;
                                const int heightDiff = hei-size_b - plane[z].bee[j].y;
                                const quint16 downStep =  static_cast<quint16>((heightDiff > down_fall)? down_fall : heightDiff);
                                if(plane[k].bee[j].y<hei-size_b){
                                    plane[k].bee[j].y=plane[z].bee[j].y+downStep;

                                }/*else {
                                    //plane[k].bee[j].fall=0;
                                }*/}
                            else{plane[k].bee[j].fall = localFall++;}
                            z = k;
                            }
                        }
                        plane[i].bee[j].setState(Make_bee::JustKilled);

                        break;
                    }

                    case Make_bee::JustKilled :{
                        ++kill_bee[2];

                        break;
}
                    case Make_bee::Dead :{
                        ++kill_bee[3];
                        break;
}
                    }
                }
            }

            break;
        }


    }


    /*if (stopDbg){
            quint8 dbgIdx = plane_currIdx;
            for (int i = 0; i < 32; ++i,decPlaneIdx(dbgIdx)){
                qDebug()<<QString("TM: "+QString::number(plane[dbgIdx].time) +
                        " State: "+QString::number(static_cast<int>(plane[dbgIdx].bee[0].state())) +
                        " X Y: "+QString::number(plane[dbgIdx].bee[0].x) +"; " +QString::number(plane[dbgIdx].bee[0].y));
            }
            qDebug()<<"--------------";
        }*/


    if(type == 0){
        int static_dec=0;
        static_dec+=Player[idx].id_sesia;

        if(kill_bee[1] >= 2){
            static_dec+=32;
            Player[idx].kol_pat++;
        }
        if(!control)Player[idx].s-=3;
        if(kill_bee[1] != 0){
            static_dec+=128;
            Player[idx].s +=pow(5,kill_bee[1]);

        }else{
            Player[idx].s -= (15*(kill_bee[3]+kill_bee[2]));
        }
        fillMessage06();
        dec.doing = static_cast<quint8>(static_dec);
        decali.append(dec);
    }
    //Q_ASSERT( k != -1);
}



//----------------------------Многопоточность------------------------------------------------------------------------------------------------------



//BugServerThread::~BugServerThread()
//{
//    terminate_r();
//}

//bool BugServerThread::restartServer(quint16 port, quint16 boardWidth, quint16 boardHeight, quint8 bugCount)
//{
//if(!argMutex.tryLock(1000)) return false;
//this->port= port;
//this->boardHeight= boardHeight;
//this->boardWidth= boardWidth;
//this->bugCount= bugCount;
//argMutex.unlock();
//terminate_r();
//return true;
//}
//void BugServerThread::terminate_r(){
//    if(this->isRunning()){
//        this->exit();
//    }
//    while( !runMutex.tryLock()){
//        QApplication::processEvents();

//    }
//    runMutex.unlock();

//    this->start();
//}

//void BugServerThread::run()
//{
//    if (!argMutex.tryLock(1000)) {
//        return;
//    }

//    if (!runMutex.tryLock(1000)) {
//        argMutex.unlock();
//        return;
//    }


//    bugserver = new BugServer(port,boardWidth,boardHeight,bugCount);

//    argMutex.unlock();

//    exec();

//    delete bugserver;
//    bugserver = nullptr;

//    runMutex.unlock();

//}
