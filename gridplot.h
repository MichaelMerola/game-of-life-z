#ifndef GRIDPLOT_H
#define GRIDPLOT_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "cell.h"
#include "bar.h"

using namespace std;

// HW4
// Names: Michael Merola
//        Gustav Solis
//

namespace Ui {
class Plot;
}

class GridPlot : public QMainWindow
{
    Q_OBJECT

public:

    explicit GridPlot(QWidget *parent = 0);
    ~GridPlot();

    void Turn();
    bool Play(bool playing);

    int get_pop() const { return pop_; }  // inline member function

    Cell* GetCell_At(int x, int y);
    bool isCellAlive_At(int x, int y);

private slots:

    void on_stepButton_clicked();

    void on_playButton_clicked();

    void on_pauseButton_clicked();

    void on_speedSlider_sliderMoved(int position);

public slots:


private:

    int GetNeighbors(Cell* c);

    vector<Cell*> grid_;
    vector<Bar*> bars_;

    Ui::Plot *ui;

    QGraphicsScene *MainScene_;
    QGraphicsScene *GraphScene_;

    int turn_ = 0;
    int pop_ = 100;
    int z_pop_ = 0;
    int total_ = 200;
    int speed_ = 1;

    bool paused_ = false;

    //graph stuff
    int y_offset_;
    double h_offset_;
    int maxBars_;

};

#endif // GRIDPLOT_H
