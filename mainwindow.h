#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void getSerialPort(QSerialPort *port);

  void on_actionSerial_Config_triggered();

private:
  Ui::MainWindow *ui;
  QSerialPort *serial;
};

#endif // MAINWINDOW_H
