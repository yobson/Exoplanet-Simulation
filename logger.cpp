#include "logger.h"

Logger::Logger()
{
    ready = false;
}

void Logger::run()
{
    QString s;
    out = new double[sampleNumber];
    for (int i = 0; i < sampleNumber; i++) {
        while (!ready) ;
        s = QString::fromLatin1(serial->readAll());
        sampleNumber[i] = s.toDouble();
    }

}

void Logger::setReady()
{
    ready = true;
}
