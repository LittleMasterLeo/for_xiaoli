#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QDataStream>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>
#include <QtSql/qsql.h>
#include <cstring>
#include <QFile>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <QMessageBox>

#include "muscle_setting.h"
#include "emg_data.h"
#include "robot_data.h"

//#define SZ_INPUT    (256)

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);

    typedef struct change_setting{
        int flag;
        int mode;
        int value;
        char cmd[30];
    }cs_t;

    typedef struct up_data{
        int emg_data_ready;
        int robot_data_ready;
        float emg_data[432];
        double force_data[3];
        double vector_data[3];
    }data_t;

    ~Widget();

private slots:
    void on_link_button_clicked();
    void ClientRecvData();
    void write_message_slot(QString str);
    void on_disconnect_button_clicked();

    void on_link_robot_button_clicked();

    void on_disconnect_robot_button_clicked();

    void on_link_muscle_button_clicked();

    void on_disconnect_muscle_button_clicked();

    void on_zero_radioButton_clicked();

    void on_EA_radioButton_clicked();

    void on_set_k_button_clicked();

    void send_message(cs_t *setting_change, int n);

    void on_emg_button_clicked();

    void on_robot_button_clicked();

private:
    Ui::Widget *ui;
    QTcpSocket *sockfd;
    QString ip;
    QString port;
    muscle_setting mus_set;
    emg_data emg_set;
    robot_data rbt_set;


};

#endif // WIDGET_H
