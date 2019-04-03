#ifndef DUALMAKERS_H
#define DUALMAKERS_H

#include <cstdio>
#include "TTigress.h"
#include "TCSM.h"
#include <TChain.h>
#include <TList.h>
// #include <TCutG.h>
// #include <TH2D.h>
// #include <TH1D.h>
#include "States.hh"
#include "Excites.hh"
#include "Gamma.hh"
#include "Rings.hh"
#include "CorrParticles.hh"
#include <TH2I.h>

void ProcessDual10Be(TChain* chain,TList* outlist,TFile* ringFile, bool sim);
void ProcessDualElastic(TChain* chain,TList* outlist,TFile* ringFile, bool sim);

#endif