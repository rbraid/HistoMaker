#ifndef MAKEHISTOS_H
#define MAKEHISTOS_H

#include <cstdio>
// #include <fstream>

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
#include <TGraphAsymmErrors.h>
#include <TF1.h>
#include <TFitResult.h>
#include <TStopwatch.h>
#include <TKey.h>
#include <TTree.h>
#include <TApplication.h>

// #include "TNucleus.h"
// #include "TKinematics.h"
// #include "Globals.h"
#include "TTigress.h"
#include "TCSM.h"

#include "../include/GlobalSettings.hh"

#include "../include/CenterOfMass.hh"
#include "../include/CorrParticles.hh"
#include "../include/Excites.hh"
#include "../include/Fraction.hh"
#include "../include/Gamma.hh"
#include "../include/Misc.hh"
#include "../include/Rings.hh"
#include "../include/States.hh"
#include "../include/SetupHistos_only.hh"

#define DEBUG 0

extern bool SIMULATED_DATA;

extern TFile* ringFile;
extern TFile* edgeFile;
// extern TFile* SAFile;

#endif