#include "emg_data.h"
#include "ui_emg_data.h"

emg_data::emg_data(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::emg_data)
{
    ui->setupUi(this);
    m = 3.14;
    QTimer *timer = new QTimer(this);
    //connect(timer,SIGNAL(timeout()),this,SLOT(test()));
    timer->start(1000);
}

emg_data::~emg_data()
{
    delete ui;
}

void emg_data::on_start_Button_clicked()
{
//    float m = 3.67;
//      Recvdata();
}

void emg_data::test_new()
{
    ui->data_1->setNum(emg_flt[0]);
    ui->data_2->setNum(emg_flt[1]);
    ui->data_3->setNum(emg_flt[2]);
    ui->data_4->setNum(emg_flt[3]);
    ui->data_5->setNum(emg_flt[4]);
    ui->data_6->setNum(emg_flt[5]);
    ui->data_7->setNum(emg_flt[6]);
    ui->data_8->setNum(emg_flt[7]);
    ui->data_9->setNum(emg_flt[8]);
    ui->data_10->setNum(emg_flt[9]);
    ui->data_11->setNum(emg_flt[10]);
    ui->data_12->setNum(emg_flt[11]);
    ui->data_13->setNum(emg_flt[12]);
    ui->data_14->setNum(emg_flt[13]);
    ui->data_15->setNum(emg_flt[14]);
    ui->data_16->setNum(emg_flt[15]);
}



void emg_data::test()
{
//    ui->data_1->setNum(m);
//    m += 0.6;
    ui->data_1->setNum(emg_flt[0]);
    ui->data_2->setNum(emg_flt[1]);
    ui->data_3->setNum(emg_flt[2]);
    ui->data_4->setNum(emg_flt[3]);
    ui->data_5->setNum(emg_flt[4]);
    ui->data_6->setNum(emg_flt[5]);
    ui->data_7->setNum(emg_flt[6]);
    ui->data_8->setNum(emg_flt[7]);
    ui->data_9->setNum(emg_flt[8]);
    ui->data_10->setNum(emg_flt[9]);
    ui->data_11->setNum(emg_flt[10]);
    ui->data_12->setNum(emg_flt[11]);
    ui->data_13->setNum(emg_flt[12]);
    ui->data_14->setNum(emg_flt[13]);
    ui->data_15->setNum(emg_flt[14]);
    ui->data_16->setNum(emg_flt[15]);
}
