#include "../include/Rings.hh"

int RingNumber(int stripX, int stripY, int detector)
{  
  TH2D* histo = (TH2D*)ringFile->Get(Form("Total_Rings_0_d%i_pid_Be10",detector));
  int binNo = histo->GetBin(stripX+1,stripY+1);
  int Ring = histo->GetBinContent(binNo);
  Ring -= 1;
  
  return Ring;
}

int RingNumber(TCSMHit* Hit)
{
  return(RingNumber(Hit->GetDVerticalStrip(),Hit->GetDHorizontalStrip(),Hit->GetDetectorNumber()));
}

double RingSA(int Ring, int Det)
{  
  TH1D* spec = (TH1D*)ringFile->Get(Form("SA_0_d%i_pid",Det));
  double TotalSolidAngle = spec->GetBinContent(Ring+1);
  
  return TotalSolidAngle;
}


void RingRange(int Ring, int Det, int State)
{
  TGraph* g = (TGraph*)ringFile->Get(Form("COM_d%i_s%i",Det,State));
  Double_t xp, yp;
  g->GetPoint(Ring,xp,yp);
  cout<<"PID RingRange: " <<yp<<" +/- "<<g->GetErrorY(Ring)<<endl;
}

double RingSA_err(int Ring, int Det)
{  
  TH1D* spec = (TH1D*)ringFile->Get(Form("SA_0_d%i_pid_err",Det));
  double TotalSolidAngle_err = spec->GetBinContent(Ring+1);
  
  return TotalSolidAngle_err;
}

double PixelSA(int StripX, int StripY)
{  
  TH1D* spec = (TH1D*)SAFile->Get("sa1d");
  double SA = spec->GetBinContent(StripX+1,StripY+1);
  return SA;
}

// double PixelSAErr(int StripX, int StripY)
// {  
//   TH1D* spec_min = (TH1D*)SAFile_min->Get("sa1d");
//   double SA_min = spec_min->GetBinContent(StripX+1,StripY+1);
//   TH1D* spec_max = (TH1D*)SAFile_max->Get("sa1d");
//   double SA_max = spec_max->GetBinContent(StripX+1,StripY+1);
//   return sqrt(2)*(SA_max-SA_min);
// }

double PixelSA(TCSMHit* hit)
{
  return(PixelSA(hit->GetDVerticalStrip(),hit->GetDHorizontalStrip()));
}
