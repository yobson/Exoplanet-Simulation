#include "experimentconfig.h"
#include "ui_experimentconfig.h"
#include <QMessageBox>
#include <QtCore>

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
        break;
    case 2:
        qDebug() << "Syncing number of samples";
        serial->write(ui->SampleNumber->text().toLatin1());
        break;
    case 3:
        qDebug() << "Syncing number of samples";
        serial->write(ui->voltage->text().toLatin1());
        break;
    case 4:
        qDebug() << "Sync successful!";
        ui->pushButton_3->setText("Done");
        syncStage = 5;
        break;
    case 5:
        this->close();
    }
}

void experimentConfig::on_SampleRate_editingFinished()
{
    if (syncStage == 5) {
        ui->pushButton_3->setText("Sync Changes");
        syncStage = 0;
    }
}

void experimentConfig::on_SampleNumber_editingFinished()
{
    if (syncStage == 5) {
        ui->pushButton_3->setText("Sync Changes");
        syncStage = 0;
    }
}
