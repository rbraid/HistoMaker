#include "../include/BasicMaker.hh"

void ProcessBasic(TChain* chain,TList* outlist)
{
  
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
    }
  }
  delete csm;
}