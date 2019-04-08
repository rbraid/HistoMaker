#include "../include/DualMakers.hh"

void ProcessDual10Be(TChain* chain,TList* outlist,TFile* ringFile, bool sim)
{
  TStopwatch w;
  w.Start();
  
  TTigress *tigress =  new TTigress;
  TCSM *csm =  new TCSM;
  chain->SetBranchAddress("TCSM",&csm);
  if(!sim)
    chain->SetBranchAddress("TTigress",&tigress);
  
  int nentries = chain->GetEntries();
  for(int x=0; x<nentries; x++)
  {
    chain->GetEntry(x);
    
    if(csm->GetMultiplicity()==0)
      continue;
    
    if(csm->GetMultiplicity() == 2)
    {
      TCSMHit *hita;
      TCSMHit *hitb;
      TRandom *rdm = new TRandom(x);
      if(rdm->Integer(2))
      {
        hita = csm->GetHit(0);
        hitb = csm->GetHit(1);
      }
      else
      {
        hita = csm->GetHit(1);
        hitb = csm->GetHit(0);
      }
      delete rdm;
      
      if(hita->GetDetectorNumber() == hitb->GetDetectorNumber())
        continue;
      
      if(hita->IsotopeSet()) //This avoids double counting from the PID spectrum
        continue;
      if(hitb->IsotopeSet())
        continue;
      
      double* CorrVals = CorrParticle(hita, 10);
      
      TH2I *dualn = (TH2I*)outlist->FindObject("Dual10Be_nocut");
      dualn->Fill(hita->GetThetaDeg(),hita->GetEnergyMeV());
      dualn->Fill(hitb->GetThetaDeg(),hitb->GetEnergyMeV());
      double energydiff = (hitb->GetEnergy() - CorrVals[0])/1000.; // MeV
      double thetadiff = (hitb->GetPosition().Theta() - CorrVals[1])*180./TMath::Pi(); // Degrees
      double phidiff = (hitb->GetPosition().Phi() - CorrVals[2])*180./TMath::Pi(); // Degrees
      if(phidiff >= -10 && phidiff <= 10)
      {
        TH2I *dualp = (TH2I*)outlist->FindObject("Dual10Be_phicut");
        dualp->Fill(hita->GetThetaDeg(),hita->GetEnergyMeV());
        dualp->Fill(hitb->GetThetaDeg(),hitb->GetEnergyMeV());
        
        if(thetadiff >= -3 && thetadiff <= 5)
        {
          TH2I *dualt = (TH2I*)outlist->FindObject("Dual10Be_thetacut");
          dualt->Fill(hita->GetThetaDeg(),hita->GetEnergyMeV());
          dualt->Fill(hitb->GetThetaDeg(),hitb->GetEnergyMeV());
        }
        
        if(energydiff >= -2.5 && energydiff <= .5)
        {
          TH2I *duale = (TH2I*)outlist->FindObject("Dual10Be_encut");
          duale->Fill(hita->GetThetaDeg(),hita->GetEnergyMeV());
          duale->Fill(hitb->GetThetaDeg(),hitb->GetEnergyMeV());
          
          if(thetadiff >= -3 && thetadiff <= 5)
          {            
            TH2I *duala = (TH2I*)outlist->FindObject("Dual10Be_allcut");
            duala->Fill(hita->GetThetaDeg(),hita->GetEnergyMeV());
            duala->Fill(hitb->GetThetaDeg(),hitb->GetEnergyMeV());
            
            TH2I *dualac = (TH2I*)outlist->FindObject("Dual10Be_allcut_corrected");
            dualac->Fill(hita->GetThetaDeg(),hita->GetCorrectedEnergyMeV("10be"));
            dualac->Fill(hitb->GetThetaDeg(),hitb->GetCorrectedEnergyMeV("10be"));
            
            double excitecA = GetExciteE_Heavy_Corrected(hita,10);
            double excitecB = GetExciteE_Heavy_Corrected(hitb,10);
            
            TH1I *dualex = (TH1I*)outlist->FindObject("DualBe10_ex_allcut");
            dualex->Fill(excitecA);
            dualex->Fill(excitecB);
            
            
            TH1I *gamptr = (TH1I*)outlist->FindObject("DualBe10_allcut_gammas");
            TH1I *gamptrs = (TH1I*)outlist->FindObject("DualBe10_allcut_gammas_supp");
            TH1I *gamptrh = (TH1I*)outlist->FindObject("DualBe10_allcut_gammas_dopp_high");
            TH1I *gamptrl = (TH1I*)outlist->FindObject("DualBe10_allcut_gammas_dopp_low");
            TH1I *gamptreff = (TH1I*)outlist->FindObject("DualBe10_allcut_gammas_eff");
            TH1I *gamptrheff = (TH1I*)outlist->FindObject("DualBe10_allcut_gammas_dopp_high_eff");
            TH1I *gamptrleff = (TH1I*)outlist->FindObject("DualBe10_allcut_gammas_dopp_low_eff");
            TCSMHit* Hhit;
            TCSMHit* Lhit;
            
            if(hita->GetEnergyMeV()>=hitb->GetEnergyMeV())
            {
              Hhit = hita;
              Lhit = hitb;
            }
            else
            {
              Hhit = hitb;
              Lhit = hita;
            }
            
            TH1I *dualexh = (TH1I*)outlist->FindObject("DualBe10_ex_allcut_highonly");
            dualexh->Fill(GetExciteE_Heavy_Corrected(Hhit,10));
            TH1I *dualexl = (TH1I*)outlist->FindObject("DualBe10_ex_allcut_lowonly");
            dualexl->Fill(GetExciteE_Heavy_Corrected(Lhit,10));
            
            for(int y=0; y<tigress->GetAddBackMultiplicity();y++)
            {
              TTigressHit *tigresshit = tigress->GetAddBackHit(y);
              
              if(tigresshit->GetCore()->GetEnergy()>10)
              {
                gamptr->Fill(tigresshit->GetCore()->GetEnergy()/1000.);
                if(!tigresshit->Suppress())
                  gamptrs->Fill(tigresshit->GetCore()->GetEnergy()/1000.);
                gamptrh->Fill(Doppler(tigresshit,Hhit,10));
                gamptrl->Fill(Doppler(tigresshit,Lhit,10));
                
                gamptreff->Fill(tigresshit->GetCore()->GetEnergy()/1000.,EfficiencyWeight(tigresshit));
                gamptrheff->Fill(Doppler(tigresshit,Hhit,10),EfficiencyWeight(tigresshit));
                gamptrleff->Fill(Doppler(tigresshit,Lhit,10),EfficiencyWeight(tigresshit));
                
                double doppH = Doppler(tigresshit,Hhit,10);
                double doppL = Doppler(tigresshit,Lhit,10);
                                
                int DopplerIH = GetGamState(doppH);
                int DopplerIL = GetGamState(doppL);
                
                if(DopplerIH > 0)
                {
                  TH1I* dualexgcuth = (TH1I*)outlist->FindObject(Form("DualBe10Ex_gcut_%i_high",DopplerIH));
                  if(dualexgcuth) dualexgcuth->Fill(GetExciteE_Heavy_Corrected(Hhit,10));
                }
                if(DopplerIL > 0)
                {
                  TH1I* dualexgcutl = (TH1I*)outlist->FindObject(Form("DualBe10Ex_gcut_%i_low",DopplerIL));
                  if(dualexgcutl) dualexgcutl->Fill(GetExciteE_Heavy_Corrected(Lhit,10));
                }
              }
            }         
            
            double ex10cA =GetExciteE_Heavy_Corrected(hita,10);
            double ex10cB =GetExciteE_Heavy_Corrected(hitb,10);
                        
            int stateA = GetExState(ex10cA,10,sim);
            int stateB = GetExState(ex10cB,10,sim);
            
            if(stateA != -1)
            {              
              int ring = RingNumber(hita,ringFile);
              
              TH1D* tmpptr = (TH1D*)outlist->FindObject(Form("RingCounts_s%i_d%i_dual",stateA,hita->GetDetectorNumber()));
              if(tmpptr) tmpptr->Fill(ring);
            }
            if(stateB != -1)
            {              
              int ring = RingNumber(hitb,ringFile);
              
              TH1D* tmpptr = (TH1D*)outlist->FindObject(Form("RingCounts_s%i_d%i_dual",stateB,hitb->GetDetectorNumber()));
              if(tmpptr) tmpptr->Fill(ring);
            }
          }
        }
      }
    }
    if(x%200000==0)
    {
      printf("ProcessDual10Be " DYELLOW "%i" RESET_COLOR "/" DBLUE "%i" RESET_COLOR " entries in " DRED "%.02f" RESET_COLOR " seconds\r",x,nentries,w.RealTime());
      fflush(stdout);
      w.Continue();
    }
  }
  printf("ProcessDual10Be " DYELLOW "%i" RESET_COLOR "/" DBLUE "%i" RESET_COLOR " entries in " DRED "%.02f" RESET_COLOR " seconds\n",nentries,nentries,w.RealTime());
  chain->ResetBranchAddresses();
  delete tigress;
  delete csm;
}

void ProcessDualElastic(TChain* chain,TList* outlist,TFile* ringFile, bool sim)
{
  TStopwatch w;
  w.Start();
  
//   TTigress *tigress =  new TTigress;
  TCSM *csm =  new TCSM;
  chain->SetBranchAddress("TCSM",&csm);
  
  int nentries = chain->GetEntries();
  for(int x=0; x<nentries; x++)
  {
    chain->GetEntry(x);
    
    if(csm->GetMultiplicity()==0)
      continue;
    
    if(csm->GetMultiplicity() == 2)
    {
      TCSMHit *hita;
      TCSMHit *hitb;
      TRandom *rdm = new TRandom(x);
      if(rdm->Integer(2))
      {
        hita = csm->GetHit(0);
        hitb = csm->GetHit(1);
      }
      else
      {
        hita = csm->GetHit(1);
        hitb = csm->GetHit(0);
      }
      delete rdm;
      
      if(hita->GetDetectorNumber() == hitb->GetDetectorNumber())
        continue;
      
      if(hita->IsotopeSet()) //This avoids double counting from the PID spectrum
        continue;
      if(hitb->IsotopeSet())
        continue;
      
      for(int iso = 9; iso <=11; iso+=2)
      {
        int aNum = iso;
        int bNum = -1;
        if(aNum == 11)
          bNum = 9;
        else if(aNum == 9)
          bNum = 11;
        else
          cout<<"Erorr: iso = "<<iso<<endl;
        
        if(hita->GetEnergy() > hitb->GetEnergy())
        {
          TCSMHit *tmp = hita;
          hita = hitb;
          hitb = tmp;
        }
        
        double* CorrVals = CorrParticle(hita, aNum);
        double energydiff = (hitb->GetEnergy() - CorrVals[0])/1000.; // MeV
        double thetadiff = (hitb->GetPosition().Theta() - CorrVals[1])*180./TMath::Pi(); // Degrees
        double phidiff = (hitb->GetPosition().Phi() - CorrVals[2])*180./TMath::Pi(); // Degrees
        
        if(phidiff >= -10 && phidiff <= 10)
        {
          if(energydiff >= -2.5 && energydiff <= .5)
          {
            if(thetadiff >= -3 && thetadiff <= 5)
            {
              //               hita->SetIsotope(aNum,"Be");
              //               hitb->SetIsotope(bNum,"Be");
              TH2D* tmpptr2d = (TH2D*)outlist->FindObject(Form("EvTheta_%i_%iBe_corr",hita->GetDetectorNumber(),aNum));
              if(tmpptr2d) tmpptr2d->Fill(hita->GetThetaDeg(),hita->GetEnergyMeV());
              tmpptr2d = (TH2D*)outlist->FindObject(Form("EvTheta_%i_%iBe_corr",hitb->GetDetectorNumber(),bNum));
              if(tmpptr2d) tmpptr2d->Fill(hitb->GetThetaDeg(),hitb->GetEnergyMeV());
              
              double excA =GetExciteE_Heavy_Corrected(hita,aNum);
              double excB =GetExciteE_Heavy_Corrected(hitb,bNum);
              
              int stateA = GetExState(excA,11,sim);
              int stateB = GetExState(excB,11,sim);
              
              if(stateA != -1)
              {
                int ring = RingNumber(hita,ringFile);                  
                TH1D* tmpptr = (TH1D*)outlist->FindObject(Form("RingCounts_s%i_d%i_%iBe_corr",stateA,hita->GetDetectorNumber(),aNum));
                if(tmpptr) tmpptr->Fill(ring);
              }
              if(stateB != -1)
              {
                int ring = RingNumber(hitb,ringFile);
                TH1D* tmpptr = (TH1D*)outlist->FindObject(Form("RingCounts_s%i_d%i_%iBe_corr",stateB,hitb->GetDetectorNumber(),bNum));
                if(tmpptr) tmpptr->Fill(ring);
              }
            }
          }
        }
      }
    }
    if(x%200000==0)
    {
      printf("ProcessDualElastic " DYELLOW "%i" RESET_COLOR "/" DBLUE "%i" RESET_COLOR " entries in " DRED "%.02f" RESET_COLOR " seconds\r",x,nentries,w.RealTime());
      fflush(stdout);
      w.Continue();
    }
  }
  printf("ProcessDualElastic " DYELLOW "%i" RESET_COLOR "/" DBLUE "%i" RESET_COLOR " entries in " DRED "%.02f" RESET_COLOR " seconds\n",nentries,nentries,w.RealTime());
  chain->ResetBranchAddresses();
  delete csm;
}