#ifndef LOGGER_H
#define LOGGER_H

#include <QtCore>
#include <QSerialPort>

class Logger : public QThread
{
public:
    Logger();
    void run();
    void setReady();

private:
    QSerialPort *serial;
    bool ready;
    int sampleNumber;
    double *out;
};

#endif // LOGGER_H
