#include "menu.h"
#include "gridplot.h"
#include "ui_menu.h"

Menu::Menu(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Menu)
{
    ui->setupUi(this);

    this->setFixedWidth(500);

    ui->rulesText->hide();
    rulesEnabled = false;
}

Menu::~Menu()
{
    delete ui;
}

void Menu::on_playButton_clicked()
{
    this->hide();

    w_.show();
}

void Menu::on_quitButton_clicked()
{
    QApplication::quit();
}

void Menu::on_rulesButton_clicked()
{
    if (!rulesEnabled) {
        ui->rulesText->show();
        rulesEnabled = true;
        this->setFixedWidth(900);
    }
    else {
        ui->rulesText->hide();
        rulesEnabled = false;
        this->setFixedWidth(500);
    }
}

