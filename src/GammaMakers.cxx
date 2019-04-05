#include "../include/GammaMakers.hh"

void ProcessGammas(TChain* chain,TList* outlist)
{
  TStopwatch w;
  w.Start();
  
  TTigress *tigress =  new TTigress;
  chain->SetBranchAddress("TTigress",&tigress);
  
  int nentries = chain->GetEntries();
  for(int x=0; x<nentries; x++)
  {
    chain->GetEntry(x);
    
    if(tigress->GetAddBackMultiplicity() == 0)
      continue;
    
    for(int y=0; y<tigress->GetAddBackMultiplicity();y++)
    {
      TTigressHit *hit = tigress->GetAddBackHit(y);
      TH1D* temp = 0;
      
      temp = (TH1D*)outlist->FindObject("GammaSum");
      temp->Fill(hit->GetCore()->GetEnergy()/1000.);
      if(hit->GetCore()->GetEnergy() > 10 && hit->GetCore()->GetEnergy() < 3000){
        temp = (TH1D*)outlist->FindObject("GammaSum_eff");
        temp->Fill(hit->GetCore()->GetEnergy()/1000.,EfficiencyWeight(hit));
      }
    }
    if(x%200000==0)
    {
      printf("\tprocessed " DYELLOW "%i" RESET_COLOR "/" DBLUE "%i" RESET_COLOR " entries in " DRED "%.02f" RESET_COLOR " seconds\r",x,nentries,w.RealTime());
      fflush(stdout);
      w.Continue();
    }
  }
  printf("\tprocessed " DYELLOW "%i" RESET_COLOR "/" DBLUE "%i" RESET_COLOR " entries in " DRED "%.02f" RESET_COLOR " seconds\n",nentries,nentries,w.RealTime());
  chain->ResetBranchAddresses();
  delete tigress;
}