#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::ClientRecvData()
{
    char recvMsg[1024] = {0};
    //QByteArray buffer;
    /*
    char type;
    int recvRe = sockfd->read((char *)&type, 1); // 接收信息类型
    if(recvRe == -1)
            {
                QMessageBox::information(this, "QT网络通信", "接收服务端数据失败！");
                return;
            }
    */
    int recvRe = sockfd->read(recvMsg,sizeof(recvMsg));
    if(recvRe == -1)
            {
                QMessageBox::information(this, "QT网络通信", "接收服务端数据失败！");
                return;
            }
    QString showQstr = recvMsg;
    ui->recvTextEdit->append(showQstr);
    //ui->recvTextEdit->append("\n");
    //ui->recvTextEdit->setText("\n");
}

void Widget::write_message_slot(QString str)
{
    QByteArray data;//实例化字节数组
    char *ch;
    data=str.toLatin1();
    ch=data.data();
    //sockfd->write(ch,strlen(ch));//发送数据给对方
    int sendRe = sockfd->write(ch,128);//发送数据给对方
    if(sendRe == -1)
    {
         QMessageBox::information(this, "QT网络通信", "向服务端发送数据失败！");
         return;
    }
}

void Widget::on_link_button_clicked()
{
    ip = ui->line_ip->text();
    port = ui->line_port->text();
    sockfd = new QTcpSocket;
    sockfd->connectToHost(ip,port.toInt());
    connect(sockfd,&QTcpSocket::readyRead,this,&Widget::ClientRecvData);
}

void Widget::on_disconnect_button_clicked()
{
    sockfd->close();
}

void Widget::on_link_robot_button_clicked()
{
    write_message_slot("connect_robot");
}

void Widget::on_disconnect_robot_button_clicked()
{
    write_message_slot("disconnect_robot");
}

void Widget::on_link_muscle_button_clicked()
{
    write_message_slot("connect_muscle_equipment");
    mus_set.setSocket(sockfd);
    mus_set.show();
}

void Widget::on_disconnect_muscle_button_clicked()
{
    write_message_slot("disconnect_muscle_equipment");
}

void Widget::on_zero_radioButton_clicked()
{
    write_message_slot("zero");
}

void Widget::on_EA_radioButton_clicked()
{
    write_message_slot("EA");
}

void Widget::on_set_k_button_clicked()
{
    cs_t setting_change;
    setting_change.flag = 1;
    setting_change.mode = 3;
    setting_change.value = ui->line_port_k->text().toInt();
    write_message_slot("change_set_k");
    int sendRe = sockfd->write((char *)&setting_change,sizeof(setting_change));
    if(sendRe == -1)
    {
         QMessageBox::information(this, "QT网络通信", "向服务端发送数据失败！");
         return;
    }else{
        qDebug()<<sizeof(setting_change);
        QMessageBox::information(this, "QT网络通信", "向服务端发送数据成功！");
        return;
    }
    return;
}
