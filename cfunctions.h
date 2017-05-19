#ifndef CFUNCTIONS_H
#define CFUNCTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <math.h>
#include <complex.h>

double PI;
typedef double complex cplx;

void fft(cplx buf[], int n);
void _fft(cplx buf[], cplx out[], int n, int step);
void show(const char * s, cplx buf[]);

#ifdef __cplusplus
}
#endif

#endif // CFUNCTIONS_H
