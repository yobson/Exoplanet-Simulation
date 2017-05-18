#ifndef EXPERIMENTCONFIG_H
#define EXPERIMENTCONFIG_H

#include <QDialog>
#include <QtSerialPort>

namespace Ui {
class experimentConfig;
}

class experimentConfig : public QDialog
{
    Q_OBJECT

public:
    explicit experimentConfig(QWidget *parent = 0);
    void setSerialPort(QSerialPort *SP);
    ~experimentConfig();

private:
    Ui::experimentConfig *ui;
    QSerialPort *serial;
};

#endif // EXPERIMENTCONFIG_H
