#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>

QT_CHARTS_USE_NAMESPACE

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

  void on_actionExperiment_Settings_triggered();

  void on_StartButton_clicked();
  void setIndefiniateLog();
  void setDefiniateLog();
  void setSampleNumber(int);
  void setSampleRate(double);
  void setReferenceVoltage(double);
  void log();

  void on_actionStart_Experiment_triggered();

  void on_recalc_clicked();

private:
  Ui::MainWindow *ui;
  QSerialPort *serial;
  bool indefiniateLog;
  bool inLog;
  double *logValues;
  int logPointer;
  int numberOfSamples;
  double sampleRate;
  double referenceVoltage;

  double *im_in;
  double *re_out;
  double *im_out;
  double *out;

  QChart *rawChart;
  QChart *fftChart;
  QLineSeries *rawLine;
  QLineSeries *fftLine;
  QChartView *rawChartView;
  QChartView *fftChartView;

  void finishedLogging();
  void analyse();
};

#endif // MAINWINDOW_H
