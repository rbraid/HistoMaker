#include "../include/BasicMaker.hh"

void ProcessBasic(TChain* chain,TList* outlist,TList* cutlist)
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
            
//             if(hit->GetDVerticalStrip() == 7 && hit->GetDHorizontalStrip() == 9)
//             if(hit->GetDVerticalStrip() == 2 )
            if(hit->GetDVerticalStrip() == 2 && hit->GetDHorizontalStrip() == 0)
            {
              temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_summed_thickness_isolate",hit->GetDetectorNumber()));
              if(temp2) temp2->Fill(hit->GetEnergyMeV(),hit->GetDdE_dx()); 
            }
          }
        }
      }
      
      if(hit->GetDetectorNumber() == 2)
      {
      if(TCutG *cut = (TCutG*)(cutlist->FindObject("bullshit")))
        {
          if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()) && hit->GetEEnergy() > 10)
          {
            TH2I* tmp = (TH2I*)outlist->FindObject(Form("HP_%i_bs",hit->GetDetectorNumber()));
            tmp->Fill(hit->GetDVerticalStrip(),hit->GetDHorizontalStrip());
          }
        }
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