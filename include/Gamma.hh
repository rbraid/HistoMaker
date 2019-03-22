#ifndef GAMMA_H
#define GAMMA_H

#include <cstdio>

double Doppler(TTigressHit* thit, TCSMHit* chit, int mass);
double Doppler(double tenergy, double ttheta, double tphi, double cenergy, double ctheta, double cphi, int mass);
double Doppler(TTigressHit* thit, double cenergy, double ctheta, double cphi, int mass);
double EfficiencyWeight(TTigressHit* thit);
vector<double> GetEffAndError(double Energy, bool Error = 0);

#endif