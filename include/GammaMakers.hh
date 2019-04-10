#ifndef GAMMAMAKERS_H
#define GAMMAMAKERS_H

#include <cstdio>
#include "TTigress.h"
#include <TChain.h>
#include <TList.h>
#include <TH1D.h>
#include "Gamma.hh"
#include <TStopwatch.h>

void ProcessGammas(TChain* chain,TList* outlist,TList* suppList);

#endif