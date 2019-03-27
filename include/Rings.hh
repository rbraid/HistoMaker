#ifndef RINGS_H
#define RINGS_H

#include <cstdio>
#include "TCSM.h"
#include <TFile.h>
#include <TH2D.h>
#include <TH1D.h>

int RingNumber(TCSMHit *Hit, TFile* ringFile);
int RingNumber(int, int, int, TFile* ringFile);
double RingSA(int Ring, int Det, TFile* SAFile);
// double RingSA(TCSMHit* Hit, TFile* SAFile);
double RingSA_err(int Ring, int Det, TFile* SAFile);

#endif