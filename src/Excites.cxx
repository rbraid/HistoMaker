double GetExciteE_Heavy(double energy, double theta, double mass)
{
  //   cout<<"BeamE: "<<BeamE<<endl;
  //   cout<<"BeE: "<<energy<<endl;
  //   cout<<"BeT: "<<theta<<endl;
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
  
  //   cout<<"EX: "<<ExcitedState<<endl<<endl;
  
  return(ExcitedState);
}

double GetExciteE_Heavy(TCSMHit* Hit, int Z)
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
    case 9:
      MASS = MASS_BE9;
      break;
    case 0:
      MASS = Hit->GetMassMeV();
      break;
    default:
      cerr<<"unrecognized Z in GetExciteE_Heavy: "<<Z<<endl;
      MASS = Z;
  }
  
  return(GetExciteE_Heavy(Hit->GetEnergy(),Hit->GetDPosition().Theta(),MASS));
}

double GetExciteE_Heavy_Corrected(TCSMHit* Hit, int Z)
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
  
  return(GetExciteE_Heavy(Hit->GetCorrectedEnergyMeV(isotope)*1000.,Hit->GetDPosition().Theta(),MASS));
}
