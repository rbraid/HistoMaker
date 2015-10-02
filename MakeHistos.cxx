// g++ MakeHistos.cxx -Wl,--no-as-needed -o RunMe `grsi-config --cflags --all-libs --root`

#define DEBUG 0

#include <cstdio>
#include <fstream>

#include <TObject.h>

#include <TFile.h>
#include <TChain.h>
#include <TList.h>
#include <TCut.h>
#include <TCutG.h>
#include <TH3.h>
#include <TH2.h>
#include <TH1.h>
#include <TStopwatch.h>
#include <TKey.h>
#include <TTree.h>

#include <TApplication.h>
#include "TNucleus.h"
#include "TKinematics.h"
#include "Globals.h"
#include "TTigress.h"
#include "TCSM.h"
#include "GlobalSettings.hh"

TTigress *tigress =  new TTigress;
TCSM *csm =  new TCSM;
TList *cutlist = new TList;

double Doppler(TTigressHit* thit, TCSMHit* chit);
double* CorrParticle(double E, double Theta, double Phi, double Mass);
double* CorrParticle(TCSMHit*);

void SetupHistos(TList *outlist)
{
  if(DEBUG) cout<<"Setting up Histos"<<endl;
  TH1D *temp1 = 0;
  TH2D *temp2 = 0;
  TH2I *temp2INT = 0;
  TH3D *temp3 = 0;

  for(int id = 1; id<=2;id++)
  {

    for(int strip = 0; strip<16; strip++)
    {
      outlist->Add(new TH2D(Form("stripPID_det%i_strip%02i",id,strip),Form("Detector %i PID strip %i",id,strip),3200,0,40,800,0,800));
      temp2 = (TH2D*)outlist->FindObject(Form("stripPID_det%i_strip%02i",id,strip));
      temp2->GetXaxis()->SetTitle("Total Energy deposited in MeV");
      temp2->GetYaxis()->SetTitle("dE/dX in MeV/um");

      outlist->Add(new TH2D(Form("EvTheta_%i_BE_strip%02i",id,strip),Form("EvTheta %i, cut on Be Strip",id,strip),100,0,100,700,0,70));
      temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE_strip%02i",id,strip));
      temp2->GetXaxis()->SetTitle("Theta in Degrees");
      temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    }

    for(int theta=10;theta<55;theta++)
    {
      outlist->Add(new TH1D(Form("BeEx%i_theta%02i",id,theta),Form("Be-12 Excitation Energy Theta %i",id,theta),500,-10,25));
      temp1 = (TH1D*)outlist->FindObject(Form("BeEx%i_theta%02i",id,theta));
      temp1->GetXaxis()->SetTitle("Energy in MeV");
      temp1->GetYaxis()->SetTitle("Counts");

      outlist->Add(new TH1D(Form("BeEx%i_theta%02i_corr",id,theta),Form("Be-12 Excitation Energy Theta %i energy corrected",id,theta),500,-10,25));
      temp1 = (TH1D*)outlist->FindObject(Form("BeEx%i_theta%02i_corr",id,theta));
      temp1->GetXaxis()->SetTitle("Energy in MeV");
      temp1->GetYaxis()->SetTitle("Counts");
    }

    outlist->Add(new TH1D(Form("GammaCut_%i",id),Form("Gamma spectrum cut on Ex Spectrum",id),3000,0,30));
    temp1 = (TH1D*)outlist->FindObject(Form("GammaCut_%i",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");

    outlist->Add(new TH1D(Form("GammaCut_dopp_%i",id),Form("Gamma spectrum cut on Ex Spectrum, doppler corrected",id),3000,0,30));
    temp1 = (TH1D*)outlist->FindObject(Form("GammaCut_dopp_%i",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");
    
    outlist->Add(new TH1D(Form("BeEx%i",id),Form("Be-12 Excitation Energy",id),350,-10,25));
      temp1 = (TH1D*)outlist->FindObject(Form("BeEx%i",id));
      temp1->GetXaxis()->SetTitle("Energy in MeV");
      temp1->GetYaxis()->SetTitle("Counts");

    outlist->Add(new TH1D(Form("BeEx%i_corr",id),Form("Be-12 Excitation Energy, with straggling correction",id),350,-10,25));
      temp1 = (TH1D*)outlist->FindObject(Form("BeEx%i_corr",id));
      temp1->GetXaxis()->SetTitle("Energy in MeV");
      temp1->GetYaxis()->SetTitle("Counts");
      
    outlist->Add(new TH2D(Form("pid_%i",id),Form("Particle ID, detector %i",id),700,0,70,700,0,70));//
      temp2 = (TH2D*)outlist->FindObject(Form("pid_%i",id));
      temp2->GetXaxis()->SetTitle("E Energy deposited in MeV");
      temp2->GetYaxis()->SetTitle("dE Energy deposited in MeV");

    outlist->Add(new TH2D(Form("pid_%i_thickness",id),Form("Particle ID, detector %i, with thickness correction",id),3200,0,160,800,0,2000));//
      temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_thickness",id));
      temp2->GetXaxis()->SetTitle("E Energy deposited in MeV");
      temp2->GetYaxis()->SetTitle("dE/dX in MeV/um");

    outlist->Add(new TH2D(Form("pid_%i_fvb",id),Form("Particle ID, detector %i, cut on 1:1",id),700,0,70,700,0,70));//
      temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_fvb",id));
      temp2->GetXaxis()->SetTitle("E Energy deposited in MeV");
      temp2->GetYaxis()->SetTitle("dE Energy deposited in MeV");
    
    outlist->Add(new TH2D(Form("pid_%i_summed",id),Form("Particle ID, detector %i, summed",id),400,0,40,250,0,25));//
      temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_summed",id));
      temp2->GetXaxis()->SetTitle("Total Energy deposited in MeV");
      temp2->GetYaxis()->SetTitle("dE Energy deposited in MeV");

    outlist->Add(new TH2D(Form("pid_%i_summed_thickness",id),Form("Particle ID, detector %i, summed, with thickness correction",id),3200,0,160,800,0,2000));//
      temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_summed_thickness",id));
      temp2->GetXaxis()->SetTitle("Total Energy deposited in MeV");
      temp2->GetYaxis()->SetTitle("dE/dX in MeV/um");
      
    outlist->Add(new TH2D(Form("EvTheta_%iTotal",id),Form("EvTheta %i",id),400,0,100,1400,0,70));
      temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%iTotal",id));
      temp2->GetXaxis()->SetTitle("Theta in Degrees");
      temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");

    outlist->Add(new TH2D(Form("EvTheta_%i_BE8",id),Form("EvTheta %i of the reconstructed 8Be",id),400,0,100,1400,0,70));
      temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE8",id));
      temp2->GetXaxis()->SetTitle("Theta in Degrees");
      temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
      
    outlist->Add(new TH2D(Form("EvTheta_%i_BE",id),Form("EvTheta %i, cut on Be",id),100,0,100,700,0,70));
      temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE",id));
      temp2->GetXaxis()->SetTitle("Theta in Degrees");
      temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");

      outlist->Add(new TH2D(Form("EvTheta_%i_BE_correlated",id),Form("EvTheta %i, cut on Be",id),360,0,360,700,0,70));
      temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE_correlated",id));
      temp2->GetXaxis()->SetTitle("Theta in Degrees");
      temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");

    outlist->Add(new TH2D(Form("EvTheta_%i_BE_exCut_gs",id),Form("EvTheta %i, cut on Be12 GS",id),100,0,100,700,0,70));
      temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE_exCut_gs",id));
      temp2->GetXaxis()->SetTitle("Theta in Degrees");
      temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");

    outlist->Add(new TH2D(Form("EvTheta_%i_BE_exCut_1e",id),Form("EvTheta %i, cut on Be 2.24",id),100,0,100,700,0,70));
      temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE_exCut_1e",id));
      temp2->GetXaxis()->SetTitle("Theta in Degrees");
      temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");

    outlist->Add(new TH2D(Form("EvTheta_%i_BE_exCut_sc",id),Form("EvTheta %i, cut on Be <5",id),100,0,100,700,0,70));
      temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE_exCut_sc",id));
      temp2->GetXaxis()->SetTitle("Theta in Degrees");
      temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    
    outlist->Add(new TH2D(Form("CheckCalE_%i",id),Form("Front Energy vs Back Energy, Detector %i E",id),600,0,60,600,0,60));
      temp2 = (TH2D*)outlist->FindObject(Form("CheckCalE_%i",id));
      temp2->GetXaxis()->SetTitle("Energy deposited in Vertical (Front)");
      temp2->GetYaxis()->SetTitle("Energy deposited in Horizontal (Back)");

    outlist->Add(new TH2D(Form("twohitEVT_%i_side",id),Form("Corresponding hit from TwoHit in detector %i",id),400,0,100,1400,0,70));
      temp2 = (TH2D*)outlist->FindObject(Form("twohitEVT_%i_side",id));
      temp2->GetXaxis()->SetTitle("Theta in Degrees");
      temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");

    outlist->Add(new TH2D(Form("twohitEVT_%i_telescope",id),Form("Corresponding hit from TwoHit in detector %i",id),400,0,100,1400,0,70));
      temp2 = (TH2D*)outlist->FindObject(Form("twohitEVT_%i_telescope",id));
      temp2->GetXaxis()->SetTitle("Theta in Degrees");
      temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
      
    for(int mid = 1; mid<=4;mid++)
    {
      outlist->Add(new TH2D(Form("pid_%i_mult%i",id,mid),Form("Particle ID, detector %i with Multiplicity %i",id,mid),700,0,70,700,0,70));
      temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_mult%i",id,mid));
      temp2->GetXaxis()->SetTitle("E Energy deposited in MeV");
      temp2->GetYaxis()->SetTitle("dE Energy deposited in MeV");
      
      outlist->Add(new TH2D(Form("EvTheta_%iTotal_mult%i",id,mid),Form("E vs Theta Detector %i with Multiplicity %i",id,mid),100,0,100,700,0,70));
      temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%iTotal_mult%i",id,mid));
	temp2->GetXaxis()->SetTitle("Theta in Degrees");
	temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    }
  }

  for(int det=1;det<=4;det++)
  {

    outlist->Add(new TH2D(Form("Alphacone_%i",det),Form("Alpha cone in detector %i",det),200,0,20,200,0,20));//
    temp2 = (TH2D*)outlist->FindObject(Form("Alphacone_%i",det));
    temp2->SetContour(666);
    temp2->SetOption("colz");
    temp2->GetXaxis()->SetTitle("Energy deposited in MeV");
    temp2->GetYaxis()->SetTitle("Energy deposited in MeV");

    outlist->Add(new TH1D(Form("AlphaEx%i",det),Form("AlphaEx Excitation Energy",det),350,-10,25));
    temp1 = (TH1D*)outlist->FindObject(Form("AlphaEx%i",det));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");

    outlist->Add(new TH2D(Form("twohit_%i",det),Form("2 Hits in detector %i",det),200,0,20,200,0,20));//
    temp2 = (TH2D*)outlist->FindObject(Form("twohit_%i",det));
    temp2->SetContour(666);
    temp2->SetOption("colz");
    temp2->GetXaxis()->SetTitle("Energy deposited in MeV");
    temp2->GetYaxis()->SetTitle("Energy deposited in MeV");

    outlist->Add(new TH2D(Form("EvTheta_%i_HE",det),Form("EvTheta %i, cut on He",det),100,0,100,700,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_HE",det));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    
    outlist->Add(new TH2D(Form("CheckCalD_%i",det),Form("Front Energy vs Back Energy, Detector %i D",det),600,0,60,600,0,60));
    temp2 = (TH2D*)outlist->FindObject(Form("CheckCalD_%i",det));
    temp2->GetXaxis()->SetTitle("Energy deposited in Vertical (Back)");
    temp2->GetYaxis()->SetTitle("Energy deposited in Horizontal (Front)");
    for(char type='D';type<='E';type++) //Wow I can't believe this works.  I am glad they are in alphabetical order
    {
      if(det>2 && type=='E')//This skips 3 and 4 E, which don't exist
	continue;

      outlist->Add(new TH2D(Form("EvTheta_%i%c",det,type),Form("EvTheta, Detector %i, Position %c",det,type),100,0,100,700,0,70));//
      temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i%c",det,type));
      temp2->GetXaxis()->SetTitle("Theta in Degrees");
      temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
      
      outlist->Add(new TH2I(Form("CSM_HitPattern_%i%c",det,type),Form("HitPattern, Detector %i, Position %c",det,type),16,0,16,16,0,16));//
      temp2INT = (TH2I*)outlist->FindObject(Form("CSM_HitPattern_%i%c",det,type));
      temp2INT->GetXaxis()->SetTitle("Vertical Strip Number");
      temp2INT->GetYaxis()->SetTitle("Horizontal Strip Number");
    }
  }


  outlist->Add(new TH3D("positions","positions",200,0,100,120,-30,30,400,-100,100));
  outlist->Add(new TH2D("positions_proj","positions_proj",200,0,100,400,-100,100));
  temp2 = (TH2D*)outlist->FindObject("positions_proj");
    temp2->SetOption("colz");
    temp2->GetXaxis()->SetTitle("Z");
    temp2->GetYaxis()->SetTitle("X");
  
  outlist->Add(new TH2D("CSM_HP_Theta_Phi","Angular Coverage Map",45,0,90,90,-180,180));
    temp2 = (TH2D*)outlist->FindObject("CSM_HP_Theta_Phi");
    temp2->GetXaxis()->SetTitle("Theta (Degrees)");
    temp2->GetYaxis()->SetTitle("Phi (Degrees)");

  outlist->Add(new TH1D("Multiplicity","Multiplicity",10,0,10));//

  outlist->Add(new TH1D("GammaSum","Gamma Spectrum",2000,0,200));
    temp1 = (TH1D*)outlist->FindObject("GammaSum");
    temp1->GetXaxis()->SetTitle("Energy of Gamma in MeV");
    temp1->GetYaxis()->SetTitle("Counts per .1 MeV bin");

  outlist->Add(new TH2D("EnergyCheck","Energy for each strip",240,0,240,7000,0,70));//
    temp2 = (TH2D*)outlist->FindObject("EnergyCheck");
    temp2->GetXaxis()->SetTitle("Strip Number");
    temp2->GetYaxis()->SetTitle("Energy");

  outlist->Add(new TH2D("ChargeCheck","Charge for each strip",240,0,240,3000,0,6000));//
    temp2 = (TH2D*)outlist->FindObject("ChargeCheck");
    temp2->GetXaxis()->SetTitle("Strip Number");
    temp2->GetYaxis()->SetTitle("Charge");

  outlist->Add(new TH1I("StatCheck","Checking Cases",20,0,20));
    
  if(DEBUG)
  {
    cout<<"Histos Set"<<endl;
  }
}

double GetExciteE_Heavy(double be12E, double be12T)
{
//   cout<<"BeamE: "<<BeamE<<endl;
//   cout<<"BeE: "<<be12E<<endl;
//   cout<<"BeT: "<<be12T<<endl;
  be12E=be12E/1000.;
  const double pi = TMath::Pi();

  const double M1 = MASS_BE11;
  const double M2 = MASS_BE9;
  const double M3 = MASS_BE8;
  const double M4 = MASS_BE12;
  double mQ = M1+M2-M3-M4;
  
  double V1 = sqrt(2*BEAM_ENERGY/M1);
  double COMV = ( M1 / ( M1 + M2 ) ) * V1;
  double V4 = sqrt(2*be12E/M4);
  double kPrimeM4 = COMV / V4;
  
  double COMTotalE = M2 / ( M1 + M2 ) * BEAM_ENERGY;
  double COMEnergyM4 = be12E * ( 1 + kPrimeM4*kPrimeM4 - 2*kPrimeM4*cos( be12T ) );
  double QVal =  ( COMEnergyM4*( M3 + M4 ) ) / M3 - COMTotalE;
  double ExcitedState = mQ - QVal;

//   cout<<"EX: "<<ExcitedState<<endl<<endl;
  
  return(ExcitedState);
  
}

double GetExciteE_Heavy(TCSMHit* Hit)
{
  return(GetExciteE_Heavy(Hit->GetEnergy(),Hit->GetDPosition().Theta()));
}

double* CalcBe8fromAlpha(TCSMHit *A1H,TCSMHit *A2H)
{
  const double pi=TMath::Pi();
  
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
  //Be8Values[2] = atan( pBe[1]/pBe[0] ); //Phi
  
  if( pBe[0] > 0.0 && pBe[1] > 0.0 )
    Be8Values[2] = atan( pBe[1] / pBe[0] );
  else if( pBe[0] < 0.0 && pBe[1] > 0.0 )
    Be8Values[2] = atan( pBe[1] / pBe[0] ) + pi;
  else if( pBe[0] < 0.0 && pBe[1] < 0.0 )
    Be8Values[2] = atan( pBe[1] / pBe[0] ) + pi;
  else if( pBe[0] > 0.0 && pBe[1] < 0.0 )
    Be8Values[2] = atan( pBe[1] / pBe[0] ) + 2*pi;
  
  return Be8Values;
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

void ProcessChain(TChain *chain,TList *outlist)//, MakeFriend *myFriend)
{
  int nentries = chain->GetEntries();
//   cout<<DRED;
//   chain->Print();
//   cout<<RESET_COLOR;
  TStopwatch w;
  w.Start();

//   ofstream ofile;
//   ofile.open("interesting.h");
//   ofile<<"bool IsInteresting["<<nentries<<"] = {";
//   

  if(DEBUG)
  {
    cout<<"Starting ProcessChain"<<endl;
  }

  for(int x=0; x<nentries; x++)
  {
    chain->GetEntry(x);

    bool IsInteresting = 0;
    
    if(DEBUG)
    {
      cout<<"nentries: "<<nentries<<endl;
    }

    if(csm->GetMultiplicity()==0)
      continue;

    ((TH1D *)outlist->FindObject("Multiplicity"))->Fill(csm->GetMultiplicity());

    TCSMHit *Be11Hit;
    TCSMHit *Alpha1Hit;
    TCSMHit *Alpha2Hit;
    TCSMHit *Be12Hit;

    bool Be11Flag = 0;
    bool Alpha1Flag = 0;
    bool Alpha2Flag = 0;
    bool Be12Flag = 0;

    if(csm->GetMultiplicity()>=2)
    {

      int hits[4] = {0};
      int Ahits[4] = {0};
      int Bhits[4] = {0};

      for(int y=0; y<csm->GetMultiplicity(); y++)
      {
	if(DEBUG)
	{
	  cout<<"Be's Get Multiplicity()"<<endl;
	}

	if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form("thickness_alpha_%i_1",csm->GetHit(y)->GetDetectorNumber()))))
	{
	  if(cut->IsInside(csm->GetHit(y)->GetEnergyMeV(),csm->GetHit(y)->GetDdE_dx()) && csm->GetHit(y)->GetEEnergy() > 10)
	  {
	    if(!Alpha1Flag)
	    {
	      csm->GetHit(y)->SetIsotope(4,"He");
	      Alpha1Hit = csm->GetHit(y);
	      Alpha1Flag = 1;
	      Ahits[csm->GetHit(y)->GetDetectorNumber()-1]++;
	    }
	    else if(!Alpha2Flag)
	    {
	      csm->GetHit(y)->SetIsotope(4,"He");
	      Alpha2Hit = csm->GetHit(y);
	      Alpha2Flag = 1;
	      Ahits[csm->GetHit(y)->GetDetectorNumber()-1]++;
	    }
	    else
	    {
	      cout<<"Too Many Alphas!"<<endl;
	      //IsInteresting = 1;
	    }
	  }
	}
	//if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form("theta_Be_%i_%02i",hit->GetDetectorNumber(),int(hit->GetThetaDeg())))))
	//{
	//  if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()))
	 // {
	  //}
	//}
	if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form("Be12_thick_%i_v1",csm->GetHit(y)->GetDetectorNumber()))))
	{
	  if(cut->IsInside(csm->GetHit(y)->GetEnergyMeV(),csm->GetHit(y)->GetDdE_dx()) && csm->GetHit(y)->GetEEnergy() > 10)
	  {
	    if(!Be12Flag)
	    {
	      csm->GetHit(y)->SetIsotope(12,"Be");
	      Be12Hit = csm->GetHit(y);
	      Be12Flag = 1;
	      Bhits[csm->GetHit(y)->GetDetectorNumber()-1]++;
	    }
	    else
	    {
	      cout<<"Too Many Be!"<<endl;
	      //IsInteresting = 1;
	    }
	  }
	}

	hits[csm->GetHit(y)->GetDetectorNumber()-1]++;
      }

      for(int det=0;det<4;det++)
      {
	int corrdet;
	if(det==0)
	  corrdet=1;
	else if(det==1)
	  corrdet=0;
	else if(det==2)
	  corrdet=1;
	else if(det==3)
	  corrdet=0;
	else
	  corrdet=-1;
	
	if(hits[det]>=2)
	{
	  int loc1 = -1;
	  int loc2 = -1;
	  int corrloc = -1;
	  
	  for(int search=0; search<csm->GetMultiplicity();search++)
	  {
	    if(csm->GetHit(search)->GetDetectorNumber() == det+1)
	    {
	      if(loc1==-1)
		loc1=search;
	      else if(loc2==-1)
		loc2=search;
	      else
		cerr<<"Too many hits in one detector"<<endl;
	    }
	    if(csm->GetHit(search)->GetDetectorNumber() == corrdet+1)
	    {
	      if(corrloc == -1)
		corrloc = search;
	      else
		cerr<<"Too many correlated location hits"<<endl;
	    }
	  }
	  if(loc2!=-2)
	  {
	    TRandom *rnd = new TRandom(x);
	    if(rnd->Uniform(1)>.5)
	    {
	      int temp;
	      temp = loc1;
	      loc1 = loc2;
	      loc2 = temp;
	    }
	    TH2D* conepointer = (TH2D*)outlist->FindObject(Form("twohit_%i",det+1));
	    conepointer->Fill(csm->GetHit(loc1)->GetEnergyMeV(),csm->GetHit(loc2)->GetEnergyMeV());
	  }
	  if(corrloc != -1)
	  {
	    TH2D* evtpointer;
	    if(det<2)
	      evtpointer = (TH2D*)outlist->FindObject(Form("twohitEVT_%i_telescope",det+1));
	    else
	      evtpointer = (TH2D*)outlist->FindObject(Form("twohitEVT_%i_side",det-1));
	    
	    evtpointer->Fill(csm->GetHit(corrloc)->GetThetaDeg(),csm->GetHit(corrloc)->GetEnergyMeV());
	  }
	}
      }

       int AlphaSum = Ahits[0]+Ahits[1]+Ahits[2]+Ahits[3];
       int BeSum = Bhits[0]+Bhits[1]+Bhits[2]+Bhits[3];
//       if(BeSum >=1)
//       {
// 	cout<<"Alpha Sum: "<<AlphaSum<<endl;
// 	cout<<"Be12 Sum: "<<BeSum<<endl;
//       }

      double be8thetawindow = 15./180*TMath::Pi();//Simulations indicate the max spread of the Be8 should be 15 degrees.

      if(Ahits[0]==2&&hits[1]==1)
      {
	((TH1I *)outlist->FindObject("StatCheck"))->Fill(2);
	
	for(int iter=0; iter<csm->GetMultiplicity(); iter++)
	{
	  if(csm->GetHit(iter)->GetDetectorNumber()==1+1)
	  {
	    csm->GetHit(iter)->SetIsotope(12,"Be");
	    Be12Hit = csm->GetHit(iter);
	    Be12Flag = 1;
	    break;
	  } 
	}
      }
      else if(Ahits[1]==2&&hits[0]==1)
      {
	((TH1I *)outlist->FindObject("StatCheck"))->Fill(3);
	
	for(int iter=0; iter<csm->GetMultiplicity(); iter++)
	{
	  if(csm->GetHit(iter)->GetDetectorNumber()==0+1)
	  {
	    csm->GetHit(iter)->SetIsotope(12,"Be");
	    Be12Hit = csm->GetHit(iter);
	    Be12Flag = 1;
	    break;
	  }
	}
      }
      else if(Bhits[0]==1&&hits[1]==2)
      {
	((TH1I *)outlist->FindObject("StatCheck"))->Fill(4);
	int loc1=-1;
	int loc2=-1;
	for(int iter=0; iter<csm->GetMultiplicity(); iter++)
	{
	  if(csm->GetHit(iter)->GetDetectorNumber()==1+1)
	  {
	    if(loc1==-1)
	      loc1=iter;
	    else if(loc2==-1)
	      loc2=iter;
	    else
	      cerr<<"Too many alphas!"<<endl;
	  }
	}
	if(abs(csm->GetHit(loc1)->GetDPosition().Theta()-csm->GetHit(loc2)->GetDPosition().Theta()) < be8thetawindow)
	{
	  csm->GetHit(loc1)->SetIsotope(4,"He");
	  Alpha1Hit = csm->GetHit(loc1);
	  Alpha1Flag = 1;
	  csm->GetHit(loc2)->SetIsotope(4,"He");
	  Alpha2Hit = csm->GetHit(loc2);
	  Alpha2Flag = 1;
	}
      }
      else if(Bhits[1]==1&&hits[0]==2)
      {
	((TH1I *)outlist->FindObject("StatCheck"))->Fill(5);
	int loc1=-1;
	int loc2=-1;
	for(int iter=0; iter<csm->GetMultiplicity(); iter++)
	{
	  if(csm->GetHit(iter)->GetDetectorNumber()==0+1)
	  {
	    if(loc1==-1)
	      loc1=iter;
	    else if(loc2==-1)
	      loc2=iter;
	    else
	      cerr<<"Too many alphas!"<<endl;
	  }
	}
	if(abs(csm->GetHit(loc1)->GetDPosition().Theta()-csm->GetHit(loc2)->GetDPosition().Theta()) < be8thetawindow)
	{
	  csm->GetHit(loc1)->SetIsotope(4,"He");
	  Alpha1Hit = csm->GetHit(loc1);
	  Alpha1Flag = 1;
	  csm->GetHit(loc2)->SetIsotope(4,"He");
	  Alpha2Hit = csm->GetHit(loc2);
	  Alpha2Flag = 1;
	}
      }
      else if(Ahits[0]==1&&hits[0]==2)
      {
	((TH1I *)outlist->FindObject("StatCheck"))->Fill(6);
	
	for(int iter=0; iter<csm->GetMultiplicity(); iter++)
	{
	  if(csm->GetHit(iter)->GetDetectorNumber()==0+1)
	  {
	    if(csm->GetHit(iter)->GetIsotope()=="default")
	    {
	      csm->GetHit(iter)->SetIsotope(4,"He");
	      Alpha2Hit = csm->GetHit(iter);
	      Alpha2Flag = 1;
	    }
	  }
	}
      }
      else if(Ahits[1]==1&&hits[1]==2)
      {
	((TH1I *)outlist->FindObject("StatCheck"))->Fill(7);
	
	for(int iter=0; iter<csm->GetMultiplicity(); iter++)
	{
	  if(csm->GetHit(iter)->GetDetectorNumber()==1+1)
	  {
	    if(csm->GetHit(iter)->GetIsotope()=="default")
	    {
	      csm->GetHit(iter)->SetIsotope(4,"He");
	      Alpha2Hit = csm->GetHit(iter);
	      Alpha2Flag = 1;
	    }
	  }
	}
      }
      else if(hits[0]==1&&hits[1]==2&&Bhits[1]==0&&Ahits[0]==0)
      {
	((TH1I *)outlist->FindObject("StatCheck"))->Fill(8);
	
	for(int y=0; y<csm->GetMultiplicity(); y++)
	{
	  if(csm->GetHit(y)->GetDetectorNumber()==1+1 && csm->GetHit(y)->GetEnergy()>1 )
	  {
	    if(!Alpha1Flag)
	    {
	      csm->GetHit(y)->SetIsotope(4,"He");
	      Alpha1Hit = csm->GetHit(y);
	      Alpha1Flag = 1;
	    }
	    else if(!Alpha2Flag)
	    {
	      csm->GetHit(y)->SetIsotope(4,"He");
	      Alpha2Hit = csm->GetHit(y);
	      Alpha2Flag = 1;
	    }
	    else
	      cerr<<" Too many alphas"<<endl;
	  }
	  else if(csm->GetHit(y)->GetDetectorNumber()==0+1 && csm->GetHit(y)->GetEnergy()>1)
	  {
	    if(!Be12Flag)
	    {
	      csm->GetHit(y)->SetIsotope("12Be");
	      Be12Hit = csm->GetHit(y);
	      Be12Flag = 1;
	    }
	    else
	    {
	      cerr<<" Too many Be12"<<endl;
	    }
	  }
	}
      }
      else if(hits[0]==2&&hits[1]==1&&Bhits[0]==0&&Ahits[1]==0)
      {
	((TH1I *)outlist->FindObject("StatCheck"))->Fill(9);
	
	for(int y=0; y<csm->GetMultiplicity(); y++)
	{
	  if(csm->GetHit(y)->GetDetectorNumber()==0+1 && csm->GetHit(y)->GetEnergy()>1 )
	  {
	    if(!Alpha1Flag)
	    {
	      csm->GetHit(y)->SetIsotope(4,"He");
	      Alpha1Hit = csm->GetHit(y);
	      Alpha1Flag = 1;
	    }
	    else if(!Alpha2Flag)
	    {
	      csm->GetHit(y)->SetIsotope(4,"He");
	      Alpha2Hit = csm->GetHit(y);
	      Alpha2Flag = 1;
	    }
	    else
	      cerr<<" Too many alphas"<<endl;
	  }
	  else if(csm->GetHit(y)->GetDetectorNumber()==1+1 && csm->GetHit(y)->GetEnergy()>1)
	  {
	    if(!Be12Flag)
	    {
	      csm->GetHit(y)->SetIsotope("12Be");
	      Be12Hit = csm->GetHit(y);
	      Be12Flag = 1;
	    }
	    else
	    {
	      cerr<<" Too many Be12"<<endl;
	    }
	  }
	}
      }
      else if(hits[0]==1&&hits[3]==2&&Ahits[0]==0)
      {
	((TH1I *)outlist->FindObject("StatCheck"))->Fill(10);
	
	for(int y=0; y<csm->GetMultiplicity(); y++)
	{
	  if(csm->GetHit(y)->GetDetectorNumber()==3+1 && csm->GetHit(y)->GetEnergy()>1 )
	  {
	    if(!Alpha1Flag)
	    {
	      csm->GetHit(y)->SetIsotope(4,"He");
	      Alpha1Hit = csm->GetHit(y);
	      Alpha1Flag = 1;
	    }
	    else if(!Alpha2Flag)
	    {
	      csm->GetHit(y)->SetIsotope(4,"He");
	      Alpha2Hit = csm->GetHit(y);
	      Alpha2Flag = 1;
	    }
	    else
	      cerr<<" Too many alphas"<<endl;
	  }
	}
      }
      else if(hits[1]==1&&hits[2]==2&&Ahits[1]==0)
      {
	((TH1I *)outlist->FindObject("StatCheck"))->Fill(11);
	
	for(int y=0; y<csm->GetMultiplicity(); y++)
	{
	  if(csm->GetHit(y)->GetDetectorNumber()==2+1 && csm->GetHit(y)->GetEnergy()>1 )
	  {
	    if(!Alpha1Flag)
	    {
	      csm->GetHit(y)->SetIsotope(4,"He");
	      Alpha1Hit = csm->GetHit(y);
	      Alpha1Flag = 1;
	    }
	    else if(!Alpha2Flag)
	    {
	      csm->GetHit(y)->SetIsotope(4,"He");
	      Alpha2Hit = csm->GetHit(y);
	      Alpha2Flag = 1;
	    }
	    else
	      cerr<<" Too many alphas"<<endl;
	  }
	}
      }

//     if(IsInteresting)
//       if(TCutG *cut = (TCutG*)(cutlist->FindObject("alphaCone_Interesting")))
//       {
// 	if(Alpha2Flag)
// 	{
// 	  if(cut->IsInside(Alpha1Hit->GetEnergyMeV(),Alpha2Hit->GetEnergyMeV()))
// 	  {
// 	    for(int iter=0;iter<csm->GetMultiplicity();iter++)
// 	    {
// 	      csm->GetHit(iter)->Print();
// 	    }
// 	    cout<<GREEN<<"*******"<<RESET_COLOR<<endl;
// 	  }
// 	}
//       }	
    }

    if(Alpha2Flag && Alpha1Flag)//This randomizes who is alpha1 and who is alpha 2
    {
      TRandom *rnd = new TRandom(x);
      if(rnd->Uniform(1)>.5)
      {
	TCSMHit *temp;
	temp = Alpha1Hit;
	Alpha1Hit = Alpha2Hit;
	Alpha2Hit = temp;
      }
    }

    if(Alpha1Flag && !Alpha2Flag)
    {
      TH2D* evtpointer = (TH2D*)outlist->FindObject(Form("EvTheta_%i_HE",Alpha1Hit->GetDetectorNumber()));
      evtpointer->Fill(Alpha1Hit->GetThetaDeg(),Alpha1Hit->GetEnergyMeV());
    }

    if(Alpha2Flag && Alpha1Flag)
    {
      TH2D* evtpointer = (TH2D*)outlist->FindObject(Form("EvTheta_%i_HE",Alpha1Hit->GetDetectorNumber()));
      evtpointer->Fill(Alpha1Hit->GetThetaDeg(),Alpha1Hit->GetEnergyMeV());
      TH2D* evtpointer2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_HE",Alpha2Hit->GetDetectorNumber()));
      evtpointer2->Fill(Alpha2Hit->GetThetaDeg(),Alpha2Hit->GetEnergyMeV());

      if(Alpha1Hit->GetDetectorNumber() == Alpha2Hit->GetDetectorNumber())
      {
	TH2D* alphaconepointer = (TH2D*)outlist->FindObject(Form("Alphacone_%i",Alpha1Hit->GetDetectorNumber()));
	alphaconepointer->Fill(Alpha1Hit->GetEnergyMeV(),Alpha2Hit->GetEnergyMeV());
	
	if(Alpha1Hit->GetDetectorNumber()<3)
	{
	  if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form("alphacone_%i_1",Alpha1Hit->GetDetectorNumber()))))
	  {
	    if(cut->IsInside(Alpha1Hit->GetEnergyMeV(),Alpha2Hit->GetEnergyMeV()))
	    {
	      double* Be8 = CalcBe8fromAlpha(Alpha1Hit, Alpha2Hit);
	      TH2D* be8pointer = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE8",Alpha1Hit->GetDetectorNumber()));
	      be8pointer->Fill(Be8[1]*180/3.14159,Be8[0]);
	      TH1D* alphaEXpointer = (TH1D*)outlist->FindObject(Form("AlphaEx%i",Alpha1Hit->GetDetectorNumber()));
	      alphaEXpointer->Fill(GetExciteE_Light(Alpha1Hit,Alpha2Hit));
	    }
	  }
	}
	else
	{
	  double* Be8 = CalcBe8fromAlpha(Alpha1Hit, Alpha2Hit);
	  
	  int sidetostack=-1;
	  if(Alpha1Hit->GetDetectorNumber()==3)
	    sidetostack=2;
	  else if(Alpha1Hit->GetDetectorNumber()==4)
	    sidetostack=1;
	  
	  TH2D* be8pointer = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE8",sidetostack));
	  be8pointer->Fill(Be8[1]*180/3.14159,Be8[0]);
	  TH1D* alphaEXpointer = (TH1D*)outlist->FindObject(Form("AlphaEx%i",Alpha1Hit->GetDetectorNumber()));
	  alphaEXpointer->Fill(GetExciteE_Light(Alpha1Hit,Alpha2Hit));
	}
      }
    }

    if(Be12Flag)
    {
      TH1D *temp1 = 0;
      TH2D *temp2 = 0;

      double excite = GetExciteE_Heavy(Be12Hit);
      temp1 = (TH1D*)outlist->FindObject(Form("BeEx%i",Be12Hit->GetDetectorNumber()));
      if(temp1) temp1->Fill(excite);
      temp1 = (TH1D*)outlist->FindObject(Form("BeEx%i_theta%02i",Be12Hit->GetDetectorNumber(),int(Be12Hit->GetThetaDeg())));
      if(temp1) temp1->Fill(excite);
      if(excite<3.2 && excite>1.9)
      {
	for(int y=0; y<tigress->GetAddBackMultiplicity();y++)
	{
	  TTigressHit *tigresshit = tigress->GetAddBackHit(y);

	  if(tigresshit->GetCore()->GetEnergy()>10)
	  {
	    temp1 = (TH1D*)outlist->FindObject(Form("GammaCut_%i",Be12Hit->GetDetectorNumber()));
	    temp1->Fill(tigresshit->GetCore()->GetEnergy()/1000.);

	    temp1 = (TH1D*)outlist->FindObject(Form("GammaCut_dopp_%i",Be12Hit->GetDetectorNumber()));
	    temp1->Fill(Doppler(tigresshit, Be12Hit));
	  }
	}
      }

      temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE_correlated",Be12Hit->GetDetectorNumber()));
      double *tmpValues = CorrParticle(Be12Hit);
      temp2->Fill(tmpValues[1]*180/3.14159,tmpValues[0]/1000.);

      temp1 = (TH1D*)outlist->FindObject(Form("BeEx%i_theta%02i_corr",Be12Hit->GetDetectorNumber(),int(Be12Hit->GetThetaDeg())));
      double excite2 = GetExciteE_Heavy(Be12Hit->GetCorrectedEnergy(),Be12Hit->GetDPosition().Theta());
      if(temp1) temp1->Fill(excite2);
      temp1 = (TH1D*)outlist->FindObject(Form("BeEx%i_corr",Be12Hit->GetDetectorNumber()));
      if(temp1) temp1->Fill(excite2);

      if(excite<3.8 && excite>1.8)
      {
	temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE_exCut_1e",Be12Hit->GetDetectorNumber()));
	if(temp2) temp2->Fill(Be12Hit->GetThetaDeg(),Be12Hit->GetEnergyMeV());
      }
      if(excite<1.3 && excite>0)
      {
	temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE_exCut_gs",Be12Hit->GetDetectorNumber()));
	if(temp2) temp2->Fill(Be12Hit->GetThetaDeg(),Be12Hit->GetEnergyMeV());
      }
      if(excite<5)
      {
	temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE_exCut_sc",Be12Hit->GetDetectorNumber()));
	if(temp2) temp2->Fill(Be12Hit->GetThetaDeg(),Be12Hit->GetEnergyMeV());
      }
      temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE",Be12Hit->GetDetectorNumber()));
      if(temp2) temp2->Fill(Be12Hit->GetThetaDeg(),Be12Hit->GetEnergyMeV());
      temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE_strip%02i",Be12Hit->GetDetectorNumber(),Be12Hit->GetDVerticalStrip()));
      if(temp2) temp2->Fill(Be12Hit->GetThetaDeg(),Be12Hit->GetEnergyMeV());
    }

    for(int y=0; y<csm->GetMultiplicity(); y++)
    {
      if(DEBUG)
      {
	cout<<"Main Get Multiplicity()"<<endl;
      }
//***********************
//        General
//***********************

      TH1D *temp1 = 0;
      TH2D *temp2 = 0;
      TH2I *temp2INT = 0;
      TH3D *temp3 = 0;
      TCSMHit *hit = csm->GetHit(y);

      if(DEBUG)
      {
        cout<<"General"<<endl;
      }

      if(DEBUG) hit->Print();

      /*
      1DV = 0
      1DH = 20
      1EV = 40
      1EH = 60
      2DV = 80
      2DH = 100
      2EV = 120
      2EH = 140
      3DV = 160
      3DH = 180
      4DV = 200
      4DH = 220
      */

      int offset = -1;
      if(hit->GetDetectorNumber()==1)
	offset=0;
      else if(hit->GetDetectorNumber()==2)
	offset=80;
      else if(hit->GetDetectorNumber()==3)
	offset=160;
      else if(hit->GetDetectorNumber()==4)
	offset=200;
      
      temp2 = (TH2D*)outlist->FindObject("EnergyCheck");
      if(hit->GetDVerticalEnergy()>10.)
	temp2->Fill(hit->GetDVerticalStrip()+offset,hit->GetDVerticalEnergy()/1000.);
      if(hit->GetDHorizontalEnergy()>10.)
	temp2->Fill(hit->GetDHorizontalStrip()+offset+20,hit->GetDHorizontalEnergy()/1000.);
      
      if(hit->GetDetectorNumber()<3)
      {
	if(hit->GetEVerticalEnergy()>10.)
	  temp2->Fill(hit->GetEVerticalStrip()+offset+40,hit->GetEVerticalEnergy()/1000.);
	if(hit->GetEHorizontalEnergy()>10.)
	  temp2->Fill(hit->GetEHorizontalStrip()+offset+60,hit->GetEHorizontalEnergy()/1000.);
      }
      
      temp2 = (TH2D*)outlist->FindObject("ChargeCheck");
      if(hit->GetDVerticalCharge() != 0)
	temp2->Fill(hit->GetDVerticalStrip()+offset,hit->GetDVerticalCharge()/125.);
      if(hit->GetDHorizontalCharge() != 0)
	temp2->Fill(hit->GetDHorizontalStrip()+offset+20,hit->GetDHorizontalCharge()/125.);
      
      if(hit->GetDetectorNumber()<3)
      {
	if(hit->GetEVerticalCharge() != 0)
	  temp2->Fill(hit->GetEVerticalStrip()+offset+40,hit->GetEVerticalCharge()/125.);
	if(hit->GetEHorizontalCharge() != 0)
	  temp2->Fill(hit->GetEHorizontalStrip()+offset+60,hit->GetEHorizontalCharge()/125.);
      }

      if(hit->GetDPosition().Z() != 1 && hit->GetDPosition().Y() != 0 && hit->GetDPosition().X() != 0)
      {
	temp3 = (TH3D*)outlist->FindObject("positions");
	temp3->Fill(hit->GetDPosition().Z(),hit->GetDPosition().Y(),hit->GetDPosition().X());
	if(hit->GetEEnergy()>1)
	  temp3->Fill(hit->GetEPosition().Z(),hit->GetEPosition().Y(),hit->GetEPosition().X());

	temp2 = (TH2D*)outlist->FindObject("positions_proj");
	temp2->Fill(hit->GetDPosition().Z(),hit->GetDPosition().X());
	if(hit->GetEEnergy()>1)
	  temp2->Fill(hit->GetEPosition().Z(),hit->GetEPosition().X());

	temp2 = (TH2D*)outlist->FindObject("CSM_HP_Theta_Phi");
	temp2->Fill(hit->GetEPosition().Theta()*180/TMath::Pi(),hit->GetEPosition().Phi()*180/TMath::Pi());
      }

      if(DEBUG) cout<<"EVTheta"<<endl;
      temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%iD",hit->GetDetectorNumber()));
      temp2->Fill(hit->GetThetaDeg(),hit->GetDEnergy()/1000.);

      temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%iE",hit->GetDetectorNumber()));
      if(temp2) temp2->Fill(hit->GetEPosition().Theta()*180/TMath::Pi(),hit->GetEEnergy()/1000.);
      if(DEBUG) cout<<"HitPattern"<<endl;

      if(hit->GetDEnergy()>500)
      {
	temp2INT = (TH2I*)outlist->FindObject(Form("CSM_HitPattern_%iD",hit->GetDetectorNumber()));
	temp2INT->Fill(hit->GetDVerticalStrip(),hit->GetDHorizontalStrip());
      }
      
      temp2INT = (TH2I*)outlist->FindObject(Form("CSM_HitPattern_%iE",hit->GetDetectorNumber()));
      if(temp2INT) temp2INT->Fill(hit->GetEVerticalStrip(),hit->GetEHorizontalStrip());
      if(DEBUG) cout<<"PID"<<endl;


      if(hit->GetEEnergy()>0 && hit->GetDEnergy()>0)
      {
	temp2 = (TH2D*)outlist->FindObject(Form("pid_%i",hit->GetDetectorNumber()));
	temp2->Fill(hit->GetEEnergy()/1000.,hit->GetDEnergy()/1000.);
	temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_summed",hit->GetDetectorNumber()));
	temp2->Fill(hit->GetEnergyMeV(),hit->GetDEnergy()/1000.);
	if(hit->GetDthickness()>5)
	{
	  //cout<<thickness<<endl;
	  temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_summed_thickness",hit->GetDetectorNumber()));
	  temp2->Fill(hit->GetEnergyMeV(),hit->GetDdE_dx());

	  temp2 = (TH2D*)outlist->FindObject(Form("stripPID_det%i_strip%02i",hit->GetDetectorNumber(),hit->GetDVerticalStrip()));
	  temp2->Fill(hit->GetEnergyMeV(),hit->GetDdE_dx());
	
	  temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_thickness",hit->GetDetectorNumber()));
	  temp2->Fill(hit->GetEEnergy()/1000.,hit->GetDdE_dx());
	}
	temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%iTotal",hit->GetDetectorNumber()));
	temp2->Fill(hit->GetThetaDeg(),hit->GetEnergyMeV());
      }

      if(hit->GetDetectorNumber()==3)
      {
	if(hit->GetDEnergy()>0)
	{
	  temp2 = (TH2D*)outlist->FindObject("EvTheta_1Total");
	  temp2->Fill(hit->GetThetaDeg(),hit->GetEnergyMeV());
	}
      }
      else if(hit->GetDetectorNumber()==4)
      {
	if(hit->GetDEnergy()>0)
	{
	  temp2 = (TH2D*)outlist->FindObject("EvTheta_2Total");
	  temp2->Fill(hit->GetThetaDeg(),hit->GetEnergyMeV());
	}
      }

      if(hit->GetDVerticalEnergy()>0 && hit->GetDHorizontalEnergy()>0)
      {
	temp2 = (TH2D*)outlist->FindObject(Form("CheckCalD_%i",hit->GetDetectorNumber()));
	if(temp2) temp2->Fill(hit->GetDVerticalEnergy()/1000.,hit->GetDHorizontalEnergy()/1000.);
      }
      if(hit->GetEVerticalEnergy()>0 && hit->GetEHorizontalEnergy()>0)
      {
	temp2 = (TH2D*)outlist->FindObject(Form("CheckCalE_%i",hit->GetDetectorNumber()));
	if(temp2) temp2->Fill(hit->GetEVerticalEnergy()/1000.,hit->GetEHorizontalEnergy()/1000.);
      }

    //Multiplicity cut plots

      if(hit->GetEEnergy()>100.)
      {
	temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_mult%i",hit->GetDetectorNumber(),csm->GetMultiplicity()));
	if(temp2) temp2->Fill(hit->GetEEnergy()/1000.,hit->GetDEnergy()/1000.);
      }

      temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%iTotal_mult%i",hit->GetDetectorNumber(),csm->GetMultiplicity()));
      if(temp2) temp2->Fill(hit->GetThetaDeg(),(hit->GetEEnergy()+hit->GetDEnergy())/1000.);

      if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form("thetas_Be11_%i_%02i",hit->GetDetectorNumber(),int(hit->GetThetaDeg())))))
      {
	if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()))
	{
	  temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE11",hit->GetDetectorNumber()));
	  if(temp2) temp2->Fill(hit->GetThetaDeg(),hit->GetEnergyMeV());	  
	}
      }
    }

//***********************
//        Gammas
//***********************
    for(int y=0; y<tigress->GetAddBackMultiplicity();y++)
    {
      TTigressHit *hit = tigress->GetAddBackHit(y);
      TH1D* temp = 0;

      temp = (TH1D*)outlist->FindObject("GammaSum");
      temp->Fill(hit->GetCore()->GetEnergy()/1000.);
    }
//***********************
//         End
//***********************

////////////////////////////////////////////////////////////////////////

    if(x%20000==0)
    {
      printf("\tprocessed " DYELLOW "%i" RESET_COLOR "/" DBLUE "%i" RESET_COLOR " entries in " DRED "%.02f" RESET_COLOR " seconds\r",x,nentries,w.RealTime());
      fflush(stdout);
      w.Continue();
    }
  }

//   ofile<<"};"<<endl;
//   ofile.close();
  
  //printf("\tprocessed " DYELLOW "%i" RESET_COLOR "/" DBLUE "%i" RESET_COLOR " entries in " DRED "%.02f" RESET_COLOR " seconds\n",x,nentries,w.RealTime());
  cout<<endl;
  return;
}



int main(int argc, char **argv)
{
  if(argc<2)
  {
    printf("try adding analysis trees to the command line.\n");
    return 1;
  }

  TApplication *app = new TApplication("app",0,0);
  TFile cf("cuts.root");
  TIter *iter = new TIter(cf.GetListOfKeys());

  while(TObject *obj = iter->Next())
  {
    obj = ((TKey *)obj)->ReadObj();

    //printf("obj->ClassName() = %s\n", obj->ClassName());
    if(strcmp(obj->ClassName(),"TCutG")!=0)
    {
      continue;
    }

    cutlist->Add(obj);
    printf("found a cut! %s \n",((TNamed *)obj)->GetName());
  }

  TFile cf2("thetacuts.root");
  TIter *iter2 = new TIter(cf2.GetListOfKeys());

  while(TObject *obj = iter2->Next())
  {
    obj = ((TKey *)obj)->ReadObj();

    //printf("obj->ClassName() = %s\n", obj->ClassName());
    if(strcmp(obj->ClassName(),"TCutG")!=0)
    {
      continue;
    }

    cutlist->Add(obj);
    printf("found a cut! %s \n",((TNamed *)obj)->GetName());
  }
  
  TChain *chain = new TChain("AnalysisTree");
  int i =1;

  while(i<argc)
  {
    chain->Add(argv[i++]);
  }


  printf("%i analysis trees added to chain.\n",i-1);
  chain->SetBranchAddress("TTigress",&tigress);
  chain->SetBranchAddress("TCSM",&csm);
  
  TList *outlist = new TList;
  SetupHistos(outlist);
  ProcessChain(chain,outlist);
  outlist->Sort();

  if(DEBUG)
  {
    cout<<"Done Sorting"<<endl;
  }

  TFile f("output.root","recreate");
  f.cd();
  outlist->Write();
  f.Close();
  
  if(DEBUG)
  {
    cout<<"All done"<<endl;
  }

  return 0;
}

double Doppler(TTigressHit* thit, TCSMHit* chit)
{
  double pi = TMath::Pi();
  double M4 = MASS_BE12;
    
  double LabEnergyHeavy = chit->GetEnergyMeV();
  
  double beta =  sqrt( (LabEnergyHeavy*(LabEnergyHeavy + 2.0*M4) )/( ( LabEnergyHeavy + M4 )*( LabEnergyHeavy + M4 ) ) );
  
  double CosTheta = cos( chit->GetPosition().Angle( thit->GetPosition() ) );
    
  double RelativisticCorr = (1.0 / ( sqrt( 1.0 - beta*beta ) ) );
  
  double EGammaDopplerCorr = ( (thit->GetCore()->GetEnergy()/1000. * RelativisticCorr) * ( (1.0 - beta*CosTheta) ) );
    
  return EGammaDopplerCorr;
}

double CalcCOMProductTheta(double theta, double energy)
{
  const double pi = TMath::Pi();
  const double BeamE = BEAM_ENERGY;

  const double M1 = MASS_BE11;
  const double M2 = MASS_BE9;
  const double M3 = MASS_BE8;
  const double M4 = MASS_BE12;
  
  double COMVelBeam = sqrt(2*BeamE/M1);
  double COMV = ( M1 / ( M1 + M2 ) ) * COMVelBeam;
  double productVel = sqrt(2*energy/M3);//to get the 12Be COM theta
  
  double kPrime = COMV / productVel;
  
  double thetaCOM = atan( sin(theta) / ( cos(theta)-kPrime ) );
  
  if( thetaCOM < 0 )
    thetaCOM += pi;
  
  return thetaCOM; //12Be
}

double* CorrParticle(double Energy, double Theta, double Phi, double Mass)
{
  bool debug = 0;
  const double pi = TMath::Pi();
  const double QVal = 1.50619;//from http://www.nndc.bnl.gov/qcalc/
  
  if(debug)
    cout<<"CORR PARTICLE DEBUG ACTIVE, E: "<<Energy<<" T: "<<Theta*180./pi<<" P: "<<Phi*180./pi<<" M: "<<Mass<<" EXPECTED MASS: "<<MASS_BE12<<" or "<<MASS_BE8<<endl;
  
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

double* CorrParticle(TCSMHit* Hit)
{
  return CorrParticle(Hit->GetEnergy(),Hit->GetDPosition().Theta(),Hit->GetDPosition().Phi(),Hit->GetMassMeV());
}






