#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "serialsetup.h"
#include <QtCore>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::getSerialPort(QSerialPort *port)
{
    serial = port;
}

void MainWindow::on_actionSerial_Config_triggered()
{
    SerialSetup *ss = new SerialSetup(this);
    connect(ss, SIGNAL(setSerialGlobal(QSerialPort*)), this, SLOT(getSerialPort(QSerialPort*)));
    ss->show();
}
