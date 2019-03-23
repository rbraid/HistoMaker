#ifndef CENTEROFMASS_H
#define CENTEROFMASS_H

#include <cstdio>
#include <TVector3.h>
#include "TCSM.h"

TVector3 CalcCOMmomentum(TVector3 pos, double energy, double mass);
TVector3 CalcCOMmomentum(TCSMHit* Hit, int Z = 0);
double CalcCOMEnergyMeV(TCSMHit* Hit, int Z = 0);
double CalcCOMThetaDeg(TCSMHit* Hit, int Z = 0);

#endif