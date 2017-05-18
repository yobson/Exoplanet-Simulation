#ifndef SERIALSETUP_H
#define SERIALSETUP_H

#include <QDialog>
#include <QtSerialPort/QSerialPort>

namespace Ui {
  class SerialSetup;
}

class SerialSetup : public QDialog
{
  Q_OBJECT

public:
  explicit SerialSetup(QWidget *parent = 0);
  ~SerialSetup();

signals:
  void setSerialGlobal(QSerialPort*);

private slots:
  void on_pushButton_clicked();

  void readData();
  //void writeData(const QByteArrayData &data);

  //void handleError(QSerialPort::SerialPortError error);

  void on_pushButton_3_clicked();

  void on_pushButton_4_clicked();

private:
  Ui::SerialSetup *ui;
  QSerialPort *SP;

  QString getData();
  QByteArray data;
  int dataSize;

};

#endif // SERIALSETUP_H
