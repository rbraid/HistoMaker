#ifndef FRACTION_H
#define FRACTION_H

#include <cstdio>

double GetfCOM(TCSMHit* Hit, int Z);
double GetfLab(TCSMHit* Hit, int Z);
double GetK(double energy, double theta, double mass);
double GetK(TCSMHit* Hit, int Z);
double GetK_Corrected(TCSMHit* Hit, int Z);
double Keri_GetfCM(double Exstate, double ThetaCM);
double Keri_GetKTransfer(double Exstate);

#endif