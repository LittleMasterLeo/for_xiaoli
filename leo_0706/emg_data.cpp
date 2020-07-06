#include "emg_data.h"
#include "ui_emg_data.h"

emg_data::emg_data(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::emg_data)
{
    ui->setupUi(this);
}

emg_data::~emg_data()
{
    delete ui;
}

void emg_data::on_start_Button_clicked()
{
    Recvdata();
}

void emg_data::Recvdata()
{
    char recvMsg[1728] = {0};
    float emg_data[432] = {0};
    int i;
    int recvRe = socket->read(recvMsg,sizeof(recvMsg));
    if(recvRe == -1)
            {
                QMessageBox::information(this, "QT网络通信", "接收服务端数据失败！");
                return;
            }
    memcpy(emg_data,recvMsg,sizeof(emg_data));
    for (i=0;i<27;i++){
            ui->lcdNumber->display(emg_data[i*16+0]);
            ui->lcdNumber_2->display(emg_data[i*16+1]);
            ui->lcdNumber_3->display(emg_data[i*16+2]);
            ui->lcdNumber_4->display(emg_data[i*16+3]);
            ui->lcdNumber_5->display(emg_data[i*16+4]);
            ui->lcdNumber_6->display(emg_data[i*16+5]);
            ui->lcdNumber_7->display(emg_data[i*16+6]);
            ui->lcdNumber_8->display(emg_data[i*16+7]);
            ui->lcdNumber_9->display(emg_data[i*16+8]);
            ui->lcdNumber_10->display(emg_data[i*16+9]);
            ui->lcdNumber_11->display(emg_data[i*16+10]);
            ui->lcdNumber_12->display(emg_data[i*16+11]);
            ui->lcdNumber_13->display(emg_data[i*16+12]);
            ui->lcdNumber_14->display(emg_data[i*16+13]);
            ui->lcdNumber_15->display(emg_data[i*16+14]);
            ui->lcdNumber_16->display(emg_data[i*16+15]);
    }
}
