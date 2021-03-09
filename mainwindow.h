#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QTextCursor>
#include<QTextBlock>
#include<QTextDocument>
#include<QImage>
#include<QColor>
#include<QTextCharFormat>
#include<QIcon>
#include <QTimer>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QPoint>
#include <QPixmap>
#include <QString>
#include <QLabel>
#include<random>
#include <QList>
#include<QMediaPlayer>
#include<QMediaPlaylist>
#include<QMediaContent>
#include <QMessageBox>
#include"bugclient.h"
#include"bugserver.h"
#include<QTreeWidgetItem>
#include<QSoundEffect>
#include <QUrl>
#include <QApplication>
#include<QBrush>
#include<QPixmap>
#include<QCursor>
#include<QSound>
#include<QIcon>
#include<QMovie>
#include<QtMultimedia/QSoundEffect>
#include<QGraphicsView>
#include<QGraphicsScene>
#include<QGraphicsItem>
#include<QGraphicsPixmapItem>
#include<QImageReader>
#include <QPointer>
#include "play_table.h"
#include<QNetworkInterface>
#include <QWebSocket>
#include<QTcpSocket>
#include"../Chat/chat.h"
#include <QApplication>
#include <QSettings>
#include <QGuiApplication>
#include <QScreen>
#include <QRect>
#include <QKeyEvent>
#include <QMutex>
#include"buferbee.h"
#include <QGraphicsSimpleTextItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QByteArray process_f1();
    void process_f2(const QByteArray &message);
    QByteArray process_f3();
    void process_f4(const QByteArray &message);
    void process_f6(const QByteArray &message);
    void process_f8(const QByteArray &message);
    void process_f0a(const QByteArray &message);
    void process_f0c(const QByteArray &message);
    void process_f10(const QByteArray &message);
    void process_f12(const QByteArray &message);




    Ui::MainWindow *ui;

private:
    bool feature = true;
    //bool Check_IP();
    //QList<std::pair<QString,QImage>>smiles;
    QList<BuferBee>bufer_bee;

//void keyPressEvent(QKeyEvent *event);
    //Chat *chat;

    QImageReader *read_bee_right;
    QVector<QPixmap> bee_right;

    QImageReader *read_bee_left=nullptr;
    QVector<QPixmap> bee_left;
    QTimer time_bee;
    QTimer time_go_bee;
    QList<int>dir_bee;

    QList<int>hidden_bee;
    QList<int>show_bee;
    QList<int>flag_bee;
    ///////////
    QTimer timer_proc_10;
    //////////

    QImageReader *read;
    QImageReader *read_bomb;
    QVector<QPixmap> boom;
    QVector<QPixmap> min_boom;
    QVector<QPixmap> max_boom;
    QTreeWidget *List;

    const QString sndPath = QApplication::applicationDirPath()+QDir::separator()+QStringLiteral("sounds")+QDir::separator();


    bool create_server = false;
    bool create_client = false;

    Play_table *scene;
    BugServer *server;
//    BugServerThread *server;
    QPointer<BugClient> client;
    int size_boom=0;
    int status_boom=0;

    int status_bax=0;
    int kol_be, hei, wid, be_h, be_w, fall, kol_pat, time_rep, cost_boom, r_boom, s_y, s_p, s_m ;
    quint8 id;
    QHostAddress address;
    quint16 port;


    int idx=0;
    int score=0;
    //////////////////////////////////////////////нужна норм перезарядка на сервере!!!
    int f10_time_show = 10; //время показа предупреждения, заменить в 10f (сек)
    bool on_reloading = false;
    int number_of_ammo = 7;
    int pic_rand_ammo = 0;
    /////////////////////////////////////////////

    int bee_hide=0;
    int bee_show=0;
    quint8 cl_time;
    bool on_cl_time=true;
    int idx_bufer = 0;

    int x,y;
    int muz=0;
    bool f=false;
    QSoundEffect effect[8];
    QLabel *patron[7];

    QGraphicsPixmapItem* it_pix;

    QList<QGraphicsItem*>item_bees;
    QList<QGraphicsPixmapItem*>item_pix;
    QList<int>status_bee;
    quint8 regular_time;

    QMediaPlayer *music = new QMediaPlayer();

    QList<Players>Gamer;

    QGraphicsItem* it_boom;
    QGraphicsPixmapItem* it_pix_boom;


    QList<QGraphicsItem*> it_bax;
    QList<QGraphicsPixmapItem*> it_pix_bax;
    QList<int> sost_bax;
    QList<Bombs>bombs;

    void babax(int x, int y);
    QTimer bax_time;

    QMediaPlayer play[7];
    QMediaPlayer player;
    QMediaPlayer player1;
    QString file[12];
    QTimer timer;
    ///
    QTimer timer_1;
    ///
    QTimer timer_pr;
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    QGraphicsSimpleTextItem *text_info;// = new QGraphicsSimpleTextItem();///Сделать добавление текста в сцену

    QPoint coord_of_sight;
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int sch_process_10 = (f10_time_show*1000)+1;////////////////////////////BOT ETO///////////////////////////////////////////////////////
    int bx=0,by=0;
    QTimer timer_bax;
    QPixmap *picture[12];
    QMutex argMutex;
    void do_pix();
//    void gameover();
    void close_do();
    void creating_client();
    void showMainMenu();
    void save_ap();
    void load_ap();

private slots:
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void for_10(QPoint p);
    void for_10_timer();
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void go_bee();
    void go_pix();
    void _close();
    void show_win();
    void setSignal(int p);
    void hide_win();
    void muz_volue();
    void muz_stop();
    void muz_left();
    void muz_right();
    void muz_play();
    void active();
    void goto_xy();
    void add_server();
    void add_client();
    ///
    void ammo_function();
    ///
    void show_score();
    void send_message();
    void set_pos(QPoint p, bool sost);
    void set_pix();
    void set_bax();
    void listen(bool f);
    void fun_helper();
    void close_serv();
    void Tree(QList<Players> Player);
    void Problem();
    void close_players(QTreeWidgetItem* item,int n);

signals:
    void close_num(int i);
};

#endif // MAINWINDOW_H
