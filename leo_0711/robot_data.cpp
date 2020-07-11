#include "robot_data.h"
#include "ui_robot_data.h"

robot_data::robot_data(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::robot_data)
{
    ui->setupUi(this);
}

robot_data::~robot_data()
{
    delete ui;
}

void robot_data::show_data()
{
    ui->force_data_1->setNum(force_data[0]);
    ui->force_data_2->setNum(force_data[1]);
    ui->force_data_3->setNum(force_data[2]);
    ui->vector_data_1->setNum(vector_data[0]);
    ui->vector_data_2->setNum(vector_data[1]);
    ui->vector_data_3->setNum(vector_data[2]);
}
