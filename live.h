#ifndef LIVE_H
#define LIVE_H

#include <QDialog>
#include <QSerialPort>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView>
#include "fft.hpp"

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class Live;
}

class Live : public QDialog
{
    Q_OBJECT

public:
    explicit Live(QWidget *parent = 0);
    ~Live();
    void setSerial(QSerialPort *SP);
    void setSampleRate(double);

private slots:
    void on_startButton_clicked();
    void log();

    void on_slider_sliderMoved(int position);

    void on_update_clicked();

private:
    Ui::Live *ui;
    QSerialPort *serial;
    QChart *fftChart;
    QLineSeries *fftLine;
    QChartView *fftChartView;

    bool dirty;
    int powPos;

    double *re_in;
    double *re_out;
    double *im_in;
    double *im_out;

    double interval;
    int sampleNumber;
    double sampleRate;

    bool started;
    int counter;
};

#endif // LIVE_H
