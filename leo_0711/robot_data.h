#ifndef ROBOT_DATA_H
#define ROBOT_DATA_H

#include <QWidget>

namespace Ui {
class robot_data;
}

class robot_data : public QWidget
{
    Q_OBJECT

public:
    explicit robot_data(QWidget *parent = 0);
    ~robot_data();
    double force_data[3];
    double vector_data[3];
    void show_data();

private:
    Ui::robot_data *ui;
};

#endif // ROBOT_DATA_H
