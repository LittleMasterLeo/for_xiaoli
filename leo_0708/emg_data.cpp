#include "emg_data.h"
#include "ui_emg_data.h"

emg_data::emg_data(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::emg_data)
{
    ui->setupUi(this);
    m = 3.14;
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(test()));
    timer->start(1000);
}

emg_data::~emg_data()
{
    delete ui;
}

void emg_data::on_start_Button_clicked()
{
//    float m = 3.67;

//    ui->data_2->setNum(0.33452);
//      Recvdata();

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

void emg_data::Recvdata()
{
/*
        char recvMsg[1728] = {0};

        int recvRe = socket->read(recvMsg,sizeof(recvMsg));
        if(recvRe == -1)
            {
                QMessageBox::information(this, "QT网络通信", "接收服务端数据失败！");
                return;
            }
        memcpy(emg_flt,recvMsg,sizeof(emg_flt));
        test();
*/
/*
    int i;
    for (i=0;i<27;i++){
            ui->data_1->setNum(emg_data[i*16+0]);
            ui->data_2->setNum(emg_data[i*16+1]);
            ui->data_3->setNum(emg_data[i*16+2]);
            ui->data_4->setNum(emg_data[i*16+3]);
            ui->data_5->setNum(emg_data[i*16+4]);
            ui->data_6->setNum(emg_data[i*16+5]);
            ui->data_7->setNum(emg_data[i*16+6]);
            ui->data_8->setNum(emg_data[i*16+7]);
            ui->data_9->setNum(emg_data[i*16+8]);
            ui->data_10->setNum(emg_data[i*16+9]);
            ui->data_11->setNum(emg_data[i*16+10]);
            ui->data_12->setNum(emg_data[i*16+11]);
            ui->data_13->setNum(emg_data[i*16+12]);
            ui->data_14->setNum(emg_data[i*16+13]);
            ui->data_15->setNum(emg_data[i*16+14]);
            ui->data_16->setNum(emg_data[i*16+15]);
    }
*/
}
