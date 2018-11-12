#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <QtWidgets>

#include "cell.h"
#include "gridplot.h"

// HW4
// Names: Michael Merola
//        Gustav Solis
//

//Cell constructor
Cell::Cell(bool alive, const int x, const int y) {

    zombie_ = false;
    change_ = false;
    turns_zombie = 0;
    alive_ = alive;
    x_true_ = x;
    y_true_ = y;

    x_ = x_true_/width_;
    y_ = y_true_/width_;

    if (alive_) {
        this->color_ = aliveColor_;
    } else {
        this->color_ = deadColor_;
    }
}

//Building the cell
QRectF Cell::boundingRect() const {
    return QRectF(x_true_, y_true_, width_, width_);
}

//Building the cell
QPainterPath Cell::shape() const {
    QPainterPath path;
    path.addRect(x_true_, y_true_, width_, width_);
    return path;
}

//Paints the cell using the native QBrush
void Cell::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    Q_UNUSED(widget);

    QBrush b = painter->brush();
    painter->setBrush(QBrush(color_.dark(option->state & QStyle::State_Sunken ? 100 : 100)));

    painter->drawRect(this->x_true_, this->y_true_, this->width_, this->width_);
    painter->setBrush(b);
}

//Updates the cell color and appropriate variables based on the Turn() function in GridPlot
void Cell::Step() {
    srand(time(NULL)*x_*y_);
    double survive_chance = rand() % 100;

    if (z_change_) {
        color_ = zombieColor_;
        if(zombie_){
            if(turns_zombie >= 5){
                qDebug() << "zombie turned alive on turn " << turns_zombie << " at " <<  x_ << "," << y_;
                change_ = true;
                alive_ = false;
                turns_zombie = 0;
                color_ = aliveColor_;
            } else if (survive_chance < 10){
                qDebug() << "zombie died on turn " << turns_zombie << " at " <<  x_ << "," << y_;
                change_ = true;
                alive_ = true;
                turns_zombie = 0;
                color_ = deadColor_;
            }
        }
        update();
    }

    if (change_) {
        zombie_ = false;
        alive_ = !alive_;

        if (alive_) {
            color_ = aliveColor_;
        } else {
            color_ = deadColor_;
        }
        update();
    }

    z_change_ = false;
    change_ = false; //reset for next step
}

//On mouse press of a cell, kills alive cells and bring dead cells back to life
void Cell::mousePressEvent(QGraphicsSceneMouseEvent *event) {


    if(event->button() == Qt::RightButton) {//right-click
        //kill cell
        alive_ = false;
        color_ = deadColor_;
        qDebug() << "right-click";

    } else { //left-click
        //become alive
        alive_ = true;
        color_ = aliveColor_;
        qDebug() << this->x_ << "," << this->y_;
    }


    update();
}
