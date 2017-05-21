#include "live.h"
#include "ui_live.h"
#include <math.h>
#include <QDebug>

Live::Live(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Live)
{
    ui->setupUi(this);
    im_in = NULL;
    re_in = NULL;
    re_out = NULL;
    im_out = NULL;

    started = false;
    counter = 0;

    fftLine = new QLineSeries();
    fftChart = new QChart();
    fftChart->legend()->hide();
    fftChart->setTitle("Live");

    fftChartView = new QChartView(fftChart);
    ui->graph->addWidget(fftChartView);

    dirty = false;
    powPos = 4;

    sampleNumber = 16;
}

Live::~Live()
{
    delete ui;
    if (im_in != NULL) delete im_in;
    if (re_in != NULL) delete re_in;
    if (re_out != NULL) delete re_out;
    if (im_out != NULL) delete im_out;
    delete fftLine;
}

void Live::setSerial(QSerialPort *SP)
{
    serial = SP;
}

void Live::setSampleRate(double in)
{
    sampleRate = in;
    interval = sampleRate / sampleNumber;
}

void Live::on_startButton_clicked()
{
    if (!started) {
        im_in = new double[sampleNumber];
        re_in = new double[sampleNumber];
        re_out = new double[sampleNumber];
        im_out = new double[sampleNumber];
        memset(im_in, 0, sampleNumber*sizeof(double));
        started = true;
        connect(serial, SIGNAL(readyRead()), this, SLOT(log()));
        ui->startButton->setText("Stop");
        serial->write("Live");
    } else {
        serial->write("!");
        disconnect(serial, SIGNAL(readyRead()), this, SLOT(log()));
        ui->startButton->setText("Start");
    }


}

void Live::log()
{
    re_in[counter] = QString::fromLatin1(serial->readAll()).toDouble();
    counter++;
    if (counter >= sampleNumber) {
        counter = 0;
        fftChart->removeAllSeries();
        fftLine = new QLineSeries();

        fft(re_in, im_in, sampleNumber, re_out, im_out);
        for (int i = 0; i < sampleNumber/2; i++) {
            fftLine->append(i * interval, sqrt(re_out[i] * re_out[i] + im_out[i]*im_out[i]));
        }
        fftChart->addSeries(fftLine);
        fftChart->createDefaultAxes();
        if (dirty) {
            sampleNumber = (int)pow(2, powPos);
            dirty = false;
            delete re_in;
            delete re_out;
            delete im_in;
            delete im_out;
            re_in = new double[sampleNumber];
            re_out = new double[sampleNumber];
            im_in = new double[sampleNumber];
            im_out = new double[sampleNumber];
            memset(im_in, 0, sampleNumber*sizeof(double));
            interval = sampleRate / sampleNumber;
        }
    }
}

void Live::on_slider_sliderMoved(int position)
{
    ui->SampleNumber->setText(QString("%0").arg(pow(2, position)));
    powPos = position;
}

void Live::on_update_clicked()
{
    dirty = true;
}
