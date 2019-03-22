#ifndef CORRPARTICLES_H
#define CORRPARTICLES_H

#include <cstdio>

double* CorrParticle(double E, double Theta, double Phi, double Mass);
double* CorrParticle(TCSMHit*, int Z = 0);
double* CorrParticleFromAlphas(TCSMHit* Hit1, TCSMHit* Hit2);

#endif