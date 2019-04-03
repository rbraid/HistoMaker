#ifndef PIDMAKERS_H
#define PIDMAKERS_H

#include <cstdio>
#include "TTigress.h"
#include "TCSM.h"
#include <TChain.h>
#include <TList.h>
#include <TCutG.h>
#include <TH2D.h>
#include <TH1D.h>
#include "States.hh"
#include "Excites.hh"
#include "Gamma.hh"
#include "Rings.hh"
#include <TStopwatch.h>

void Process9BePID(TChain* chain,TList* outlist,TList* cutlist,TFile* ringFile, bool sim);
void Process10BePID(TChain* chain,TList* outlist,TList* cutlist,TFile* ringFile, bool sim);
void Process11BePID(TChain* chain,TList* outlist,TList* cutlist,TFile* ringFile, bool sim);

#endif