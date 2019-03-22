TVector3 CalcCOMmomentum(TVector3 pos, double energy, double mass)
{
  bool debug = 0;
  
  energy=energy/1000.;
  
  double vParticleMag = sqrt((2.*energy)/mass);
  TVector3 vParticle = pos;
  vParticle.SetMag(vParticleMag);
  
  double vBeam = sqrt((2.*BEAM_ENERGY)/MASS_BE11);
  
  double vCOMMag = (mass*vBeam)/(mass+MASS_BE11);
  TVector3 vCOM(0.,0.,vCOMMag);
  
  if(debug)
  {
    cout<<"Calculated Theta COM: "<<((vParticle-vCOM)*mass).Theta()*180./TMath::Pi()<<endl;
  }
  
  return((vParticle-vCOM)*mass);
}

TVector3 CalcCOMmomentum(TCSMHit* Hit, int Z)
{
  double MASS = 0.;
  string type;
  
  switch(Z)
  {
    case 10:
      MASS = MASS_BE10;
      type = "10be";
      break;
    case 12:
      MASS = MASS_BE12;
      type = "12be";
      break;
    case 8:
      MASS = MASS_BE8;
      type = "8be";
      break;
    case 11:
      MASS = MASS_BE11;
      type = "11be";
      break;
    case 0:
      MASS = Hit->GetMassMeV();
      cout<<"I don't know what to do here, default mass set"<<endl;
      break;
    default:
      cerr<<"Unrecognized Z in CalcCOMmomentum: "<<Z<<endl;
      MASS = Z;
  }
  
  //   return CalcCOMmomentum(Hit->GetPosition(),Hit->GetCorrectedEnergy(type),MASS);
  return CalcCOMmomentum(Hit->GetPosition(),Hit->GetEnergy(),MASS);
}

double CalcCOMEnergyMeV(TCSMHit* Hit, int Z)
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
      cerr<<"Unrecognized Z in CalcCOMEnergyMeV: "<<Z<<endl;
      MASS = Z;
  }
  
  TVector3 pvec = CalcCOMmomentum(Hit,Z);
  
  return(pvec.Mag2()/(2*MASS));
}

double CalcCOMThetaDeg(TCSMHit* Hit, int Z)
{ 
  TVector3 pvec = CalcCOMmomentum(Hit,Z);
  
  return pvec.Theta()*180./TMath::Pi();
}
