#ifndef EMG_DATA_H
#define EMG_DATA_H

#include <QWidget>
#include <QLCDNumber>
#include <QMessageBox>
#include <QTcpSocket>
#include <QTcpServer>
#include <cstdio>
#include <cstring>
#include <QTimer>



namespace Ui {
class emg_data;
}

class emg_data : public QWidget
{
    Q_OBJECT

public:
    explicit emg_data(QWidget *parent = 0);
    ~emg_data();
    void setSocket(QTcpSocket *s){socket = s;}
    void test_new();
    float emg_flt[432];
    float m;

private slots:
    void on_start_Button_clicked();
    void test();
    void Recvdata();

private:
    Ui::emg_data *ui;
    QTcpSocket *socket;
};

#endif // EMG_DATA_H
