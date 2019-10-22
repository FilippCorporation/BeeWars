#include "play_table.h"

Play_table::Play_table(QObject *parent):QGraphicsScene(parent)
{

}

void Play_table::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent){

    if(mouseEvent->buttons()==Qt::LeftButton){
        emit shotPoint (QPoint(mouseEvent->scenePos().x(), mouseEvent->scenePos().y()),0);
    }
    if(mouseEvent->buttons()==Qt::RightButton){
        emit shotPoint (QPoint(mouseEvent->scenePos().x(), mouseEvent->scenePos().y()),1);
    }
}

void Play_table::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent){
    emit movePoint(QPoint(mouseEvent->scenePos().x(), mouseEvent->scenePos().y()));
}
