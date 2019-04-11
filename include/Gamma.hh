#ifndef GAMMA_H
#define GAMMA_H

#include <cstdio>
#include "TTigress.h"
#include "TCSM.h"
#include <TMath.h>
#include <TVector3.h>
#include "GlobalSettings.hh"
#include <TF1.h>
#include <TGraphErrors.h>
#include <TFitResult.h>
#include <TFile.h>

double Doppler(TTigressHit* thit, TCSMHit* chit, int mass);
double Doppler(double tenergy, double ttheta, double tphi, double cenergy, double ctheta, double cphi, int mass);
double Doppler(TTigressHit* thit, double cenergy, double ctheta, double cphi, int mass);
double OldEfficiencyWeight(TTigressHit* thit);
vector<double> OldGetEffAndError(double Energy, bool Error = 0);

double EfficiencyWeight(TTigressHit* thit, TFile* gammaFile);
vector<double> GetEffAndError(double Energy, TFile* gammaFile, bool error);

#endif