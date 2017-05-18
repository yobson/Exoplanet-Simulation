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
  data.append(SP->readAll());
  dataSize = data.count();
  qDebug() << data;
}
void SerialSetup::on_pushButton_3_clicked()
{
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
  }
  else {
      qDebug() <<"Error!";
      return;
  }
  qDebug() << "Attempting Handshake: ";
  getData();
  SP->write("a");
  //qDebug() << getData();
  SP->write("ok");
  //qDebug() << getData();
}

QString SerialSetup::getData()
{
  //while (!dataSize) ;
  QString s = QString::fromLatin1(data);
  data.clear();
  dataSize = 0;
  return s;
}

void SerialSetup::on_pushButton_4_clicked()
{
  SP->close();
}
