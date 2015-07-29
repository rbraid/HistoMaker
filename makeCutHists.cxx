// g++ makeCutHists.cxx -Wl,--no-as-needed -o RunHistos `grsi-config --cflags --all-libs --root`

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
  if(DEBUG) cout<<"Setting up Histos"<<endl;
  TH1D *temp1 = 0;
  TH2D *temp2 = 0;
  TH2I *temp2INT = 0;
  TH3D *temp3 = 0;

  for(int id = 1; id<=2;id++)
  {
    outlist->Add(new TH2D(Form("EvTheta_%i_BE",id),Form("EvTheta %i, cut on Be",id),100,0,100,700,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE",id));
    //temp2->SetContour(666);
    temp2->SetOption("colz");
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");

    outlist->Add(new TH2D(Form("EvTheta_%i_BE11",id),Form("EvTheta %i, cut on Be",id),100,0,100,700,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE11",id));
    //temp2->SetContour(666);
    temp2->SetOption("colz");
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    
    for(int theta=10;theta<=55;theta++)
    {
      outlist->Add(new TH2D(Form("pid_det%i_theta%02i",id,theta),Form("Single Theta PID plots. Detector %i, Theta %i",id,theta),2000,15,35,350,350,700));
      temp2 = (TH2D*)outlist->FindObject(Form("pid_det%i_theta%02i",id,theta));
      //temp2->SetContour(666);
      temp2->SetOption("colz");
      temp2->GetXaxis()->SetTitle("Total Energy deposited in MeV");
      temp2->GetYaxis()->SetTitle("dE/dX in MeV/um");
    }
    
    for(int strip = 0; strip<16; strip++)
    {

    }
    
    for(int mid = 1; mid<=4;mid++)
    {
    
    }
  }

  for(int det=1;det<=4;det++)
  {

    for(char type='D';type<='E';type++) //Wow I can't believe this works.  I am glad they are in alphabetical order
    {
      if(det>2 && type=='E')//This skips 3 and 4 E, which don't exist
	continue;

    }
  }

  
  outlist->Add(new TH1D("ThetaSpikes","Theta measurements",440,5,60));
    temp1 = (TH1D*)outlist->FindObject("ThetaSpikes");
    temp1->GetXaxis()->SetTitle("Theta in Degrees");
    temp1->GetYaxis()->SetTitle("Counts");
    
  if(DEBUG)
  {
    cout<<"Histos Set"<<endl;
  }
}

void ProcessChain(TChain *chain,TList *outlist)//, MakeFriend *myFriend)
{
  int nentries = chain->GetEntries();
//   cout<<DRED;
//   chain->Print();
//   cout<<RESET_COLOR;
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

    for(int y=0; y<csm->GetMultiplicity(); y++)
    {
      if(DEBUG)
      {
	cout<<"Main Get Multiplicity()"<<endl;
      }
//***********************
//        General
//***********************

      //myFriend->Reset();

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

      temp1 = (TH1D*)outlist->FindObject("ThetaSpikes");
      if(temp1) temp1->Fill(hit->GetDPosition().Theta()*180./3.14159);

      if(hit->GetEEnergy()>100)
      {
	temp2 = (TH2D*)outlist->FindObject(Form("pid_det%i_theta%02i",hit->GetDetectorNumber(),int(hit->GetDPosition().Theta()*180./3.14159)));
	if(temp2) temp2->Fill(hit->GetEnergy()/1000.,hit->GetDdE_dx());
      }
      
      if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form("theta_Be_%i_%02i",hit->GetDetectorNumber(),int(hit->GetDPosition().Theta()*180./3.14159)))))
      {
	if(cut->IsInside(hit->GetEnergy()/1000.,hit->GetDdE_dx()))
	{
	  temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE",hit->GetDetectorNumber()));
	  if(temp2) temp2->Fill(hit->GetDPosition().Theta()*180/3.14159,hit->GetEnergy()/1000.);
	}
      }
      
      if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form("thetas_Be11_%i_%02i",hit->GetDetectorNumber(),int(hit->GetDPosition().Theta()*180./3.14159)))))
      {
	if(cut->IsInside(hit->GetEnergy()/1000.,hit->GetDdE_dx()))
	{
	  temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE11",hit->GetDetectorNumber()));
	  if(temp2) temp2->Fill(hit->GetDPosition().Theta()*180/3.14159,hit->GetEnergy()/1000.);
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
  TFile cf("thetacuts.root");
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

  //MakeFriend *myFriend = new MakeFriend(csm);
  
  TList *outlist = new TList;
  SetupHistos(outlist);
  ProcessChain(chain,outlist);//,myFriend);
  outlist->Sort();

  if(DEBUG)
  {
    cout<<"Done Sorting"<<endl;
  }

  TFile f("histoutput.root","recreate");
  f.cd();
  outlist->Write();
  f.Close();

  //myFriend->Write();
  
  if(DEBUG)
  {
    cout<<"All done"<<endl;
  }

  return 0;
}













