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

    void defSN(int);
    void defSR(double);
    void defRV(double);

signals:
    void indef();
    void def();
    void setSN(int);
    void setSR(double);
    void setRV(double);

private slots:
    void readData();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

    void on_SampleRate_textChanged(const QString &arg1);

    void on_SampleNumber_textChanged(const QString &arg1);

    void on_voltage_textChanged(const QString &arg1);

private:
    Ui::experimentConfig *ui;
    QSerialPort *serial;
    int syncStage;
};

#endif // EXPERIMENTCONFIG_H
