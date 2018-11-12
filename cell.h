#ifndef CELL_H
#define CELL_H

#include <QObject>
#include <QWidget>
#include <QColor>
#include <QGraphicsItem>

// HW4
// Names: Michael Merola
//        Gustav Solis
//

class Cell : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    bool change_;
    bool z_change_;
    bool zombie_;
    int turns_zombie;

    //constructor
    Cell(bool alive, const int x, const int y);

    int get_x() const { return x_; }  // inline member function
    int get_y() const { return y_; }  // inline member function

    int get_xtrue() const { return x_true_; }  // inline member function
    int get_ytrue() const { return y_true_; }  // inline member function

    bool get_alive() const { return alive_; }  // inline member function
    static int get_w() { return width_; }  // inline member function

    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    void Step();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    //
    bool alive_;
    QColor aliveColor_ = QColor(27, 156, 252);
    QColor deadColor_ = QColor("white");
    QColor zombieColor_ = QColor(106, 176, 76);

    //coordinates in polt window
    int x_true_;
    int y_true_;

    //coordinates in grid
    int x_;
    int y_;

    QColor color_;

    static const int width_ = 30;


signals:
    void CellClicked();

public slots:
    //

};

#endif // CELL_H
