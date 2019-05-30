#include "../include/BasicMaker.hh"

void ProcessBasic(TChain* chain,TList* outlist)
{
  TStopwatch w;
  w.Start();
  TCSM *csm =  new TCSM;
  
  chain->SetBranchAddress("TCSM",&csm);
  
  int nentries = chain->GetEntries();
  
  for(int x=0; x<nentries; x++)
  {
    chain->GetEntry(x);
    
    if(csm->GetMultiplicity()==0)
      continue;
    
    for(int y=0; y<csm->GetMultiplicity(); y++)
    {

      TH2D *temp2 = 0;
      TCSMHit *hit = csm->GetHit(y);
      
      if(hit->GetDetectorNumber()<3)
      {
        temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%iTotal",hit->GetDetectorNumber()));
        temp2->Fill(hit->GetThetaDeg(),hit->GetEnergyMeV());
        if(hit->GetEEnergy()>0 && hit->GetDEnergy()>0)
        {
          temp2 = (TH2D*)outlist->FindObject(Form("pid_%i",hit->GetDetectorNumber()));
          if(temp2) temp2->Fill(hit->GetEEnergy()/1000.,hit->GetDEnergy()/1000.);
          if(hit->GetDthickness()>5)
          {
            temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_summed_thickness",hit->GetDetectorNumber()));
            if(temp2) temp2->Fill(hit->GetEnergyMeV(),hit->GetDdE_dx());
          }
        }
      }

      if(hit->GetDVerticalStrip() >= 0 && hit->GetDHorizontalStrip() >= 0)
      {
        TH2I* temp2INT = (TH2I*)outlist->FindObject(Form("CSM_HitPattern_%iD",hit->GetDetectorNumber()));
        if(temp2INT) temp2INT->Fill(hit->GetDVerticalStrip(),hit->GetDHorizontalStrip());
      }

      if(hit->GetEVerticalStrip() >= 0 && hit->GetEHorizontalStrip() >= 0)
      {
        TH2I* temp2INT = (TH2I*)outlist->FindObject(Form("CSM_HitPattern_%iE",hit->GetDetectorNumber()));
        if(temp2INT) temp2INT->Fill(hit->GetEVerticalStrip(),hit->GetEHorizontalStrip());
      }
    }
    if(x%200000==0)
    {
      printf("ProcessBasic " DYELLOW "%i" RESET_COLOR "/" DBLUE "%i" RESET_COLOR " entries in " DRED "%.02f" RESET_COLOR " seconds\r",x,nentries,w.RealTime());
      fflush(stdout);
      w.Continue();
    }
  }
  printf("ProcessBasic " DYELLOW "%i" RESET_COLOR "/" DBLUE "%i" RESET_COLOR " entries in " DRED "%.02f" RESET_COLOR " seconds\n",nentries,nentries,w.RealTime());
  chain->ResetBranchAddresses();
  delete csm;
}

void SetupBasicHistos(TList* hlist)
{
  for(int det=1;det<=4;det++)
  {
    for(char type='D';type<='E';type++) //Wow I can't believe this works.  I am glad they are in alphabetical order
    {
      if(det>2 && type=='E')//This skips 3 and 4 E, which don't exist
        continue;

      hlist->Add(new TH2I(Form("CSM_HitPattern_%i%c",det,type),Form("HitPattern, Detector %i, Position %c",det,type),16,0,16,16,0,16));//
      TH2I* temp2INT = (TH2I*)hlist->FindObject(Form("CSM_HitPattern_%i%c",det,type));
      temp2INT->GetXaxis()->SetTitle("Vertical Strip Number");
      temp2INT->GetYaxis()->SetTitle("Horizontal Strip Number");
    }
  }
}