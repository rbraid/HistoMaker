#ifndef STATES_H
#define STATES_H

#include <iostream>
#include <TObject.h>

Bool_t InGaussian(double value, double median, double sigma, double nStdDev);

int GetGamState(double doppVal, int nSigma = 4, int iso = 10);
int GetExState(double exVal, int iso, bool sim);

#endif