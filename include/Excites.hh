#ifndef EXCITES_H
#define EXCITES_H

#include <cstdio>
#include "TCSM.h"
#include <TMath.h>
#include "GlobalSettings.hh"

double GetExciteE_Heavy(double be12E, double be12T,int Z);
double GetExciteE_Heavy(TCSMHit* Hit,int Z);
double GetExciteE_Heavy_Corrected(TCSMHit* Hit,int Z);

#endif