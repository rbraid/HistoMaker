#include "../include/Fraction.hh"

double GetK(double energy, double theta, double mass)
{
  energy=energy/1000.;
  const double pi = TMath::Pi();
  
  double othermass;
  
  switch(int(mass))
  {
    case 7456: //be8
      othermass = MASS_BE12;
      break;
    case 8394: //be9
      othermass = MASS_BE11;
      break;
    case 9327: //be10
      othermass = MASS_BE10;
      break; 
    case 10266: //be11
      othermass = MASS_BE9;
      break;
    case 11203: //be12
      othermass = MASS_BE8;
      break;
    default:
      cerr<<"Unknown mass in GetExciteE_Heavy: "<<mass<<endl;
      return(-1);
  }
  
  const double M1 = MASS_BE11;
  const double M2 = MASS_BE9;
  const double M3 = othermass;
  const double M4 = mass;
  double mQ = M1+M2-M3-M4;
  
  double V1 = sqrt(2*BEAM_ENERGY/M1);
  double COMV = ( M1 / ( M1 + M2 ) ) * V1;
  double V4 = sqrt(2*energy/M4);
  double kPrimeM4 = COMV / V4;
  
  double COMTotalE = M2 / ( M1 + M2 ) * BEAM_ENERGY;
  double COMEnergyM4 = energy * ( 1 + kPrimeM4*kPrimeM4 - 2*kPrimeM4*cos( theta ) );
  double QVal =  ( COMEnergyM4*( M3 + M4 ) ) / M3 - COMTotalE;
  double ExcitedState = mQ - QVal;
  
  
  double K = sqrt((M1*M4*COMTotalE)/(M2*M3*(COMTotalE+ExcitedState)));
  return(K);
}

double GetK(TCSMHit* Hit, int Z)
{
  double MASS = 0.;
  
  switch(Z)
  {
    case 10:
      MASS = MASS_BE10;
      break;
    case 12:
      MASS = MASS_BE12;
      break;
    case 8:
      MASS = MASS_BE8;
      break;
    case 11:
      MASS = MASS_BE11;
      break;
    case 0:
      MASS = Hit->GetMassMeV();
      break;
    default:
      cerr<<"unrecognized Z in GetExciteE_Heavy: "<<Z<<endl;
      MASS = Z;
  }
  
  return(GetK(Hit->GetEnergy(),Hit->GetPosition().Theta(),MASS));
}

double GetK_Corrected(TCSMHit* Hit, int Z)
{
  double MASS = 0.;
  TString isotope;
  
  switch(Z)
  {
    case 10:
      MASS = MASS_BE10;
      isotope = "10be";
      break;
    case 12:
      MASS = MASS_BE12;
      isotope = "12be";
      break;
    case 11:
      MASS = MASS_BE11;
      isotope = "11be";
      break;
    case 8:
      MASS = MASS_BE8;
      isotope = "8be";
      break;
    case 9:
      MASS = MASS_BE9;
      isotope = "9be";
      break;
    case 0:
      MASS = Hit->GetMassMeV();
      isotope = Hit->GetIsotope();
      break;
    default:
      cerr<<"unrecognized Z in GetExciteE_Heavy_Corrected: "<<Z<<endl;
      MASS = Z;
      isotope = "12be";
  }
  
  return(GetK(Hit->GetCorrectedEnergy(isotope),Hit->GetPosition().Theta(),MASS));
}

double GetfCOM(TCSMHit *Hit, int Z)
{
  double K = GetK_Corrected(Hit,Z);
  double ThetaCM = CalcCOMmomentum(Hit, Z).Theta();
  
  double Num = 1. + K * cos(ThetaCM);
  double Den = pow(1. + K*K + 2*K*cos(ThetaCM),1.5);
  return (abs(Num/Den));
}

double GetfLab(TCSMHit *Hit, int Z)
{
  double K = GetK_Corrected(Hit,Z);
  double ThetaLab = Hit->GetPosition().Theta();
  
  double Num = sqrt(1.-pow(K*sin(ThetaLab),2.));
  double Den = pow(Num+K*cos(ThetaLab),2.);
  return (abs(Num/Den));
}
double ManualFracCOM(double ExcitedState, double ThetaCOM)
{
  if(ExcitedState > 15.)
    cerr<<"ManualFracCOM takes MeV"<<endl;
  const double M1 = MASS_BE11;
  const double M2 = MASS_BE9;
  const double M3 = MASS_BE10;
  const double M4 = MASS_BE10;
  
  ExcitedState -= MASS_BE11 + MASS_BE9 - (MASS_BE10*2.);
  ExcitedState = -ExcitedState;
  
  //   cout<<"Excited State after: "<<ExcitedState<<endl;
  
  double COMTotalE = M2 / ( M1 + M2 ) * BEAM_ENERGY;
  double K = sqrt((M1*M4*COMTotalE)/(M2*M3*(COMTotalE+ExcitedState)));
  
  //   cout<<"ManualFracCOM ExcitedState: "<<ExcitedState<<endl;
  //   
  //   cout<<"ManualFracCOM K: "<<K<<endl;
  
  double Num = 1. + K * cos(ThetaCOM);
  double Den = pow(1. + K*K + 2*K*cos(ThetaCOM),1.5);
  
  //   cout<<"ManualFracCOM Num: "<<Num<<", Den: "<<Den<<", Result: "<<abs(Num/Den)<<endl;
  
  return (abs(Num/Den));
}


double Keri_GetfCM(double Exstate, double ThetaCM)
{
  double K = Keri_GetKTransfer(Exstate);
  
  //   cout<< "Keri K: "<<K<<endl;
  
  double Num = 1. + K * cos(ThetaCM);
  double Den = pow(1. + K*K + 2*K*cos(ThetaCM),1.5);
  
  //   cout<<"Keri Num: "<<Num<<", Den: "<<Den<<", Result: "<<abs(Num/Den)<<endl;
  
  if (Num>0)
    return (Num/Den);
  else
    return (-Num/Den);
}

double Keri_GetKTransfer(double Exstate)
{
  double m1 = MASS_BE11;
  double m2 = MASS_BE9;
  double m3 = MASS_BE10;
  double m4 = MASS_BE10;
  
  double ExciteM = (m4 + Exstate);
  double QExcite = (m2 + m1 - m3 - ExciteM);
  double CenterOfMassTotalE = (m2*BEAM_ENERGY)/(m1+m2);
  double K = sqrt((m1*m4*CenterOfMassTotalE)/(m2*m3*(CenterOfMassTotalE+QExcite)));
  //   cout<<"Keri QExcite: "<<QExcite<<endl;
  //   double K = sqrt((M1*M4*COMTotalE)/(M2*M3*(COMTotalE+ExcitedState)));
  
  return (K);
}
