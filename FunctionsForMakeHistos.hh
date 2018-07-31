#ifndef FUNCFORMAKEHISTOS_H
#define FUNCFORMAKEHISTOS_H

#include <cstdio>
#include <fstream>

#include <TObject.h>
#include <TFile.h>
#include <TChain.h>
#include <TList.h>
#include <TCut.h>
#include <TCutG.h>
#include <TH3.h>
#include <TH2.h>
#include <TH1.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TFitResult.h>
#include <TStopwatch.h>
#include <TKey.h>
#include <TTree.h>
#include <TApplication.h>

#include "TNucleus.h"
#include "TKinematics.h"
#include "Globals.h"
#include "TTigress.h"
#include "TCSM.h"

#include "GlobalSettings.hh"

#define DEBUG 0

extern double BEAM_ENERGY;
extern bool ANGULAR_DISTRIBUTION;
extern bool SIMULATED_DATA;

double Doppler(TTigressHit* thit, TCSMHit* chit, int mass);
double Doppler(double tenergy, double ttheta, double tphi, double cenergy, double ctheta, double cphi, int mass);
double Doppler(TTigressHit* thit, double cenergy, double ctheta, double cphi, int mass);

double* CorrParticle(double E, double Theta, double Phi, double Mass);
double* CorrParticle(TCSMHit*, int Z = 0);
double* CorrParticleFromAlphas(TCSMHit* Hit1, TCSMHit* Hit2);
void SetupHistos(TList *outlist);
double GetExciteE_Heavy(double be12E, double be12T,int Z);
double GetExciteE_Heavy(TCSMHit* Hit,int Z);
double GetExciteE_Heavy_Corrected(TCSMHit* Hit,int Z);
double* CalcBe8fromAlpha(TCSMHit *A1H,TCSMHit *A2H);
double* CalcBe10fromHe64(TCSMHit *He6Hit, TCSMHit *He4Hit);
double GetExciteE_Light(TCSMHit *A1H, TCSMHit *A2H);
bool AlmostEqual(double a, double b, double threshold = .2);
TVector3 CalcCOMmomentum(TVector3 pos, double energy, double mass);
TVector3 CalcCOMmomentum(TCSMHit* Hit, int Z = 0);

double EfficiencyWeight(TTigressHit* thit);
vector<double> GetEffAndError(double Energy, bool Error = 0);

double CalcCOMEnergyMeV(TCSMHit* Hit, int Z = 0);
double CalcCOMThetaDeg(TCSMHit* Hit, int Z = 0);
// double CalcCOMEnergyMeV(TVector3 pvec, int Z = 0);
// double CalcCOMThetaDeg(TVector3 pvec, int Z = 0);

TVector3 GetPositionsNew(int detector,char pos, int horizontalstrip, int verticalstrip);
TVector3 GetPositionsNew(TCSMHit* Hit, char pos);

double GetfCOM(TCSMHit* Hit, int Z);
double GetfLab(TCSMHit* Hit, int Z);
double GetK(double energy, double theta, double mass);
double GetK(TCSMHit* Hit, int Z);
double GetK_Corrected(TCSMHit* Hit, int Z);
int GetRingNo(TCSMHit *Hit, int state,char detTypeChar);
double* RingInfo(TCSMHit *Hit, int state, char detTypeChar);


#endif