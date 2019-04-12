#include "../include/Gamma.hh"

double Doppler(double tenergy, double ttheta, double tphi, double cenergy, double ctheta, double cphi, int mass)
{  
  TVector3 tVec = TVector3(tenergy/1000.,0,0);
  tVec.SetTheta(ttheta);
  tVec.SetPhi(tphi);
  
  TVector3 cVec = TVector3(cenergy/1000.,0,0);
  cVec.SetTheta(ctheta);
  cVec.SetPhi(cphi);
  
  double M4;
  
  switch(mass)
  {
    case 12:
      M4 = MASS_BE12;
      break;
    case 10:
      M4 = MASS_BE10;
      break;
    default:
      cerr<<"Doppler correcting for unknown mass, reverting to 12Be.  Note Doppler assumes a Be isotope for now."<<endl;
      M4 = MASS_BE12;
  }
  
  double LabEnergyHeavy = cVec.Mag();
  
  double beta =  sqrt( (LabEnergyHeavy*(LabEnergyHeavy + 2.0*M4) )/( ( LabEnergyHeavy + M4 )*( LabEnergyHeavy + M4 ) ) );
  
  double CosTheta = cos( cVec.Angle( tVec) );
  
  double RelativisticCorr = (1.0 / ( sqrt( 1.0 - beta*beta ) ) );
  
  double EGammaDopplerCorr = ( (tVec.Mag() * RelativisticCorr) * ( (1.0 - beta*CosTheta) ) );
  
  return EGammaDopplerCorr;
}

double Doppler(TTigressHit* thit, TCSMHit* chit, int mass)
{
  return Doppler(thit->GetCore()->GetEnergy(), thit->GetPosition().Theta(), thit->GetPosition().Phi(), chit->GetEnergy(), chit->GetPosition().Theta(), chit->GetPosition().Phi(), mass);
}

double Doppler(TTigressHit* thit, double cenergy, double ctheta, double cphi, int mass)
{
  return Doppler(thit->GetCore()->GetEnergy(), thit->GetPosition().Theta(), thit->GetPosition().Phi(), cenergy, ctheta, cphi, mass);
}

double OldEfficiencyWeight(TTigressHit* thit)
{
  return(OldGetEffAndError(thit->GetEnergy(),0).at(0));
}

vector<double> OldGetEffAndError(double Energy, bool Error)
{
  //Takes Energy in keV
  //Returns fractional eff, not percent
  vector<double> retVec;
  
  TF1 *GrandFit = new TF1("GrandFit","exp(pow(pow([0]+[1]*log(x/100)+[2]*log(x/100)*log(x/100),-[3])+pow([4]+[5]*log(x/10)+[6]*log(x/10)*log(x/10),-[3]),-1/[3]))/100.",0,10000);
  
  GrandFit->SetNDF(4);
  GrandFit->SetParameter(0,4.72185);
  GrandFit->SetParError(0,0);
  GrandFit->SetParameter(1,0.773074);
  GrandFit->SetParError(1,0);
  GrandFit->SetParameter(2,0);
  GrandFit->SetParError(2,0);
  GrandFit->SetParameter(3,2.81689);
  GrandFit->SetParError(3,0.0353034);
  GrandFit->SetParameter(4,5.82002);
  GrandFit->SetParError(4,0.0279105);
  GrandFit->SetParameter(5,-0.868251);
  GrandFit->SetParError(5,0.00977358);
  GrandFit->SetParameter(6,0.0146214);
  GrandFit->SetParError(6,0.00163102);
  
  double retval = 1./GrandFit->Eval(Energy);
  if(retval <=0.)
    retval = -1.;
  
  retVec.push_back(1./GrandFit->Eval(Energy));
  if(!Error)
  {
    delete GrandFit;
    return(retVec);
  }
  
  double x[8];
  double y[8];
  double ey[8];
  
  x[0]=121.178;
  x[1]=244.69 ;
  x[2]=344.27 ;
  x[3]=778.9  ; 
  x[4]=964.1  ; 
  x[5]=1170   ; 
  x[6]=1330   ; 
  x[7]=1408   ; 
  
  y[0]=0.260881 ;
  y[1]=0.183108 ;
  y[2]=0.164983 ;
  y[3]=0.091441 ;
  y[4]=0.0766207;
  y[5]=0.0750149;
  y[6]=0.0681534;
  y[7]=0.0665476;
  
  ey[0]=0.000487264;
  ey[1]=0.00139078 ;
  ey[2]=0.000613603;
  ey[3]=0.00128622 ;
  ey[4]=0.001336   ;
  ey[5]=0.00103761 ;
  ey[6]=0.00108187 ;
  ey[7]=0.0010485  ;
  
  TGraphErrors *EffPlot = new TGraphErrors(8,x,y,0,ey);
  
  TFitResultPtr ResPtr = EffPlot->Fit(GrandFit,"MRQS");
  
  double point[1];
  point[0] = Energy;
  double err[1];  
  ResPtr->GetConfidenceIntervals(1, 1, 1, point, err, 0.683, false); // 1 sigma  
  
  double error = err[0];
  
  retVec.push_back(error);
  
  delete GrandFit;
  delete EffPlot;
  return(retVec);
}

double EfficiencyWeight(TTigressHit* thit, TFile* gammaFile)
{
  return(GetEffAndError(thit->GetEnergy(),gammaFile).at(0));
}

vector<double> GetEffAndError(double Energy, TFile* gammaFile)
{
  //the old one takes 54 seconds 
  vector<double> retVec;
  TGraphErrors* graph = (TGraphErrors*)gammaFile->Get("EffandErr");
  
  double *yarr = graph->GetY();
  
  int graphpoint = int((Energy)/10.);

  double tmpVal = yarr[graphpoint];

  double *yEarr = graph->GetEY();
  
  double abserror = yEarr[graphpoint];
  double percenterror = abserror/tmpVal;
  
  if(tmpVal >0)
    tmpVal = 1./tmpVal;
  else
  {
    tmpVal = -1;
    retVec.push_back(-1);
    retVec.push_back(0);
    return retVec; 
  }
  retVec.push_back(tmpVal);
  abserror = percenterror*tmpVal;
  retVec.push_back(abserror);

//   cout<<"GetEffAndError Debug"<<endl;
//   cout<<"Energy: "<<Energy<<endl;
//   cout<<"Eff: "<<retVec.at(0)<<endl;
//   cout<<"Err: "<<retVec.at(1)<<endl<<endl;
  
  return retVec;
}
