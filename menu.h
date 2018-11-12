#ifndef MENU_H
#define MENU_H

#include <QMainWindow>
#include "gridplot.h"

namespace Ui {
class Menu;
}

class Menu : public QMainWindow
{
    Q_OBJECT

public:
    explicit Menu(QWidget *parent = nullptr);
    ~Menu();

private slots:
    void on_playButton_clicked();

    void on_rulesButton_clicked();

    void on_quitButton_clicked();

private:
    Ui::Menu *ui;

    GridPlot w_;

    bool rulesEnabled;
};

#endif // MENU_H
