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
  outlist->Add(new TH2D("pid_1","pid_1",700,0,70,700,0,70));//
  outlist->Add(new TH2D("pid_2","pid_2",700,0,70,700,0,70));//
  outlist->Add(new TH2D("pid_1_summed","pid_1_summed",1400,0,140,700,0,70));//
  outlist->Add(new TH2D("pid_1_summed_single_pixel","pid_1_summed_single_pixel",1400,0,140,700,0,70));//
  outlist->Add(new TH2D("pid_2_summed","pid_2_summed",1400,0,140,700,0,70));//
  outlist->Add(new TH2D("pid_2_summed_single_pixel","pid_2_summed_single_pixel",1400,0,140,700,0,70));//

  outlist->Add(new TH2D("pid_1_mult1","pid_1_mult1",700,0,70,700,0,70));//
  outlist->Add(new TH2D("pid_2_mult1","pid_2_mult1",700,0,70,700,0,70));//
  outlist->Add(new TH2D("pid_1_mult2","pid_1_mult2",700,0,70,700,0,70));//
  outlist->Add(new TH2D("pid_2_mult2","pid_2_mult2",700,0,70,700,0,70));//
  outlist->Add(new TH2D("pid_1_mult3","pid_1_mult3",700,0,70,700,0,70));//
  outlist->Add(new TH2D("pid_2_mult3","pid_2_mult3",700,0,70,700,0,70));//
  outlist->Add(new TH2D("pid_1_mult4","pid_1_mult4",700,0,70,700,0,70));//
  outlist->Add(new TH2D("pid_2_mult4","pid_2_mult4",700,0,70,700,0,70));//

  outlist->Add(new TH3D("positions","positions",100,0,100,60,-30,30,200,-100,100));
  
  outlist->Add(new TH2D("EvTheta_1D","EvTheta_1D",240,0,60,200,10,60));//
  outlist->Add(new TH2D("EvTheta_1E","EvTheta_1E",240,0,60,200,10,60));//
  outlist->Add(new TH2D("EvTheta_2D","EvTheta_2D",240,0,60,200,10,60));//
  outlist->Add(new TH2D("EvTheta_2E","EvTheta_2E",240,0,60,200,10,60));//
  outlist->Add(new TH2D("EvTheta_3D","EvTheta_3D",100,0,100,200,0,70));//
  outlist->Add(new TH2D("EvTheta_4D","EvTheta_4D",100,0,100,200,0,70));//
  outlist->Add(new TH2D("EvTheta_1Total","EvTheta_1Total",100,0,100,700,0,70));
  outlist->Add(new TH2D("EvTheta_2Total","EvTheta_2Total",100,0,100,700,0,70));
  
  outlist->Add(new TH2D("EvTheta_1Total_mult1","EvTheta_1Total_mult1",100,0,100,700,0,70));
  outlist->Add(new TH2D("EvTheta_2Total_mult1","EvTheta_2Total_mult1",100,0,100,700,0,70));
  outlist->Add(new TH2D("EvTheta_1Total_mult2","EvTheta_1Total_mult2",100,0,100,700,0,70));
  outlist->Add(new TH2D("EvTheta_2Total_mult2","EvTheta_2Total_mult2",100,0,100,700,0,70));
  outlist->Add(new TH2D("EvTheta_1Total_mult3","EvTheta_1Total_mult3",100,0,100,700,0,70));
  outlist->Add(new TH2D("EvTheta_2Total_mult3","EvTheta_2Total_mult3",100,0,100,700,0,70));
  outlist->Add(new TH2D("EvTheta_1Total_mult4","EvTheta_1Total_mult4",100,0,100,700,0,70));
  outlist->Add(new TH2D("EvTheta_2Total_mult4","EvTheta_2Total_mult4",100,0,100,700,0,70));
  
  outlist->Add(new TH2D("EvTheta_3Total_mult1","EvTheta_3Total_mult1",100,0,100,700,0,70));
  outlist->Add(new TH2D("EvTheta_4Total_mult1","EvTheta_4Total_mult1",100,0,100,700,0,70));
  outlist->Add(new TH2D("EvTheta_3Total_mult2","EvTheta_3Total_mult2",100,0,100,700,0,70));
  outlist->Add(new TH2D("EvTheta_4Total_mult2","EvTheta_4Total_mult2",100,0,100,700,0,70));
  outlist->Add(new TH2D("EvTheta_3Total_mult3","EvTheta_3Total_mult3",100,0,100,700,0,70));
  outlist->Add(new TH2D("EvTheta_4Total_mult3","EvTheta_4Total_mult3",100,0,100,700,0,70));
  outlist->Add(new TH2D("EvTheta_3Total_mult4","EvTheta_3Total_mult4",100,0,100,700,0,70));
  outlist->Add(new TH2D("EvTheta_4Total_mult4","EvTheta_4Total_mult4",100,0,100,700,0,70));

  outlist->Add(new TH2D("EvTheta_1_BE","EvTheta_1_BE",100,0,100,700,0,70));
  outlist->Add(new TH2D("EvTheta_2_BE","EvTheta_2_BE",100,0,100,700,0,70));
  outlist->Add(new TH2D("EvTheta_1_HE","EvTheta_1_HE",100,0,100,700,0,70));
  outlist->Add(new TH2D("EvTheta_2_HE","EvTheta_2_HE",100,0,100,700,0,70));
  
  outlist->Add(new TH2I("CSM_HitPattern_1D","HitPattern_1D",16,0,16,16,0,16));//
  outlist->Add(new TH2I("CSM_HitPattern_1E","HitPattern_1E",16,0,16,16,0,16));//
  outlist->Add(new TH2I("CSM_HitPattern_2D","HitPattern_2D",16,0,16,16,0,16));//
  outlist->Add(new TH2I("CSM_HitPattern_2E","HitPattern_2E",16,0,16,16,0,16));//
  outlist->Add(new TH2I("CSM_HitPattern_3D","HitPattern_3D",16,0,16,16,0,16));//
  outlist->Add(new TH2I("CSM_HitPattern_4D","HitPattern_4D",16,0,16,16,0,16));//
  
  outlist->Add(new TH2D("CheckCalD","CheckCalD",600,0,60,600,0,60));
  outlist->Add(new TH2D("CheckCalE","CheckCalE",600,0,60,600,0,60));
  
  outlist->Add(new TH2D("CSM_HP_Theta_Phi","HP_Theta_Phi",90,0,180,180,-180,180));

  outlist->Add(new TH1D("Multiplicity","Multiplicity",10,0,10));//

  outlist->Add(new TH1D("GammaSum","GammaSum",2000,0,200));

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

      temp2INT = (TH2I*)outlist->FindObject(Form("CSM_HitPattern_%iD",hit->GetDetectorNumber()));
      temp2INT->Fill(hit->GetDVerticalStrip(),hit->GetDHorizontalStrip());
      
      temp2INT = (TH2I*)outlist->FindObject(Form("CSM_HitPattern_%iE",hit->GetDetectorNumber()));
      if(temp2INT) temp2INT->Fill(hit->GetEVerticalStrip(),hit->GetEHorizontalStrip());
      if(DEBUG) cout<<"PID"<<endl;

      if(hit->GetDetectorNumber()==1)
      {
        if(hit->GetEEnergy()>0 && hit->GetDEnergy()>0)
        {
          temp2 = (TH2D*)outlist->FindObject("pid_1");
          temp2->Fill(hit->GetEEnergy()/1000.,hit->GetDEnergy()/1000.);
          temp2 = (TH2D*)outlist->FindObject("pid_1_summed");
          temp2->Fill(hit->GetEEnergy()/1000.+hit->GetDEnergy()/1000.,hit->GetDEnergy()/1000.);
          temp2 = (TH2D*)outlist->FindObject("EvTheta_1Total");
          temp2->Fill(hit->GetDPosition().Theta()*180/3.14159,(hit->GetEEnergy()+hit->GetDEnergy())/1000.);
	  
	  if(hit->GetDHorizontalStrip()==8 && hit->GetDVerticalStrip()==0)
	  {
	    temp2 = (TH2D*)outlist->FindObject("pid_1_summed_single_pixel");
	    temp2->Fill(hit->GetEEnergy()/1000.+hit->GetDEnergy()/1000.,hit->GetDEnergy()/1000.);
	  }
        }
      }
      else if(hit->GetDetectorNumber()==2)
      {
        if(hit->GetEEnergy()>0 && hit->GetDEnergy()>0)
        {
          temp2 = (TH2D*)outlist->FindObject("pid_2");
          temp2->Fill(hit->GetEEnergy()/1000.,hit->GetDEnergy()/1000.);
          temp2 = (TH2D*)outlist->FindObject("pid_2_summed");
          temp2->Fill(hit->GetEEnergy()/1000.+hit->GetDEnergy()/1000.,hit->GetDEnergy()/1000.);
          temp2 = (TH2D*)outlist->FindObject("EvTheta_2Total");
          temp2->Fill(hit->GetDPosition().Theta()*180/3.14159,(hit->GetEEnergy()+hit->GetDEnergy())/1000.);
	  
	  if(hit->GetDHorizontalStrip()==8 && hit->GetDVerticalStrip()==15)
	  {
	    temp2 = (TH2D*)outlist->FindObject("pid_2_summed_single_pixel");
	    temp2->Fill(hit->GetEEnergy()/1000.+hit->GetDEnergy()/1000.,hit->GetDEnergy()/1000.);
	  }
        }
      }

      temp2 = (TH2D*)outlist->FindObject("CheckCalD");
      if(temp2) temp2->Fill(hit->GetDVerticalEnergy()/1000.,hit->GetDHorizontalEnergy()/1000.);
      temp2 = (TH2D*)outlist->FindObject("CheckCalE");
      if(temp2) temp2->Fill(hit->GetEVerticalEnergy()/1000.,hit->GetEHorizontalEnergy()/1000.);

    //Multiplicity cut plots

      if(hit->GetEEnergy()>0.)
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
	  if(temp2) temp2->Fill(hit->GetDPosition().Theta()*180/3.14159,(hit->GetEEnergy()+hit->GetDEnergy())/1000.);      }
      }
      else if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form("pid%i_Be_high_1",hit->GetDetectorNumber()))))
      {
	if(!cut) cerr<<"Error: Beryllium high energy cut not found!"<<endl;
	else if(cut->IsInside(hit->GetEEnergy()/1000., hit->GetDEnergy()/1000. ) )
	{
	  temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE",hit->GetDetectorNumber()));
	  if(temp2) temp2->Fill(hit->GetDPosition().Theta()*180/3.14159,(hit->GetEEnergy()+hit->GetDEnergy())/1000.);      }
      }

      if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form("pid%i_Alphas_1",hit->GetDetectorNumber()))))
      {
	if(!cut) cerr<<"Error: Alpha cut not found!"<<endl;
	else if(cut->IsInside(hit->GetEEnergy()/1000., hit->GetDEnergy()/1000. ) )
	{
	  temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_HE",hit->GetDetectorNumber()));
	  if(temp2) temp2->Fill(hit->GetDPosition().Theta()*180/3.14159,(hit->GetEEnergy()+hit->GetDEnergy())/1000.);      }
      }
      else if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form("pid%i_Alphas_high_1",hit->GetDetectorNumber()))))
      {
	if(!cut) cerr<<"Error: Alpha high energy cut not found!"<<endl;
	else if(cut->IsInside(hit->GetEEnergy()/1000., hit->GetDEnergy()/1000. ) )
	{
	  temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_HE",hit->GetDetectorNumber()));
	  if(temp2) temp2->Fill(hit->GetDPosition().Theta()*180/3.14159,(hit->GetEEnergy()+hit->GetDEnergy())/1000.);      }
      }

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













