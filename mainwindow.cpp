#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "serialsetup.h"
#include "experimentconfig.h"
#include <QtCore>
#include <QMessageBox>
#include <math.h>
#include <live.h>

#define PI 3.14159265358979323846

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  serial = NULL;
  inLog = false;
  logValues = NULL;
  numberOfSamples = 256;
  sampleRate = 10;
  referenceVoltage = 5;

  im_in = NULL;
  re_out = NULL;
  im_out = NULL;
  out = NULL;

  rawLine = new QLineSeries();
  fftLine = new QLineSeries();

  rawChart = new QChart();
  fftChart = new QChart();


  rawChart->legend()->hide();
  rawChart->setTitle("Raw Data");

  fftChart->legend()->hide();
  fftChart->setTitle("FFT Data");

  rawChartView = new QChartView(rawChart);
  fftChartView = new QChartView(fftChart);

  ui->RawLayout->addWidget(rawChartView);
  ui->FFTLayout->addWidget(fftChartView);
}

MainWindow::~MainWindow()
{
  delete ui;
  if (logValues != NULL) delete logValues;
  if (im_in != NULL) delete im_in;
  if (im_out != NULL) delete im_out;
  if (re_out != NULL) delete re_out;
  if (out != NULL) delete out;
  delete fftLine;
  delete rawLine;
  delete rawChart;
  delete fftChart;
  delete rawChartView;
  delete fftChartView;
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
    connect(win, SIGNAL(setSN(int)), this, SLOT(setSampleNumber(int)));
    connect(win, SIGNAL(setSR(double)), this, SLOT(setSampleRate(double)));
    connect(win, SIGNAL(setRV(double)), this, SLOT(setReferenceVoltage(double)));
    win->defRV(referenceVoltage);
    win->defSN(numberOfSamples);
    win->defSR(sampleRate);
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
        ui->StartButton->setText("Getting Data");
        logPointer = 0;
        if (logValues != NULL) delete logValues;
        logValues = new double[numberOfSamples];
        connect (serial, SIGNAL(readyRead()), this, SLOT(log()));
        serial->write("Live");
        inLog = true;
    }
}

void MainWindow::setSampleNumber(int sn)
{
    qDebug() << "Setting Sample Number to " << sn;
    numberOfSamples = sn;
}

void MainWindow::setSampleRate(double sr)
{
    qDebug() << "Setting Sample Rate to " << sr;
    sampleRate = sr;
}

void MainWindow::setReferenceVoltage(double rv)
{
    qDebug() << "Setting Reference Voltage to " << rv;
    referenceVoltage = rv;
}

void MainWindow::log()
{
    logValues[logPointer] = QString::fromLatin1(serial->readAll()).toDouble();
    logPointer++;
    if (logPointer == numberOfSamples-1) {
        disconnect (serial, SIGNAL(readyRead()), this, SLOT(log()));
        serial->write("!");
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
    ui->StartButton->setText("Start");
    qDebug() << "Adding to raw graph";
    rawChart->removeAllSeries();
    fftChart->removeAllSeries();
    fftLine = new QLineSeries();
    rawLine = new QLineSeries();
    double interval = 1 / sampleRate;
    double highest = 0;
    for (int i = 0; i < numberOfSamples; i++){
        rawLine->append(interval * i, logValues[i]);
        if (logValues[i] > highest) highest = logValues[i];
    }
    rawChart->addSeries(rawLine);
    rawChart->createDefaultAxes(); 
    qDebug() << "FFTing";
    im_in = new double[numberOfSamples];
    re_out = new double[numberOfSamples];
    im_out = new double[numberOfSamples];
    out = new double[numberOfSamples/2];

    memset(im_in, 0, numberOfSamples*sizeof(double));
    fft(logValues, im_in, numberOfSamples, re_out, im_out);
    interval = sampleRate / numberOfSamples;
    highest = 0;
    for (int i = 0; i < numberOfSamples/2; i++) {
        out[i] = sqrt(re_out[i] * re_out[i] + im_out[i] * im_out[i]);
        fftLine->append(interval*i, out[i]);
        if (out[i] > highest) highest = logValues[i];
    }
    fftChart->addSeries(fftLine);
    fftChart->createDefaultAxes();
    analyse();
}

void MainWindow::analyse()
{
    typedef struct {
        double magnitude;
        double freq;
    } peak;

    double threshold, sum = 0;
    int count = 0;
    double interval = sampleRate / numberOfSamples;
    peak *peaks;

    for (int i = 0; i < numberOfSamples/2; i++) {
        sum += out[i];
    }
    threshold = 6 * sum / (numberOfSamples/2);
    peaks = new peak[numberOfSamples/2];
    for (int i = 1; i < numberOfSamples/2; i++) {
        if (out[i] > threshold && out[i] > out[i+1] && out[i] > out[i-1]) {
            peaks[count].magnitude = out[i];
            peaks[count].freq = i * interval;
            qDebug() << "Mag: " << peaks[count].magnitude << "Freq: " << peaks[count].freq;
            count++;
        }
    }
    QString current;
    current.append(QString("\nNumber Of Detected Exoplanets: %0").arg(count));
    for (int i = 0; i < count; i++) {
        current.append(QString("\n\nPlanet %0:\n").arg(i));
        double orbit = ui->time->text().toDouble() * pow(10, ui->timeE->text().toDouble()) * (1/peaks[i].freq);
        current.append(QString("    Time for orbit: %0 %1\n").arg(orbit).arg(ui->TimeUnits->currentText()));
    }
    ui->Results->setPlainText(current);
    delete peaks;
}

void MainWindow::on_recalc_clicked()
{
    ui->Results->setPlainText("");
    analyse();
}

void MainWindow::on_actionLive_Mode_triggered()
{
    Live *l = new Live();
    l->setSerial(serial);
    l->setSampleRate(sampleRate);
    l->show();
}
