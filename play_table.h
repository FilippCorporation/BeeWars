#ifndef PLAY_TABLE_H
#define PLAY_TABLE_H

#include<QGraphicsScene>
#include<QGraphicsView>
#include<QGraphicsItem>
#include<QGraphicsPixmapItem>
#include<QMoveEvent>
#include<QGraphicsSceneMouseEvent>
#include<QLabel>
#include<QPoint>
#include<QDebug>
class Play_table :  public QGraphicsScene
{
    Q_OBJECT
public:

    Play_table(QObject *parent = Q_NULLPTR);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);

    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
signals:
    void shotPoint(QPoint shotPoint, bool sost);
    void movePoint(QPoint movePoint);
};

#endif // PLAY_TABLE_H
