#include "muscle_setting.h"
#include "ui_muscle_setting.h"


muscle_setting::muscle_setting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::muscle_setting)
{
    ui->setupUi(this);

}

muscle_setting::~muscle_setting()
{
    delete ui;
}


void muscle_setting::on_pushButton_back_clicked()
{
    muscle_setting::close();
}

void muscle_setting::on_pushButton_setting_clicked()
{
    menv_t data_setting;
    strcpy(data_setting.szIpAddress,ui->lineEdit_ip->text().toLatin1());
    strcpy(data_setting.szCollectDuration ,ui->lineEdit_time->text().toLatin1());
    data_setting.szStartTrigger = starttrigger;
    data_setting.szStopTrigger = stoptrigger;
    //qDebug()<<"ip"<<data_setting.szIpAddress<<"time"<<data_setting.szCollectDuration<<"starttrigger"<<data_setting.szStartTrigger<<"stoptrigger"<<data_setting.szStopTrigger<<endl;
    //qDebug()<<"size:"<<sizeof(data_setting);
    //int sendRe=socket->write("connect_muscle_equipment",128);
    int sendRe = socket->write((char *)&data_setting,sizeof(data_setting));
    if(sendRe == -1)
    {
         QMessageBox::information(this, "QT网络通信", "向服务端发送数据失败！");
         return;
    }else{

        //qDebug()<<data.data()<<endl;
        qDebug()<<sizeof(data_setting);
        QMessageBox::information(this, "QT网络通信", "向服务端发送数据成功！");
        return;
    }
}

void muscle_setting::on_radioButton_start_on_clicked()
{
    starttrigger = 'y';
}

void muscle_setting::on_radioButton_start_off_clicked()
{
    starttrigger = 'n';
}

void muscle_setting::on_radioButton_stop_on_clicked()
{
    stoptrigger = 'y';
}

void muscle_setting::on_radioButton_stop_off_clicked()
{
    stoptrigger = 'n';
}
