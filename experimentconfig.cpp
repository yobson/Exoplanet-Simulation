#include "experimentconfig.h"
#include "ui_experimentconfig.h"
#include <QMessageBox>
#include <QtCore>

const int finalStage = 5;

experimentConfig::experimentConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::experimentConfig)
{
    ui->setupUi(this);
    syncStage = 0;
}

void experimentConfig::setSerialPort(QSerialPort *SP)
{
    serial = SP;
    if (SP == NULL) {
        QMessageBox err;
        err.setText("AHHHH set up serial First!!!");
        err.exec();
        this->close();
    } else {
        SP->clear();
        connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
    }

}

experimentConfig::~experimentConfig()
{
    disconnect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
    delete ui;
}

void experimentConfig::defSN(int SN)
{
    ui->SampleNumber->setText(QString("%0").arg(SN));
}

void experimentConfig::defSR(double sr)
{
    ui->SampleRate->setText(QString("%0").arg(sr));
}

void experimentConfig::defRV(double rv)
{
    ui->voltage->setText(QString("%0").arg(rv));
}

void experimentConfig::readData()
{
    QString data = QString::fromLatin1(serial->readAll());
    qDebug() << "Got reply: " << data;
    if (data == "SReady") {
        syncStage++;
        on_pushButton_3_clicked();
    }
}

void experimentConfig::on_pushButton_4_clicked()
{
    this->close();
    delete this;
}

void experimentConfig::on_pushButton_3_clicked()
{
    switch(syncStage) {
    case 0:
        qDebug() << "Putting arduino into settings mode";
        serial->write("Settings");
        break;
    case 1:
        qDebug() << "Syncing syncing sample rate";
        serial->write(ui->SampleRate->text().toLatin1());
        emit(setSR(ui->SampleRate->text().toDouble()));
        break;
    case 2:
        qDebug() << "Syncing number of samples";
        serial->write(ui->SampleNumber->text().toLatin1());
        emit (setSN(ui->SampleNumber->text().toInt()));
        if (ui->SampleNumber->text() == "0") {
            emit(indef());
            emit (setSN(1000000));
        } else emit (def());

        break;
    case 3:
        qDebug() << "Syncing reference voltage";
        serial->write(ui->voltage->text().toLatin1());
        emit(setRV(ui->voltage->text().toDouble()));
        break;
    case finalStage - 1:
        qDebug() << "Sync successful!";
        ui->pushButton_3->setText("Done");
        syncStage = finalStage;
        break;
    case 5:
        this->close();
        delete this;
    }
}

void experimentConfig::on_SampleRate_textChanged(const QString &arg1)
{
    if (syncStage == finalStage) {
        ui->pushButton_3->setText("Sync Changes");
        syncStage = 0;
    }
}

void experimentConfig::on_SampleNumber_textChanged(const QString &arg1)
{
    if (syncStage == finalStage) {
        ui->pushButton_3->setText("Sync Changes");
        syncStage = 0;
    }
}

void experimentConfig::on_voltage_textChanged(const QString &arg1)
{
    if (syncStage == finalStage) {
        ui->pushButton_3->setText("Sync Changes");
        syncStage = 0;
    }
}
