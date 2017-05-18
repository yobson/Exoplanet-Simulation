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

private slots:
    void readData();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

    void on_SampleRate_editingFinished();

    void on_SampleNumber_editingFinished();

private:
    Ui::experimentConfig *ui;
    QSerialPort *serial;
    int syncStage;
};

#endif // EXPERIMENTCONFIG_H
