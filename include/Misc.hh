#ifndef MISC_H
#define MISC_H

#include <cstdio>
#include <TMath.h>
#include "TCSM.h"

int SideOpposite(int det);
int SideAdjacent(int det);
int SideOpposite(TCSMHit* hit);
int SideAdjacent(TCSMHit* hit);

bool AlmostEqual(double a, double b, double threshold = .2);
double toDegrees(double);
double toRadians(double);

#endif