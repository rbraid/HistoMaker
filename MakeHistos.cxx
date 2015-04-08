// g++ -g MakeHistos.cxx -Wl,--no-as-needed -L$GRSISYS/libraries -lGRSIFormat -lGRSIDetector -lTigress  -lCSM -I$GRSISYS/include --std=c++0x -o RunMe  -O2 `root-config --cflags --libs` -lTreePlayer -lgsl -lgslcblas -ggdb

#define DEBUG 0

#include <cstdio>

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
TTigress *tigress =  new TTigress;
TCSM *csm =  new TCSM;
TList *cutlist = new TList;

void SetupHistos(TList *outlist)
{
  TH1D *temp1 = 0;
  TH2D *temp2 = 0;
  TH2I *temp2INT = 0;
  TH3D *temp3 = 0;

  for(int id = 1; id<=2;id++)
  {
    
    outlist->Add(new TH2D(Form("pid_%i",id),Form("Particle ID, detector %i",id),700,0,70,700,0,70));//
      temp2 = (TH2D*)outlist->FindObject(Form("pid_%i",id));
      temp2->GetXaxis()->SetTitle("E Energy deposited in MeV");
      temp2->GetYaxis()->SetTitle("dE Energy deposited in MeV");

    outlist->Add(new TH2D(Form("pid_%i_fvb",id),Form("Particle ID, detector %i, cut on 1:1",id),700,0,70,700,0,70));//
      temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_fvb",id));
      temp2->GetXaxis()->SetTitle("E Energy deposited in MeV");
      temp2->GetYaxis()->SetTitle("dE Energy deposited in MeV");
    
    outlist->Add(new TH2D(Form("pid_%i_summed",id),Form("Particle ID, detector %i, summed",id),1400,0,140,700,0,70));//
      temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_summed",id));
      temp2->GetXaxis()->SetTitle("Total Energy deposited in MeV");
      temp2->GetYaxis()->SetTitle("dE Energy deposited in MeV");
    
    outlist->Add(new TH2D(Form("pid_%i_summed_single_pixel",id),Form("Single Pixel Particle ID, detector %i, summed",id),1400,0,140,700,0,70));//
      temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_summed_single_pixel",id));
      temp2->GetXaxis()->SetTitle("Total Energy deposited in MeV");
      temp2->GetYaxis()->SetTitle("dE Energy deposited in MeV");

    outlist->Add(new TH2D(Form("pid_%i_single_pixel",id),Form("Single Pixel Particle ID, detector %i",id),1400,0,140,700,0,70));//
      temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_single_pixel",id));
      temp2->GetXaxis()->SetTitle("E Energy deposited in MeV");
      temp2->GetYaxis()->SetTitle("dE Energy deposited in MeV");
      
    outlist->Add(new TH2D(Form("EvTheta_%iTotal",id),Form("EvTheta %i",id),100,0,100,700,0,70));
      temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%iTotal",id));
      temp2->GetXaxis()->SetTitle("Theta in Degrees");
      temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    
    outlist->Add(new TH2D(Form("EvTheta_%i_BE",id),Form("EvTheta %i, cut on Be",id),100,0,100,700,0,70));
      temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE",id));
      temp2->GetXaxis()->SetTitle("Theta in Degrees");
      temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    
    outlist->Add(new TH2D(Form("EvTheta_%i_HE",id),Form("EvTheta %i, cut on He",id),100,0,100,700,0,70));
      temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_HE",id));
      temp2->GetXaxis()->SetTitle("Theta in Degrees");
      temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");

    outlist->Add(new TH2D(Form("EvTheta_%i_fvb",id),Form("EvTheta %i, cut on 1:1",id),100,0,100,700,0,70));
      temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_fvb",id));
      temp2->GetXaxis()->SetTitle("Theta in Degrees");
      temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    
    outlist->Add(new TH2D(Form("CheckCalE_%i",id),Form("Front Energy vs Back Energy, Detector %i E",id),600,0,60,600,0,60));
      temp2 = (TH2D*)outlist->FindObject(Form("CheckCalE_%i",id));
      temp2->GetXaxis()->SetTitle("Energy deposited in Vertical (Front)");
      temp2->GetYaxis()->SetTitle("Energy deposited in Horizontal (Back)");
    
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

  outlist->Add(new TH1D("energyspec","Energy Spectrum",2000,0,20));
    temp1 = (TH1D*)outlist->FindObject("energyspec");
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts per bin");

  outlist->Add(new TH3D("positions","positions",100,0,100,60,-30,30,200,-100,100));
  
  
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
    
  if(DEBUG)
  {
    cout<<"Histos Set"<<endl;
  }
}


void ProcessChain(TChain *chain,TList *outlist)
{
  int nentries = chain->GetEntries();
  TStopwatch w;
  w.Start();

  if(DEBUG)
  {
    cout<<"Starting ProcessChain"<<endl;
  }

  for(int x=0; x<nentries; x++)
  {
    chain->GetEntry(x);

    if(DEBUG)
    {
      cout<<"nentries: "<<nentries<<endl;
    }

    if(csm->GetMultiplicity()==0)
      continue;

    ((TH1D *)outlist->FindObject("Multiplicity"))->Fill(csm->GetMultiplicity());

    for(int y=0; y<csm->GetMultiplicity(); y++)
    {
      if(DEBUG)
      {
        cout<<"First Get Multiplicity()"<<endl;
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
      
      temp3 = (TH3D*)outlist->FindObject("positions");
      temp3->Fill(hit->GetDPosition().Z(),hit->GetDPosition().Y(),hit->GetDPosition().X());

      temp2 = (TH2D*)outlist->FindObject("CSM_HP_Theta_Phi");
      temp2->Fill(hit->GetEPosition().Theta()*180/3.141597,hit->GetEPosition().Phi()*180/3.141597);
      
      if(DEBUG) cout<<"EVTheta"<<endl;
      temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%iD",hit->GetDetectorNumber()));
      temp2->Fill(hit->GetDPosition().Theta()*180/3.141597,hit->GetDEnergy()/1000.);

      temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%iE",hit->GetDetectorNumber()));
      if(temp2) temp2->Fill(hit->GetEPosition().Theta()*180/3.141597,hit->GetEEnergy()/1000.);
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
	temp2 = (TH2D*)outlist->FindObject(Form("pid_%i",hit->GetDetectorNumber());
	temp2->Fill(hit->GetEEnergy()/1000.,hit->GetDEnergy()/1000.);
	temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_summed",hit->GetDetectorNumber());
	temp2->Fill(hit->GetEnergy()/1000.,hit->GetDEnergy()/1000.);
	temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%iTotal",hit->GetDetectorNumber());
	temp2->Fill(hit->GetDPosition().Theta()*180/3.14159,hit->GetEnergy()/1000.);

	if(hit->GetDetectorNumber()==1)
	{
	  if(hit->GetDHorizontalStrip()==8 && hit->GetDVerticalStrip()==8)
	  {
	    temp2 = (TH2D*)outlist->FindObject("pid_1_summed_single_pixel");
	    temp2->Fill(hit->GetEnergy()/1000.,hit->GetDEnergy()/1000.);

	    temp2 = (TH2D*)outlist->FindObject("pid_1_single_pixel");
	    temp2->Fill(hit->GetEEnergy()/1000.,hit->GetDEnergy()/1000.);
	  }
	}
	else if(hit->GetDetectorNumber()==2)
	{
	  if(hit->GetDHorizontalStrip()==8 && hit->GetDVerticalStrip()==8)
	  {
	    temp2 = (TH2D*)outlist->FindObject("pid_2_summed_single_pixel");
	    temp2->Fill(hit->GetEnergy()/1000.,hit->GetDEnergy()/1000.);

	    temp2 = (TH2D*)outlist->FindObject("pid_2_single_pixel");
	    temp2->Fill(hit->GetEEnergy()/1000.,hit->GetDEnergy()/1000.);
	  }
	}
      }

      }
      else if(hit->GetDetectorNumber()==3)
      {
	if(hit->GetDEnergy()>0)
	{
	  temp2 = (TH2D*)outlist->FindObject("EvTheta_1Total");
	  temp2->Fill(hit->GetDPosition().Theta()*180/3.14159,hit->GetEnergy()/1000.);
	}
      }
      else if(hit->GetDetectorNumber()==4)
      {
	if(hit->GetDEnergy()>0)
	{
	  temp2 = (TH2D*)outlist->FindObject("EvTheta_2Total");
	  temp2->Fill(hit->GetDPosition().Theta()*180/3.14159,hit->GetEnergy()/1000.);
	}
      }
      
      if(hit->GetDetectorNumber()==1)
      {
	if(hit->GetDHorizontalStrip()==9)
	{
	  temp1 = (TH1D*)outlist->FindObject("energyspec");
	  temp1->Fill(hit->GetDVerticalEnergy()/1000.);
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
      if(temp2) temp2->Fill(hit->GetDPosition().Theta()*180/3.14159,(hit->GetEEnergy()+hit->GetDEnergy())/1000.);

      //Particle cut plots

      if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form("pid%i_Be_1",hit->GetDetectorNumber()))))
      {
	if(!cut) cerr<<"Error: Beryllium cut not found!"<<endl;
	else if(cut->IsInside(hit->GetEEnergy()/1000., hit->GetDEnergy()/1000. ) )
	{
	  temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE",hit->GetDetectorNumber()));
	  if(temp2) temp2->Fill(hit->GetDPosition().Theta()*180/3.14159,(hit->GetEEnergy()+hit->GetDEnergy())/1000.);
	}
      }
      else if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form("pid%i_Be_high_1",hit->GetDetectorNumber()))))
      {
	if(!cut) cerr<<"Error: Beryllium high energy cut not found!"<<endl;
	else if(cut->IsInside(hit->GetEEnergy()/1000., hit->GetDEnergy()/1000. ) )
	{
	  temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE",hit->GetDetectorNumber()));
	  if(temp2) temp2->Fill(hit->GetDPosition().Theta()*180/3.14159,(hit->GetEEnergy()+hit->GetDEnergy())/1000.);
	}
      }

      if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form("pid%i_Alphas_1",hit->GetDetectorNumber()))))
      {
	if(!cut) cerr<<"Error: Alpha cut not found!"<<endl;
	else if(cut->IsInside(hit->GetEEnergy()/1000., hit->GetDEnergy()/1000. ) )
	{
	  temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_HE",hit->GetDetectorNumber()));
	  if(temp2) temp2->Fill(hit->GetDPosition().Theta()*180/3.14159,(hit->GetEEnergy()+hit->GetDEnergy())/1000.);
	}
      }
      else if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form("pid%i_Alphas_high_1",hit->GetDetectorNumber()))))
      {
	if(!cut) cerr<<"Error: Alpha high energy cut not found!"<<endl;
	else if(cut->IsInside(hit->GetEEnergy()/1000., hit->GetDEnergy()/1000. ) )
	{
	  temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_HE",hit->GetDetectorNumber()));
	  if(temp2) temp2->Fill(hit->GetDPosition().Theta()*180/3.14159,(hit->GetEEnergy()+hit->GetDEnergy())/1000.);
	}
      }
      if(TCutG *cut = (TCutG*)(cutlist->FindObject("d1_good_fvb")))
      {
	if(!cut) cerr<<"Error: Front vs Back good cut not found!"<<endl;
	else if(cut->IsInside(hit->GetDVerticalEnergy()/1000., hit->GetDHorizontalEnergy()/1000. ) )
	{
	  if(hit->GetEEnergy()>10.)
	  {
	    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_fvb",hit->GetDetectorNumber()));
	    if(temp2) temp2->Fill(hit->GetDPosition().Theta()*180/3.14159,(hit->GetEEnergy()+hit->GetDEnergy())/1000.);
	    temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_fvb",hit->GetDetectorNumber()));
	    if(temp2) temp2->Fill(hit->GetEEnergy()/1000.,hit->GetDEnergy()/1000.);
	  }
	}
      }

      /*if(TCutG *cut = (TCutG*)(cutlist->FindObject("d1_bad_fvb_1")))
      {
	if(!cut) cerr<<"Error: Front vs Back cut not found!"<<endl;
	else if(cut->IsInside(hit->GetDVerticalEnergy()/1000., hit->GetDHorizontalEnergy()/1000. ) )
	{
	  hit->Print();
	}
      }*/
	

    //example cut
    /*if(TCutG *cut = (TCutG*)(cutlist->FindObject("pid1_beryllium"))))
    {
      if(!cut) cerr<<"Error: Beryllium cut not found!"<<endl;
      else if(cut->IsInside(hit->GetEEnergy(), hit->GetDEnergy() ) )
      {
        cout<<".";
      }
    }*/
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
  
  TChain *chain = new TChain("AnalysisTree");
  int i =1;

  while(i<argc)
  {
    chain->Add(argv[i++]);
  }


  printf("%i analysis trees added to chain.\n",i-1);
  chain->SetBranchAddress("TTigress",&tigress);
  chain->SetBranchAddress("TCSM",&csm);

  //TTree *ftree = new TTree("friendtree","friendtree");
  //ftree->Branch(

  //chain->AddFriend("AnalysisTree","friendtree.root");
  
  
  TList *outlist = new TList;
  SetupHistos(outlist);
  ProcessChain(chain,outlist);
  outlist->Sort();

  if(DEBUG)
  {
    cout<<"Done Sorting"<<endl;
  }

  TFile f("output.root","recreate");
  outlist->Write();
  f.Close();

  if(DEBUG)
  {
    cout<<"All done"<<endl;
  }

  return 0;
}













