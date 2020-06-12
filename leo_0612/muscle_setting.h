#ifndef MUSCLE_SETTING_H
#define MUSCLE_SETTING_H

#include <QWidget>
#include <QMessageBox>
#include <QTcpSocket>
#include <QTcpServer>
#include <cstdio>
#include <cstring>

#define SZ_INPUT    (29)

namespace Ui {
class muscle_setting;
}

class muscle_setting : public QWidget
{
    Q_OBJECT

public:
    explicit muscle_setting(QWidget *parent = 0);
    void setSocket(QTcpSocket *s){socket = s;}
    ~muscle_setting();
    char starttrigger;
    char stoptrigger;

private slots:
    void on_pushButton_back_clicked();

    void on_pushButton_setting_clicked();

    void on_radioButton_start_on_clicked();

    void on_radioButton_start_off_clicked();

    void on_radioButton_stop_on_clicked();

    void on_radioButton_stop_off_clicked();

private:
    Ui::muscle_setting *ui;
    QTcpSocket *socket;


    typedef struct muscle_env
    {
        char szIpAddress[SZ_INPUT];
        char szCollectDuration[SZ_INPUT];
        char szStartTrigger;
        char szStopTrigger;
    }menv_t;

};

#endif // MUSCLE_SETTING_H
