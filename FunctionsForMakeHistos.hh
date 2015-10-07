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

double Doppler(TTigressHit* thit, TCSMHit* chit);
double* CorrParticle(double E, double Theta, double Phi, double Mass);
double* CorrParticle(TCSMHit*);
double* CorrParticleFromAlphas(TCSMHit* Hit1, TCSMHit* Hit2);
void SetupHistos(TList *outlist);
double GetExciteE_Heavy(double be12E, double be12T);
double GetExciteE_Heavy(TCSMHit* Hit);
double* CalcBe8fromAlpha(TCSMHit *A1H,TCSMHit *A2H);
double GetExciteE_Light(TCSMHit *A1H, TCSMHit *A2H);


#endif