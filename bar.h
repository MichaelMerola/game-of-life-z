#ifndef BAR_H
#define BAR_H

#include <QObject>
#include <QWidget>
#include <QColor>
#include <QGraphicsItem>

// HW4
// Names: Michael Merola
//        Gustav Solis
//

class Bar : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    Bar(const int x, const int y, int h);

    static int get_w() { return width_; }  // inline member function
    int get_x() { return x_; }  // inline member function

    void set_x(int offset) { x_ = x_ + offset; } //inline setter

    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

private:
    int x_;
    int y_;

    QColor color_;

    static const int width_ = 30;
    int height_;

};

#endif // BAR_H
