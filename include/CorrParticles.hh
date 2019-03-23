#ifndef CORRPARTICLES_H
#define CORRPARTICLES_H

#include <cstdio>
#include "TCSM.h"
#include <TMath.h>
#include "TCSM.h"
#include "GlobalSettings.hh"
#include <TVector3.h>

double* CorrParticle(double E, double Theta, double Phi, double Mass);
double* CorrParticle(TCSMHit*, int Z = 0);
double* CorrParticleFromAlphas(TCSMHit* Hit1, TCSMHit* Hit2);

#endif