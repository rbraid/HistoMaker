#include "../include/DualAndPIDMaker.hh"

void ProcessPIDandDual(TChain* chain,TList* outlist,TList* cutlist,TFile* ringFile, bool sim)
{
  TStopwatch w;
  w.Start();
  
  TTigress *tigress =  new TTigress;
  TCSM *csm =  new TCSM;
  
  chain->SetBranchAddress("TCSM",&csm);
  if(!sim)
    chain->SetBranchAddress("TTigress",&tigress);
  
  TString Be10Cut;
  Be10Cut = "pid_low_thick_10Be_%i_v2";
  if(sim)
    Be10Cut = "pid_low_thick_10Be_%i_sim";
  
  int nentries = chain->GetEntries();
  for(int x=0; x<nentries; x++)
  {
    chain->GetEntry(x);
    
    if(csm->GetMultiplicity()<2)
      continue;
    
    int be10loc = -2;
    
    for(int y=0; y<csm->GetMultiplicity(); y++)
    {
      TCSMHit *hit = csm->GetHit(y);
      
      if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form(Be10Cut,hit->GetDetectorNumber()))))
      {
        if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()) && hit->GetEEnergy() > 10)
        {
          hit->SetIsotope(10,"be");
          be10loc = y;
        }
      }
    }
    
    if(be10loc>=0)
    {
      TCSMHit *Be10Hit = csm->GetHit(be10loc);
      
      for(int y=0; y<csm->GetMultiplicity(); y++)
      {
        if(y==be10loc)
          continue;
        
        TCSMHit *hit = csm->GetHit(y);
        double* CorrVals = CorrParticle(Be10Hit, 10);
        double energydiff = (hit->GetEnergy() - CorrVals[0])/1000.; // MeV
        double thetadiff = (hit->GetPosition().Theta() - CorrVals[1])*180./TMath::Pi(); // Degrees
        double phidiff = (hit->GetPosition().Phi() - CorrVals[2])*180./TMath::Pi(); // Degrees
        
        if(phidiff >= -10 && phidiff <= 10)
        {
          if(energydiff >= -2.5 && energydiff <= .5)
          {
            if(thetadiff >= -3 && thetadiff <= 5)
            {  
              TH2D* temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE10_DualAndPID",hit->GetDetectorNumber()));
              if(temp2) temp2->Fill(hit->GetThetaDeg(),hit->GetEnergyMeV());
              
              double excitec = GetExciteE_Heavy_Corrected(hit,10);
              TH1D* temp1 = (TH1D*)outlist->FindObject(Form("Be10Ex%i_corr_DualAndPID",hit->GetDetectorNumber()));
              if(temp1) temp1->Fill(excitec);
            }
          }
        }
      }
    }
    
    if(x%200000==0)
    {
      printf("ProcessPIDandDual " DYELLOW "%i" RESET_COLOR "/" DBLUE "%i" RESET_COLOR " entries in " DRED "%.02f" RESET_COLOR " seconds\r",x,nentries,w.RealTime());
      fflush(stdout);
      w.Continue();
    }
  }
  printf("ProcessPIDandDual " DYELLOW "%i" RESET_COLOR "/" DBLUE "%i" RESET_COLOR " entries in " DRED "%.02f" RESET_COLOR " seconds\n",nentries,nentries,w.RealTime());
  chain->ResetBranchAddresses();
  delete csm;
  delete tigress;
}

