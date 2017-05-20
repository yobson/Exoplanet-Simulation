#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "serialsetup.h"
#include "experimentconfig.h"
#include <QtCore>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  serial = NULL;
  indefiniateLog = false;
  inLog = false;
  logValues = NULL;
  numberOfSamples = 256;
}

MainWindow::~MainWindow()
{
  delete ui;
  if (logValues != NULL) delete logValues;
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

void MainWindow::on_actionExperiment_Settings_triggered()
{
    experimentConfig *win = new experimentConfig(this);
    connect(win, SIGNAL(indef()), this, SLOT(setIndefiniateLog()));
    connect(win, SIGNAL(def()), this, SLOT(setDefiniateLog()));
    win->show();
    win->setSerialPort(serial);
}

void MainWindow::on_StartButton_clicked()
{
    if (!inLog) {
        if (indefiniateLog) {
            ui->StartButton->setText("Stop");
        }
    }
    logPointer = 0;
    if (logValues != NULL) delete logValues;
    logValues = new double[numberOfSamples];
    connect (serial, SIGNAL(readyRead()), this, SLOT(log()));
    serial->write("Start");
}

void MainWindow::setIndefiniateLog()
{
    indefiniateLog = true;
}

void MainWindow::setDefiniateLog()
{
    indefiniateLog = false;
}

void MainWindow::setSampleNumber(int sn)
{
    numberOfSamples = sn;
}

void MainWindow::log()
{
    logValues[logPointer] = QString::fromLatin1(serial->readAll()).toDouble();
    qDebug() << logValues[logPointer];
    logPointer++;
}
