#ifndef MUSCLE_DATA_H
#define MUSCLE_DATA_H

#include <QWidget>
#include <QMessageBox>
#include <QTcpSocket>
#include <QTcpServer>
#include <cstdio>
#include <cstring>

namespace Ui {
class muscle_data;
}

class muscle_data : public QWidget
{
    Q_OBJECT

public:
    explicit muscle_data(QWidget *parent = 0);
    ~muscle_data();
    void setSocket(QTcpSocket *s){socket = s;}

private slots:
    void Recvdata();
    void on_start_Button_clicked();

private:
    Ui::muscle_data *ui;
    QTcpSocket *socket;

//    typedef struct muscle_emg_data
//    {
//        char szIpAddress[SZ_INPUT];
//        char szCollectDuration[SZ_INPUT];
//        char szStartTrigger;
//        char szStopTrigger;
//    }emg_t;

};

#endif // MUSCLE_DATA_H
