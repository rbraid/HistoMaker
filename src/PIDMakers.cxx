#include "../include/PIDMakers.hh"

void Process10BePID(TChain* chain,TList* outlist,TList* cutlist,TList *suppList, bool sim)
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

  TH1D *temp1 = 0;
  TH2D *temp2 = 0;
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
          hit->SetIsotope(10,"be");

          temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE10",hit->GetDetectorNumber()));
          if(temp2) temp2->Fill(hit->GetThetaDeg(),hit->GetEnergyMeV());

          double excite = GetExciteE_Heavy(hit,10);
          temp1 = (TH1D*)outlist->FindObject(Form("Be10Ex%i",hit->GetDetectorNumber()));
          if(temp1) temp1->Fill(excite);

          double excitec = GetExciteE_Heavy_Corrected(hit,10);
          temp1 = (TH1D*)outlist->FindObject(Form("Be10Ex%i_corr",hit->GetDetectorNumber()));
          if(temp1) temp1->Fill(excitec);

          temp2 = (TH2D*)outlist->FindObject(Form("Be10Ex%i_corr_v_tigressMult",hit->GetDetectorNumber()));
          if(temp2) temp2->Fill(excitec,tigress->GetAddBackMultiplicity());

          temp2 = (TH2D*)outlist->FindObject(Form("Be10Ex%i_corr_v_csmMult",hit->GetDetectorNumber()));
          if(temp2) temp2->Fill(excitec,csm->GetMultiplicity());

          if(csm->GetMultiplicity() > 1)
          {
            for(int iter = 0; iter <csm->GetMultiplicity(); iter++)
            {
              if(iter != y)
              {
                TCSMHit *otherhit = csm->GetHit(iter);
                temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE10_Fred",otherhit->GetDetectorNumber()));
                if(temp2) temp2->Fill(otherhit->GetThetaDeg(),otherhit->GetEnergyMeV());

                if(otherhit->GetDetectorNumber() == SideAdjacent(hit))
                {
                  temp1 = (TH1D*)outlist->FindObject(Form("Be10Ex%i_corr_adj",hit->GetDetectorNumber()));
                  if(temp1) temp1->Fill(excitec);
                }
                else if(otherhit->GetDetectorNumber() == SideOpposite(hit))
                {
                  temp1 = (TH1D*)outlist->FindObject(Form("Be10Ex%i_corr_opp",hit->GetDetectorNumber()));
                  if(temp1) temp1->Fill(excitec);
                }
              }
            }
          }

          int ring = RingNumber(hit,ringFile);
          int state = GetExState(excitec,10,sim);

          for(int y=0; y<tigress->GetAddBackMultiplicity();y++)
          {
            TTigressHit *tigresshit = tigress->GetAddBackHit(y);

            if(tigresshit->GetCore()->GetEnergy()>10)
            {
              double dopp = Doppler(tigresshit,hit,10);

              temp1 = (TH1D*)outlist->FindObject(Form("Be10_Gamma_%i",hit->GetDetectorNumber()));
              temp1->Fill(tigresshit->GetCore()->GetEnergy()/1000.);
              temp1 = (TH1D*)outlist->FindObject(Form("Be10_Gamma_%i_dopp",hit->GetDetectorNumber()));
              temp1->Fill(dopp);

              if(state != -1)
              {
                temp1 = (TH1D*)outlist->FindObject(Form("Be10_Gamma_s%i",state));
                temp1->Fill(tigresshit->GetCore()->GetEnergy()/1000.);
              }

              double weight = EfficiencyWeight(tigresshit,gammaFile);
              temp1 = (TH1D*)outlist->FindObject(Form("Be10_Gamma_%i_eff",hit->GetDetectorNumber()));
              temp1->Fill(tigresshit->GetCore()->GetEnergy()/1000.,weight);
              temp1 = (TH1D*)outlist->FindObject(Form("Be10_Gamma_%i_dopp_eff",hit->GetDetectorNumber()));
              temp1->Fill(dopp,weight);

              int Gamma = GetGamState(dopp,4);
              if(Gamma >0)
              {
                TH1D* expg = (TH1D*)outlist->FindObject(Form("Be10Ex%i_gcut_%i",hit->GetDetectorNumber(),Gamma));
                if(expg) expg->Fill(excitec);

                TH2D* temp2 = (TH2D*)outlist->FindObject(Form("ExPerRing_10Be_s%i_d%i_pid",Gamma,hit->GetDetectorNumber()));
                if(temp2) temp2->Fill(excitec,ring);
              }
            }
          }

          temp2 = (TH2D*)outlist->FindObject(Form("ExPerRing_10Be_d%i",hit->GetDetectorNumber()));
          if(temp2) temp2->Fill(excitec,ring);

          if(state != -1)
          {
            TH1D* tmpptr = (TH1D*)outlist->FindObject(Form("RingCounts_s%i_d%i_pid",state,hit->GetDetectorNumber()));
            tmpptr->Fill(ring);
          }
        }
      }
    }
    if(x%200000==0)
    {
      printf("Process10BePID " DYELLOW "%i" RESET_COLOR "/" DBLUE "%i" RESET_COLOR " entries in " DRED "%.02f" RESET_COLOR " seconds\r",x,nentries,w.RealTime());
      fflush(stdout);
      w.Continue();
    }
  }
  printf("Process10BePID " DYELLOW "%i" RESET_COLOR "/" DBLUE "%i" RESET_COLOR " entries in " DRED "%.02f" RESET_COLOR " seconds\n",nentries,nentries,w.RealTime());
  chain->ResetBranchAddresses();
  delete csm;
  delete tigress;
}

void Process9BePID(TChain* chain,TList* outlist,TList* cutlist,TList *suppList, bool sim)
{
  TStopwatch w;
  w.Start();

  TCSM *csm =  new TCSM;

  chain->SetBranchAddress("TCSM",&csm);

  TH1D *temp1 = 0;
  TH2D *temp2 = 0;
  TString Be9Cut;
  Be9Cut = "pid_thick_9Be_%i_v10";
  if(sim)
    Be9Cut = "pid_low_thick_9Be_%i_v2_sim";

  TFile* ringFile = (TFile*)suppList->FindObject("inputRootFiles/DumbRings.root");

  int nentries = chain->GetEntries();
  for(int x=0; x<nentries; x++)
  {
    chain->GetEntry(x);

    if(csm->GetMultiplicity()==0)
      continue;

    for(int y=0; y<csm->GetMultiplicity(); y++)
    {
      TCSMHit *hit = csm->GetHit(y);

      if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form(Be9Cut,hit->GetDetectorNumber()))))
      {
        if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()) && hit->GetEEnergy() > 10)
        {
          if(!hit->IsotopeSet())
            hit->SetIsotope(9,"be");

          temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_Be9",hit->GetDetectorNumber()));
          temp2->Fill(hit->GetThetaDeg(),hit->GetEnergyMeV());

          temp1 = (TH1D*)outlist->FindObject(Form("Be9Ex%i",hit->GetDetectorNumber()));
          if(temp1) temp1->Fill(GetExciteE_Heavy(hit,9));

          double ex11c =GetExciteE_Heavy_Corrected(hit,9);
          temp1 = (TH1D*)outlist->FindObject(Form("Be9Ex%i_corr",hit->GetDetectorNumber()));
          if(temp1) temp1->Fill(ex11c);

          double ex9c =GetExciteE_Heavy_Corrected(hit,9);
          int state = GetExState(ex9c,9,sim);

          if(state != -1)
          {

            int ring = RingNumber(hit,ringFile);
            TH1D* tmpptr = (TH1D*)outlist->FindObject(Form("RingCounts_s%i_d%i_9Be",state,hit->GetDetectorNumber()));
            tmpptr->Fill(ring);
          }
        }
      }
    }
    if(x%200000==0)
    {
      printf("Process9BePID " DYELLOW "%i" RESET_COLOR "/" DBLUE "%i" RESET_COLOR " entries in " DRED "%.02f" RESET_COLOR " seconds\r",x,nentries,w.RealTime());
      fflush(stdout);
      w.Continue();
    }
  }
  printf("Process9BePID " DYELLOW "%i" RESET_COLOR "/" DBLUE "%i" RESET_COLOR " entries in " DRED "%.02f" RESET_COLOR " seconds\n",nentries,nentries,w.RealTime());
  chain->ResetBranchAddresses();
  delete csm;
}

void Process11BePID(TChain* chain,TList* outlist,TList* cutlist,TList* suppList, bool sim)
{
  TStopwatch w;
  w.Start();

  TCSM *csm =  new TCSM;
  TTigress *tigress = new TTigress;
  chain->SetBranchAddress("TCSM",&csm);
  if(!sim)
    chain->SetBranchAddress("TTigress",&tigress);

  TH1D *temp1 = 0;
  TH2D *temp2 = 0;
  TString Be11Cut;
  Be11Cut = "pid_thick_11Be_%i_v10";//v1 is elastic only, v2 is everything, v3 is super broad
  if(sim)
    Be11Cut = "pid_sum_thick_11Be_%i_v5_sim";

  TFile* ringFile = (TFile*)suppList->FindObject("inputRootFiles/DumbRings.root");

  int nentries = chain->GetEntries();
  for(int x=0; x<nentries; x++)
  {
    chain->GetEntry(x);

    if(csm->GetMultiplicity()==0)
      continue;

    for(int y=0; y<csm->GetMultiplicity(); y++)
    {
      TCSMHit *hit = csm->GetHit(y);

      if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form(Be11Cut,hit->GetDetectorNumber()))))
      {
        if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()) && hit->GetEEnergy() > 10)
        {
          if(!hit->IsotopeSet())
            hit->SetIsotope(11,"be");

          if(hit->GetDVerticalStrip() >= 0 && hit->GetDHorizontalStrip() >= 0)
          {
            TH2I* temp2INT = (TH2I*)outlist->FindObject(Form("Be11_HitPattern_%iD",hit->GetDetectorNumber()));
            if(temp2INT) temp2INT->Fill(hit->GetDVerticalStrip(),hit->GetDHorizontalStrip());
          }

          if(hit->GetEVerticalStrip() >= 0 && hit->GetEHorizontalStrip() >= 0)
          {
            TH2I* temp2INT = (TH2I*)outlist->FindObject(Form("Be11_HitPattern_%iE",hit->GetDetectorNumber()));
            if(temp2INT) temp2INT->Fill(hit->GetEVerticalStrip(),hit->GetEHorizontalStrip());
          }

          temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_Be11",hit->GetDetectorNumber()));
          if(temp2) temp2->Fill(hit->GetThetaDeg(),hit->GetEnergyMeV());

          temp1 = (TH1D*)outlist->FindObject(Form("Be11Ex%i",hit->GetDetectorNumber()));
          if(temp1) temp1->Fill(GetExciteE_Heavy(hit,11));

          double ex11c =GetExciteE_Heavy_Corrected(hit,11);
          temp1 = (TH1D*)outlist->FindObject(Form("Be11Ex%i_corr",hit->GetDetectorNumber()));
          if(temp1) temp1->Fill(ex11c);

//           if(hit->GetDVerticalStrip() < 2 || hit->GetDVerticalStrip() > 13)
//             continue;
//           if(hit->GetDHorizontalStrip() < 2 || hit->GetDHorizontalStrip() > 13)
//             continue;

          int ring = RingNumber(hit,ringFile);
          temp2 = (TH2D*)outlist->FindObject(Form("ExPerRing_11Be_d%i",hit->GetDetectorNumber()));
          if(temp2) temp2->Fill(ex11c,ring);

          temp2 = (TH2D*)outlist->FindObject(Form("LabThetaPerRing_11Be_d%i",hit->GetDetectorNumber()));
          if(temp2) temp2->Fill(hit->GetThetaDeg(),ring);
          int state = GetExState(ex11c,11,sim);

          if(state != -1)
          {
            TH1D* tmpptr = (TH1D*)outlist->FindObject(Form("RingCounts_s%i_d%i_11Be",state,hit->GetDetectorNumber()));
            tmpptr->Fill(ring);
          }

          for(int y=0; y<tigress->GetAddBackMultiplicity();y++)
          {
            TTigressHit *tigresshit = tigress->GetAddBackHit(y);

            if(tigresshit->GetCore()->GetEnergy()>10)
            {
              double dopp = Doppler(tigresshit,hit,11);

              temp1 = (TH1D*)outlist->FindObject(Form("Be11_Gamma_%i",hit->GetDetectorNumber()));
              temp1->Fill(tigresshit->GetCore()->GetEnergy()/1000.);
              temp1 = (TH1D*)outlist->FindObject(Form("Be11_Gamma_%i_dopp",hit->GetDetectorNumber()));
              temp1->Fill(dopp);

              int Gamma = GetGamState(dopp,4,11);
              if(Gamma >0)
              {
                TH1D* expg = (TH1D*)outlist->FindObject(Form("Be11Ex%i_gcut_%i",hit->GetDetectorNumber(),Gamma));
                if(expg) expg->Fill(ex11c);

                int ring = RingNumber(hit,ringFile);
                TH1D* tmpptr = (TH1D*)outlist->FindObject(Form("RingCounts_d%i_11Be_gtag_%i",hit->GetDetectorNumber(),Gamma));
                tmpptr->Fill(ring);

                temp2 = (TH2D*)outlist->FindObject(Form("ExPerRing_11Be_d%i_gtag_%i",hit->GetDetectorNumber(),Gamma));
                if(temp2) temp2->Fill(ex11c,ring);
              }
            }
          }
        }
      }
    }
    if(x%200000==0)
    {
      printf("Process11BePID " DYELLOW "%i" RESET_COLOR "/" DBLUE "%i" RESET_COLOR " entries in " DRED "%.02f" RESET_COLOR " seconds\r",x,nentries,w.RealTime());
      fflush(stdout);
      w.Continue();
    }
  }
  printf("Process11BePID " DYELLOW "%i" RESET_COLOR "/" DBLUE "%i" RESET_COLOR " entries in " DRED "%.02f" RESET_COLOR " seconds\n",nentries,nentries,w.RealTime());
  chain->ResetBranchAddresses();
  delete csm;
  delete tigress;
}

void Setup10BePIDHistos(TList* hlist)
{
  for(int det = 1; det<=2;det++)
  {
    hlist->Add(new TH2D(Form("ExPerRing_10Be_d%i",det),"Excitation Energy Vs Ring",1400,-10,60,50,0,50));
    TH2D* temp = (TH2D*)hlist->FindObject(Form("ExPerRing_10Be_d%i",det));
    temp->GetXaxis()->SetTitle("Excitation in MeV");
    temp->GetYaxis()->SetTitle("Ring");
  }

  for(int state = 0; state<13; state++)
  {
    if(state != 0 && state != 3 && state != 6 && state!= 7 && state != 9 && state != 12)
      continue;
    hlist->Add(new TH1D(Form("Be10_Gamma_s%i",state),Form("Doppler Corrected Gamma Spectrum, with 10Be in State %i",state),30000,0,30));
    TH1D* temp1 = (TH1D*)hlist->FindObject(Form("Be10_Gamma_s%i",state));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");
  }
}

void Setup11BePIDHistos(TList* hlist)
{
  for(int det = 1; det<=2;det++)
  {
    hlist->Add(new TH2D(Form("ExPerRing_11Be_d%i",det),"Excitation Energy Vs Ring",1400,-10,60,50,0,50));
    TH2D* temp = (TH2D*)hlist->FindObject(Form("ExPerRing_11Be_d%i",det));
    temp->GetXaxis()->SetTitle("Excitation in MeV");
    temp->GetYaxis()->SetTitle("Ring");

    hlist->Add(new TH2D(Form("LabThetaPerRing_11Be_d%i",det),"Lab Theta Vs Ring",180,0,90,50,0,50));
    temp = (TH2D*)hlist->FindObject(Form("LabThetaPerRing_11Be_d%i",det));
    temp->GetXaxis()->SetTitle("Lab Theta in Degrees");
    temp->GetYaxis()->SetTitle("Ring");

    int gammas11[1] = {320};
    for(int gammaiter = 0; gammaiter<1; gammaiter++)
    {
      hlist->Add(new TH2D(Form("ExPerRing_11Be_d%i_gtag_%i",det,gammas11[gammaiter]),"Excitation Energy Vs Ring",1400,-10,60,50,0,50));
      TH2D* tempg = (TH2D*)hlist->FindObject(Form("ExPerRing_11Be_d%i_gtag_%i",det,gammas11[gammaiter]));
      tempg->GetXaxis()->SetTitle("Excitation in MeV");
      tempg->GetYaxis()->SetTitle("Ring");
    }
  }

  for(int det=1;det<=4;det++)
  {
    for(char type='D';type<='E';type++) //Wow I can't believe this works.  I am glad they are in alphabetical order
    {
      if(det>2 && type=='E')//This skips 3 and 4 E, which don't exist
        continue;

      hlist->Add(new TH2I(Form("Be11_HitPattern_%i%c",det,type),Form("Identified 11Be HitPattern, Detector %i, Position %c",det,type),16,0,16,16,0,16));//
      TH2I* temp2INT = (TH2I*)hlist->FindObject(Form("Be11_HitPattern_%i%c",det,type));
      temp2INT->GetXaxis()->SetTitle("Vertical Strip Number");
      temp2INT->GetYaxis()->SetTitle("Horizontal Strip Number");
    }
  }
}