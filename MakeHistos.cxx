// g++ -g -O3 MakeHistos.cxx FunctionsForMakeHistos.cxx -Wl,--no-as-needed -o RunMe `grsi-config --cflags --all-libs --root`

#include "FunctionsForMakeHistos.hh"

bool SIMULATED_DATA;

TFile* ringFile;
TFile* edgeFile;
TFile* SAFile;

TTigress *tigress =  new TTigress;
TCSM *csm =  new TCSM;
TList *cutlist = new TList;

void ProcessChain(TChain *chain,TList *outlist)//, MakeFriend *myFriend)
{
  int nentries = chain->GetEntries();
  TStopwatch w;
  w.Start();
  
  
  TString Be12Cut;
  TString Be11Cut;
  TString Be10Cut;
  TString Be9Cut;
  TString He4Cut;
  TString He6Cut;
  
  Be12Cut = "pid_low_thick_12Be_%i_v2";
  Be11Cut = "pid_low_thick_11Be_%i_v2";//v1 is elastic only, v2 is everything
  if(SIMULATED_DATA)
    Be11Cut = "pid_low_thick_11Be_%i_v2_sim";
  Be10Cut = "pid_low_thick_10Be_%i_v2";
  if(SIMULATED_DATA)
    Be10Cut = "pid_low_thick_10Be_%i_sim";
  
  Be9Cut = "pid_low_thick_9Be_%i_v1";
  if(SIMULATED_DATA)
    Be9Cut = "pid_low_thick_9Be_%i_v1_sim";
  
  He4Cut = "pid_low_thick_4He_%i_v1";
  He6Cut = "pid_low_thick_6He_%i_v1";
  
  
  
  
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
    
    
    int hits[4] = {0};
    for(int y=0; y<csm->GetMultiplicity(); y++)
    {
      hits[csm->GetHit(y)->GetDetectorNumber()-1]++;
    }
    
    for(int y=0; y<csm->GetMultiplicity(); y++)
    {
      if(DEBUG)
      {
        cout<<"Main Get Multiplicity()"<<endl;
      }
      //***********************
      //        General
      //***********************
      
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
          
          for(int y=0; y<tigress->GetAddBackMultiplicity();y++)
          {
            TTigressHit *tigresshit = tigress->GetAddBackHit(y);
            
            if(tigresshit->GetCore()->GetEnergy()>10)
            {
              temp1 = (TH1D*)outlist->FindObject(Form("Be10_Gamma_%i",hit->GetDetectorNumber()));
              temp1->Fill(tigresshit->GetCore()->GetEnergy()/1000.);
              temp1 = (TH1D*)outlist->FindObject(Form("Be10_Gamma_%i_dopp",hit->GetDetectorNumber()));
              temp1->Fill(Doppler(tigresshit,hit,10));
              
              temp1 = (TH1D*)outlist->FindObject(Form("Be10_Gamma_%i_eff",hit->GetDetectorNumber()));
              temp1->Fill(tigresshit->GetCore()->GetEnergy()/1000.,EfficiencyWeight(tigresshit));
              temp1 = (TH1D*)outlist->FindObject(Form("Be10_Gamma_%i_dopp_eff",hit->GetDetectorNumber()));
              temp1->Fill(Doppler(tigresshit,hit,10),EfficiencyWeight(tigresshit));
              
              double dopp = Doppler(tigresshit,hit,10);
              int Gamma = GetGamState(dopp);
              if(Gamma >0)
              {
                TH1D* expg = (TH1D*)outlist->FindObject(Form("Be10Ex%i_gcut_%i",hit->GetDetectorNumber(),Gamma));
                if(expg) expg->Fill(tigresshit->GetCore()->GetEnergy()/1000.);
              }
            }
          }
          
          double ex10c =GetExciteE_Heavy_Corrected(hit,10);
          if(DEBUG) cout<<"Have 10Be Excite"<<endl;
          
          int state = GetExState(ex10c,10);

          if(state != -1)
          {
            if(DEBUG) cout<<"Have 10Be state assigned"<<endl;
            int ring = RingNumber(hit);
            
            TH1D* tmpptr = (TH1D*)outlist->FindObject(Form("RingCounts_s%i_d%i_pid",state,hit->GetDetectorNumber()));   
            tmpptr->Fill(ring);           
          }
        }
      }
      //////////////////////////////////////////////////
      if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form(Be11Cut,hit->GetDetectorNumber()))))
      {
        if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()) && hit->GetEEnergy() > 10)
        {
          if(!hit->IsotopeSet())
            hit->SetIsotope(11,"be");
          
          temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_Be11",hit->GetDetectorNumber()));
          if(temp2) temp2->Fill(hit->GetThetaDeg(),hit->GetEnergyMeV());
          
          temp1 = (TH1D*)outlist->FindObject(Form("Be11Ex%i",hit->GetDetectorNumber()));
          if(temp1) temp1->Fill(GetExciteE_Heavy(hit,11));
          
          double ex11c =GetExciteE_Heavy_Corrected(hit,11);
          temp1 = (TH1D*)outlist->FindObject(Form("Be11Ex%i_corr",hit->GetDetectorNumber()));
          if(temp1) temp1->Fill(ex11c);
          
          if(DEBUG) cout<<"Have 11Be"<<endl;
          int state = GetExState(ex11c,11);

          if(state != -1)
          {
            if(DEBUG) cout<<"Looking for "<<Form("RingCounts_s%i_d%i_11Be",state,hit->GetDetectorNumber())<<endl;
            
            int ring = RingNumber(hit);
            TH1D* tmpptr = (TH1D*)outlist->FindObject(Form("RingCounts_s%i_d%i_11Be",state,hit->GetDetectorNumber()));   
            tmpptr->Fill(ring);
          }
        }
      }    
      
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
          if(DEBUG) cout<<"Have 9Be"<<endl;
          int state = GetExState(ex9c,9);
          
          if(state != -1)
          {
            if(DEBUG) cout<<"Looking for "<<Form("RingCounts_s%i_d%i_9Be",state,hit->GetDetectorNumber())<<endl;
            
            int ring = RingNumber(hit);
            TH1D* tmpptr = (TH1D*)outlist->FindObject(Form("RingCounts_s%i_d%i_9Be",state,hit->GetDetectorNumber()));   
            tmpptr->Fill(ring);
          }
        }
      }     
      
      if(DEBUG) cout<<"PID"<<endl;
      if(hit->GetEEnergy()>0 && hit->GetDEnergy()>0)
      {
        temp2 = (TH2D*)outlist->FindObject(Form("pid_%i",hit->GetDetectorNumber()));
        if(temp2) temp2->Fill(hit->GetEEnergy()/1000.,hit->GetDEnergy()/1000.);
        if(hit->GetDthickness()>5)
        {
          if(hit->GetDEnergy()>0 && hit->GetEEnergy()>0)
          {
            temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_summed_thickness",hit->GetDetectorNumber()));
            if(temp2) temp2->Fill(hit->GetEnergyMeV(),hit->GetDdE_dx());
          }
        }
      }
      
      if(hit->GetDetectorNumber()<3)
      {
        temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%iTotal",hit->GetDetectorNumber()));
        temp2->Fill(hit->GetThetaDeg(),hit->GetEnergyMeV());
      }
    }
    
    //***********************
    //        Gammas
    //***********************
    if(DEBUG) cout<<"Gammas"<<endl;
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
    
    //***********************
    //  Other 10Be
    //***********************
    for(int i =0; i<csm->GetMultiplicity();i++)
    {	
      TCSMHit *hit = csm->GetHit(i);
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
                if(tp) tp->Fill(RingNumber(hit));
              }
            }
          }
        }
      }
    }
    
    //***********************
    //   End of event loops
    //***********************
    
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
      
      if(hita->GetDetectorNumber() > 2 || hitb->GetDetectorNumber() > 2)
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
            if(DEBUG) cout<<"all diffs"<<endl;
            
//             hita->SetIsotope("10Be");
//             hitb->SetIsotope("10Be");
            
            TH2I *duala = (TH2I*)outlist->FindObject("Dual10Be_allcut");
            duala->Fill(hita->GetThetaDeg(),hita->GetEnergyMeV());
            duala->Fill(hitb->GetThetaDeg(),hitb->GetEnergyMeV());
            
            TH2I *dualac = (TH2I*)outlist->FindObject("Dual10Be_allcut_corrected");
            dualac->Fill(hita->GetThetaDeg(),hita->GetCorrectedEnergyMeV("10be"));
            dualac->Fill(hitb->GetThetaDeg(),hitb->GetCorrectedEnergyMeV("10be"));
            
            double excitecA = GetExciteE_Heavy_Corrected(hita,10);
            double excitecB = GetExciteE_Heavy_Corrected(hitb,10);
            if(DEBUG) cout<<"excites"<<endl;
            
            TH1I *dualex = (TH1I*)outlist->FindObject("DualBe10_ex_allcut");
            dualex->Fill(excitecA);
            dualex->Fill(excitecB);
            
            if(DEBUG) cout<<"before gamptrs"<<endl;
            
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
            if(DEBUG) cout<<"before tigressloop"<<endl;
            
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
                
                if(DEBUG) cout<<"after dopp"<<endl;
                
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
            
            
            if(DEBUG) cout<<"Have Correlated 2x 10Be"<<endl;
            
            double ex10cA =GetExciteE_Heavy_Corrected(hita,10);
            double ex10cB =GetExciteE_Heavy_Corrected(hitb,10);
            
            if(DEBUG) cout<<"Have Excited States"<<endl;
                        
            int stateA = GetExState(ex10cA);
            int stateB = GetExState(ex10cB);
            
            if(stateA != -1)
            {
              if(DEBUG) cout<<"Have State A "<<stateA<<endl;
              
              int ring = RingNumber(hita);
              
              TH1D* tmpptr = (TH1D*)outlist->FindObject(Form("RingCounts_s%i_d%i_dual",stateA,hita->GetDetectorNumber()));
              tmpptr->Fill(ring);
            }
            if(stateB != -1)
            {
              if(DEBUG) cout<<"Have State B "<<stateB<<endl;
              
              int ring = RingNumber(hitb);
              
              TH1D* tmpptr = (TH1D*)outlist->FindObject(Form("RingCounts_s%i_d%i_dual",stateB,hitb->GetDetectorNumber()));
              tmpptr->Fill(ring);
            }
          }
        }
      }
      if(DEBUG) cout<<"Below PID 11Be"<<endl;
      for(int iso = 9; iso <=11; iso+=2)
      {
        int aNum = iso;
        int bNum = -1;
        if(aNum = 11)
          bNum = 9;
        else if(aNum = 9)
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
              tmpptr2d->Fill(hita->GetThetaDeg(),hita->GetEnergyMeV());
              tmpptr2d = (TH2D*)outlist->FindObject(Form("EvTheta_%i_%iBe_corr",hitb->GetDetectorNumber(),bNum));
              tmpptr2d->Fill(hitb->GetThetaDeg(),hitb->GetEnergyMeV());
              
              double excA =GetExciteE_Heavy_Corrected(hita,aNum);
              double excB =GetExciteE_Heavy_Corrected(hitb,bNum);
              
              int stateA = GetExState(excA,11);
              int stateB = GetExState(excB,11);
              
              if(stateA != -1)
              {
                int ring = RingNumber(hita);                  
                TH1D* tmpptr = (TH1D*)outlist->FindObject(Form("RingCounts_s%i_d%i_%iBe_corr",stateA,hita->GetDetectorNumber(),aNum));
                tmpptr->Fill(ring);
              }
              if(stateB != -1)
              {
                int ring = RingNumber(hitb);
                TH1D* tmpptr = (TH1D*)outlist->FindObject(Form("RingCounts_s%i_d%i_%iBe_corr",stateB,hitb->GetDetectorNumber(),bNum));
                tmpptr->Fill(ring);
              }
            }
          }
        }
      }
    }
    
    
    if(x%20000==0)
    {
      printf("\tprocessed " DYELLOW "%i" RESET_COLOR "/" DBLUE "%i" RESET_COLOR " entries in " DRED "%.02f" RESET_COLOR " seconds\r",x,nentries,w.RealTime());
      fflush(stdout);
      w.Continue();
    }
  }
  
  cout<<endl;  
}



int main(int argc, char **argv)
{
  if(argc<2)
  {
    printf("try adding analysis trees to the command line.\n");
    return 1;
  }
  
  int i;
  
  if(strcmp(argv[1], "--real") == 0 || strcmp(argv[1], "-r") == 0)
  {
    i = 2;
    SIMULATED_DATA = false;
  }
  else if(strcmp(argv[1], "--sim") == 0 || strcmp(argv[1], "-s") == 0)
  {
    i = 2;
    SIMULATED_DATA = true;
  }
  else
  {
    cerr<<"Error: no option set for SIMULATED_DATA.  Use flag --real or --sim"<<endl;
    SIMULATED_DATA = false;
  }
  
  TString outputname = "default.root";
  if(strstr(argv[2],"--out=") != NULL)
  {
    i = 3;
    string tmp = argv[2];
    //     cout<<"Looking at: "<<argv[4]<<endl;
    //     cout<<"Setting outputname to: "<<tmp.substr(6)<<endl;
    outputname = tmp.substr(6);
  }
  
  
  TApplication *app = new TApplication("app",0,0);
  TFile cf("cuts.root");
  TIter *iter = new TIter(cf.GetListOfKeys());
  
  int ncuts = 0;
  
  while(TObject *obj = iter->Next())
  {
    obj = ((TKey *)obj)->ReadObj();
    
    if(strcmp(obj->ClassName(),"TCutG")!=0)
    {
      continue;
    }
    
    cutlist->Add(obj);
    ncuts++;
  }
  
  if(ncuts==0)
    cout<<RED;
  else
    cout<<DGREEN;
  
  cout<<"Found "<<ncuts<<" cuts."<<RESET_COLOR<<endl;
  
  TChain *chain = new TChain("AnalysisTree");
  
  while(i<argc)
  {
    chain->Add(argv[i++]);
  }
  
  if(outputname != "default.root")
    printf("%i analysis trees added to chain.\n",i-3);
  else
    printf("%i analysis trees added to chain.\n",i-2);
  if(!SIMULATED_DATA)
    chain->SetBranchAddress("TTigress",&tigress);
  chain->SetBranchAddress("TCSM",&csm);
  
  ringFile = TFile::Open("DumbRings.root","read");
  edgeFile = TFile::Open("edge.root","read");
  SAFile = TFile::Open("solidAngleDiag.root","read");
  
  TList *outlist = new TList;
  SetupHistos(outlist);
  ProcessChain(chain,outlist);
  outlist->Sort();
  
  if(DEBUG)
  {
    cout<<"Done Sorting"<<endl;
  }
  
  if(!DEBUG)
  {    
    TH1* hist = (TH1*)outlist->First();
    TH1* nexthist;// = (TH1*)outlist->After(outlist->First());
    while(hist)
    {
      nexthist = (TH1*)outlist->After(hist);
      if(hist->GetEntries() < 1)
      {
        outlist->Remove(hist);
      }
      hist = nexthist;
    }
  }
  
  if(outputname != "default.root")
    cout<<"Setting output name to: "<<outputname<<endl;
  else
  {
    if(SIMULATED_DATA)
      outputname = "simoutput.root";
    else
      outputname = "output.root";
  }
  
  TFile f(outputname,"recreate");
  f.cd();
  outlist->Write();
  f.Close();
  
  if(DEBUG)
  {
    cout<<"All done"<<endl;
  }
  
  return 0;
}





