#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "serialsetup.h"
#include "experimentconfig.h"
#include <QtCore>
#include <fft.hpp>
#include <QMessageBox>

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

  im_in = NULL;
  re_out = NULL;
  im_out = NULL;
  out = NULL;
}

MainWindow::~MainWindow()
{
  delete ui;
  if (logValues != NULL) delete logValues;
  if (im_in != NULL) delete im_in;
  if (im_out != NULL) delete im_out;
  if (re_out != NULL) delete re_out;
  if (out != NULL) delete out;
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
    connect(win, SIGNAL(setSN(int)), this, SLOT(setSampleNumber(int)));
    win->show();
    win->setSerialPort(serial);
}

void MainWindow::on_StartButton_clicked()
{
    if (serial == NULL) {
        QMessageBox err;
        err.setText("AHHHH set up serial First!!!");
        err.exec();
        return;
    }
    if (!inLog) {
        if (indefiniateLog) {
            ui->StartButton->setText("Stop");
        }
        logPointer = 0;
        if (logValues != NULL) delete logValues;
        logValues = new double[numberOfSamples];
        connect (serial, SIGNAL(readyRead()), this, SLOT(log()));
        serial->write("Start");
        inLog = true;
    } else if (indefiniateLog){
        serial->write("a");
        ui->StartButton->setText("Stop");
    }
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
    qDebug() << "Setting Sample Number to " << sn;
    numberOfSamples = sn;
}

void MainWindow::log()
{
    logValues[logPointer] = QString::fromLatin1(serial->readAll()).toDouble();
    qDebug() << logValues[logPointer];
    logPointer++;
    if (logPointer == numberOfSamples-1) {
        disconnect (serial, SIGNAL(readyRead()), this, SLOT(log()));
        inLog = false;
        finishedLogging();
    }
}

void MainWindow::on_actionStart_Experiment_triggered()
{
    on_StartButton_clicked();
}

void MainWindow::finishedLogging()
{
    qDebug() << "FFTing";
    im_in = new double[numberOfSamples];
    re_out = new double[numberOfSamples];
    im_out = new double[numberOfSamples];
    out = new double[numberOfSamples/2];

    memset(im_in, 0, numberOfSamples*sizeof(double));
    fft(logValues, im_in, numberOfSamples, re_out, im_out);
    for (int i = 0; i < numberOfSamples/2; i++) {
        out[i] = re_out[i] * re_out[i] + im_out[i] * im_out[i];
        qDebug() << out[i];
    }
}

void MainWindow::on_pushButton_clicked()
{
    finishedLogging();
}
