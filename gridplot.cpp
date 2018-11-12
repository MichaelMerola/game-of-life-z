#include <iostream>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <chrono>
#include <thread>
#include <random>
#include <ctime>

#include "gridplot.h"
#include "cell.h"
#include "bar.h"
#include "ui_plot.h"

#include <random>

using namespace std;

// HW4
// Names: Michael Merola
//        Gustav Solis
//


//GridPlot constructor
GridPlot::GridPlot(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Plot)
{
    //setup
    ui->setupUi(this);

    // set up Main Plot
    MainScene_ = new QGraphicsScene;
    QGraphicsView * viewMain = ui->MainPlot;
    viewMain->setScene(MainScene_);
    viewMain->setSceneRect(0,0,viewMain->frameSize().width(),viewMain->frameSize().height());

    // set up Graph Plot
    GraphScene_ = new QGraphicsScene;
    QGraphicsView * viewGraph = ui->GraphPlot;
    viewGraph->setScene(GraphScene_);
    viewGraph->setSceneRect(0,0,viewGraph->frameSize().width(),viewGraph->frameSize().height());


    //BUILD GRID
    int cells = Cell::get_w();
    Cell gridStart(false, 1*cells, 1*cells);
    Cell* item;

    string sPop = "Population: 100 (50%)" ;
    ui->popLabel->setText(QString::fromStdString(sPop));

    string zPop = "Zombies: 0" ;
    ui->zombieLabel->setText(QString::fromStdString(zPop));

    string speed = "Speed: " + to_string(speed_) ;
    ui->speedLabel->setText(QString::fromStdString(speed));

    //srand( time( NULL ) );

    for (int x = gridStart.get_xtrue(); x <= 20*cells; x = x + cells) {

        for (int y = gridStart.get_ytrue(); y <= 10*cells; y = y + cells) {

            double r = (rand() % 100);

            if (r < 50 ) {
                item = new Cell(true, x, y);
            } else {
                item = new Cell(false, x, y);
            }

            MainScene_->addItem(item);
            grid_.push_back(item);
        }
    }


    //BUILD GRAPH
    y_offset_ = viewGraph->frameSize().height() - 2 ;
    h_offset_ = viewGraph->frameSize().height() ;
    maxBars_ =  viewGraph->frameSize().width() / Bar::get_w();

    Bar* initBar = new Bar(0, y_offset_, int(0.5 * h_offset_));
    bars_.push_back(initBar);
    GraphScene_->addItem(initBar);

}//end grid constructor

GridPlot::~GridPlot() {
    delete ui;
} //end destructor



//The function for each turn when a user clicks step or loops through play
//Goes through each cell in the grid and gets its neighbors via GetNeighbors()
//If alive, determines if it stays alive or dies based on the number of neighbors that are alive
//If dead, cells run the chance of becoming zombies, or otherwise coming back to life based on neighbors that are alive
void GridPlot::Turn() {
    int neighbors;
    double time_multiplier = 0;
    // evaluate changes
    for (Cell* c : grid_) {
        time_multiplier++;
        srand(time(NULL)*time_multiplier);
        double z_chance = rand() % 100;

        // evaluates # of alive neighbors
        neighbors = GetNeighbors(c);

        if(z_chance <= 1 and !c->get_alive() and !c->zombie_){ //1 percent chance for the dead to turn into a zombie
            c->zombie_ = true;
        }

        if (c->get_alive()) {
            if (neighbors < 2) {
                //dies
                c->change_ = true;
            } else if (neighbors == 2  || neighbors == 3) {
                //stays alive
            } else if (neighbors > 3) {
                //dies
                c->change_ = true;
            }
        } else {
            if(c->zombie_){
                c->turns_zombie++;
                c->z_change_ = true;
            } else if (neighbors == 3) {
                //becomes alive
                c->change_ = true;
            }
        }
    }

    int popCount = 0;
    int zCount = 0;

    //update
    for (Cell* c : grid_) {
        c->Step();

        if (c->get_alive() + c->zombie_) {
            popCount++;
        }

        if (c->zombie_) {
            zCount++;
        }
    }

    z_pop_ = zCount;
    pop_ = popCount;
    turn_++;

    string sTurn = "Turn: " + to_string(turn_) ; //turn label
    ui->turnLabel->setText(QString::fromStdString(sTurn));

    double popPercent = (double(pop_) / double(total_));

    string sPop = "Population: " + to_string(pop_) + " (" + to_string( int( popPercent*100 ) ) + "%)" ;
    ui->popLabel->setText(QString::fromStdString(sPop)); //population label

    string zPop = "Zombies: " + to_string(z_pop_) ;
    ui->zombieLabel->setText(QString::fromStdString(zPop)); //population label

    //Graph

    if (bars_.size() > maxBars_) {

        int prev = 0;

        for (Bar* b : bars_) {
            b->set_x(-1 * b->get_w()); // set each bar back -1 bar
            prev = b->get_x();
        }
        Bar* bar = new Bar(prev+Bar::get_w() , y_offset_, int(popPercent * h_offset_));
        bars_.push_back(bar);
        GraphScene_->addItem(bar);

    } else {
        Bar* bar = new Bar(turn_*Bar::get_w(), y_offset_, int(popPercent * h_offset_));
        bars_.push_back(bar);
        GraphScene_->addItem(bar);
    }

    GraphScene_->update();
}

//Called by the on click play event
//Takes a bool based on whether play or pause was clicked
bool GridPlot::Play(bool playing) {
    if(playing){
        Turn();
        if(pop_ > 0){
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}


//gets the cell at an x and y coordinate
Cell* GridPlot::GetCell_At(int x, int y) {

    for (Cell* c : grid_) {
        if (c->get_x() == x && c->get_y() == y) {
            return c;
        }
    }

    Cell* error = new Cell(false, -1, -1);
    return error;
}

//returns a bool based on whether a cell is alive at a certain x and y
bool GridPlot::isCellAlive_At(int x, int y) {

    for (Cell* c : grid_) {
        if (c->get_x() == x && c->get_y() == y) {
            return c->get_alive();
        }
    }

    bool error = false;
    return error;
}

//step button event
void GridPlot::on_stepButton_clicked()
{
    Turn();
    QCoreApplication::processEvents();
}

//play button event
//runs the loop of the game using a sleep function to control the seconds
void GridPlot::on_playButton_clicked()
{
    bool x = true;
    while(x && !paused_){
        x = Play(true);
        QCoreApplication::processEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000/speed_));
    }
    paused_ = !paused_;
}


//pause buttone event
//changes the private variable paused_
void GridPlot::on_pauseButton_clicked()
{
    paused_ = !paused_;
}







//GETS NEIGHBORS //----------------------------------------------------------

int GridPlot::GetNeighbors(Cell* c) {
    int n_alive = 0;
    vector< tuple<int, int> > loc_neighbors;
    int x = c->get_x();
    int y = c->get_y();
    if(x > 1 && y > 1 && x < 20 && y < 10){ //not on a corner or side
        loc_neighbors.push_back({x+1, y+1}); //bottom-right
        loc_neighbors.push_back({x, y+1}); //bottom
        loc_neighbors.push_back({x-1, y+1}); //bottom-left
        loc_neighbors.push_back({x+1, y-1}); //top-right
        loc_neighbors.push_back({x, y-1}); //top
        loc_neighbors.push_back({x-1, y-1}); //top-left
        loc_neighbors.push_back({x-1, y}); //left
        loc_neighbors.push_back({x+1, y}); //right
    } else if(x == 1 && y == 1){ //top left corner
        loc_neighbors.push_back({x+1, y+1}); //bottom-right
        loc_neighbors.push_back({x, y+1}); //bottom
        loc_neighbors.push_back({x*20, y+1}); //bottom-left
        loc_neighbors.push_back({x+1, y*10}); //top-right
        loc_neighbors.push_back({x, y*10}); //top
        loc_neighbors.push_back({x*20, y*10}); //top-left
        loc_neighbors.push_back({x*20, y}); //left
        loc_neighbors.push_back({x+1, y}); //right
    } else if(x == 20 && y == 1){ //top right corner
        loc_neighbors.push_back({x/20, y+1}); //bottom-right
        loc_neighbors.push_back({x, y+1}); //bottom
        loc_neighbors.push_back({x-1, y+1}); //bottom-left
        loc_neighbors.push_back({x/20, y*10}); //top-right
        loc_neighbors.push_back({x, y*10}); //top
        loc_neighbors.push_back({x-1, y*10}); //top-left
        loc_neighbors.push_back({x-1, y}); //left
        loc_neighbors.push_back({x/20, y}); //right
    } else if(x == 1 && y == 10){ //bottom left corner
        loc_neighbors.push_back({x+1, y/10}); //bottom-right
        loc_neighbors.push_back({x, y/10}); //bottom
        loc_neighbors.push_back({x*20, y/10}); //bottom-left
        loc_neighbors.push_back({x+1, y-1}); //top-right
        loc_neighbors.push_back({x, y-1}); //top
        loc_neighbors.push_back({x*20, y-1}); //top-left
        loc_neighbors.push_back({x*20, y}); //left
        loc_neighbors.push_back({x+1, y}); //right
    } else if(x == 20 && y == 10){ //bottom right corner
        loc_neighbors.push_back({x/20, y/10}); //bottom-right
        loc_neighbors.push_back({x, y/10}); //bottom
        loc_neighbors.push_back({x-1, y/10}); //bottom-left
        loc_neighbors.push_back({x/20, y-1}); //top-right
        loc_neighbors.push_back({x, y-1}); //top
        loc_neighbors.push_back({x-1, y-1}); //top-left
        loc_neighbors.push_back({x-1, y}); //left
        loc_neighbors.push_back({x/20, y}); //right
    } else if(x == 1){ //block is on the left side
        loc_neighbors.push_back({x+1, y+1}); //bottom-right
        loc_neighbors.push_back({x, y+1}); //bottom
        loc_neighbors.push_back({x*20, y+1}); //bottom-left *
        loc_neighbors.push_back({x+1, y-1}); //top-right
        loc_neighbors.push_back({x, y-1}); //top
        loc_neighbors.push_back({x*20, y-1}); //top-left *
        loc_neighbors.push_back({x*20, y}); //left *
        loc_neighbors.push_back({x+1, y}); //right
    } else if(x == 20){ //block is on the right side
        loc_neighbors.push_back({x/20, y+1}); //bottom-right *
        loc_neighbors.push_back({x, y+1}); //bottom
        loc_neighbors.push_back({x-1, y+1}); //bottom-left
        loc_neighbors.push_back({x/20, y-1}); //top-right *
        loc_neighbors.push_back({x, y-1}); //top
        loc_neighbors.push_back({x-1, y-1}); //top-left
        loc_neighbors.push_back({x-1, y}); //left
        loc_neighbors.push_back({x/20, y}); //right *
    } else if(y == 1){ //block is on the top side
        loc_neighbors.push_back({x+1, y+1}); //bottom-right
        loc_neighbors.push_back({x, y+1}); //bottom
        loc_neighbors.push_back({x-1, y+1}); //bottom-left
        loc_neighbors.push_back({x+1, y*10}); //top-right *
        loc_neighbors.push_back({x, y*10}); //top *
        loc_neighbors.push_back({x-1, y*10}); //top-left *
        loc_neighbors.push_back({x-1, y}); //left
        loc_neighbors.push_back({x+1, y}); //right
    } else if(y == 10){ //block is on the bottom side
        loc_neighbors.push_back({x+1, y/10}); //bottom-right *
        loc_neighbors.push_back({x, y/10}); //bottom *
        loc_neighbors.push_back({x-1, y/10}); //bottom-left *
        loc_neighbors.push_back({x+1, y-1}); //top-right
        loc_neighbors.push_back({x, y-1}); //top
        loc_neighbors.push_back({x-1, y-1}); //top-left
        loc_neighbors.push_back({x-1, y}); //left
        loc_neighbors.push_back({x+1, y}); //right
    }

    for(int i = 0; i < 8; i++){
        tuple<int,int> cords = loc_neighbors.at(i);
        int x_n = get<0>(cords);
        int x_y = get<1>(cords);
        if(isCellAlive_At(x_n, x_y)){
            n_alive++;
        }
    }

    loc_neighbors.clear();
    return n_alive;
}

//slider to update the private variable of speed_
void GridPlot::on_speedSlider_sliderMoved(int position)
{
    speed_ = position;

    string speed = "Speed: " + to_string(speed_) ;
    ui->speedLabel->setText(QString::fromStdString(speed));
}
