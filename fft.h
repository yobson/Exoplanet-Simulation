#ifndef FFT_H
#define FFT_H

#include <QtCore>
#include <fft.hpp>


class FFT : public QThread
{
public:
    FFT();
    void run();

private:

};

#endif // FFT_H
