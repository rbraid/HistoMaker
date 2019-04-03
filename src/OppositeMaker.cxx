#include "../include/OppositeMaker.hh"

void ProcessOpposite(TChain* chain,TList* outlist,TList* cutlist,TFile* ringFile, bool sim)
{
  TTigress *tigress =  new TTigress;
  TCSM *csm =  new TCSM;

  TString Be10Cut;
  Be10Cut = "pid_low_thick_10Be_%i_v2";
  if(sim)
    Be10Cut = "pid_low_thick_10Be_%i_sim";
  
  int nentries = chain->GetEntries();
  for(int x=0; x<nentries; x++)
  {
    chain->GetEntry(x);
    
    if(csm->GetMultiplicity()==0)
      continue;
    
    for(int y=0; y<csm->GetMultiplicity(); y++)
    {
      TCSMHit *hit = csm->GetHit(y);
      if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form(Be10Cut,hit->GetDetectorNumber()))))
      {
        if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()) && hit->GetEEnergy() > 10)
        {
          double* CorrVals = CorrParticle(hit, 10);
          
          TH2D* mathptr = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE10_opp_math",hit->GetDetectorNumber()));
          mathptr->Fill(CorrVals[1]*180./TMath::Pi(),CorrVals[0]/1000);
          
          for(int asdf=0; asdf<tigress->GetAddBackMultiplicity();asdf++)
          {
            TTigressHit *tigresshit = tigress->GetAddBackHit(asdf);
            
            if(tigresshit->GetCore()->GetEnergy()>10)
            {
              double dopp = Doppler(tigresshit,CorrVals[0],CorrVals[1],CorrVals[2],10);
              TH1D* dopptr = (TH1D*)outlist->FindObject(Form("Be10_Gamma_%i_dopp_opp_math",hit->GetDetectorNumber()));
              dopptr->Fill(dopp);
              TH1D* dopptreff = (TH1D*)outlist->FindObject(Form("Be10_Gamma_%i_dopp_opp_math_eff",hit->GetDetectorNumber()));
              dopptreff->Fill(dopp,EfficiencyWeight(tigresshit));
              
              int Doppler = GetGamState(dopp);
              
              if(Doppler > 0)
              {
                double excite = GetExciteE_Heavy_Corrected(hit,10);
                TH1D* expg = (TH1D*)outlist->FindObject(Form("Be10Ex%i_gcut_%i_opp",hit->GetDetectorNumber(),Doppler));
                if(expg) expg->Fill(excite);
                TH1D* tp = (TH1D*)outlist->FindObject(Form("RingCounts_d%i_10Be_opp_%i",hit->GetDetectorNumber(),Doppler));
                if(tp) tp->Fill(RingNumber(hit,ringFile));
              }
            }
          }
        }
      }
    }
  }
}

