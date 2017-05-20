#include "serialsetup.h"
#include "ui_serialsetup.h"
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QtCore>

SerialSetup::SerialSetup(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SerialSetup)
{
  ui->setupUi(this);
  this->on_pushButton_clicked();
  SP = new QSerialPort();
  connect(SP, SIGNAL(readyRead()), this, SLOT(readData()));
  handshake = false;
}

SerialSetup::~SerialSetup()
{
  delete ui;
}

void SerialSetup::on_pushButton_clicked()
{
    ui->comboBox->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ui->comboBox->addItem(info.portName());
      }
}

void SerialSetup::readData()
{
  QByteArray data = SP->readAll();
  //qDebug() << "Recived " << data;
  if (QString::fromLatin1(data) == "Ready\n") {
      SP->write("A");
      qDebug() << "Attempting Handshake";
  }
  else if (QString::fromLatin1(data) == "A") {
      SP->write("ok");
      qDebug() << "Got Reply sending ok";
  }
  else if (QString::fromLatin1(data) == "Connected\n") {
      qDebug() << "Handshake complete!";
      emit (setSerialGlobal(SP));
      handshake = true;
      ui->pushButton_3->setText("Okay");
  }
  else { qDebug() << "Unmatched Data read"; }
}
void SerialSetup::on_pushButton_3_clicked()
{
  if (handshake) {
      on_pushButton_4_clicked();
  } else {
      SP->close();
      const QSerialPortInfo port = QSerialPortInfo::availablePorts().at(ui->comboBox->currentIndex());
      qDebug() << "Attempting to connect to " << port.portName();
      SP->setPort(port);
      SP->setBaudRate(QSerialPort::Baud115200);
      SP->setDataBits(QSerialPort::Data8);
      SP->setFlowControl(QSerialPort::NoFlowControl);
      SP->setStopBits(QSerialPort::OneStop);
      if (SP->open(QIODevice::ReadWrite)) {
          qDebug() << "Success!!";
          ui->pushButton_3->setText("Connecting");
      }
      else {
          qDebug() <<"Error!";
          return;
      }
  }
}


void SerialSetup::on_pushButton_4_clicked()
{
  disconnect(SP, SIGNAL(readyRead()), this, SLOT(readData()));
  this->close();
}
