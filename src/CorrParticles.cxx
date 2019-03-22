
double* CorrParticle(double Energy, double Theta, double Phi, double Mass)
{
  bool debug = 0;
  const double pi = TMath::Pi();
  
  if(debug)
    cout<<"CORR PARTICLE DEBUG ACTIVE, E: "<<Energy<<" T: "<<Theta*180./pi<<" P: "<<Phi*180./pi<<" M: "<<Mass<<" EXPECTED MASS: "<<MASS_BE12<<" or "<<MASS_BE8<<" or "<<MASS_BE10<<endl;
  
  Energy = Energy/1000.;
  
  double *Values = new double[3];
  
  double pParticleMag = sqrt( 2. * Mass * Energy);
  
  TVector3 pParticle = TVector3(pParticleMag,0.,0.);
  pParticle.SetTheta(Theta);
  pParticle.SetPhi(Phi);
  
  if(debug)
    cout<<"MASS_BE11: "<<MASS_BE11<<", BEAM_ENERGY: "<<BEAM_ENERGY<<" sqrt: "<<sqrt( 2. * MASS_BE11 * BEAM_ENERGY)<<", prod: "<<double(MASS_BE11) * double(BEAM_ENERGY)<<endl;
  double pBeamMag = sqrt( 2. * MASS_BE11 * BEAM_ENERGY); //This is all in the z direction
  
  if(debug)
    cout<<"MASS_BE11: "<<MASS_BE11<<", BEAM_ENERGY: "<<BEAM_ENERGY<<", pBeamMag: "<<pBeamMag<<endl;
  
  if(debug)
  {
    cout<<"PARTICLE X: "<<pParticle.X()<<" Y: "<<pParticle.Y()<<" Z: "<<pParticle.Z()<<endl;
  }
  
  double CorrMass = 1.;
  
  if(int(Mass) == int(MASS_BE12))
    CorrMass = MASS_BE8;
  else if(int(Mass) == int(MASS_BE8))
    CorrMass = MASS_BE12;
  else if(int(Mass) == int(MASS_HE4))
    cerr<<"Error in Corr Particle, I can't use a helium, it has to be Be8"<<endl;
  else if(int(Mass) == int(MASS_BE10))
    CorrMass = MASS_BE10;
  else if(int(Mass) == int(MASS_BE11))
    CorrMass = MASS_BE9;
  else if(int(Mass) == int(MASS_BE9))
    CorrMass = MASS_BE11;
  else
    cerr<<"Error in Corr Particle, I don't recognize the mass"<<endl;
  
  double ECorr = BEAM_ENERGY - Energy;
  
  double pCorrMag = pBeamMag*pBeamMag/MASS_BE11 - pParticle.Mag2()/Mass;
  
  TVector3 pCorr;
  pCorr.SetX(-pParticle.X());
  pCorr.SetY(-pParticle.Y());
  pCorr.SetZ(pBeamMag-pParticle.Z());
  
  if(debug)
  {
    cout<<"CORR X: "<<pCorr.X()<<" Y: "<<pCorr.Y()<<" Z: "<<pCorr.Z()<<endl;
    cout<<"Magnitudes    Particle: "<<pParticle.Mag()<<", Beam: "<<pBeamMag<<", Corr: "<<pCorr.Mag()<<", other way: "<<pCorrMag*pCorrMag<<endl;
    cout<<"Energies from Mag  Particle: "<<pParticle.Mag2()/(2*Mass)<<", Beam: "<<pBeamMag*pBeamMag/(2*MASS_BE11)<<", Corr: "<<pCorr.Mag2()/(2*CorrMass)<<endl;
  }
  
  Values[0] = pCorr.Mag2() / (2.*CorrMass )*1000.;  //Energy, from E=p^2/2m
  Values[1] = pCorr.Theta();
  Values[2] = pCorr.Phi();
  
  if(debug)
    cout<<"Energy: "<<Values[0]<<" Theta: "<<Values[1]*180/pi<<" Phi: "<<Values[2]*180/pi<<endl;
  
  if(debug)
    cout<<"Checking energy: "<<"Particle E: "<<Energy<<", Corresponding Energy: "<<Values[0]<<", "<<BEAM_ENERGY<<" MeV - those two: "<<BEAM_ENERGY - (Energy + Values[0])<<endl;
  
  if(debug)
    cout<<endl;
  
  return Values;
  
}

double* CorrParticle(TCSMHit* Hit, int Z)
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
      cerr<<"Unrecognized Z in Corr Particle: "<<Z<<endl;
      MASS = Z;
  }
  
  return CorrParticle(Hit->GetEnergy(),Hit->GetDPosition().Theta(),Hit->GetDPosition().Phi(),MASS);
  
}
