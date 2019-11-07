#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    load_ap();


    //chat = new Chat(ui->textBrowser,ui->send_mes,25);
    //chat->createSmileButtons(ui->panel_smile);

    connect(ui->toolButton,SIGNAL(clicked()),this,SLOT(hide_win()));
    connect(ui->toolButton_3,SIGNAL(clicked()),this,SLOT(add_server()));
    connect(ui->toolButton_4,SIGNAL(clicked()),this,SLOT(add_client()));
    connect(ui->toolButton_2,SIGNAL(clicked()),this,SLOT(show_score()));
    connect(ui->exit,SIGNAL(clicked()),this,SLOT(_close()));
    connect(&timer,SIGNAL(timeout()),this,SLOT(goto_xy()));
    connect(&time_bee,SIGNAL(timeout()),this,SLOT(go_pix()));
    connect(&time_go_bee,SIGNAL(timeout()),this,SLOT(go_bee()));
    connect(&timer_1,SIGNAL(timeout()),this,SLOT(ammo_function()));
    connect(ui->left,SIGNAL(clicked()),this,SLOT(muz_left()));
    connect(ui->right,SIGNAL(clicked()),this,SLOT(muz_right()));
    connect(ui->play,SIGNAL(clicked()),this,SLOT(muz_play()));
    connect(ui->stop,SIGNAL(clicked()),this,SLOT(muz_stop()));
    connect(ui->volue,SIGNAL(clicked()),this,SLOT(muz_volue()));
    connect(ui->send,SIGNAL(clicked()),this,SLOT(send_message()));
    connect(&timer_pr,SIGNAL(timeout()),this,SLOT(set_pix()));
    connect(&timer_bax,SIGNAL(timeout()),this,SLOT(set_bax()));
    connect(ui->server_off,SIGNAL(clicked()),this,SLOT(close_serv()));
    connect(ui->problems,SIGNAL(clicked()),this,SLOT(Problem()));
    connect(ui->treeWidget,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(close_players(QTreeWidgetItem*,int)));
    connect(ui->send_mes,SIGNAL(returnPressed()),this,SLOT(send_message()));
    connect(&timer_proc_10,SIGNAL(timeout()),this,SLOT(for_10_timer()));

    ui->label_15->hide();
    ui->label_16->hide();
    ui->radioButton->hide();
    ui->radioButton_2->hide();
    ui->label_8->hide();




    patron[0]=ui->p1;
    patron[1]=ui->p2;
    patron[2]=ui->p3;
    patron[3]=ui->p4;
    patron[4]=ui->p5;
    patron[5]=ui->p6;
    patron[6]=ui->p7;


    music->setMedia(QUrl::fromLocalFile(sndPath+"theme.mp3"));
    music->setVolume(70);
    music->play();




    List = nullptr;
    read = nullptr;
    read_bomb = nullptr;
    scene = new Play_table();
    server = nullptr;
    client = nullptr;
    ui->widget_2->hide();
    ui->server_menu->hide();
    QPalette sample_palette;
    sample_palette.setColor(QPalette::Window, Qt::white);
    sample_palette.setColor(QPalette::WindowText, Qt::blue);

    connect(scene,SIGNAL(shotPoint(QPoint, bool)),this,SLOT(set_pos(QPoint, bool)));
    connect(scene,SIGNAL(movePoint(QPoint)),this,SLOT(for_10(QPoint)));

    ui->widget_5->hide();
    ui->play->setStyleSheet("#play\n{\nbackground-color:transparent;\nborder-image:url(:/пауза.png);\nbackground: none;\nborder:none;\nbackground-repeat:none;\n}\n#play:pressed\n{\nborder-image:url(:/пауза_н.png);\nbackground-repeat:none;\n}");
    QIcon icon(":/клякса1.png");
    this->setWindowIcon(icon);
    QBrush *brush_for_text_info = new QBrush(QColor(255,0,0));
    text_info = new QGraphicsSimpleTextItem();
    text_info->setVisible(false);
    text_info->setZValue(5);
    text_info->setBrush(*brush_for_text_info);
    text_info->setPen(QColor(Qt::red));
    text_info->setX(0);
    text_info->setY(0);
    scene->addItem(text_info);
    setMouseTracking(true);

}

MainWindow::~MainWindow()
{
    save_ap();
    delete ui;
}

QByteArray MainWindow:: process_f1(){
    QByteArray fil_1;
    QByteArray name = ui->lineEdit->text().toUtf8();
    fil_1.reserve(3+name.size());
    fil_1.resize(3);
    quint8 *ptr = reinterpret_cast<quint8*> (fil_1.data());
    *ptr = 0x00; ++ptr;
    *ptr = 0x01; ++ptr;
    *ptr = static_cast<const quint8>(name.size()); ++ptr;
    fil_1.append(name);
    ui->name_pl->setText(name);
    //memcpy(ptr, name.toUtf8(),name.size());
    //ptr+=name.size();
    ui->message_text->setText("Регистрация игрока на сервере ...");
    return fil_1;
}

void MainWindow::process_f2(const QByteArray &message)
{
    qDebug()<<"process02";


    if(message.size() < 2) return ;
    if(*reinterpret_cast<const quint8*>(message.constData()+2)==0){
        kol_be=*reinterpret_cast<const quint8*>(message.constData()+3);
        wid =*reinterpret_cast<const quint16*>(message.constData()+4);
        hei =*reinterpret_cast<const quint16*>(message.constData()+6);
        be_w =*reinterpret_cast<const quint8*>(message.constData()+8);
        be_h =*reinterpret_cast<const quint8*>(message.constData()+9);
        fall =*reinterpret_cast<const quint8*>(message.constData()+10);
        kol_pat =*reinterpret_cast<const quint8*>(message.constData()+11);
        time_rep =*reinterpret_cast<const quint8*>(message.constData()+12);
        cost_boom =*reinterpret_cast<const quint8*>(message.constData()+13);
        r_boom =*reinterpret_cast<const quint8*>(message.constData()+14);
        s_y =*reinterpret_cast<const qint8*>(message.constData()+15);
        s_p =*reinterpret_cast<const qint8*>(message.constData()+16);
        s_m =*reinterpret_cast<const qint8*>(message.constData()+17);
        id =*reinterpret_cast<const quint8*>(message.constData()+18);

        ui->widget->hide();
        ui->widget_2->show();
        music->stop();

        show_win();

        QPixmap pix1(":/фон1.png");
        QPixmap pix(":/фон.png");
        pix1=pix1.scaled(wid,hei,Qt::IgnoreAspectRatio,Qt::FastTransformation);
        pix=pix.scaled(wid,hei,Qt::IgnoreAspectRatio,Qt::FastTransformation);

        scene->setSceneRect(0,0, wid, hei);

        ui->graphicsView->setScene(scene);
        ui->graphicsView->show();

        if(ui->radioButton->isChecked()){
            scene->addPixmap(pix);
            ui->graphicsView->setBackgroundBrush(QBrush(QPixmap("://88.png")));
        }else{
            scene->addPixmap(pix1);
            ui->graphicsView->setBackgroundBrush(QBrush(QPixmap("://зап_фона_4.png")));
        }

        //        ui->graphicsView->setCursor(Qt::CrossCursor);
        //        ui->graphicsView->setCursor(QPixmap(QString("://курсор_1.png")));
        ui->graphicsView->setCursor(QPixmap(QString("://прицел_1.png")));
        ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground);

        player.play();
        play[muz].play();

        do_pix();
        List= new QTreeWidget(ui->graphicsView/*scrollArea*/);
        List->setColumnCount(2);
        List->headerItem()->setText(0,"Имя игрока");
        List->headerItem()->setText(1,"Счёт");
        f=true;
        timer.start(500);
        time_bee.start(25);

    }else{
        QString warning=message.mid(3);
        //qDebug()<<"error ---- "<<warning;
        ui->message_text->setText(warning);


    }
}

QByteArray MainWindow::process_f3()
{
    QByteArray fil_3;
    QByteArray name = ui->lineEdit->text().toUtf8();
    fil_3.reserve(2+name.size());
    fil_3.resize(2);
    quint8 *ptr = reinterpret_cast<quint8*> (fil_3.data());
    *ptr = 0x03; ++ptr;
    *ptr = static_cast<const quint8>(name.size()); ++ptr;
    fil_3.append(name);
    return fil_3;
}

void MainWindow::process_f4(const QByteArray &message)
{
    List->clear();
    Gamer.clear();
    if(message.size() < 2) return;
    const int size =*reinterpret_cast< const quint8*>(message.constData()+2);
    int k=3;
    for(int i=0;i<size;++i){
        Players players;
        players.id_sesia=*reinterpret_cast< const quint8*>(message.constData()+k);
        ++k;
        const int size_name =*reinterpret_cast< const quint8*>(message.constData()+k);
        ++k;
        QString name = QString::fromUtf8(message.constData()+k, (size_name));
        k+=size_name;
        players.name=name;
        players.s=0;
        Gamer.append(players);

    }

}

void MainWindow::process_f6(const QByteArray &message){
    List->clear();
    if(message.size() < 2) return;
    const int size =*reinterpret_cast< const quint8*>(message.constData()+2);
    int k=3;
    for(int i=0;i<size;++i){
        const quint8 id=*reinterpret_cast< const qint8*>(message.constData()+k);
        k++;
        qint32 score =*reinterpret_cast< const qint32*>(message.constData()+k);
        score&=0xFFFFFF;
        if(score&0x800000){
            score|=0xFF000000;
        }
        int kol_patron =*reinterpret_cast< const qint32*>(message.constData()+k);
        kol_patron&=0xFF000000;
        kol_patron>>=24;
        k+=4;
        if(Gamer[i].id_sesia == id){
            Gamer[i].s=score;
            Gamer[i].kol_pat=kol_patron;
        }
        if(Gamer[i].id_sesia == this->id){
            this->score=score;
            this->kol_pat=kol_patron;
        }
        QTreeWidgetItem *citi = new QTreeWidgetItem(List);
        citi->setText(0, Gamer[i].name);
        citi->setText(1, QString::number(Gamer[i].s,10));
        qDebug()<<"kol_patron - "<<Gamer[i].kol_pat;
        qDebug()<<"gamers - "<<Gamer.size();
    }
    ui->score->setText(QString::number(score,10));


}

void MainWindow::process_f8(const QByteArray &message)
{
    if(message.size() < 2) return;
    if(on_cl_time){
        cl_time = *reinterpret_cast<const quint8*>(message.constData()+2);
        on_cl_time=false;
    }
    quint8 time = *reinterpret_cast<const quint8*>(message.constData()+2);
    int dif=0;
    if(cl_time>time){
        dif=cl_time-time;
    }else if(cl_time<time){
        dif=256-time+cl_time;
    }
    if(dif>time){
        dif=time-cl_time;
    }
    if(dif<0){
        dif+=256;
    }
    int p=0;
    if(idx_bufer>=dif){
        p=idx_bufer-dif;
    }else{
        p=11-dif+idx_bufer;
    }
    if(dif>=8){
        p=0;
        bufer_bee.clear();
        idx=0;
    }
//        qDebug()<<"cl_t --- "<<cl_time<<"   pos_t --- "<<time<<endl;
//        qDebug()<<"p --- "<<p<<" idx --- "<<idx<<"dif --- "<<dif<<endl;
//        qDebug()<<"DIF ---- "<<dif<<"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";

    int k=3;
    for(int i=0,j=p;i<10;i++,j++){
        if(j>10){ j=0;}
        const int size= *reinterpret_cast<const quint8*>(message.constData()+k);
        k++;
        BuferBee bees;
        bees.time=time+i;
        cl_time=bees.time;
        idx_bufer=j;
        bees.kol=size;
        int beeIdx = 0;
        for(;beeIdx<size;++beeIdx){
            GoBee be;
            be.s=*reinterpret_cast<const quint8*>(message.constData()+k);
            ++k;
            be.x =*reinterpret_cast<const quint16*>(message.constData()+k);
            k+=2;
            be.y =*reinterpret_cast<const quint16*>(message.constData()+k);
            k+=2;
            //qDebug()<<"Bx --- "<<be.x<<" By --- "<<be.y<<" IDX_bufer --- "<<j<<"BEE_idx --- "<<beeIdx;
            bees.bee.append(be);
        }
        //        qDebug()<<endl;

        if(bufer_bee.size()<11){
            bufer_bee.append(bees);
        }
        else{
            bufer_bee[j]=bees;
        }

    }
    if(!time_go_bee.isActive()){time_go_bee.start(50);}

}

void MainWindow::process_f0a(const QByteArray &message)
{
    if(message.size() < 2) return;
    const quint8 id_pl =*reinterpret_cast<const quint8*>(message.constData()+2);
    const quint8 time = *reinterpret_cast<const quint8*>(message.constData()+3);
    const int x=*reinterpret_cast<const quint16*>(message.constData()+5);
    const int y=*reinterpret_cast<const quint16*>(message.constData()+7);
    const quint8 time_active = *reinterpret_cast<const quint8*>(message.constData()+9);
    if(id_pl!=id){
    Bombs bomb;
    bomb.time=time;
    bomb.id_create=5u;
    bomb.x=x-25;
    bomb.y=y-25;
    bomb.status_bax=0;
    bomb.it_pix_bax=scene->addPixmap(boom[0]);
    bomb.it_pix_bax->setZValue(5);
    bomb.it_bax=bomb.it_pix_bax;
    bomb.active_boom=1;
    bomb.it_bax->setPos(bomb.x,bomb.y);
    bombs.append(bomb);
    if(!timer_bax.isActive())timer_bax.start(120);
    }
}

void MainWindow::process_f10(const QByteArray &message)
{
//    qDebug()<<"IA TYTA++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++";
    const quint8 strLength =*reinterpret_cast<const quint8*>((message.constData()+2));
    const QString text_player (QString::fromUtf8(message.constData()+3, strLength));
    text_info->setText(text_player);
    if(text_info->text() == "Вас отключили от сервера!!!"){
//        qDebug()<<"выйти";
        //ui->exit->click();
    }else {
        text_info->setVisible(true);
        sch_process_10 = (f10_time_show*1000)+1;
        if(timer_proc_10.isActive())
        {
            //timer_proc_10.stop();
        }
        timer_proc_10.start(10);
        if(!coord_of_sight.isNull()){
            text_info->setX(coord_of_sight.x()-150);
            text_info->setY(coord_of_sight.y()+40);
        }
        qDebug()<<"Timer started";
    }
}
void MainWindow::for_10(QPoint coord)
{
    coord_of_sight = coord;
    if(sch_process_10 <= (f10_time_show*1000)){
        text_info->setX(coord_of_sight.x()-150);
        text_info->setY(coord_of_sight.y()+40);
    }
}

void MainWindow::for_10_timer()
{
//    qDebug()<<"In Time; sch == "<<sch_process_10;
    sch_process_10--;
    if(sch_process_10 <=0)
    {
        timer_proc_10.stop();
        sch_process_10 = (f10_time_show*100)+1;
        text_info->setVisible(false);
//        qDebug()<<"Timer stopped";
    }
}
void MainWindow::process_f12(const QByteArray &message)
{
    if(message.size() < 2) return;
    const int size_decal = *reinterpret_cast<const quint16*>(message.constData()+2);
    int k=4;
    for(int i=0;i<size_decal;i++){
        int decal_item = *reinterpret_cast<const quint8*>(message.constData()+k);
        ++k;
        int x = *reinterpret_cast<const qint16*>(message.constData()+k);
        k+=2;
        int y = *reinterpret_cast<const qint16*>(message.constData()+k);
        k+=2;
        if(decal_item>=128){
            decal_item-=128;
        }
        if(decal_item>=32){
            //booom
            decal_item-=32;
            if(decal_item!=id){
                babax(x,y);
            }
        }

    }
}

void MainWindow::babax(int x, int y)
{
    QGraphicsPixmapItem* it_pix = scene->addPixmap(max_boom[0]);
    it_pix->setZValue(5);
    QGraphicsItem* it_boom = it_pix;
    it_boom->setPos(x-75,y-75);
    it_pix->hide();
    it_bax.append(it_boom);
    it_pix_bax.append(it_pix);
    sost_bax.append(1);
    connect(&bax_time,SIGNAL(timeout()),this,SLOT(active()),Qt::UniqueConnection);
    bax_time.start(70);
}

void MainWindow::process_f0c(const QByteArray &message)
{
    const quint8 id_mes=*reinterpret_cast<const quint8*>(message.constData()+2);
    const quint8 strLength =*reinterpret_cast<const quint8*>((message.constData()+3));
    if (message.size() < (3 + strLength)) return;
    const QString text_player (QString::fromUtf8(message.constData() + 4, strLength) );
    for(int i=0; i<Gamer.size();i++){
        if(id_mes==Gamer[i].id_sesia){
            QString name_pl=Gamer[i].name;
//            ui->textBrowser->append(name_pl+": "+text_player);
            //chat->appendMessage(name_pl+": "+text_player);
        }
    }
}

void MainWindow::go_bee()
{
    if(on_reloading == true)
    {
        switch (pic_rand_ammo) {
        case 0:
            ui->graphicsView->setCursor(QPixmap(QString("://перезарядка1.png")));
            break;
        case 1:
            ui->graphicsView->setCursor(QPixmap(QString("://перезарядка2.png")));
            break;
        case 2:
            ui->graphicsView->setCursor(QPixmap(QString("://перезарядка3.png")));
            break;
        case 3:
            ui->graphicsView->setCursor(QPixmap(QString("://перезарядка.png")));
            break;
        }
        if(pic_rand_ammo < 4)
        {
            pic_rand_ammo++;
        }
        else{
            pic_rand_ammo = 0;
        }
    }

    if(idx>10){idx=0;}
    int size=bufer_bee[idx].kol;
    regular_time=bufer_bee[idx].time;
    cl_time=regular_time;
//    qDebug()<<"idx = "<<idx;
    const int beesToAppend = size - item_bees.size();
    for(int j=0;j<beesToAppend;++j){
        it_pix=scene->addPixmap(*picture[9]);
        it_pix->setZValue(2);
        item_pix.append(it_pix);
        item_bees.append(it_pix);
        status_bee.append(0);
        dir_bee.append(0);
        flag_bee.append(1);
        bee_show++;
    }
    if(bee_show>size){
        if(hidden_bee.size()>=bee_show-size){
            for(int i=0;i<bee_show-size;i++){
                item_bees[hidden_bee[0]]->hide();
                show_bee.append(hidden_bee[0]);
                flag_bee[hidden_bee[0]]=0;
                hidden_bee.removeFirst();
            }
            bee_hide-=bee_show-size;
            bee_show-=bee_show-size;
        }else{
            qDebug()<<"OOPS.... !!!! BIG BAG ----- "<<hidden_bee.size()<<" NEED HIDE --- "<<bee_hide;
        }
    }
    if(bee_show<size){
        if(show_bee.size()>=size-bee_show){
            for(int i=0;i<size-bee_show;i++){
                flag_bee[show_bee[0]]=1;
                item_bees[show_bee[0]]->show();
                show_bee.removeFirst();
            }
            bee_show+=size-bee_show;}else {
            qDebug()<<"OOPS.... !!!! BIG BAG #2 ----- "<<show_bee.size()<<" NEED HIDE --- "<<size-bee_show;
        }
    }
    int beeIdx = 0;
    int bee_cadr = 0;
    for(;(beeIdx<item_bees.size())&&(bee_cadr<size);++beeIdx){
        if(flag_bee[beeIdx]){
            //      qDebug()<<"kadrs ---- "<<bee_cadr;
            if(idx == 10){
                if(bufer_bee[0].kol<bufer_bee[idx].kol){
                    dir_bee[beeIdx]=0;
                }else{
                    dir_bee[beeIdx]=bufer_bee[idx].bee[bee_cadr].x-bufer_bee[0].bee[bee_cadr].x;
                }
            }else{
                if(bufer_bee[idx+1].kol<bufer_bee[idx].kol){
                    dir_bee[beeIdx]=0;
                }else{
                    dir_bee[beeIdx]=bufer_bee[idx].bee[bee_cadr].x-bufer_bee[idx+1].bee[bee_cadr].x;
                }
            }
//            if(((dir_bee[beeIdx]!=0)||(show_bee.size()>0))&&(flag_bee[beeIdx]==3)){
//                //   qDebug()<<"ERROR ---- PICTURE ";
//                if(hidden_bee.indexOf(beeIdx)!=-1){
//                    //     qDebug()<<"REM ---- ERR"<<beeIdx;
//                    // qDebug()<<"STAY_BEE ---- "<<show_bee.size();
//                    hidden_bee.removeAt(hidden_bee.indexOf(beeIdx));
//                }
//                flag_bee[beeIdx]=1;
//            }

            int sost=bufer_bee[idx].bee[bee_cadr].s;
            int x=bufer_bee[idx].bee[bee_cadr].x;
            int y=bufer_bee[idx].bee[bee_cadr].y;
           //    qDebug()<< "sost ---- "<<sost<<"     bee_idx ---"<<beeIdx;

            if(sost<=63){
                dir_bee[beeIdx]=-20;
                qDebug()<<"ERROR";
            }
            if(sost>=192){
                if(flag_bee[beeIdx]==1){
                    if(fall==0){
                        flag_bee[beeIdx]=3;
                    }else{
                        flag_bee[beeIdx]=4;
                    }
                    item_pix[beeIdx]->setPixmap(*picture[9]);
                    item_pix[beeIdx]->setZValue(1);
                    hidden_bee.append(beeIdx);
//                    bee_hide++;

                }
                sost-=192;
            }
            if(sost>=128){
                if(flag_bee[beeIdx]==1){
                    if(fall==0){
                        flag_bee[beeIdx]=3;
                    }else{
                        flag_bee[beeIdx]=4;
                    }
                    item_pix[beeIdx]->setPixmap(*picture[9]);
                    item_pix[beeIdx]->setZValue(1);
                    hidden_bee.append(beeIdx);
                }
                sost-=128;

            }
            if(sost>=64){
                if((flag_bee[beeIdx]==3)||(flag_bee[beeIdx]==4)){
                    item_bees[beeIdx]->hide();
                    flag_bee[beeIdx]=1;
                    hidden_bee.removeAt(hidden_bee.indexOf(beeIdx));
//                    qDebug()<<"bee_show --- "<<bee_show<<"  hidden_bee.size() --- "<<hidden_bee.size()<<"size --- "<<size;
//                    hidden_bee.clear();
                    hidden_bee.removeAt(hidden_bee.indexOf(beeIdx));
//                    bee_hide--;
                }
            }
            item_bees[beeIdx]->setPos(x,y);



            bee_cadr++;
        }
    }
    idx++;

}

void MainWindow::go_pix()
{

//    text_info.show();
//    text_info.move(ui->graphicsView->cursor().pos().x(),ui->centralWidget->cursor().pos().y());
//    qDebug()<<text_info.pos()<<"    -----     info"<<ui->centralWidget->cursor().pos()<<"  -----  ----- cursor"<<endl;

    if(read_bee_left==nullptr){
        read_bee_left = new QImageReader(QString("://пчёлка.gif"));
        for(int i=0;i<read_bee_left->imageCount();++i){
            bee_left.push_back(QPixmap::fromImageReader(read_bee_left).scaled(be_w,be_h));
        }
        read_bee_right = new QImageReader(QString("://пчёлка_л.gif"));
        for(int i=0;i<read_bee_right->imageCount();++i){
            bee_right.push_back(QPixmap::fromImageReader(read_bee_right).scaled(be_w,be_h));
        }
    }
    for(int i=0;i<item_pix.size();i++){
        if(flag_bee[i]==0){
            item_bees[i]->hide();
            item_pix[i]->setPixmap(bee_left[0]);
            item_pix[i]->setZValue(2);
        }
        if(flag_bee[i]==3){
            item_pix[i]->setPixmap(*picture[9]);
            item_pix[i]->setZValue(1);
            item_bees[i]->show();
        }
        if(flag_bee[i]==4){
            const int heightDiff = hei-be_h - item_bees[i]->pos().toPoint().y();
                    const int downStep = (heightDiff > fall)? fall : heightDiff;
                    if(item_bees[i]->pos().toPoint().y()<hei-be_h){
                        item_bees[i]->setY(item_bees[i]->pos().toPoint().y()+downStep);
                    }else{
                        flag_bee[i]=3;
                    }
        }
        if(flag_bee[i]==1){
            if(dir_bee[i]>=0){
                if(status_bee[i]<read_bee_left->imageCount()){
                    item_pix[i]->setPixmap(bee_left[status_bee[i]]);
                    item_pix[i]->setZValue(2);
                    item_bees[i]->show();
                    status_bee[i]++;
                }
                else{
                    status_bee[i]=0;
                }
            }else{
                if(status_bee[i]<read_bee_right->imageCount()){
                    item_pix[i]->setPixmap(bee_right[status_bee[i]]);
                    item_pix[i]->setZValue(2);
                    item_bees[i]->show();
                    status_bee[i]++;
                }
                else{
                    status_bee[i]=0;
                }
            }
        }
    }
//    List->setStyleSheet("background-color: rgba(255,255,255,220)");
    List->move(ui->graphicsView->geometry().center().x()-128,ui->graphicsView->geometry().center().y()-96);

}
void MainWindow::do_pix()
{
    read = new QImageReader(QString(file[11]));
    read_bomb = new QImageReader(QString("://бомба.gif"));
    for(int i=0;i<read_bomb->imageCount();++i){
        boom.push_back(QPixmap::fromImageReader(read_bomb).scaled(70,40));
    }
    for(int i=0;i<read->imageCount();++i){
        min_boom.push_back(QPixmap::fromImageReader(read).scaled(100,100));
        max_boom.push_back(QPixmap::fromImageReader(read));
    }
    it_pix_boom=scene->addPixmap(min_boom[0]);
    it_pix_boom->setZValue(4);
    it_boom=it_pix_boom;
    it_boom->setPos(50,50);
    it_pix_boom->hide();
}
void MainWindow::set_pix()
{
    if(size_boom == 0){
        if(status_boom<read->imageCount()){
            it_pix_boom->show();
            it_pix_boom->setPixmap(min_boom[status_boom]);
            status_boom++;
        }
        else{
            status_boom=0;
            it_pix_boom->hide();
            timer_pr.stop();
        }
    }else{
        if(status_boom<read->imageCount()){
            if(!it_pix_boom->isVisible()){
                it_pix_boom->show();}
            it_pix_boom->setPixmap(max_boom[status_boom]);
            status_boom++;
        }
        else{
            it_pix_boom->hide();
            status_boom=0;
            timer_pr.stop();
        }
    }
}
void MainWindow::set_bax()
{
    if(bombs.size()==0){
        timer_bax.stop();
        return;
    }

    for(int i=0;i<bombs.size();i++){
        if((bombs[i].status_bax<boom.size())&&(bombs[i].active_boom)){
            bombs[i].it_pix_bax->show();
            bombs[i].it_pix_bax->setPixmap(boom[bombs[i].status_bax]);
            bombs[i].status_bax++;
        }
        else{
            if(bombs[i].active_boom){
                bombs[i].status_bax=0;
                bombs[i].it_pix_bax->setPixmap(max_boom[bombs[i].status_bax]);
                bombs[i].status_bax++;
                bombs[i].it_bax->setPos(bombs[i].x-50,bombs[i].y-50);
                bombs[i].active_boom=0;
            }else {
                if(bombs[i].status_bax<read->imageCount()){
                    bombs[i].it_pix_bax->setPixmap(max_boom[bombs[i].status_bax]);
                    bombs[i].status_bax++;
                    if(!effect[3].isPlaying()){
                       effect[3].play();
                    }
                }else {
                    scene->removeItem(bombs[i].it_bax);
                    bombs.removeAt(i);
                }
            }
        }
    }
}

void MainWindow::ammo_function(){
    if(number_of_ammo < 7)
    {
        on_reloading = true;
        number_of_ammo++;
        patron[number_of_ammo-1]->show();
        //patron[number_of_ammo-1]->show();
        //kol_pat++;
        if(!effect[4].isPlaying()){
            effect[4].play();
        }
        qDebug()<<"Идет перезарядка - "<<kol_pat;
    }
else if(number_of_ammo == 7){
        //kol_pat = 7;
        effect[4].stop();
        timer_1.stop();
        ui->graphicsView->setCursor(QPixmap(QString("://прицел_1.png")));
        on_reloading = false;
        qDebug()<<"перезарядка закончилась";
    }else {
        return;
    }
}

void MainWindow::set_pos(QPoint p, bool sost)
{
    if(on_reloading == true)
    {
        return;
    }

    if(sost){
        if(score>=cost_boom){
        Bombs bomb;
        bomb.time=regular_time;
        bomb.id_create=5u;
        bomb.x=p.x()-25;
        bomb.y=p.y()-25;
        bomb.status_bax=0;
        bomb.it_pix_bax=scene->addPixmap(boom[0]);
        bomb.it_pix_bax->setZValue(4);
        bomb.it_bax=bomb.it_pix_bax;
        bomb.active_boom=1;
        bomb.it_bax->setPos(bomb.x,bomb.y);

        bombs.append(bomb);
        if(!timer_bax.isActive())timer_bax.start(120);
        QByteArray message_f09;
        message_f09.resize(8);
        quint8 *ptr = reinterpret_cast<quint8*> (message_f09.data());
        *ptr = id; ++ptr;
        *ptr = 0x09; ++ptr;
        *ptr = bomb.time; ++ptr;
        *ptr = sost; ++ptr;
        *reinterpret_cast<quint16*> (ptr) = bomb.x; ptr += sizeof(quint16);
        *reinterpret_cast<quint16*> (ptr) = bomb.y; ptr += sizeof(quint16);
        client->udpSocket.writeDatagram(message_f09, address,port);
        }

    }
    else
    {


        if(f){
            if(kol_pat == 1){
                if(on_reloading == false)
                {
                    on_reloading = true;
                }
            }
            ///Добавить патроны?!
            //number_of_ammo--;
            if(kol_pat <= 7){
                patron[kol_pat-1]->hide();
            }
            if(!effect[5].isPlaying()){
                effect[5].play();
            }


            this->x=p.x();
            this->y=p.y();
            quint8 time_cl=regular_time;
            int idx_buf=idx;
            if(idx_buf>10){idx_buf=10;}
            BuferBee fire_bee=bufer_bee[idx_buf];
            bool control = false;
            int kill_bee[3] {0};
            //        qDebug()<<"kol --- "<<fire_bee.kol<<endl;
            for(int i=0;i<fire_bee.kol;i++){
                //            qDebug()<<"x -- "<<fire_bee.bee[i].x<<" y -- "<<fire_bee.bee[i].y<<" s -- "<<fire_bee.bee[i].s;
                if((fire_bee.bee[i].x<=x)&&(fire_bee.bee[i].y<=y)&&(fire_bee.bee[i].x+be_w>x)&&(fire_bee.bee[i].y+be_h>y)){
                    control = true;
                    if(fire_bee.bee[i].s>=192){++kill_bee[2];fire_bee.bee[i].s-=192;}
                    if(fire_bee.bee[i].s>=128){++kill_bee[1];fire_bee.bee[i].s-=128;}
                    if(fire_bee.bee[i].s>=64){++kill_bee[0];}
                }
            }

            if(!control){score+=s_p;}else{
                if(kill_bee[0] != 0){
                    score +=pow(s_y,kill_bee[0]);
                }else {
                    score +=(s_m*(kill_bee[2]+kill_bee[1]));
                }

            }
            ui->score->setText(QString::number(score,10));

            QByteArray message_f09;
            message_f09.resize(8);

            quint8 *ptr = reinterpret_cast<quint8*> (message_f09.data());
            *ptr = id; ++ptr;
            *ptr = 0x09; ++ptr;
            *ptr = time_cl; ++ptr;
            *ptr = sost; ++ptr;
            *reinterpret_cast<quint16*> (ptr) = x; ptr += sizeof(quint16);
            *reinterpret_cast<quint16*> (ptr) = y; ptr += sizeof(quint16);
            client->udpSocket.writeDatagram(message_f09, address,port);



            if(kill_bee[0]>=3){
                timer_pr.stop();
                effect[3].setVolume(100);
                effect[3].play();
                size_boom=1;
                status_boom=0;
                it_boom->setPos(x-75,y-75);
                timer_pr.start(60);
            }
            if(kill_bee[0]==2){
                timer_pr.stop();
                size_boom=0;
                status_boom=0;
                it_boom->setPos(x-50,y-50);
                timer_pr.start(60);
                effect[7].setVolume(100);
                effect[7].play();
            }
            if(kill_bee[0]==1){
                effect[6].setVolume(100);
                effect[6].play();
            }

            if(kill_bee[0] == 0){
                effect[0].setVolume(100);
                effect[0].play();
            }
            if(on_reloading == true)
            {
                number_of_ammo = 0;
                ammo_function();
                qDebug()<<"таймер пошел";
                timer_1.start(1000);
                set_pos(p,sost);
                return;
            }
        }

    }
}

void MainWindow::listen(bool f)
{
    if (!f){
        ui->server_menu->hide();
        QMessageBox::critical(this, QString::fromUtf8(u8"Ошибка сервера"), QString::fromUtf8(u8"Не удалось запустить сервер на порту №")+ui->spinBox_4->text());
        showMainMenu();
        return;
    }
    if(create_client){
        creating_client();
    }
}

void MainWindow::fun_helper()
{
    if(create_server){

        if(server != nullptr){
            delete server;
            server = nullptr;
        }
        //server = new BugServerThread(ui->spinBox_4->value(),ui->spinBox->value(),ui->spinBox_2->value(),ui->spinBox_3->value());
        server = new BugServer(ui->spinBox_4->value(),ui->spinBox->value(),ui->spinBox_2->value(),ui->spinBox_3->value());
        //connect(server,SIGNAL(server_state(bool)),this,SLOT(listen(bool)));
        connect(server,SIGNAL(client_list(QList<Players>)),this,SLOT(Tree(QList<Players>)));

        connect(this,SIGNAL(close_num(int)),server,SLOT(remove_cl(int)));

        create_server = false;

    }
    if(create_client){

        creating_client();

    }

}

void MainWindow::close_serv()
{
    ui->problems->setChecked(false);
    ui->label_15->hide();
    ui->label_16->hide();
    ui->widget->show();
    ui->server_menu->hide();
    if(server!= nullptr){
        delete server;
        server = nullptr;
    }
    ui->treeWidget->clear();
}

void MainWindow::Tree(QList<Players> Player)
{
    ui->treeWidget->clear();
    for(int i=0;i<Player.size();i++){
        QTreeWidgetItem *cities = new QTreeWidgetItem(ui->treeWidget);
        cities->setText(0, QString::number(Player[i].id_sesia,10));
        cities->setText(1,Player[i].name);
        cities->setText(2, QString::number(Player[i].s,10));
        cities->setText(3, QString::number(Player[i].kol_pat,10));

        QString str = Player[i].playDm.senderAddress().toString();
        QString str2 = str.mid(7,str.size());
        if(str2=="")str2="127.0.0.1";
        cities->setText(4, str2);
        cities->setText(5, QString::number(Player[i].time_close,10));
    }
    ui->treeWidget->show();
}

void MainWindow::Problem()
{
    if((ui->label_15->isVisible())&&(ui->label_16->isVisible())){
        ui->label_15->hide();
        ui->label_16->hide();
    }else{
        ui->label_15->show();
        ui->label_16->show();}
}

void MainWindow::close_players(QTreeWidgetItem *item, int n)
{
    int i = ui->treeWidget->indexOfTopLevelItem(item);
    qDebug()<<" string --  "<<i;

    emit close_num(i);

}

void MainWindow::creating_client()
{
    if(client != nullptr){
        delete client;
        client = nullptr;
    }

    QString ip = ui->ip->text();
    if(ip=="")
    {
        ip+="127.0.0.1";
    }
    if(ip!="")
    {
        quint8 kol_dots = 0;
        char dot = '.';
        QString ip_check = "";
        bool ip_check_fail = false;
        for(int i = 0;i < ip.size();++i)
        {
            if((ip[i] != ",")&&(ip[i] != "."))
            {
                ip_check += ip[i];
            }
            if((ip[i] == ",")||(ip[i] == "."))
            {
                kol_dots++;
                ip[i] = dot;
                if((ip_check.toInt() > 255)||(ip_check.toInt() < 0))
                {
                    ip_check_fail = true;
                }
                ip_check = "";
            }
            if(i == ip.size() - 1)
            {
                if((ip_check.toInt() > 255)||(ip_check.toInt() < 0))
                {
                    ip_check_fail = true;
                }
                ip_check = "";
            }
        }
        if((kol_dots != 3)||(ip_check_fail == true))
        {
            ui->message_text->setText("Неправильно введенный ip-адрес");
            return;
        }
    }
    ui->ip->setText(ip);

    address=QHostAddress(ip);
    port=ui->spinBox_4->value();
    client = new BugClient(address, port);

    connect(client,SIGNAL(mySignal(int)),this,SLOT(setSignal(int)));
    create_client = false;

    //    if(client != nullptr){
    //        delete client;
    //        client = nullptr;
    //    }

    //    QString ip = ui->ip->text();
    //    if(ip==""){
    //        ip+="127.0.0.1";
    //    }
    //    address=QHostAddress(ip);
    //    port=ui->spinBox_4->value();
    //    client = new BugClient(address, port);

    //    connect(client,SIGNAL(mySignal(int)),this,SLOT(setSignal(int)));
    //    //connect(client.data()->webSocket,SIGNAL(textMessageReceived(QString)),this->chat,SLOT(appendMessage(QString)));
    //    create_client = false;
}

void MainWindow::showMainMenu()
{
    static QMutex mutex;
    if (!mutex.try_lock())
        // error и disconnected высылают mySignal 12.
        //И сервер неуспевая удалиться и установиться в nullptr ещё раз пытается удалиться,
        //т.к. там идут вызовы QApplication::ProcessEvents(), что порождает ошибку сегментирования!
        return;
    close_do();
    if(server != nullptr){
        delete server;
        server = nullptr;
    }

    if(client != nullptr){
        client.data()->deleteLater();
        client = nullptr;
    }
    mutex.unlock();
}

void MainWindow::save_ap()
{
    QSettings setting(QSettings::IniFormat,QSettings::UserScope,QApplication::organizationName(),QApplication::applicationName());
    setting.beginGroup("Set");

    setting.setValue("MainWindow", this->geometry());
    setting.setValue("WindowState", static_cast<int>(this->windowState()));

    setting.endGroup();

    setting.beginWriteArray("Dask",6);

    setting.setValue("Play_name",ui->lineEdit->text());
    setting.setValue("IP",ui->ip->text());
    setting.setValue("Port",ui->spinBox_4->value());
    setting.setValue("Wid",ui->spinBox->value());
    setting.setValue("Hei",ui->spinBox_2->value());
    setting.setValue("Kol_Bee",ui->spinBox_3->value());

    setting.endArray();

}

void MainWindow::load_ap()
{
    QSettings setting(QSettings::IniFormat,QSettings::UserScope,QApplication::organizationName(),QApplication::applicationName());
    setting.beginGroup("Set");
    QVariant val = setting.value("MainWindow");
    if (val.isValid()){
        QRect rect(val.toRect());
        if (rect.isValid()){
            this->setGeometry(rect);
        }
    }
    setting.value("WindowState");
    setting.endGroup();


    setting.beginReadArray("Dask");


    val = setting.value("Play_name");
    if(val.isValid()){
        ui->lineEdit->setText(setting.value("Play_name").toString());
    }
    else{
        ui->lineEdit->setText(QString("Игрок 1"));
    }

    val = setting.value("IP");
    if(val.isValid()){
        if(setting.value("IP").toString()==""){
            ui->ip->setText(QString("127.0.0.1"));
        }else{
            ui->ip->setText(setting.value("IP").toString());
        }
    }
    else{
        ui->ip->setText(QString("127.0.0.1"));
    }

    val = setting.value("Port");
    if(val.isValid()){
        ui->spinBox_4->setValue(setting.value("Port").toInt());
    }
    else{
        ui->spinBox_4->setValue(49152);
    }

    val = setting.value("Wid");
    if(val.isValid()){
        ui->spinBox->setValue(setting.value("Wid").toInt());
    }
    else{
        ui->spinBox->setValue(640);
    }

    val = setting.value("Hei");
    if(val.isValid()){
        ui->spinBox_2->setValue(setting.value("Hei").toInt());
    }
    else{
        ui->spinBox_2->setValue(480);
    }

    val = setting.value("Kol_Bee");
    if(val.isValid()){
        ui->spinBox_3->setValue(setting.value("Kol_Bee").toInt());
    }
    else{
        ui->spinBox_3->setValue(16);
    }

    setting.endArray();

}

void MainWindow::show_score()
{
    if(Gamer.size()==0){
        QByteArray fil_3;
        fil_3.resize(2);
        quint8 *ptr = reinterpret_cast<quint8*> (fil_3.data());
        *ptr = id; ++ptr;
        *ptr = 0x03; ++ptr;
        client->udpSocket.writeDatagram(fil_3, address,port);
    }
    List->show();
}

void MainWindow::setSignal(int p)
{
    switch (p) {
    case 1:
        client->udpSocket.writeDatagram(process_f1(), address,port);
        break;
    case 2:
        process_f2(client->sendMsg);
        break;
    case 4:
        process_f4(client->sendMsg);
        break;
    case 6:
        process_f6(client->sendMsg);
        break;
    case 8:
        process_f8(client->sendMsg);//don't have bagi
        break;
    case 10:
        process_f0a(client->sendMsg);
        break;
    case 12:
        process_f0c(client->sendMsg);
        break;
    case 16:
        process_f10(client->sendMsg);
        break;
    case 18:
        process_f12(client->sendMsg);

        /*if(!argMutex.tryLock(1000)) break;//kakay to dich
        close_do();
        if(client==nullptr){


        }else{
            delete (client);
            client = nullptr;
        }
        argMutex.unlock();
        */
        break;
    default:
        break;
    }

}

void MainWindow::add_server()
{
    ui->ip->setText(QString("127.0.0.1"));
    create_server = true;
    create_client = false;
    fun_helper();

    QString ip;
    QString all_ip;


    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost)){
            //qDebug() << address.toString()<<" -------------------- ip";
            all_ip+=address.toString()+"  ";
            ip=address.toString();
        }
    }

    ui->label_15->setText(all_ip);
    ui->ip_serv->setText(ip);
    ui->port_serv->setText(QString::number(ui->spinBox_4->value(),10));
    ui->widget->hide();
    ui->treeWidget->clear();
    ui->server_menu->show();
    music->stop();

}

void MainWindow::add_client()
{
    create_server = false;
    create_client = true;
    fun_helper();

    //    client = new BugClient(/*QHostAddress::LocalHost,49152*/QHostAddress(ui->ip->text()),ui->spinBox_4->value());
    //    connect(client,SIGNAL(mySignal(int)),this,SLOT(setSignal(int)));
    //    connect(client.data()->webSocket,SIGNAL(textMessageReceived(QString)),this->chat,SLOT(appendMessage(QString)));
}

void MainWindow::hide_win()
{
    ui->ip->setText(QString("127.0.0.1"));
    create_server = true;
    create_client = true;
    fun_helper();

    music->stop();

    //    qDebug()<<"server creator";
    //    server = new BugServer(ui->spinBox_4->value()/*,ui->spinBox->value(),ui->spinBox_2->value(),ui->spinBox_3->value()*/);
    //    //client = new BugClient(ui->spinBox_4->value(),ui->ip->text());
    //connect(client,SIGNAL(mySignal(int)),this,SLOT(setSignal(int)));

}

void MainWindow::muz_volue(){
    //ui->verticalSlider->show();
    if(play[0].volume()==100){
        for(int i=0;i<6;i++)play[i].setVolume(0);
        ui->volue->setStyleSheet("#volue\n{\nbackground-color:transparent;\nborder-image:url(:/нм.png);"
                                 "\nbackground: none;\nborder:none;\nbackground-repeat:none;\n}\n"
                                 "#volue:pressed\n{\nborder-image:url(:/нм_н.png);"
                                 "\nbackground-repeat:none;\n}");}
    else{
        for(int i=0;i<6;i++)play[i].setVolume(100);
        ui->volue->setStyleSheet("#volue\n{\nbackground-color:transparent;"
                                 "\nborder-image:url(:/вм.png);\nbackground: "
                                 "none;\nborder:none;\nbackground-repeat:none;\n}\n"
                                 "#volue:pressed\n{\nborder-image:url(:/вм_н.png);"
                                 "\nbackground-repeat:none;\n}");}

}
void MainWindow::muz_stop(){
    play[muz].stop();
    ui->play->setStyleSheet("#play\n{\nbackground-color:transparent;\nborder-image:url(:/пл.png);\nbackground: none;\nborder:none;\nbackground-repeat:none;\n}\n#play:pressed\n{\nborder-image:url(:/пл_н.png);\nbackground-repeat:none;\n}");
    muz=0;
}
void MainWindow::muz_left(){
    if(muz==0){
        muz=5;
        play[0].stop();
        play[muz].play();
    }else{
        play[muz].stop();
        muz--;
        play[muz].play();
        ui->play->setStyleSheet("#play\n{\nbackground-color:transparent;\nborder-image:url(:/пауза.png);\nbackground: none;\nborder:none;\nbackground-repeat:none;\n}\n#play:pressed\n{\nborder-image:url(:/пауза_н.png);\nbackground-repeat:none;\n}");
    }
}
void MainWindow::muz_right(){
    if(muz==5){
        muz=0;
        play[5].stop();
        play[muz].play();
    }else{
        play[muz].stop();
        muz++;
        play[muz].play();
        ui->play->setStyleSheet("#play\n{\nbackground-color:transparent;\nborder-image:url(:/пауза.png);\nbackground: none;\nborder:none;\nbackground-repeat:none;\n}\n#play:pressed\n{\nborder-image:url(:/пауза_н.png);\nbackground-repeat:none;\n}");
    }
}
void MainWindow::muz_play(){
    if(play[muz].state()==QMediaPlayer::PausedState){
        ui->play->setStyleSheet("#play\n{\nbackground-color:transparent;\nborder-image:url(:/пауза.png);\nbackground: none;\nborder:none;\nbackground-repeat:none;\n}\n#play:pressed\n{\nborder-image:url(:/пауза_н.png);\nbackground-repeat:none;\n}");
        play[muz].play();
        qDebug()<<"play";
    }else{
        if(play[muz].state()==QMediaPlayer::StoppedState){
            play[muz].play();
            ui->play->setStyleSheet("#play\n{\nbackground-color:transparent;\nborder-image:url(:/пауза.png);\nbackground: none;\nborder:none;\nbackground-repeat:none;\n}\n#play:pressed\n{\nborder-image:url(:/пауза_н.png);\nbackground-repeat:none;\n}");
        }else{
            ui->play->setStyleSheet("#play\n{\nbackground-color:transparent;\nborder-image:url(:/пл.png);\nbackground: none;\nborder:none;\nbackground-repeat:none;\n}\n#play:pressed\n{\nborder-image:url(:/пл_н.png);\nbackground-repeat:none;\n}");
            qDebug()<<"pause";
            play[muz].pause();
        }

    }
}

void MainWindow::send_message()
{
    QByteArray process_f0b;
    QByteArray name = ui->send_mes->text().toUtf8();
    process_f0b.reserve(3+name.size());
    process_f0b.resize(3);
    quint8 *ptr = reinterpret_cast<quint8*> (process_f0b.data());
    *ptr = id; ++ptr;
    *ptr = 0x0b; ++ptr;
    *ptr = static_cast<const quint8>(name.size()); ++ptr;
    process_f0b.append(name);
    client->udpSocket.writeDatagram(process_f0b,address,port);
    ui->send_mes->setText("");
}

void MainWindow::active(){
    if(it_bax.size()==0){
        bax_time.stop();
    }
    for(int i=0;i<it_bax.size();i++){
        if(sost_bax[i]<max_boom.size()){
            if(!it_pix_bax[i]->isVisible()){
                it_pix_bax[i]->show();}
            it_pix_bax[i]->setPixmap(max_boom[sost_bax[i]]);
            sost_bax[i]++;
        }
        else{
            it_pix_bax[i]->hide();
            sost_bax.removeAt(i);
            it_pix_bax.removeAt(i);
            it_bax.removeAt(i);
        }
    }
}



void MainWindow::goto_xy(){

    if(play[muz].mediaStatus()==QMediaPlayer::EndOfMedia){
        if(muz==5){
            muz=0;
            play[muz].play();
        }else{
            muz++;
            play[muz].play();
        }
    }
    if((player.mediaStatus()==QMediaPlayer::EndOfMedia)&&(ui->widget->isHidden())){
        player.play();
    }
    if(ui->toolButton_2->isChecked()){

        // client->webSocket->sendBinaryMessage(process_f3());//------------------------------------------------------------------------------------------------------------;


    }else{
        List->hide();
    }

    if(ui->chat->isChecked()){
        ui->widget_5->show();
    }else{
        ui->widget_5->hide();
    }
}

void MainWindow::close_do()
{
    bax_time.stop();
    it_bax.clear();
    it_pix_bax.clear();
    sost_bax.clear();

    timer_bax.stop();
    bombs.clear();
    bufer_bee.clear();
    idx=0;
    score=0;

    bee_hide=0;
    bee_show=0;

    on_cl_time=true;
    idx_bufer = 0;


    if(read_bee_right != nullptr){
        delete read_bee_right;
    }
    read_bee_right=nullptr;
    bee_right.clear();
    if(read_bee_left != nullptr){
        delete read_bee_left;
    }
    read_bee_left=nullptr;
    bee_left.clear();
    time_bee.stop();
    time_go_bee.stop();
    dir_bee.clear();

    hidden_bee.clear();
    show_bee.clear();
    flag_bee.clear();

    List->clear();


    create_server = false;
    create_client = false;

    if(server!=nullptr){
        delete server;
    }
    server=nullptr;
    if(client != nullptr){
        delete client;
    }
    client=nullptr;

    size_boom=0;
    status_boom=0;

    status_bax=0;


    if(read != nullptr){
        delete read;
        read = nullptr;
    }
    ui->server_menu->hide();
    boom.clear();
    min_boom.clear();
    max_boom.clear();
    item_bees.clear();
    item_pix.clear();
    scene->clear();
    for(int i=0;i<7;i++){
        patron[i]->show();
    }
    ui->score->setText(QString::number(0,10));
    ui->message_text->setText("");
    ui->send_mes->setText("");
    ui->textBrowser->clear();
    ui->widget->show();
    ui->chat->setChecked(false);
    play[muz].stop();
    timer.stop();
    player.stop();

    muz=0;
    number_of_ammo=7;

    ui->widget_2->hide();
    //    ui->graphicsView->setCursor(Qt::CrossCursor);
    //    ui->graphicsView->setCursor(QPixmap(QString("://курсор_1.png")));
    ui->graphicsView->setCursor(QPixmap(QString("://прицел_1.png")));
    QIcon icon(":/клякса1.png");
    this->setWindowIcon(icon);
    ui->toolButton_2->setChecked(false);
    if(List!=nullptr) List->hide();
    ui->widget_5->hide();
    ui->play->setStyleSheet("#play\n{\nbackground-color:transparent;\nborder-image:url(:/пауза.png);\nbackground: none;\nborder:none;\nbackground-repeat:none;\n}\n#play:pressed\n{\nborder-image:url(:/пауза_н.png);\nbackground-repeat:none;\n}");
    ui->volue->setStyleSheet("#volue\n{\nbackground-color:transparent;"
                             "\nborder-image:url(:/вм.png);\nbackground: "
                             "none;\nborder:none;\nbackground-repeat:none;\n}\n"
                             "#volue:pressed\n{\nborder-image:url(:/вм_н.png);"
                             "\nbackground-repeat:none;\n}");
    f=false;

}

void MainWindow::_close()
{
    QByteArray message_f13;
    message_f13.resize(2);
    quint8 *ptr = reinterpret_cast<quint8*> (message_f13.data());
    *ptr = id; ++ptr;
    *ptr = 0x0d; ++ptr;
    client->udpSocket.writeDatagram(message_f13, address, port);
    client->udpSocket.close();
    close_do();
}

void MainWindow::show_win()
{

    if(ui->radioButton->isChecked()){

        QIcon icon("://icon.png");
        this->setWindowIcon(icon);
        file[0]=":/вверх.png";
        file[1]=":/низ.png";
        file[6]=":/право.png";
        file[3]=":/лево.png";
        file[4]=":/лево-низ.png";
        file[5]=":/право-низ.png";
        file[2]=":/лево-вверх.png";
        file[7]=":/право-вверх.png";
        file[8]=":/клякса.png";
        file[9]=":/клякса1.png";
        file[10]="://icon.png";
        file[11]="://boom15.gif";
        player.setMedia(QUrl::fromLocalFile(QApplication::applicationDirPath()+"/g.mp3"));
        player.setVolume(50);//40


        effect[0].setSource(QUrl::fromLocalFile(sndPath+"/shot_miss0.wav"));
        effect[1].setSource(QUrl::fromLocalFile(sndPath+"/shot_down.wav"));
        effect[2].setSource(QUrl::fromLocalFile(sndPath+"/great_shot.wav"));
        effect[3].setSource(QUrl::fromLocalFile(sndPath+"/mega_shot.wav"));
        effect[4].setSource(QUrl::fromLocalFile(sndPath+"/reload.wav"));
        effect[5].setSource(QUrl::fromLocalFile(sndPath+"vistrel.wav"));
        effect[6].setSource(QUrl::fromLocalFile(sndPath+"smert_pcheli.wav"));
        effect[7].setSource(QUrl::fromLocalFile(sndPath+"big_smert.wav"));

        for(int i=0;i<8;i++) effect[i].setVolume(100);


        for(int i=0;i<6;i++){
            play[i] .setMedia(QUrl::fromLocalFile(QApplication::applicationDirPath()+QString("/fon/%1.mp3").arg(i+1)));
            play[i].setVolume(100);}
    }else{

        QIcon icon("://img/icon.png");
        this->setWindowIcon(icon);
        file[0]=":/img/0.png";
        file[1]=":/img/1.png";
        file[6]=":/img/6.png";
        file[3]=":/img/3.png";
        file[4]=":/img/4.png";
        file[5]=":/img/5.png";
        file[2]=":/img/2.png";
        file[7]=":/img/7.png";
        file[8]=":/img/10.png";
        file[9]=":/img/10.png";
        file[10]="://img/icon.png";
        file[11]="://boom25.gif";
        player.setMedia(QUrl::fromLocalFile("://r.mp3"));
        player.setVolume(50);//40

        effect[0].setSource(QUrl::fromLocalFile(sndPath+"промах.wav"));
        effect[1].setSource(QUrl::fromLocalFile(sndPath+"5.wav"));
        effect[2].setSource(QUrl::fromLocalFile(sndPath+"10.wav"));
        effect[3].setSource(QUrl::fromLocalFile(sndPath+"20.wav"));
        effect[4].setSource(QUrl::fromLocalFile(sndPath+"reload.wav"));
        effect[5].setSource(QUrl::fromLocalFile(sndPath+"vistrel.wav"));
        effect[6].setSource(QUrl::fromLocalFile(sndPath+"smert_pcheli.wav"));
        effect[7].setSource(QUrl::fromLocalFile(sndPath+"big_smert.wav"));

        for(int i=0;i<8;i++) effect[i].setVolume(100);

        for(int i=0;i<6;i++){
            play[i] .setMedia(QUrl::fromLocalFile(QApplication::applicationDirPath()+QString("/fon1/%1.mp3").arg(i+1)));
            play[i].setVolume(100);}

    }

    for(int i=0;i<12;i++){

        if((i==9)&&(!ui->radioButton->isChecked())){

            picture[i]= new QPixmap(QPixmap(file[i]).scaled(be_w/2,be_h/2));
            break;
        }
        picture[i]= new QPixmap(QPixmap(file[i]).scaled(be_w,be_h));

    }

}

