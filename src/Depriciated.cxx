#include "../include/Depriciated.hh"

double* CalcBe8fromAlpha(TCSMHit *A1H,TCSMHit *A2H)
{  
  double *Be8Values = new double[3];
  
  //Make the masses for the 8Be and 4He
  const double mBe8 = 8.0*931.494027 + 4.9416;
  const double mAlpha = 4.0*931.494027 + 2.4249156;
  
  vector<double> PVecAlpha1, PVecAlpha2, pBe;
  //Convert from energy to momentum
  double PAlpha1 = sqrt( 2.0*mAlpha*A1H->GetEnergyMeV() );
  double PAlpha2 = sqrt( 2.0*mAlpha*A2H->GetEnergyMeV() );
  
  //fill the momentum vector for the first alpha
  PVecAlpha1.push_back( PAlpha1*sin( A1H->GetDPosition().Theta() )*cos( A1H->GetDPosition().Phi() ) );
  PVecAlpha1.push_back( PAlpha1*sin( A1H->GetDPosition().Theta() )*sin( A1H->GetDPosition().Phi() ) );
  PVecAlpha1.push_back( PAlpha1*cos( A1H->GetDPosition().Theta() ) );
  
  //fill the momentum vector for the second alpha
  PVecAlpha2.push_back( PAlpha2*sin( A2H->GetDPosition().Theta() )*cos( A1H->GetDPosition().Phi() ) );
  PVecAlpha2.push_back( PAlpha2*sin( A2H->GetDPosition().Theta() )*sin( A1H->GetDPosition().Phi() ) );
  PVecAlpha2.push_back( PAlpha2*cos( A2H->GetDPosition().Theta() ) );
  
  //fill the 8Be vector
  pBe.push_back( ( PVecAlpha1[0]+PVecAlpha2[0] ) );
  pBe.push_back( ( PVecAlpha1[1]+PVecAlpha2[1] ) );
  pBe.push_back( ( PVecAlpha1[2]+PVecAlpha2[2] ) );
  
  //make the 8Be physical parameters, energy, theta, phi
  Be8Values[0] = ( (pBe[0]*pBe[0] + pBe[1]*pBe[1] + pBe[2]*pBe[2]) )/ (2.*mBe8 );  //Energy, from E=p^2/2m
  Be8Values[1] = acos( pBe[2]/ ( sqrt( pBe[0]*pBe[0] + pBe[1]*pBe[1] + pBe[2]*pBe[2] ) ) );  //Theta
  Be8Values[2] = atan2( pBe[1],pBe[0] ); //Phi
  
  return Be8Values;
}

double* CalcBe10fromHe64(TCSMHit *He6Hit,TCSMHit *He4Hit)
{  
  double *Be10Values = new double[3];
  
  //Make the masses for the 8Be and 4He
//   const double mBe8 = 8.0*931.494027 + 4.9416;
  const double mHe4 = 4.0*931.494027 + 2.4249156;
  const double mHe6 = 6.0*931.494027 + 17.5928;
  const double mBe10 = 10.*931.494027 + 12.6074;
  
  const double BreakupQ = -7.409523;
  
  //Convert from energy to momentum
  double PHe6Mag = sqrt( 2.0*mHe6*He6Hit->GetEnergyMeV() );
  double PHe4Mag = sqrt( 2.0*mHe4*He4Hit->GetEnergyMeV() );
  
  TVector3 PHe6 = He6Hit->GetDPosition();
  TVector3 PHe4 = He4Hit->GetDPosition();
  
  PHe6.SetMag(PHe6Mag);
  PHe4.SetMag(PHe4Mag);
  
  TVector3 PBe;
  
  PBe = PHe6 + PHe4;
  
  Be10Values[0] = PBe.Mag2()/(2*mBe10) - BreakupQ;  //Energy, from E=p^2/2m
  Be10Values[1] = PBe.Theta();
  Be10Values[2] = PBe.Phi();
  
  return Be10Values;
}

double GetExciteE_Light(TCSMHit *A1H, TCSMHit *A2H)
{
  
  double *Be8Values = new double[3];
  
  Be8Values = CalcBe8fromAlpha(A1H,A2H);
  
  //11Be(9Be,8Be)12Be*
  const double M1 = MASS_BE11;
  const double M2 = MASS_BE9;
  const double M3 = MASS_BE8;
  const double M4 = MASS_BE12;
  double mQ = M1+M2-M3-M4;
  
  double VelBeam = sqrt(2*BEAM_ENERGY/M1);
  double COMV = ( M1 / ( M1 + M2 ) ) * VelBeam;
  double VelocityM3 = sqrt(2 * (Be8Values[0]) / M3);
  double kPrimeM3 = COMV / VelocityM3;
  
  double COMTotalE = M2 / ( M1 + M2 ) * BEAM_ENERGY;
  double COMEnergyM3 = Be8Values[0] * ( 1 + kPrimeM3*kPrimeM3 - 2*kPrimeM3*cos( Be8Values[1] ) );
  double QVal =  ( COMEnergyM3*( M3 + M4 ) ) / M4 - COMTotalE;
  double ExcitedState = mQ - QVal;
  
  return(ExcitedState);
  
}

// double* CorrParticleFromAlphas(TCSMHit* Hit1, TCSMHit* Hit2)
// {
//   double* be8vals;
//   be8vals = CalcBe8fromAlpha(Hit1,Hit2);
//   return CorrParticle(be8vals[0],be8vals[1],be8vals[2],MASS_BE8);
// }

// double EdgeEffectFactor(int StripX, int StripY, int Detector)
// {
//   TH2D* histo = (TH2D*)edgeFile->Get(Form("PunchThrough%i_D_normalized_adjusted",Detector));
//   int binNo = histo->GetBin(StripX+1,StripY+1);
//   double edgeFact = histo->GetBinContent(binNo);
//   if (edgeFact <=0)
//     return 0.;
//   return 1./edgeFact;
// }
// 
// double EdgeEffectFactor(TCSMHit* hit)
// {
//   return(EdgeEffectFactor(hit->GetDVerticalStrip(),hit->GetDHorizontalStrip(),hit->GetDetectorNumber()));
// }
