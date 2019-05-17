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
// #include <TH3.h>
// #include <TH2.h>
// #include <TH1.h>
// #include <TGraphErrors.h>
// #include <TGraphAsymmErrors.h>
// #include <TF1.h>
// #include <TFitResult.h>
#include <TStopwatch.h>
#include <TKey.h>
#include <TTree.h>
#include <TApplication.h>

// #include "TNucleus.h"
// #include "TKinematics.h"
// #include "Globals.h"
#include "TTigress.h"
#include "TCSM.h"

#include "GlobalSettings.hh"

#include "CenterOfMass.hh"
#include "CorrParticles.hh"
#include "Excites.hh"
#include "Fraction.hh"
#include "Gamma.hh"
#include "Misc.hh"
#include "Rings.hh"
#include "States.hh"
#include "SetupHistos_only.hh"
#include "PIDMakers.hh"
#include "DualMakers.hh"
#include "GammaMakers.hh"
#include "OppositeMaker.hh"
#include "BasicMaker.hh"
#include "DualAndPIDMaker.hh"

#define DEBUG 0

#endif