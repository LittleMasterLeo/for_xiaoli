#ifndef EMG_DATA_H
#define EMG_DATA_H

#include <QWidget>
#include <QLCDNumber>
#include <QMessageBox>
#include <QTcpSocket>
#include <QTcpServer>
#include <cstdio>
#include <cstring>

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
    void Recvdata();

private slots:
    void on_start_Button_clicked();

private:
    Ui::emg_data *ui;
    QTcpSocket *socket;
};

#endif // EMG_DATA_H
