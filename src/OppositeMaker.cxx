#include "../include/OppositeMaker.hh"

void ProcessOpposite(TChain* chain,TList* outlist,TList* cutlist,TList* suppList, bool sim)
{
  TStopwatch w;
  w.Start();

  TTigress *tigress =  new TTigress;
  TCSM *csm =  new TCSM;

  chain->SetBranchAddress("TCSM",&csm);
  if(!sim)
    chain->SetBranchAddress("TTigress",&tigress);

  TFile* ringFile = (TFile*)suppList->FindObject("inputRootFiles/DumbRings.root");
  TFile* gammaFile = (TFile*) suppList->FindObject("inputRootFiles/GammaInfo.root");

  TString Be10Cut;
  Be10Cut = "pid_thick_10Be_%i_v10";
  if(sim)
    Be10Cut = "pid_thick_10Be_sim_%i_v2";

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
              dopptreff->Fill(dopp,EfficiencyWeight(tigresshit,gammaFile));

              int ring = RingNumber(hit,ringFile);
              TH2D* temp2 = (TH2D*)outlist->FindObject(Form("GamPerRing_10Be_d%i_pid_opp",hit->GetDetectorNumber()));
              if(temp2) temp2->Fill(dopp,ring);

              double excitec = GetExciteE_Heavy_Corrected(hit,10);
              int state = GetExState(excitec,10,sim);

              if(state != -1)
              {
                TH1D* temp1 = (TH1D*)outlist->FindObject(Form("Be10_Gamma_s%i_opp",state));
                temp1->Fill(dopp);

                TH2D* gprs = (TH2D*)outlist->FindObject(Form("GamPerRing_10Be_d%i_pid_opp_s%i",hit->GetDetectorNumber(),state));
                if(gprs) gprs->Fill(dopp,ring);
              }

              double centroid = 6.27223;
              double sigma = .539652;
              double nSig = 4.25;

              if(excitec >= centroid - nSig*sigma  && excitec <= centroid + nSig*sigma)
              {
                TH2D* gprs6 = (TH2D*)outlist->FindObject(Form("GamPerRing_10Be_d%i_pid_opp_sBig6",hit->GetDetectorNumber()));
                if(gprs6) gprs6->Fill(dopp,ring);
              }

              if(dopp > 4.8)
              {
                double excitec = GetExciteE_Heavy_Corrected(hit,10);

                TH2D* temp2 = (TH2D*)outlist->FindObject(Form("SIvGAM_d%i_r%i",hit->GetDetectorNumber(),ring));
                if(temp2) temp2->Fill(dopp,excitec);
              }

              int Doppler = GetGamState(dopp,4);

              if(Doppler > 0)
              {
                double excite = GetExciteE_Heavy_Corrected(hit,10);
                TH1D* expg = (TH1D*)outlist->FindObject(Form("Be10Ex%i_gcut_%i_opp",hit->GetDetectorNumber(),Doppler));
                if(expg) expg->Fill(excite);
                TH1D* tp = (TH1D*)outlist->FindObject(Form("RingCounts_d%i_10Be_opp_%i",hit->GetDetectorNumber(),Doppler));
                if(tp) tp->Fill(ring);

                TH2D* temp2 = (TH2D*)outlist->FindObject(Form("ExPerRing_10Be_s%i_d%i_pid_opp",Doppler,hit->GetDetectorNumber()));
                if(temp2) temp2->Fill(excite,ring);
              }
            }
          }
          double excitec = GetExciteE_Heavy_Corrected(hit,10);

          TH2D* temp2 = (TH2D*)outlist->FindObject(Form("Be10Ex%i_corr_v_tigressMult_opp",hit->GetDetectorNumber()));
          if(temp2) temp2->Fill(excitec,tigress->GetAddBackMultiplicity());
        }
      }
    }
    if(x%200000==0)
    {
      printf("ProcessOpposite " DYELLOW "%i" RESET_COLOR "/" DBLUE "%i" RESET_COLOR " entries in " DRED "%.02f" RESET_COLOR " seconds\r",x,nentries,w.RealTime());
      fflush(stdout);
      w.Continue();
    }
  }
  printf("ProcessOpposite " DYELLOW "%i" RESET_COLOR "/" DBLUE "%i" RESET_COLOR " entries in " DRED "%.02f" RESET_COLOR " seconds\n",nentries,nentries,w.RealTime());
  chain->ResetBranchAddresses();
  delete csm;
  delete tigress;
}

void SetupOppositeHistos(TList* hlist)
{
  for(int det = 1; det<=2; det++)
  {
    for(int ring=0; ring<30; ring++)
    {
      hlist->Add(new TH2D(Form("SIvGAM_d%i_r%02i",det,ring),"Excitation Energy Vs Ring",1000,0,10,1500,0,15));
      TH2D* temp = (TH2D*)hlist->FindObject(Form("SIvGAM_d%i_r%02i",det,ring));
      temp->GetXaxis()->SetTitle("Gamma Ray Energy");
      temp->GetYaxis()->SetTitle("Excitation Energy");
    }
  }

  for(int state = 0; state<13; state++)
  {
    if(state != 0 && state != 3 && state != 6 && state!= 7 && state != 9 && state != 12)
      continue;
    hlist->Add(new TH1D(Form("Be10_Gamma_s%i_opp",state),Form("Doppler Corrected (on opposite particle) Gamma Spectrum, with 10Be in State %i",state),30000,0,30));
    TH1D* temp1 = (TH1D*)hlist->FindObject(Form("Be10_Gamma_s%i_opp",state));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");

    for(int det = 1; det<=2; det++)
    {
      hlist->Add(new TH2D(Form("GamPerRing_10Be_d%i_pid_opp_s%i",det,state),Form("Doppler Corrected Energy Vs Ring with PID Detection, based on doppler correction of opposite 10Be, cut on State %i",state),10000,0,10,50,0,50));
      TH2D* temp2 = (TH2D*)hlist->FindObject(Form("GamPerRing_10Be_d%i_pid_opp_s%i",det,state));
      temp2->GetXaxis()->SetTitle("Energy in MeV");
      temp2->GetYaxis()->SetTitle("Ring");
    }
  }

  for(int det = 1; det<=2; det++)
  {
    hlist->Add(new TH2D(Form("GamPerRing_10Be_d%i_pid_opp_sBig6",det),Form("Doppler Corrected Energy Vs Ring with PID Detection, based on doppler correction of opposite 10Be, cut on State Big6"),10000,0,10,50,0,50));
    TH2D* temp2 = (TH2D*)hlist->FindObject(Form("GamPerRing_10Be_d%i_pid_opp_sBig6",det));
    temp2->GetXaxis()->SetTitle("Energy in MeV");
    temp2->GetYaxis()->SetTitle("Ring");
  }
}
