// g++ -g -O3 MakeHistos.cxx FunctionsForMakeHistos.cxx -Wl,--no-as-needed -o RunMe `grsi-config --cflags --all-libs --root`

#include "FunctionsForMakeHistos.hh"

double BEAM_ENERGY;
bool ANGULAR_DISTRIBUTION;
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
  
  if(int(BEAM_ENERGY) == 55)
  {
    Be12Cut = "pid_high_thick_12Be_%i_v3";
    Be11Cut = "pid_high_thick_11Be_%i_v2";//v1 is elastic only, v2 is everything
    Be10Cut = "pid_high_thick_10Be_%i_v1";
    Be9Cut = "pid_high_thick_9Be_%i_v1";
    
    He4Cut = "pid_high_thick_4He_%i_v1";
    He6Cut = "pid_high_thick_6He_%i_v1";
  }
  else if(int(BEAM_ENERGY) == 30)
  {
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
  }
  else
  {
    cerr<<"Something is wrong with setting cuts.  Beam energy unrecognized, reverting to high energy"<<endl;
    Be12Cut = "pid_high_thick_12Be_%i_v3";
    Be11Cut = "pid_high_thick_11Be_%i_v2";//v1 is elastic only, v2 is everything
    Be10Cut = "pid_high_thick_10Be_%i_v1";
    Be9Cut = "pid_high_thick_9Be_%i_v1";
    
    He4Cut = "pid_high_thick_4He_%i_v1";
    He6Cut = "pid_high_thick_6He_%i_v1";
  }
  
  
  
  if(DEBUG)
  {
    cout<<"Starting ProcessChain"<<endl;
  }
  
  for(int x=0; x<nentries; x++)
  {
    chain->GetEntry(x);
    
    if(!ANGULAR_DISTRIBUTION)
    {      
      if(DEBUG)
      {
        cout<<"nentries: "<<nentries<<endl;
      }
      
      if(csm->GetMultiplicity()==0)
        continue;
      
      ((TH1D *)outlist->FindObject("Multiplicity"))->Fill(csm->GetMultiplicity());
      
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
            temp2->Fill(hit->GetThetaDeg(),hit->GetEnergyMeV());
            
            temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE10_corrected",hit->GetDetectorNumber()));
            temp2->Fill(hit->GetThetaDeg(),hit->GetCorrectedEnergyMeV("10Be"));
            
            TH1I* multpointer = (TH1I*)outlist->FindObject(Form("Be10Mult_%i",hit->GetDetectorNumber()));
            multpointer->Fill(csm->GetMultiplicity());
            
            double excite = GetExciteE_Heavy(hit,10);
            temp1 = (TH1D*)outlist->FindObject(Form("Be10Ex%i",hit->GetDetectorNumber()));
            if(temp1) temp1->Fill(excite);
            double excitec = GetExciteE_Heavy_Corrected(hit,10);
            
            temp2INT = (TH2I*)outlist->FindObject(Form("Be10Ex_Vs_Theta_%i",hit->GetDetectorNumber()));
            temp2INT->Fill(excitec,hit->GetThetaDeg());
            
            for(int y=0; y<tigress->GetAddBackMultiplicity();y++)
            {
              TTigressHit *tigresshit = tigress->GetAddBackHit(y);
              
              if(tigresshit->GetCore()->GetEnergy()>10)
              {
                double dopp = Doppler(tigresshit,hit,10);
                
                if(dopp>=2.577 && dopp<=2.612)
                {
                  TH1D* expg = (TH1D*)outlist->FindObject(Form("Be10Ex%i_gcut_2589",hit->GetDetectorNumber()));
                  expg->Fill(excitec);
                }
                else if(dopp>=2.876 && dopp<=2.913)
                {
                  TH1D* expg = (TH1D*)outlist->FindObject(Form("Be10Ex%i_gcut_2894",hit->GetDetectorNumber()));
                  expg->Fill(excitec);
                }
                else if(dopp>=3.337 && dopp<=3.402)
                {
                  TH1D* expg = (TH1D*)outlist->FindObject(Form("Be10Ex%i_gcut_3368",hit->GetDetectorNumber()));
                  expg->Fill(excitec);
                }
                else if(dopp>=5.951 && dopp<=5.986)
                {
                  TH1D* expg = (TH1D*)outlist->FindObject(Form("Be10Ex%i_gcut_5958",hit->GetDetectorNumber()));
                  expg->Fill(excitec);
                }
              }
            }
            
            if(excite>-.5 && excite<1.5)
            {
              temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE10_exCut_gs",hit->GetDetectorNumber()));
              if(temp2) temp2->Fill(hit->GetThetaDeg(),hit->GetEnergyMeV());
              
              TVector3 COMinfo = CalcCOMmomentum(hit, 10);
              temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE10_exCut_gs_COM",hit->GetDetectorNumber()));
              if(temp2) temp2->Fill(COMinfo.Theta()*180./TMath::Pi(),COMinfo.Mag2()/(2.*MASS_BE10));
              
              temp2 = (TH2D*)outlist->FindObject(Form("Be10_TvTCOM_%i",hit->GetDetectorNumber()));
              temp2->Fill(COMinfo.Theta()*180./TMath::Pi(),hit->GetThetaDeg());
            }
            
            temp1 = (TH1D*)outlist->FindObject(Form("Be10Ex%i_corr",hit->GetDetectorNumber()));
            if(temp1) temp1->Fill(GetExciteE_Heavy_Corrected(hit,10));
            
            temp2INT = (TH2I*)outlist->FindObject(Form("Be10Ex%i_mult",hit->GetDetectorNumber()));
            temp2INT->Fill(GetExciteE_Heavy_Corrected(hit,10),csm->GetMultiplicity());
            
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
              }
            }
            
            
          }
        }
        //////////////////////////////////////////////////
        if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form(Be11Cut,hit->GetDetectorNumber()))))
        {
          if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()) && hit->GetEEnergy() > 10)
          {
            temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_Be11",hit->GetDetectorNumber()));
            temp2->Fill(hit->GetThetaDeg(),hit->GetEnergyMeV());
            
            temp1 = (TH1D*)outlist->FindObject(Form("Be11Ex%i",hit->GetDetectorNumber()));
            if(temp1) temp1->Fill(GetExciteE_Heavy(hit,11));
            
            double ex11c =GetExciteE_Heavy_Corrected(hit,11);
            temp1 = (TH1D*)outlist->FindObject(Form("Be11Ex%i_corr",hit->GetDetectorNumber()));
            if(temp1) temp1->Fill(ex11c);
            
            if(ex11c >= -1 && ex11c<= 1)
            {
              temp2INT = (TH2I*)outlist->FindObject(Form("Be11_%i_EvTCOM",hit->GetDetectorNumber()));
              temp2INT->Fill(CalcCOMThetaDeg(hit,11),CalcCOMEnergyMeV(hit,11));
              
              temp2INT = (TH2I*)outlist->FindObject(Form("Be11_TvTCOM_%i",hit->GetDetectorNumber()));
              temp2INT->Fill(CalcCOMThetaDeg(hit,11),hit->GetThetaDeg());
              
              temp2INT = (TH2I*)outlist->FindObject(Form("Sigma_11Be_d%i_gs",hit->GetDetectorNumber()));
              temp2INT->Fill(hit->GetDVerticalStrip(),hit->GetDHorizontalStrip());
            }
            
            temp2INT = (TH2I*)outlist->FindObject(Form("Be11Ex%i_mult",hit->GetDetectorNumber()));
            temp2INT->Fill(GetExciteE_Heavy_Corrected(hit,11),csm->GetMultiplicity());
          }
        }    
        
        if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form(Be9Cut,hit->GetDetectorNumber()))))
        {
          if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()) && hit->GetEEnergy() > 10)
          {
            temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_Be9",hit->GetDetectorNumber()));
            temp2->Fill(hit->GetThetaDeg(),hit->GetEnergyMeV());
            
            temp1 = (TH1D*)outlist->FindObject(Form("Be9Ex%i",hit->GetDetectorNumber()));
            if(temp1) temp1->Fill(GetExciteE_Heavy(hit,9));
            
            double ex11c =GetExciteE_Heavy_Corrected(hit,9);
            temp1 = (TH1D*)outlist->FindObject(Form("Be9Ex%i_corr",hit->GetDetectorNumber()));
            if(temp1) temp1->Fill(ex11c);
          }
        }     
        
        if(DEBUG) cout<<"EVTheta"<<endl;
        temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%iD",hit->GetDetectorNumber()));
        temp2->Fill(hit->GetThetaDeg(),hit->GetDEnergy()/1000.);
        
        temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%iE",hit->GetDetectorNumber()));
        if(temp2) temp2->Fill(hit->GetEPosition().Theta()*180/TMath::Pi(),hit->GetEEnergy()/1000.);
        if(DEBUG) cout<<"HitPattern"<<endl;

        if(hit->GetDEnergy()>500)
        {
          temp2INT = (TH2I*)outlist->FindObject(Form("CSM_HitPattern_%iD",hit->GetDetectorNumber()));
          temp2INT->Fill(hit->GetDVerticalStrip(),hit->GetDHorizontalStrip());
        }
        
        if(hit->GetEEnergy()>500)
        {
          temp2INT = (TH2I*)outlist->FindObject(Form("CSM_HitPattern_%iE",hit->GetDetectorNumber()));
          if(temp2INT) temp2INT->Fill(hit->GetEVerticalStrip(),hit->GetEHorizontalStrip());
        }
        
        if(DEBUG) cout<<"PID"<<endl;
        if(hit->GetEEnergy()>0 && hit->GetDEnergy()>0)
        {
          temp2 = (TH2D*)outlist->FindObject(Form("pid_%i",hit->GetDetectorNumber()));
          temp2->Fill(hit->GetEEnergy()/1000.,hit->GetDEnergy()/1000.);
          temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_summed",hit->GetDetectorNumber()));
          temp2->Fill(hit->GetEnergyMeV(),hit->GetDEnergy()/1000.);
          if(hit->GetDthickness()>5)
          {
            //cout<<thickness<<endl;
            if(hit->GetDEnergy()>0 && hit->GetEEnergy()>0)
            {
              if(hit->GetDVerticalStrip() == 7 && hit->GetDHorizontalStrip() == 9)
                continue;
              
              temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_summed_thickness",hit->GetDetectorNumber()));
              temp2->Fill(hit->GetEnergyMeV(),hit->GetDdE_dx());
              
            }

            temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_thickness",hit->GetDetectorNumber()));
            temp2->Fill(hit->GetEEnergy()/1000.,hit->GetDdE_dx());
            
          }
        }
        
        if(hit->GetDetectorNumber()<3)
        {
        temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%iTotal",hit->GetDetectorNumber()));
        temp2->Fill(hit->GetThetaDeg(),hit->GetEnergyMeV());
        }
        else if(hit->GetDetectorNumber()==3)
        {
          if(hit->GetDEnergy()>0)
          {
            temp2 = (TH2D*)outlist->FindObject("EvTheta_1Total");
            temp2->Fill(hit->GetThetaDeg(),hit->GetEnergyMeV());
          }
        }
        else if(hit->GetDetectorNumber()==4)
        {
          if(hit->GetDEnergy()>0)
          {
            temp2 = (TH2D*)outlist->FindObject("EvTheta_2Total");
            temp2->Fill(hit->GetThetaDeg(),hit->GetEnergyMeV());
          }
        }

      }
      //***********************
      //Looking for below PID 10Be
      //***********************
      
      if(DEBUG) cout<<"Below PID 10Be"<<endl;
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
        
        
        //if(hita->GetEEnergy()<10. && hitb->GetEEnergy()<10.
        {
          if(hita->GetDetectorNumber() == hitb->GetDetectorNumber())
            continue;
          
          if(hita->GetDetectorNumber() > 2 || hitb->GetDetectorNumber() > 2)
            continue;    
          
          if(hita->GetIsotope() == "10be") //This avoids double counting from the PID spectrum
            continue;
          if(hitb->GetIsotope() == "10be")
            continue;
          
          double* CorrVals = CorrParticle(hita, 10);
          
          TH1I *diagEnergyPtr = (TH1I*)outlist->FindObject("Be10Diag_Energy");
          diagEnergyPtr->Fill((hitb->GetEnergy()-CorrVals[0])/1000.);
          TH1I *diagThetaPtr = (TH1I*)outlist->FindObject("Be10Diag_Theta");
          diagThetaPtr->Fill((hitb->GetPosition().Theta()-CorrVals[1])*180./TMath::Pi());
          TH1I *diagPhiPtr = (TH1I*)outlist->FindObject("Be10Diag_Phi");
          diagPhiPtr->Fill((hitb->GetPosition().Phi()-CorrVals[2])*180./TMath::Pi());
          
          TH1I *diagPhiPtrdat = (TH1I*)outlist->FindObject("Be10Diag_Phi_dat");
          diagPhiPtrdat->Fill((hitb->GetPosition().Phi()-hita->GetPosition().Phi())*180./TMath::Pi());
          
          TH2I *dualn = (TH2I*)outlist->FindObject("Dual10Be_nocut");
          dualn->Fill(hita->GetThetaDeg(),hita->GetEnergyMeV());
          dualn->Fill(hitb->GetThetaDeg(),hitb->GetEnergyMeV());
          if(DEBUG) cout<<"before diffs"<<endl;
          double energydiff = (hitb->GetEnergy() - CorrVals[0])/1000.; // MeV
          double thetadiff = (hitb->GetPosition().Theta() - CorrVals[1])*180./TMath::Pi(); // Degrees
          double phidiff = (hitb->GetPosition().Phi() - CorrVals[2])*180./TMath::Pi(); // Degrees
          if(phidiff >= -10 && phidiff <= 10)
          {
            TH2I *dualp = (TH2I*)outlist->FindObject("Dual10Be_phicut");
            dualp->Fill(hita->GetThetaDeg(),hita->GetEnergyMeV());
            dualp->Fill(hitb->GetThetaDeg(),hitb->GetEnergyMeV());
            
            TH2I *dualpc = (TH2I*)outlist->FindObject("Dual10Be_phicut_corrected");
            dualpc->Fill(hita->GetThetaDeg(),hita->GetCorrectedEnergyMeV("10be"));
            dualpc->Fill(hitb->GetThetaDeg(),hitb->GetCorrectedEnergyMeV("10be"));
            
            if(thetadiff >= -3 && thetadiff <= 5)
            {
              TH2I *dualt = (TH2I*)outlist->FindObject("Dual10Be_thetacut");
              dualt->Fill(hita->GetThetaDeg(),hita->GetEnergyMeV());
              dualt->Fill(hitb->GetThetaDeg(),hitb->GetEnergyMeV());
              
              TH2I *dualtc = (TH2I*)outlist->FindObject("Dual10Be_thetacut_corrected");
              dualtc->Fill(hita->GetThetaDeg(),hita->GetCorrectedEnergyMeV("10be"));
              dualtc->Fill(hitb->GetThetaDeg(),hitb->GetCorrectedEnergyMeV("10be"));
            }
            
            if(energydiff >= -2.5 && energydiff <= .5)
            {
              TH2I *duale = (TH2I*)outlist->FindObject("Dual10Be_encut");
              duale->Fill(hita->GetThetaDeg(),hita->GetEnergyMeV());
              duale->Fill(hitb->GetThetaDeg(),hitb->GetEnergyMeV());
              
              TH2I *dualec = (TH2I*)outlist->FindObject("Dual10Be_encut_corrected");
              dualec->Fill(hita->GetThetaDeg(),hita->GetCorrectedEnergyMeV("10be"));
              dualec->Fill(hitb->GetThetaDeg(),hitb->GetCorrectedEnergyMeV("10be"));
              if(thetadiff >= -3 && thetadiff <= 5)
              {
                if(DEBUG) cout<<"all diffs"<<endl;
                
                TH2I *duala = (TH2I*)outlist->FindObject("Dual10Be_allcut");
                duala->Fill(hita->GetThetaDeg(),hita->GetEnergyMeV());
                duala->Fill(hitb->GetThetaDeg(),hitb->GetEnergyMeV());

                if(hita->GetEEnergy() <  10 && hitb->GetEEnergy() < 10)
                {
                  TH2I *dualae = (TH2I*)outlist->FindObject("Dual10Be_allcut_noE");
                  dualae->Fill(hita->GetThetaDeg(),hita->GetEnergyMeV());
                  dualae->Fill(hitb->GetThetaDeg(),hitb->GetEnergyMeV());
                }
                
                TH2I *dualac = (TH2I*)outlist->FindObject("Dual10Be_allcut_corrected");
                dualac->Fill(hita->GetThetaDeg(),hita->GetCorrectedEnergyMeV("10be"));
                dualac->Fill(hitb->GetThetaDeg(),hitb->GetCorrectedEnergyMeV("10be"));
                
                double excitecA = GetExciteE_Heavy_Corrected(hita,10);
                double excitecB = GetExciteE_Heavy_Corrected(hitb,10);
                if(DEBUG) cout<<"excites"<<endl;
                
                TH1I *dualex = (TH1I*)outlist->FindObject("DualBe10_ex_allcut");
                dualex->Fill(excitecA);
                dualex->Fill(excitecB);
                
                if(hita->GetEEnergy() <  10 && hitb->GetEEnergy() < 10)
                {
                  TH1I *dualexe = (TH1I*)outlist->FindObject("DualBe10_ex_allcut_noE");
                  dualexe->Fill(excitecA);
                  dualexe->Fill(excitecB);
                }
                
                double Be10Q = 6.310;
                TH1I *dualtot = (TH1I*)outlist->FindObject("DualBe10_etot");
                dualtot->Fill((BEAM_ENERGY+Be10Q) - (hita->GetEnergyMeV() + hitb->GetEnergyMeV()));
                TH1I *dualtotcorr = (TH1I*)outlist->FindObject("DualBe10_etot_corr");
                dualtotcorr->Fill((BEAM_ENERGY+Be10Q) - (hita->GetCorrectedEnergyMeV("10be") + hitb->GetCorrectedEnergyMeV("10be")));
                
                TH2I *exvt = (TH2I*)outlist->FindObject("Be10Ex_Vs_Theta_%i_Dual");
                if(exvt) exvt->Fill(excitecA,hita->GetThetaDeg());
                if(exvt) exvt->Fill(excitecB,hitb->GetThetaDeg());
                
//                 if(TCutG *cut = (TCutG*)(cutlist->FindObject("dual10be_exvt_reject_low")))
//                 {
//                   TH1I *dualexr = (TH1I*)outlist->FindObject("DualBe10_ex_allcut_reject");
//                   TH2I *dualacr = (TH2I*)outlist->FindObject("Dual10Be_allcut_corrected_reject");
//                   
//                   if(!cut->IsInside(excitecA,hita->GetThetaDeg()))
//                   {
//                     dualacr->Fill(hita->GetThetaDeg(),hita->GetCorrectedEnergyMeV("10be"));
//                     dualexr->Fill(excitecA);
//                   }
//                   if(!cut->IsInside(excitecB,hitb->GetThetaDeg()))
//                   {
//                     dualacr->Fill(hitb->GetThetaDeg(),hitb->GetCorrectedEnergyMeV("10be"));
//                     dualexr->Fill(excitecB);
//                   }
//                 }
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
                
                TH2I *etotvt = (TH2I*)outlist->FindObject(Form("Be10Ex_Vs_Theta_%i_SumE",Hhit->GetDetectorNumber()));
                if(etotvt) etotvt->Fill(BEAM_ENERGY +6.31 - Hhit->GetCorrectedEnergyMeV("10be")-Lhit->GetCorrectedEnergyMeV("10be"),Hhit->GetThetaDeg());
                
                TH2I *dualhigh = (TH2I*)outlist->FindObject("Dual10Be_allcut_corrected_highonly");
                dualhigh->Fill(Hhit->GetThetaDeg(),Hhit->GetCorrectedEnergyMeV("10be"));
                TH2I *duallow = (TH2I*)outlist->FindObject("Dual10Be_allcut_corrected_lowonly");
                duallow->Fill(Lhit->GetThetaDeg(),Lhit->GetCorrectedEnergyMeV("10be"));
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
//                     cout<<"EfficiencyWeight: "<<EfficiencyWeight(tigresshit)<<endl;
                    gamptrheff->Fill(Doppler(tigresshit,Hhit,10),EfficiencyWeight(tigresshit));
                    gamptrleff->Fill(Doppler(tigresshit,Lhit,10),EfficiencyWeight(tigresshit));
                    
                    double dopp = Doppler(tigresshit,Hhit,10);
                    if(DEBUG) cout<<"after dopp"<<endl;
                    
                    if(dopp>=3.35 && dopp<=3.38)
                    {
                      TH1I* dualexgcut = (TH1I*)outlist->FindObject("DualBe10Ex_gcut_3368");
                      dualexgcut->Fill(excitecA);
                      dualexgcut->Fill(excitecB);
                    }
                    else if(dopp>=2.585 && dopp<=2.597)
                    {
                      TH1I* dualexgcut = (TH1I*)outlist->FindObject("DualBe10Ex_gcut_2589");
                      dualexgcut->Fill(excitecA);
                      dualexgcut->Fill(excitecB);
                    }
                    else if(dopp>=2.89 && dopp<=2.91)
                    {
                      TH1I* dualexgcut = (TH1I*)outlist->FindObject("DualBe10Ex_gcut_2894");
                      dualexgcut->Fill(excitecA);
                      dualexgcut->Fill(excitecB);
                    }
                    else if(dopp>=2.86 && dopp<=2.87)
                    {
                      TH1I* dualexgcut = (TH1I*)outlist->FindObject("DualBe10Ex_gcut_2867");
                      dualexgcut->Fill(excitecA);
                      dualexgcut->Fill(excitecB);
                    }
                  }
                }         
              }
            }
          }
        }
        if(DEBUG) cout<<"Below PID 11Be"<<endl;
        int iso = 11;
        {
          if(hita->GetEnergy() > hitb->GetEnergy())
          {
            TCSMHit *tmp = hita;
            hita = hitb;
            hitb = tmp;
          }
          if(hita->GetDetectorNumber() > 2 || hitb->GetDetectorNumber() > 2)
            continue;
          
          double* CorrVals = CorrParticle(hita, 11);
          double energydiff = (hitb->GetEnergy() - CorrVals[0])/1000.; // MeV
          double thetadiff = (hitb->GetPosition().Theta() - CorrVals[1])*180./TMath::Pi(); // Degrees
          double phidiff = (hitb->GetPosition().Phi() - CorrVals[2])*180./TMath::Pi(); // Degrees
          
          if(phidiff >= -10 && phidiff <= 10)
          {
            if(energydiff >= -2.5 && energydiff <= .5)
            {
              if(thetadiff >= -3 && thetadiff <= 5)
              {
                if(DEBUG) cout<<"sectionA"<<endl;
                TH2D* tmpptr2d = (TH2D*)outlist->FindObject(Form("EvTheta_%i_%iBe_corr",hita->GetDetectorNumber(),11));
                tmpptr2d->Fill(hita->GetThetaDeg(),hita->GetEnergyMeV());
                tmpptr2d = (TH2D*)outlist->FindObject(Form("EvTheta_%i_%iBe_corr",hitb->GetDetectorNumber(),9));
                tmpptr2d->Fill(hitb->GetThetaDeg(),hitb->GetEnergyMeV());
                
//                 tmpptr2d = (TH2D*)outlist->FindObject(Form("EvTheta_%i_%iBe_corr_high",hita->GetDetectorNumber(),iso));
//                 tmpptr2d->Fill(hita->GetThetaDeg(),hita->GetEnergyMeV()); 
//                 
              }
            }
          }
          
          CorrVals = CorrParticle(hita, 9);
          energydiff = (hitb->GetEnergy() - CorrVals[0])/1000.; // MeV
          thetadiff = (hitb->GetPosition().Theta() - CorrVals[1])*180./TMath::Pi(); // Degrees
          phidiff = (hitb->GetPosition().Phi() - CorrVals[2])*180./TMath::Pi(); // Degrees
          
          if(phidiff >= -10 && phidiff <= 10)
          {
            if(energydiff >= -2.5 && energydiff <= .5)
            {
              if(thetadiff >= -3 && thetadiff <= 5)
              {
                if(DEBUG) cout<<"sectionB"<<endl;
                
                TH2D* tmpptr2d = (TH2D*)outlist->FindObject(Form("EvTheta_%i_%iBe_corr",hita->GetDetectorNumber(),9));
                tmpptr2d->Fill(hita->GetThetaDeg(),hita->GetEnergyMeV());
                tmpptr2d = (TH2D*)outlist->FindObject(Form("EvTheta_%i_%iBe_corr",hitb->GetDetectorNumber(),11));
                tmpptr2d->Fill(hitb->GetThetaDeg(),hitb->GetEnergyMeV());
                
//                 tmpptr2d = (TH2D*)outlist->FindObject(Form("EvTheta_%i_%iBe_corr_high",hita->GetDetectorNumber(),9));
//                 tmpptr2d->Fill(hita->GetThetaDeg(),hita->GetEnergyMeV()); 
              }
            }
          }
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
        
        if(!hit->Suppress())
        {          
          TH1D* gss = (TH1D*)outlist->FindObject("GammaSum_supp");
          gss->Fill(hit->GetCore()->GetEnergy()/1000.);
        }
      }
  
      //***********************
      //  Other 10Be
      //***********************
//       for(int i =0; i<csm->GetMultiplicity();i++)
//       {	
//         TCSMHit *hit = csm->GetHit(i);
//         if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form(Be10Cut,hit->GetDetectorNumber()))))
//         {
//           if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()) && hit->GetEEnergy() > 10)
//           {
//             double* CorrVals = CorrParticle(hit, 10);
//             //double* CorrVals = CorrParticle(hit->GetCorrectedEnergyMeV("10be")*1000., hit->GetPosition().Theta(), hit->GetPosition().Phi(),MASS_BE10);
//             
//             TH2D* mathptr = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE10_opp_math",hit->GetDetectorNumber()));
//             mathptr->Fill(CorrVals[1]*180./TMath::Pi(),CorrVals[0]/1000);
//             
//             for(int asdf=0; asdf<tigress->GetAddBackMultiplicity();asdf++)
//             {
//               TTigressHit *tigresshit = tigress->GetAddBackHit(asdf);
//               
//               if(tigresshit->GetCore()->GetEnergy()>10)
//               {
//                 double dopp = Doppler(tigresshit,CorrVals[0],CorrVals[1],CorrVals[2],10);
//                 TH1D* dopptr = (TH1D*)outlist->FindObject(Form("Be10_Gamma_%i_dopp_opp_math",hit->GetDetectorNumber()));
//                 dopptr->Fill(dopp);
//                 TH1D* dopptreff = (TH1D*)outlist->FindObject(Form("Be10_Gamma_%i_dopp_opp_math_eff",hit->GetDetectorNumber()));
//                 dopptreff->Fill(dopp,EfficiencyWeight(tigresshit));
//                 
//                 if(!tigresshit->Suppress())
//                 {
//                   TH1D* dopptrsupp = (TH1D*)outlist->FindObject(Form("Be10_Gamma_%i_dopp_opp_math_supp",hit->GetDetectorNumber()));
//                   dopptrsupp->Fill(dopp);
//                 }
//                 if(tigress->GetRawBGO()==0)
//                 {
//                   TH1D* dopptrsupp = (TH1D*)outlist->FindObject(Form("Be10_Gamma_%i_dopp_opp_math_suppMax",hit->GetDetectorNumber()));
//                   dopptrsupp->Fill(dopp);
//                 }
//                 
//                 double excite = GetExciteE_Heavy_Corrected(hit,10);
//                 
//                 if(dopp>=2.577 && dopp<=2.612)
//                 {
//                   TH1D* expg = (TH1D*)outlist->FindObject(Form("Be10Ex%i_gcut_2589_opp",hit->GetDetectorNumber()));
//                   expg->Fill(excite);
//                 }
//                 else if(dopp>=2.876 && dopp<=2.913)
//                 {
//                   TH1D* expg = (TH1D*)outlist->FindObject(Form("Be10Ex%i_gcut_2894_opp",hit->GetDetectorNumber()));
//                   expg->Fill(excite);
//                 }
//                 else if(dopp>=3.337 && dopp<=3.402)
//                 {
//                   TH1D* expg = (TH1D*)outlist->FindObject(Form("Be10Ex%i_gcut_3368_opp",hit->GetDetectorNumber()));
//                   expg->Fill(excite);
//                 }
//                 else if(dopp>=5.951 && dopp<=5.986)
//                 {
//                   TH1D* expg = (TH1D*)outlist->FindObject(Form("Be10Ex%i_gcut_5958_opp",hit->GetDetectorNumber()));
//                   expg->Fill(excite);
//                 }
//               }
//             }
//             
//             for(int j = 0; j<csm->GetMultiplicity(); j++)
//             {
//               if(i==j)
//                 continue;
//               
//               TCSMHit *opphit = csm->GetHit(j);
//               
//               int conditions = 0;
//               if(AlmostEqual(hit->GetEnergy(),CorrVals[0]))
//                 conditions++;
//               if(AlmostEqual(hit->GetPosition().Theta(),CorrVals[1]))
//                 conditions++;
//               if(AlmostEqual(hit->GetPosition().Phi(),CorrVals[2]))
//                 conditions++;
//               
//               if((conditions == 3 && int(BEAM_ENERGY)==55) || int(BEAM_ENERGY)==30)
//               {
//                 for(int y=0; y<tigress->GetAddBackMultiplicity();y++)
//                 {
//                   TTigressHit *tigresshit = tigress->GetAddBackHit(y);
//                   
//                   if(tigresshit->GetCore()->GetEnergy()>10)
//                   {
//                     TH1D* dopptr = (TH1D*)outlist->FindObject(Form("Be10_Gamma_%i_dopp_opp",hit->GetDetectorNumber()));
//                     dopptr->Fill(Doppler(tigresshit,opphit,10));
//                     TH1D* dopptreff = (TH1D*)outlist->FindObject(Form("Be10_Gamma_%i_dopp_opp_eff",hit->GetDetectorNumber()));
//                     dopptreff->Fill(Doppler(tigresshit,opphit,10),EfficiencyWeight(tigresshit));
//                   }
//                 }
//                 
//                 TH1D* exptr = (TH1D*)outlist->FindObject(Form("Be10Ex%i_corr_opp",hit->GetDetectorNumber()));
//                 exptr->Fill(GetExciteE_Heavy_Corrected(opphit,10));
//                 
//                 TH2D* evtptr = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE10_opp",hit->GetDetectorNumber()));
//                 evtptr->Fill(opphit->GetThetaDeg(),opphit->GetEnergyMeV());
//                 
//                 TH1D* supexptr = (TH1D*)outlist->FindObject(Form("Be10Ex%i_corr_supp",hit->GetDetectorNumber()));
//                 supexptr->Fill(GetExciteE_Heavy_Corrected(hit,10));
//               }
//             }
//           }
//           
//         }
  
      //***********************
      //         End
      //***********************
      
      ////////////////////////////////////////////////////////////////////////
      
    }
    else if(ANGULAR_DISTRIBUTION)
    {
      bool DEBUGANG = 0;
      if(DEBUGANG) cout<<" Multiplicity: "<<csm->GetMultiplicity()<<endl;
      for(int i = 0;i<csm->GetMultiplicity();i++)
      {
        TCSMHit *hit = csm->GetHit(i);
        
        if(DEBUGANG) cout<<"I have hit "<<i<<endl;
        if(DEBUGANG) hit->Print();
//         if(hit->GetDVerticalStrip()<1 || hit->GetDVerticalStrip()>14)//For now, suppress the perimeter strip, as there is a loss of solid angle there.
//         {
//           continue;
//         }
//         if(hit->GetDHorizontalStrip()<1 || hit->GetDHorizontalStrip()>14)
//         {
//           continue;
//         }
//         
//         if(hit->GetDVerticalStrip()==1 || hit->GetDVerticalStrip()==14)
//         {
//           if(hit->GetDHorizontalStrip()==1 || hit->GetDHorizontalStrip()==14)
//           {
//             continue;
//           }
//         }
        if(DEBUGANG) cout<<"Have Hit"<<endl;
        if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form(Be11Cut,hit->GetDetectorNumber()))))
        {
          if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()) && hit->GetEEnergy() > 10)
          {           
            double ex11c =GetExciteE_Heavy_Corrected(hit,11);
            if(DEBUGANG) cout<<"Have 11Be"<<endl;
            int state = -1;
            if(SIMULATED_DATA)
            {
              if(ex11c >= -3 && ex11c <= 1)
                state = 0;
              else if(ex11c >= 1.1 && ex11c <= 4)
                state=3;
            }
            
            else
            {
              if(ex11c >= -1.5 && ex11c <= 1)
                state = 0;
              else if(ex11c >= 1.5 && ex11c <= 3.5)
                state = 3;
            }
            
            if(state != -1)
            {
//               TH1I* tmpangdist = (TH1I*)outlist->FindObject("ang_dist_11be_2.6");
//               tmpangdist->Fill(CalcCOMThetaDeg(hit,11),1./hit->GetSolidAngleD());
              
              if(DEBUGANG) cout<<"Looking for "<<Form("RingCounts_s%i_d%i_11Be",state,hit->GetDetectorNumber())<<endl;
              
              int ring = RingNumber(hit);
              TH1D* tmpptr = (TH1D*)outlist->FindObject(Form("RingCounts_s%i_d%i_11Be",state,hit->GetDetectorNumber()));   
              tmpptr->Fill(ring);
            }
            
//             for(int y=0; y<tigress->GetAddBackMultiplicity();y++)
//             {
//               TTigressHit *tigresshit = tigress->GetAddBackHit(y);
//               
//               if(tigresshit->GetCore()->GetEnergy()>10)
//               {
//                 double dopp = Doppler(tigresshit,Hhit,10);
//                 
//               }
//             }
          }
        }
        
        if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form(Be9Cut,hit->GetDetectorNumber()))))
        {
          if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()) && hit->GetEEnergy() > 10)
          {           
            double ex9c =GetExciteE_Heavy_Corrected(hit,9);
            if(DEBUGANG) cout<<"Have 9Be"<<endl;
            int state = -1;
            if(SIMULATED_DATA)
            {
              if(ex9c >= -3 && ex9c <= 1)
                state = 0;
              else if(ex9c >= 1.1 && ex9c <= 4)
                state=3;
            }
            
            else
            {
              if(ex9c >= -1.5 && ex9c <= 1)
                state = 0;
              else if(ex9c >= 1.5 && ex9c <= 3.5)
                state = 3;
            }
            
            if(state != -1)
            {

              if(DEBUGANG) cout<<"Looking for "<<Form("RingCounts_s%i_d%i_9Be",state,hit->GetDetectorNumber())<<endl;
              
              int ring = RingNumber(hit);
              TH1D* tmpptr = (TH1D*)outlist->FindObject(Form("RingCounts_s%i_d%i_9Be",state,hit->GetDetectorNumber()));   
              tmpptr->Fill(ring);
            }
          }
        }
        
        
        if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form(Be10Cut,hit->GetDetectorNumber()))))
        {
          if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()) && hit->GetEEnergy() > 10)
          { 
            if(DEBUGANG) cout<<"Have 10Be"<<endl;
            
            TH2I* tmptvtcom = (TH2I*)outlist->FindObject("ThetaVThetaCOM_PID");
            tmptvtcom->Fill(CalcCOMThetaDeg(hit,10),hit->GetThetaDeg());
            
            TH2I* tmpevtcom = (TH2I*)outlist->FindObject("EnergyVThetaCOM_PID");
            tmpevtcom->Fill(CalcCOMThetaDeg(hit,10),hit->GetEnergyMeV());
            
            // 0     -1 to 1.2     .1
            // 3.3  2.5 to 4.4     3.45
            // 6    5.5 to 7       6.25
            //9.3     9 to 10.5    9.75
            double ex10c =GetExciteE_Heavy_Corrected(hit,10);
            if(DEBUGANG) cout<<"Have 10Be Excite"<<endl;
            
            int state = -1;
            
            if(SIMULATED_DATA)
            {
              if(ex10c >= -1.5 && ex10c<= 1.2)
              {
                state = 0;
              }
              
              else if(ex10c >= 2. && ex10c<= 4.)
              {
                state = 3;
              }
              
              else if(ex10c >= 4.5 && ex10c<= 7.7)
              {
                state = 6;
              }
              
              else if(ex10c >= 7.7 && ex10c<= 10.5)
              {
                state = 9;
              }
            }
            
            else
            {
              if(ex10c >= -1 && ex10c<= 1.2)
              {
                state = 0;
              }
              
              else if(ex10c >= 2.5 && ex10c<= 4.4)
              {
                state = 3;
              }
              
              else if(ex10c >= 5.5 && ex10c<= 7)
              {
                state = 6;
              }
              
              else if(ex10c >= 9 && ex10c<= 10.5)
              {
                state = 9;
              }
            }
            
            if(state != -1)
            {
              if(DEBUGANG) cout<<"Have 10Be state assigned"<<endl;
              
              TH2I* hpPtr = (TH2I*)outlist->FindObject(Form("HP_10be_%i_d%i_pid",state,hit->GetDetectorNumber()));

              hpPtr->Fill(hit->GetDVerticalStrip(),hit->GetDHorizontalStrip());

              TH2D* comPtr = (TH2D*)outlist->FindObject(Form("RingVCOMT_s%i_d%i_pid",state,hit->GetDetectorNumber()));
              
              int ring = RingNumber(hit);
              
              comPtr->Fill(ring,CalcCOMThetaDeg(hit,10));
              TH2D* lPtr = (TH2D*)outlist->FindObject(Form("RingVLabT_s%i_d%i_pid",state,hit->GetDetectorNumber()));
              lPtr->Fill(ring,hit->GetThetaDeg());
              
//               cout<<"PID CalcCOMThetaDeg: "<<CalcCOMThetaDeg(hit,10)<<endl;
//               RingRange(ring, hit->GetDetectorNumber(), state);  
//               cout<<endl;
              TH1D* tmpptr = (TH1D*)outlist->FindObject(Form("RingCounts_s%i_d%i_pid",state,hit->GetDetectorNumber()));   
              tmpptr->Fill(ring);
              tmpptr = (TH1D*)outlist->FindObject(Form("RingCounts_s%i_d%i_pid_edge",state,hit->GetDetectorNumber()));
              tmpptr->Fill(ring,EdgeEffectFactor(hit));
              tmpptr = (TH1D*)outlist->FindObject(Form("RingCounts_s%i_pid",state));
              tmpptr->Fill(ring);                  
            }
          }
        }
      }
      
      
      if(csm->GetMultiplicity() == 2)
      {
        TCSMHit* hita;
        TCSMHit* hitb;
        if(DEBUGANG) cout<<"Have mult2"<<endl;
        
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
        
        if(hita->GetDetectorNumber() > 2)
          continue;
        if(hitb->GetDetectorNumber() > 2)
          continue;   
        
        double* CorrVals = CorrParticle(hita, 10);
        if(DEBUGANG) cout<<"Have CorrVals"<<endl;
        
        double energydiff = (hitb->GetEnergy() - CorrVals[0])/1000.; // MeV
        double thetadiff = (hitb->GetPosition().Theta() - CorrVals[1])*180./TMath::Pi(); // Degrees
        double phidiff = (hitb->GetPosition().Phi() - CorrVals[2])*180./TMath::Pi(); // Degrees
        if(phidiff >= -10 && phidiff <= 10)
        {
          if(energydiff >= -2.5 && energydiff <= .5)
          {
            if(thetadiff >= -3 && thetadiff <= 5)
            {
              if(DEBUGANG) cout<<"Have Correlated 2x 10Be"<<endl;
              
              double ex10cA =GetExciteE_Heavy_Corrected(hita,10);
              double ex10cB =GetExciteE_Heavy_Corrected(hitb,10);
              
              if(DEBUGANG) cout<<"Have Excited States"<<endl;
              
              // 6    4.5 to 7.5
              // 9.3   7.5 to 10
              // 12   11.2 to 12.7
              
              TH2I* tmptvtcom = (TH2I*)outlist->FindObject("ThetaVThetaCOM_DUAL");
              tmptvtcom->Fill(CalcCOMThetaDeg(hita,10),hita->GetThetaDeg());
              tmptvtcom->Fill(CalcCOMThetaDeg(hitb,10),hitb->GetThetaDeg());
              
              TH2I* tmpevtcom = (TH2I*)outlist->FindObject("EnergyVThetaCOM_DUAL");
              tmpevtcom->Fill(CalcCOMThetaDeg(hita,10),hita->GetEnergyMeV());
              tmpevtcom->Fill(CalcCOMThetaDeg(hitb,10),hitb->GetEnergyMeV());
              
              TH2I* tmptcomvtcom = (TH2I*)outlist->FindObject("ThetaCOMVThetaCOM_DUAL");
              tmptcomvtcom->Fill(CalcCOMThetaDeg(hita,10),CalcCOMThetaDeg(hitb,10));
              
              TH2I* tmptvt = (TH2I*)outlist->FindObject("ThetaVTheta_DUAL");
              tmptvt->Fill(hita->GetThetaDeg(),hitb->GetThetaDeg());
              
              if(DEBUGANG) cout<<"Did prelim plots "<<endl;
              
              int stateA = -1;
              int stateB = -1;
              
              if(SIMULATED_DATA)
              {
                if(ex10cA >= 5 && ex10cA <= 8)
                {
                  stateA = 6;
                }
                else if(ex10cA >= 8 && ex10cA <= 10.7)
                {
                  stateA = 9;
                }
                else if(ex10cA >= 10.7 && ex10cA <= 14)
                {
                  stateA = 12;
                }
                
                if(ex10cB >= 5 && ex10cB <= 8)
                {
                  stateB = 6;
                }
                else if(ex10cB >= 8 && ex10cB <= 10.7)
                {
                  stateB = 9;
                }
                else if(ex10cB >= 10.7 && ex10cB <= 14)
                {
                  stateB = 12;
                }
              }
              
              else
              {
                if(ex10cA >= 4.5 && ex10cA <= 7.5)
                {
                  stateA = 6;
                }
                else if(ex10cA >= 7.5 && ex10cA <= 10)
                {
                  stateA = 9;
                }
                else if(ex10cA >= 11.2 && ex10cA <= 12.7)
                {
                  stateA = 12;
                }
                
                if(ex10cB >= 4.5 && ex10cB <= 7.5)
                {
                  stateB = 6;
                }
                else if(ex10cB >= 7.5 && ex10cB <= 10)
                {
                  stateB = 9;
                }
                else if(ex10cB >= 11.2 && ex10cB <= 12.7)
                {
                  stateB = 12;
                }
                
              }
              if(stateA != -1)
              {
                if(DEBUGANG) cout<<"Have State A 10Be "<<stateA<<endl;
                
//                 if(hita->GetDVerticalStrip() == 14 && hita->GetDHorizontalStrip() == 3)
//                 {
//                   if(CalcCOMThetaDeg(hita,10)<90.)
//                     cout<<DGREEN;
//                   else
//                     cout<<DRED;
//                   cout<<"Debug output for COM: "<<endl;
//                   cout<<"Hit A"<<endl;
//                   cout<<"  ComTheta: "<<CalcCOMThetaDeg(hita,10)<<endl;
//                   cout<<"  SolidAngle: "<<hita->GetSolidAngleD()<<endl;
//                   hita->Print();
//                   cout<<RESET_COLOR;
//                 }

//                 TH1I* tmpangdist = (TH1I*)outlist->FindObject(Form("ang_dist_10be_%i_dual",stateA));
//                 TH3D* interPtr = (TH3D*)outlist->FindObject(Form("perPixel_10be_%i_d%i_dual",stateA,hita->GetDetectorNumber()));
//                 TH3D* interPtrLab = (TH3D*)outlist->FindObject(Form("perPixel_lab_10be_%i_d%i_dual",stateA,hita->GetDetectorNumber()));
                TH2I* hpPtr = (TH2I*)outlist->FindObject(Form("HP_10be_%i_d%i_dual",stateA,hita->GetDetectorNumber()));
                
//                 TH2D* interPtrProj = (TH2D*)outlist->FindObject(Form("perPixel_10be_%i_d%i_dual_proj",stateA,hita->GetDetectorNumber()));
                
//                 double weight = 1./(hita->GetSolidAngleD()*GetfCOM(hita,10));

//                 tmpangdist->Fill(CalcCOMThetaDeg(hita,10),weight);
                
//                 cout<<"Diag for Fractions, HitA:"<<endl;
//                 hita->Print();
//                 cout<<"GetK_Corrected: "<<GetK_Corrected(hita,10)<<endl;
//                 cout<<"GetfCOM: "<<GetfCOM(hita,10)<<endl;
//                 cout<<"GetfLab: "<<GetfLab(hita,10)<<endl;
//                 cout<<endl<<endl;
                
//                 interPtr->Fill(hita->GetDVerticalStrip(),hita->GetDHorizontalStrip(),CalcCOMThetaDeg(hita,10));
//                 interPtrLab->Fill(hita->GetDVerticalStrip(),hita->GetDHorizontalStrip(),hita->GetThetaDeg());
                hpPtr->Fill(hita->GetDVerticalStrip(),hita->GetDHorizontalStrip());
                
//                 interPtrProj->Fill(hita->GetDVerticalStrip(),hita->GetDHorizontalStrip());
                
                TH2D* comPtr = (TH2D*)outlist->FindObject(Form("RingVCOMT_s%i_d%i_dual",stateA,hita->GetDetectorNumber()));
                if(DEBUGANG) cout<<"Before Ring Calc"<<endl;
                int ring = RingNumber(hita);
                if(DEBUGANG) cout<<"After Ring Calc"<<endl;
                
                comPtr->Fill(ring,CalcCOMThetaDeg(hita,10));
                
                TH2D* lPtr = (TH2D*)outlist->FindObject(Form("RingVLabT_s%i_d%i_dual",stateA,hita->GetDetectorNumber()));
                lPtr->Fill(ring,hita->GetThetaDeg());
                
                TH1D* tmpptr = (TH1D*)outlist->FindObject(Form("RingCounts_s%i_d%i_dual",stateA,hita->GetDetectorNumber()));
                tmpptr->Fill(ring);
                tmpptr = (TH1D*)outlist->FindObject(Form("RingCounts_s%i_dual",stateA));
                tmpptr->Fill(ring);
                tmpptr = (TH1D*)outlist->FindObject(Form("RingWeight_s%i_d%i_dual",stateA,hita->GetDetectorNumber()));
                tmpptr->Fill(ring,GetfCOM(hita,10));
                tmpptr = (TH1D*)outlist->FindObject(Form("RingWeight_s%i_dual",stateA));
                tmpptr->Fill(ring,GetfCOM(hita,10));
              }
              if(stateB != -1)
              {
                if(DEBUGANG) cout<<"Have State B "<<stateB<<endl;
//                 if(hitb->GetDVerticalStrip() == 14 && hitb->GetDHorizontalStrip() == 3)
//                 {
//                   if(CalcCOMThetaDeg(hitb,10)<90.)
//                     cout<<DGREEN;
//                   else
//                     cout<<DRED;
//                   cout<<"Debug output for COM: "<<endl;
//                   cout<<"Hit B"<<endl;
//                   cout<<"  ComTheta: "<<CalcCOMThetaDeg(hitb,10)<<endl;
//                   cout<<"  SolidAngle: "<<hitb->GetSolidAngleD()<<endl;
//                   hitb->Print();
//                   cout<<RESET_COLOR;
//                 }
                
//                 TH1I* tmpangdist = (TH1I*)outlist->FindObject(Form("ang_dist_10be_%i_dual",stateB));
//                 TH3D* interPtr = (TH3D*)outlist->FindObject(Form("perPixel_10be_%i_d%i_dual",stateB,hitb->GetDetectorNumber()));
//                 TH3D* interPtrLab = (TH3D*)outlist->FindObject(Form("perPixel_lab_10be_%i_d%i_dual",stateB,hitb->GetDetectorNumber()));
                TH2I* hpPtr = (TH2I*)outlist->FindObject(Form("HP_10be_%i_d%i_dual",stateB,hitb->GetDetectorNumber()));
                
                if(DEBUGANG) cout<<"Looking for "<<Form("HP_10be_%i_d%i_dual",stateB,hitb->GetDetectorNumber())<<", found? "<<hpPtr<<endl;
                
//                 double weight = 1./(hitb->GetSolidAngleD()*GetfCOM(hitb,10));
                
//                 TH2D* interPtrProj = (TH2D*)outlist->FindObject(Form("perPixel_10be_%i_d%i_dual_proj",stateB,hitb->GetDetectorNumber()));
//                 tmpangdist->Fill(CalcCOMThetaDeg(hitb,10),weight);
//                 interPtr->Fill(hitb->GetDVerticalStrip(),hitb->GetDHorizontalStrip(),CalcCOMThetaDeg(hitb,10));
//                 interPtrLab->Fill(hitb->GetDVerticalStrip(),hitb->GetDHorizontalStrip(),hitb->GetThetaDeg());
                hpPtr->Fill(hitb->GetDVerticalStrip(),hitb->GetDHorizontalStrip());
                if(DEBUGANG) cout<<"Done with hpPtr"<<endl;
                
//                 interPtrProj->Fill(hitb->GetDVerticalStrip(),hitb->GetDHorizontalStrip());
                
                TH2D* comPtr = (TH2D*)outlist->FindObject(Form("RingVCOMT_s%i_d%i_dual",stateB,hitb->GetDetectorNumber()));
                int ring = RingNumber(hitb);
                comPtr->Fill(ring,CalcCOMThetaDeg(hitb,10));
                
                TH2D* lPtr = (TH2D*)outlist->FindObject(Form("RingVLabT_s%i_d%i_dual",stateB,hitb->GetDetectorNumber()));
                lPtr->Fill(ring,hitb->GetThetaDeg());
                if(DEBUGANG) cout<<"Done with comPtr"<<endl;
                
                TH1D* tmpptr = (TH1D*)outlist->FindObject(Form("RingCounts_s%i_d%i_dual",stateB,hitb->GetDetectorNumber()));
                tmpptr->Fill(ring);
                if(DEBUGANG) cout<<"Done with ringcounts A"<<endl;
                
                tmpptr = (TH1D*)outlist->FindObject(Form("RingCounts_s%i_dual",stateB));
                tmpptr->Fill(ring);
                if(DEBUGANG) cout<<"Done with ringcounts B"<<endl;
                
                tmpptr = (TH1D*)outlist->FindObject(Form("RingWeight_s%i_d%i_dual",stateB,hitb->GetDetectorNumber()));
                tmpptr->Fill(ring,GetfCOM(hitb,10));
                if(DEBUGANG) cout<<"Done with ringweight A"<<endl;
                
                tmpptr = (TH1D*)outlist->FindObject(Form("RingWeight_s%i_dual",stateB));
                tmpptr->Fill(ring,GetfCOM(hitb,10));
                if(DEBUGANG) cout<<"Done with B"<<endl;
                
              }
            }
          }
        }
        
//         for(int iso = 9; iso <12; iso +=2)
        int iso = 11;
        {
          if(hita->GetEnergy() > hitb->GetEnergy())
          {
            TCSMHit *tmp = hita;
            hita = hitb;
            hitb = tmp;
          }
          
          double* CorrVals = CorrParticle(hita, 11);//this means hit a is 11, and hit b is 9
          double energydiff = (hitb->GetEnergy() - CorrVals[0])/1000.; // MeV
          double thetadiff = (hitb->GetPosition().Theta() - CorrVals[1])*180./TMath::Pi(); // Degrees
          double phidiff = (hitb->GetPosition().Phi() - CorrVals[2])*180./TMath::Pi(); // Degrees
          
          if(phidiff >= -10 && phidiff <= 10)
          {
            if(energydiff >= -2.5 && energydiff <= .5)
            {
              if(thetadiff >= -3 && thetadiff <= 5)
              {
                if(DEBUGANG) cout<<"Have Correlated "<<iso<<"Be"<<endl;
                
                double ex10cA =GetExciteE_Heavy_Corrected(hita,11);
                double ex10cB =GetExciteE_Heavy_Corrected(hitb,9);

                if(DEBUGANG) cout<<"Have Excited States"<<endl;
                
                int stateA = -1;
                int stateB = -1;
                
                if(SIMULATED_DATA)
                {
                  if(ex10cA >= -3 && ex10cA <= 1)
                  {
                    stateA = 0;
                  }
                  else if(ex10cA >= 1.1 && ex10cA <= 4)
                    stateA=3;

                  
                  if(ex10cB >= -3 && ex10cB <= 1)
                  {
                    stateB = 0;
                  }
                  else if(ex10cB >= 1.1 && ex10cB <= 4)
                    stateB=3;

                }
                
                else
                {
                  if(ex10cA >= -1.5 && ex10cA <= 1)
                  {
                    stateA = 0;
                  }
                  else if(ex10cA >= 1.5 && ex10cA <= 3.5)
                  {
                    stateA = 3;
                  }

                  
                  if(ex10cB >= -1.5 && ex10cB <= 1)
                  {
                    stateB = 0;
                  }
                  else if(ex10cB >= 1.5 && ex10cB <= 3.5)
                  {
                    stateB = 3;
                  }

                  
                }
                if(stateA != -1)
                {
                  int ring = RingNumber(hita);                  
                  TH1D* tmpptr = (TH1D*)outlist->FindObject(Form("RingCounts_s%i_d%i_%iBe_corr",stateA,hita->GetDetectorNumber(),11));
                  tmpptr->Fill(ring);
//                   tmpptr = (TH1D*)outlist->FindObject(Form("RingCounts_s%i_d%i_%iBe_corr_Aonly",stateA,hita->GetDetectorNumber(),9));
//                   tmpptr->Fill(ring);
                }
                if(stateB != -1)
                {
                  int ring = RingNumber(hitb);
                  TH1D* tmpptr = (TH1D*)outlist->FindObject(Form("RingCounts_s%i_d%i_%iBe_corr",stateB,hitb->GetDetectorNumber(),9));
                  tmpptr->Fill(ring);
//                   tmpptr = (TH1D*)outlist->FindObject(Form("RingCounts_s%i_d%i_%iBe_corr_Bonly",stateB,hitb->GetDetectorNumber(),11));
//                   tmpptr->Fill(ring);
                }
              }
            }
          }
          
          CorrVals = CorrParticle(hita, 9);//this means hit a is 9, and hit b is 11
          energydiff = (hitb->GetEnergy() - CorrVals[0])/1000.; // MeV
          thetadiff = (hitb->GetPosition().Theta() - CorrVals[1])*180./TMath::Pi(); // Degrees
          phidiff = (hitb->GetPosition().Phi() - CorrVals[2])*180./TMath::Pi(); // Degrees
          
          if(phidiff >= -10 && phidiff <= 10)
          {
            if(energydiff >= -2.5 && energydiff <= .5)
            {
              if(thetadiff >= -3 && thetadiff <= 5)
              {
                if(DEBUGANG) cout<<"Have Correlated "<<iso<<"Be"<<endl;
                
                double ex10cA =GetExciteE_Heavy_Corrected(hita,9);
                double ex10cB =GetExciteE_Heavy_Corrected(hitb,11);
                
                if(DEBUGANG) cout<<"Have Excited States"<<endl;
                
                int stateA = -1;
                int stateB = -1;
                
                if(SIMULATED_DATA)
                {
                  if(ex10cA >= -3 && ex10cA <= 1)
                  {
                    stateA = 0;
                  }
                  else if(ex10cA >= 1.1 && ex10cA <= 4)
                    stateA=3;
                  
                  
                  if(ex10cB >= -3 && ex10cB <= 1)
                  {
                    stateB = 0;
                  }
                  else if(ex10cB >= 1.1 && ex10cB <= 4)
                    stateB=3;
                  
                }
                
                else
                {
                  if(ex10cA >= -1.5 && ex10cA <= 1)
                  {
                    stateA = 0;
                  }
                  else if(ex10cA >= 1.5 && ex10cA <= 3.5)
                  {
                    stateA = 3;
                  }
                  
                  
                  if(ex10cB >= -1.5 && ex10cB <= 1)
                  {
                    stateB = 0;
                  }
                  else if(ex10cB >= 1.5 && ex10cB <= 3.5)
                  {
                    stateB = 3;
                  }
                  
                  
                }
                if(stateA != -1)
                {
                  int ring = RingNumber(hita);                  
                  TH1D* tmpptr = (TH1D*)outlist->FindObject(Form("RingCounts_s%i_d%i_%iBe_corr",stateA,hita->GetDetectorNumber(),9));
                  tmpptr->Fill(ring);
//                   tmpptr = (TH1D*)outlist->FindObject(Form("RingCounts_s%i_d%i_%iBe_corr_Aonly",stateA,hita->GetDetectorNumber(),11));
//                   tmpptr->Fill(ring);
                }
                if(stateB != -1)
                {
                  int ring = RingNumber(hitb);
                  TH1D* tmpptr = (TH1D*)outlist->FindObject(Form("RingCounts_s%i_d%i_%iBe_corr",stateB,hitb->GetDetectorNumber(),11));
                  tmpptr->Fill(ring);
//                   tmpptr = (TH1D*)outlist->FindObject(Form("RingCounts_s%i_d%i_%iBe_corr_Bonly",stateB,hitb->GetDetectorNumber(),9));
//                   tmpptr->Fill(ring);
                }
              }
            }
          }
        }
      }
      if(DEBUGANG) cout<<"Moving to next event"<<endl;
      
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
  
  if(strcmp(argv[1], "--low") == 0 || strcmp(argv[1], "-l") == 0)
  {
    i = 2;
    BEAM_ENERGY = 30.14;
  }
  else if(strcmp(argv[1], "--high") == 0 || strcmp(argv[1], "-h") == 0)
  {
    i = 2;
    BEAM_ENERGY = 55;
  }
  else
  {
    cerr<<"Undefined beam energy.  There will be strange behavior in energy dependent functions."<<endl;
    BEAM_ENERGY = 1;
  }
  
  if(strcmp(argv[2], "--angDist") == 0 || strcmp(argv[2], "-d") == 0)
  {
    i = 3;
    ANGULAR_DISTRIBUTION = true;
  }
  else if(strcmp(argv[2], "--analysis") == 0 || strcmp(argv[2], "-a") == 0)
  {
    i = 3;
    ANGULAR_DISTRIBUTION = false;
  }
  else
  {
    cerr<<"Error: no option set for ANGULAR_DISTRIBUTION.  Use flag --analysis or --angDist"<<endl;
    ANGULAR_DISTRIBUTION = false;
  }
  
  if(strcmp(argv[3], "--real") == 0 || strcmp(argv[3], "-r") == 0)
  {
    i = 4;
    SIMULATED_DATA = false;
  }
  else if(strcmp(argv[3], "--sim") == 0 || strcmp(argv[3], "-s") == 0)
  {
    i = 4;
    SIMULATED_DATA = true;
  }
  else
  {
    cerr<<"Error: no option set for SIMULATED_DATA.  Use flag --real or --sim"<<endl;
    SIMULATED_DATA = false;
  }
  
  TString outputname = "default.root";
//   cout<<strstr(argv[4],"--out=")<<endl;
  if(strstr(argv[4],"--out=") != NULL)
  {
    i = 5;
    string tmp = argv[4];
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
    
    //printf("obj->ClassName() = %s\n", obj->ClassName());
    if(strcmp(obj->ClassName(),"TCutG")!=0)
    {
      continue;
    }
    
    cutlist->Add(obj);
    //printf("found a cut! %s \n",((TNamed *)obj)->GetName());
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
    printf("%i analysis trees added to chain.\n",i-5);
  else
    printf("%i analysis trees added to chain.\n",i-4);
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
//         cout<<"For removal:"<<endl;
//         hist->Print();
        outlist->Remove(hist);
      }
      hist = nexthist;
    }
  }
  
  if(outputname != "default.root")
    cout<<"Setting output name to: "<<outputname<<endl;
  else if(!ANGULAR_DISTRIBUTION)
  {
    if(int(BEAM_ENERGY) == 30)
    {
      outputname = "outputlow.root";
      if(SIMULATED_DATA)
        outputname = "simoutputlow.root";
    }
    else if(int(BEAM_ENERGY) == 55)
      outputname = "outputhigh.root";
    else
      outputname = "output.root";
  }
  else
  {
    if(int(BEAM_ENERGY) == 30)
    {
      outputname = "angularlow.root";
      if(SIMULATED_DATA)
        outputname = "simulatedangularlow.root";
    }
    else if(int(BEAM_ENERGY) == 55)
      outputname = "angularhigh.root";
    else
      outputname = "angular.root";
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





