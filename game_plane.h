#ifndef GAME_PLANE_H
#define GAME_PLANE_H

#include <QObject>
#include <QVector>
#include"make_bee.h"
class Game_plane : public QObject
{
    Q_OBJECT
public:
    explicit Game_plane(QObject *parent = nullptr);
    Make_bee* bee;
    quint8 time;

};

#endif // GAME_PLANE_H
