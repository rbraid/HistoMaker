#ifndef BASICMAKER_H
#define BASICMAKER_H

#include <cstdio>
// #include "TTigress.h"
#include "TCSM.h"
#include <TChain.h>
#include <TList.h>
// #include <TCutG.h>
#include <TH2D.h>
#include <TH1D.h>
// #include "States.hh"
// #include "Excites.hh"
// #include "Gamma.hh"
// #include "Rings.hh"
// #include "CorrParticles.hh"
#include <TStopwatch.h>
#include <TH2I.h>

void ProcessBasic(TChain* chain,TList* outlist);
void SetupBasicHistos(TList* hlist);

#endif