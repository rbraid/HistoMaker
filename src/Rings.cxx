#include "../include/Rings.hh"

int RingNumber(int stripX, int stripY, int detector, TFile* ringFile)
{  
  TH2D* histo = (TH2D*)ringFile->Get(Form("Total_Rings_0_d%i_pid_Be10",detector));
  int binNo = histo->GetBin(stripX+1,stripY+1);
  int Ring = -1;
  if(histo)
    Ring = histo->GetBinContent(binNo);
  
  Ring -= 1;
  
  return Ring;
}

int RingNumber(TCSMHit* Hit, TFile* ringFile)
{
  return(RingNumber(Hit->GetDVerticalStrip(),Hit->GetDHorizontalStrip(),Hit->GetDetectorNumber(),ringFile));
}

double RingSA(int Ring, int Det, TFile* ringFile)
{  
  TH1D* spec = (TH1D*)ringFile->Get(Form("SA_0_d%i_pid",Det));
  double TotalSolidAngle = -2;
  
  if(spec)
    TotalSolidAngle = spec->GetBinContent(Ring+1);
  
  return TotalSolidAngle;
}

double RingSA_err(int Ring, int Det, TFile* ringFile)
{  
  TH1D* spec = (TH1D*)ringFile->Get(Form("SA_0_d%i_pid_err",Det));
  double TotalSolidAngle_err = -2;
  
  if(spec)
spec->GetBinContent(Ring+1);
  
  return TotalSolidAngle_err;
}
