#ifndef MISC_H
#define MISC_H

#include <cstdio>
#include <TMath.h>

bool AlmostEqual(double a, double b, double threshold = .2);
double toDegrees(double);
double toRadians(double);

#endif