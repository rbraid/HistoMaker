#ifndef DEPRICIATED_H
#define DEPRICIATED_H

#include <cstdio>
#include "TCSM.h"
#include "GlobalSettings.hh"

double* CalcBe8fromAlpha(TCSMHit *A1H,TCSMHit *A2H);
double* CalcBe10fromHe64(TCSMHit *He6Hit, TCSMHit *He4Hit);
double GetExciteE_Light(TCSMHit *A1H, TCSMHit *A2H);
double PixelSA(int StripX, int StripY);
double PixelSA(TCSMHit* hit);
double EdgeEffectFactor(int StripX, int StripY, int Detector);
double EdgeEffectFactor(TCSMHit* hit);
// TVector3 GetPositionsNew(int detector,char pos, int horizontalstrip, int verticalstrip);
// TVector3 GetPositionsNew(TCSMHit* Hit, char pos);
// double ManualFracCOM(double ExcitedState, double ThetaCOM);

#endif