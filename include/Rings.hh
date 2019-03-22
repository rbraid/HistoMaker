#ifndef RINGS_H
#define RINGS_H

#include <cstdio>

void RingRange(int Ring, int Det, int State);
int RingNumber(TCSMHit *Hit);
int RingNumber(int, int, int);
double RingSA(int Ring, int Det);
double RingSA(TCSMHit* Hit);
double RingSA_err(int Ring, int Det);

#endif