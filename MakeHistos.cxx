// g++ -g -O0 MakeHistos.cxx FunctionsForMakeHistos.cxx -Wl,--no-as-needed -o RunMe `grsi-config --cflags --all-libs --root`

#include "FunctionsForMakeHistos.hh"

double BEAM_ENERGY;
bool ANGULAR_DISTRIBUTION;
bool SIMULATED_DATA;

TTigress *tigress =  new TTigress;
TCSM *csm =  new TCSM;
TList *cutlist = new TList;

void ProcessChain(TChain *chain,TList *outlist)//, MakeFriend *myFriend)
{
  int nentries = chain->GetEntries();
  //   cout<<DRED;
  //   chain->Print();
  //   cout<<RESET_COLOR;
  TStopwatch w;
  w.Start();
  
  //   ofstream ofile;
  //   ofile.open("interesting.h");
  //   ofile<<"bool IsInteresting["<<nentries<<"] = {";
  //
  
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
    Be10Cut = "pid_low_thick_10Be_%i_v2";
    if(SIMULATED_DATA)
      Be10Cut = "pid_low_thick_10Be_%i_sim";
    
    Be9Cut = "pid_low_thick_9Be_%i_v1";
    
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
      bool IsInteresting = 0;
      
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
        
        /*
         *      1DV = 0
         *      1DH = 20
         *      1EV = 40
         *      1EH = 60
         *      2DV = 80
         *      2DH = 100
         *      2EV = 120
         *      2EH = 140
         *      3DV = 160
         *      3DH = 180
         *      4DV = 200
         *      4DH = 220
         */
        
//         int offset = -1;
//         if(hit->GetDetectorNumber()==1)
//           offset=0;
//         else if(hit->GetDetectorNumber()==2)
//           offset=80;
//         else if(hit->GetDetectorNumber()==3)
//           offset=160;
//         else if(hit->GetDetectorNumber()==4)
//           offset=200;
//         
//         temp2 = (TH2D*)outlist->FindObject("EnergyCheck");
//         if(hit->GetDVerticalEnergy()>10.)
//           temp2->Fill(hit->GetDVerticalStrip()+offset,hit->GetDVerticalEnergy()/1000.);
//         if(hit->GetDHorizontalEnergy()>10.)
//           temp2->Fill(hit->GetDHorizontalStrip()+offset+20,hit->GetDHorizontalEnergy()/1000.);
//         
//         if(hit->GetDetectorNumber()<3)
//         {
//           if(hit->GetEVerticalEnergy()>10.)
//             temp2->Fill(hit->GetEVerticalStrip()+offset+40,hit->GetEVerticalEnergy()/1000.);
//           if(hit->GetEHorizontalEnergy()>10.)
//             temp2->Fill(hit->GetEHorizontalStrip()+offset+60,hit->GetEHorizontalEnergy()/1000.);
//         }
//         
//         temp2 = (TH2D*)outlist->FindObject("ChargeCheck");
//         if(hit->GetDVerticalCharge() != 0)
//           temp2->Fill(hit->GetDVerticalStrip()+offset,hit->GetDVerticalCharge()/125.);
//         if(hit->GetDHorizontalCharge() != 0)
//           temp2->Fill(hit->GetDHorizontalStrip()+offset+20,hit->GetDHorizontalCharge()/125.);
//         
//         if(hit->GetDetectorNumber()<3)
//         {
//           if(hit->GetEVerticalCharge() != 0)
//             temp2->Fill(hit->GetEVerticalStrip()+offset+40,hit->GetEVerticalCharge()/125.);
//           if(hit->GetEHorizontalCharge() != 0)
//             temp2->Fill(hit->GetEHorizontalStrip()+offset+60,hit->GetEHorizontalCharge()/125.);
//         }
        
        if(hit->GetDPosition().Z() != 1 && hit->GetDPosition().Y() != 0 && hit->GetDPosition().X() != 0)
        {
          temp3 = (TH3D*)outlist->FindObject("positions");
          temp3->Fill(hit->GetDPosition().Z(),hit->GetDPosition().Y(),hit->GetDPosition().X());
          if(hit->GetEEnergy()>1)
            temp3->Fill(hit->GetEPosition().Z(),hit->GetEPosition().Y(),hit->GetEPosition().X());
          
          temp2 = (TH2D*)outlist->FindObject("positions_proj");
          temp2->Fill(hit->GetDPosition().Z(),hit->GetDPosition().X());
          if(hit->GetEEnergy()>1)
            temp2->Fill(hit->GetEPosition().Z(),hit->GetEPosition().X());
          
          temp2 = (TH2D*)outlist->FindObject("CSM_HP_Theta_Phi");
          if(hit->GetDEnergy()>1)
            temp2->Fill(hit->GetDPosition().Theta()*180/TMath::Pi(),hit->GetDPosition().Phi()*180/TMath::Pi());
          if(hit->GetEEnergy()>1)
            temp2->Fill(hit->GetEPosition().Theta()*180/TMath::Pi(),hit->GetEPosition().Phi()*180/TMath::Pi());
        }
        //////////////////////////////////////////////////
        if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form(Be10Cut,hit->GetDetectorNumber()))))
        {
          if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()) && hit->GetEEnergy() > 10)
          {
            temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE10",hit->GetDetectorNumber()));
            temp2->Fill(hit->GetThetaDeg(),hit->GetEnergyMeV());
            
            temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE10_corrected",hit->GetDetectorNumber()));
            temp2->Fill(hit->GetThetaDeg(),hit->GetCorrectedEnergyMeV("10Be"));
            
            TH1I* correctionpointer = (TH1I*)outlist->FindObject(Form("correction_straggling_%i",hit->GetDetectorNumber()));
            correctionpointer->Fill(hit->GetCorrectedEnergyMeVTarget(hit->GetCorrectedEnergyMeVDetector("10Be"),"10Be") - hit->GetEnergyMeV());
            correctionpointer = (TH1I*)outlist->FindObject(Form("correction_deadlayer_%i",hit->GetDetectorNumber()));
            correctionpointer->Fill(hit->GetCorrectedEnergyMeVDetector("10Be") - hit->GetEnergyMeV());
            
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
        //////////////////////////////////////////////////
        
        if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form(Be12Cut,hit->GetDetectorNumber()))))
        {
          if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()) && hit->GetEEnergy() > 10)
          {
            hit->SetIsotope(11,"be");
            
            temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_12Be_%i",hit->GetDetectorNumber()));
            temp2->Fill(hit->GetThetaDeg(),hit->GetEnergyMeV());
            
            temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_12Be_%i_corrected",hit->GetDetectorNumber()));
            temp2->Fill(hit->GetThetaDeg(),hit->GetCorrectedEnergyMeV("12Be"));
            
            temp2 = (TH2D*)outlist->FindObject("EvTheta_12Be");
            temp2->Fill(hit->GetThetaDeg(),hit->GetEnergyMeV());
            
            if(csm->GetMultiplicity()==1)
            {
              temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_12Be_%i_mult1",hit->GetDetectorNumber()));
              temp2->Fill(hit->GetThetaDeg(),hit->GetEnergyMeV());
            }
            
            TH1I* multpointer = (TH1I*)outlist->FindObject(Form("Be12Mult_%i",hit->GetDetectorNumber()));
            multpointer->Fill(csm->GetMultiplicity());
            
            double totalE = 0;
            for(int i=0; i<csm->GetMultiplicity();i++)
            {
              totalE += csm->GetHit(i)->GetEnergyMeV();
            }
            
            TH2I* derp = (TH2I*)outlist->FindObject("Be12TotalEnergy_v_Mult");
            derp->Fill(totalE,csm->GetMultiplicity());
            
            TH2I* derp2 = (TH2I*)outlist->FindObject("Be12ExEnergy_v_Mult");
            derp2->Fill(BEAM_ENERGY + 1.506 - totalE,csm->GetMultiplicity());
            
            double excite = GetExciteE_Heavy(hit,12);
            temp1 = (TH1D*)outlist->FindObject(Form("BeEx%i",hit->GetDetectorNumber()));
            if(temp1) temp1->Fill(excite);
            
            double excite_corrected = GetExciteE_Heavy_Corrected(hit,12);
            temp1 = (TH1D*)outlist->FindObject(Form("BeEx%i_corr",hit->GetDetectorNumber()));
            if(temp1) temp1->Fill(excite_corrected);
            
            temp2INT = (TH2I*)outlist->FindObject(Form("Be12Ex_Vs_Theta_%i",hit->GetDetectorNumber()));
            temp2INT->Fill(excite_corrected,hit->GetThetaDeg());
            
            temp2INT = (TH2I*)outlist->FindObject(Form("BeEx%i_mult",hit->GetDetectorNumber()));
            temp2INT->Fill(GetExciteE_Heavy_Corrected(hit,12),csm->GetMultiplicity());
            
            // 	  TH1I* stat = (TH1I*)outlist->FindObject("counts");
            // 	  stat->Fill(hit->GetDetectorNumber()+1);
            
            for(int y=0; y<tigress->GetAddBackMultiplicity();y++)
            {
              TTigressHit *tigresshit = tigress->GetAddBackHit(y);
              
              if(tigresshit->GetCore()->GetEnergy()>10)
              {
                
                if(excite_corrected > 8)
                {
                  temp1 = (TH1D*)outlist->FindObject(Form("Be12_Gamma_%i_dopp_gt8",hit->GetDetectorNumber()));
                  temp1->Fill(tigresshit->GetCore()->GetEnergy()/1000.);
                }
                else
                {
                  temp1 = (TH1D*)outlist->FindObject(Form("Be12_Gamma_%i_dopp_lt8",hit->GetDetectorNumber()));
                  temp1->Fill(tigresshit->GetCore()->GetEnergy()/1000.);
                }
                
                temp1 = (TH1D*)outlist->FindObject(Form("Be12_Gamma_%i",hit->GetDetectorNumber()));
                temp1->Fill(tigresshit->GetCore()->GetEnergy()/1000.);
                temp1 = (TH1D*)outlist->FindObject("Be12Gammas");
                temp1->Fill(tigresshit->GetCore()->GetEnergy()/1000.);
                
                temp1 = (TH1D*)outlist->FindObject(Form("Be12_Gamma_%i_eff",hit->GetDetectorNumber()));
                temp1->Fill(tigresshit->GetCore()->GetEnergy()/1000.,EfficiencyWeight(tigresshit));
                temp1 = (TH1D*)outlist->FindObject("Be12Gammas_eff");
                temp1->Fill(tigresshit->GetCore()->GetEnergy()/1000.,EfficiencyWeight(tigresshit));
                
                if(hit->GetDVerticalCFD()-tigresshit->GetTimeCFD() > -140)
                {
                  temp1 = (TH1D*)outlist->FindObject("Be12Gammas_prompt");
                  temp1->Fill(tigresshit->GetCore()->GetEnergy()/1000.);
                  temp1 = (TH1D*)outlist->FindObject("Be12Gammas_prompt_doppler");
                  temp1->Fill(Doppler(tigresshit,hit,12));
                }
                
                else if(hit->GetDVerticalCFD()-tigresshit->GetTimeCFD() < -150 && hit->GetDVerticalCFD()-tigresshit->GetTimeCFD() > -400 )
                {
                  temp1 = (TH1D*)outlist->FindObject("Be12Gammas_medium");
                  temp1->Fill(tigresshit->GetCore()->GetEnergy()/1000.);
                }
                
                else if(hit->GetDVerticalCFD()-tigresshit->GetTimeCFD() < -500)
                {
                  temp1 = (TH1D*)outlist->FindObject("Be12Gammas_delayed");
                  temp1->Fill(tigresshit->GetCore()->GetEnergy()/1000.);
                }
                
                double doppE = Doppler(tigresshit,hit,12);
                
                temp1 = (TH1D*)outlist->FindObject(Form("Be12_Gamma_%i_dopp",hit->GetDetectorNumber()));
                temp1->Fill(doppE);
                temp1 = (TH1D*)outlist->FindObject(Form("Be12_Gamma_%i_dopp_eff",hit->GetDetectorNumber()));
                temp1->Fill(doppE,EfficiencyWeight(tigresshit));
                
                temp1 = (TH1D*)outlist->FindObject("Be12GammasDopp");
                temp1->Fill(doppE);
                temp1 = (TH1D*)outlist->FindObject("Be12GammasDopp_eff");
                temp1->Fill(doppE,EfficiencyWeight(tigresshit));
                
                if( (doppE > 2.09 && doppE < 2.14) ||
                  (doppE > 2.70 && doppE < 2.76))
                {
                  TH2D* evtp = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE_gammaCut_dopp",hit->GetDetectorNumber()));
                  evtp->Fill(hit->GetThetaDeg(),hit->GetEnergyMeV());
                  
                  TH2D* tp = (TH2D*)outlist->FindObject(Form("pid_%i_summed_thickness_gcut_dopp",hit->GetDetectorNumber()));
                  tp->Fill(hit->GetEnergyMeV(),hit->GetDdE_dx());
                }
                
                if(doppE>=2.090 && doppE<= 2.135)
                {
                  TH1D* expg = (TH1D*)outlist->FindObject(Form("Be12Ex%i_gcut_2102",hit->GetDetectorNumber()));
                  expg->Fill(excite_corrected);
                }
                else if(doppE>=2.700 && doppE<=2.760)
                {
                  TH1D* expg = (TH1D*)outlist->FindObject(Form("Be12Ex%i_gcut_2702",hit->GetDetectorNumber()));
                  expg->Fill(excite_corrected);
                }
                
                if(tigresshit->GetCore()->GetEnergy()>2100 && tigresshit->GetCore()->GetEnergy()<2140)
                {
                  TH1D* delta = (TH1D*)outlist->FindObject("deltaGamma");
                  delta->Fill(hit->GetDVerticalCFD()-tigresshit->GetTimeCFD());
                }
                if(tigresshit->GetCore()->GetEnergy()>2100 && tigresshit->GetCore()->GetEnergy()<2150)
                {
                  TH2D* evtp = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE_gammaCut",hit->GetDetectorNumber()));
                  evtp->Fill(hit->GetThetaDeg(),hit->GetEnergyMeV());
                }
              }
            }
          }
        }
        
        if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form(Be12Cut,hit->GetDetectorNumber()))))
        {
          if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()) )//&& csm->GetMultiplicity()>=3)
          {
            if(csm->GetMultiplicity()>=3)
            {
              TH1D* totalenergypointer=(TH1D*)outlist->FindObject(Form("TotalEnergy_%i_12Be",hit->GetDetectorNumber()));
              double TotalE = 0;
              for(int asdf=0;asdf<csm->GetMultiplicity();asdf++)
              {
                TotalE+=csm->GetHit(asdf)->GetEnergyMeV();
              }
              totalenergypointer->Fill(TotalE);
            }
          }
        }
        
        
        if(DEBUG) cout<<"EVTheta"<<endl;
        temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%iD",hit->GetDetectorNumber()));
        temp2->Fill(hit->GetThetaDeg(),hit->GetDEnergy()/1000.);
        
        temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%iE",hit->GetDetectorNumber()));
        if(temp2) temp2->Fill(hit->GetEPosition().Theta()*180/TMath::Pi(),hit->GetEEnergy()/1000.);
        if(DEBUG) cout<<"HitPattern"<<endl;
        
        if(hit->GetDetectorNumber() == 2)
        {
          TH1I *blehptr = (TH1I*)outlist->FindObject(Form("CenterCheck_%i",hit->GetDetectorNumber()));
          blehptr->Fill(15-hit->GetDVerticalStrip());
        }
        else if(hit->GetDetectorNumber() == 1)
        {
          TH1I *blehptr = (TH1I*)outlist->FindObject(Form("CenterCheck_%i",hit->GetDetectorNumber()));
          blehptr->Fill(hit->GetDVerticalStrip());
        }
        
        
        if(hit->GetDEnergy()>500)
        {
          temp2INT = (TH2I*)outlist->FindObject(Form("CSM_HitPattern_%iD",hit->GetDetectorNumber()));
          temp2INT->Fill(hit->GetDVerticalStrip(),hit->GetDHorizontalStrip());
        }
        
        temp2INT = (TH2I*)outlist->FindObject(Form("CSM_HitPattern_%iE",hit->GetDetectorNumber()));
        if(temp2INT) temp2INT->Fill(hit->GetEVerticalStrip(),hit->GetEHorizontalStrip());
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
          temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%iTotal",hit->GetDetectorNumber()));
          temp2->Fill(hit->GetThetaDeg(),hit->GetEnergyMeV());
        }
        
        if(hit->GetDetectorNumber()==3)
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
        
        if(hit->GetDVerticalEnergy()>0 && hit->GetDHorizontalEnergy()>0)
        {
          temp2 = (TH2D*)outlist->FindObject(Form("CheckCalD_%i",hit->GetDetectorNumber()));
          if(temp2) temp2->Fill(hit->GetDVerticalEnergy()/1000.,hit->GetDHorizontalEnergy()/1000.);
        }
        if(hit->GetEVerticalEnergy()>0 && hit->GetEHorizontalEnergy()>0)
        {
          temp2 = (TH2D*)outlist->FindObject(Form("CheckCalE_%i",hit->GetDetectorNumber()));
          if(temp2) temp2->Fill(hit->GetEVerticalEnergy()/1000.,hit->GetEHorizontalEnergy()/1000.);
        }
        
        //Multiplicity cut plots
        
        if(hit->GetEEnergy()>100.)
        {
          temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_mult%i",hit->GetDetectorNumber(),csm->GetMultiplicity()));
          if(temp2) temp2->Fill(hit->GetEEnergy()/1000.,hit->GetDEnergy()/1000.);
          
          temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_summed_thickness_mult%i",hit->GetDetectorNumber(),csm->GetMultiplicity()));
          temp2->Fill(hit->GetEnergyMeV(),hit->GetDdE_dx());
        }
        
        temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%iTotal_mult%i",hit->GetDetectorNumber(),csm->GetMultiplicity()));
        if(temp2) temp2->Fill(hit->GetThetaDeg(),(hit->GetEEnergy()+hit->GetDEnergy())/1000.);
        
        if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form("thetas_Be11_%i_%02i",hit->GetDetectorNumber(),int(hit->GetThetaDeg())))))
        {
          if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()))
          {
            temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE11",hit->GetDetectorNumber()));
            if(temp2) temp2->Fill(hit->GetThetaDeg(),hit->GetEnergyMeV());	  
          }
        }
      }
      //***********************
      //    Gamma Plots
      //***********************
      for(int I=0;I<csm->GetMultiplicity();I++)
      {
        TCSMHit *hit = csm->GetHit(I);
        if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form("pid_high_thick_allbe_%i_v1",hit->GetDetectorNumber()))))
        {
          if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()) && hit->GetEEnergy() > 10)
          {
            for(int y=0; y<tigress->GetAddBackMultiplicity();y++)
            {
              TTigressHit *tigresshit = tigress->GetAddBackHit(y);
              
              if(tigresshit->GetCore()->GetEnergy()>10)
              {
                TH1D* gpointer = (TH1D*)outlist->FindObject("GammaBe");
                gpointer->Fill(tigresshit->GetCore()->GetEnergy()/1000.);
              }
            }
          }
        }
        
        if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form("pid_high_thick_allli_%i_v1",hit->GetDetectorNumber()))))
        {
          if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()) && hit->GetEEnergy() > 10)
          {
            for(int y=0; y<tigress->GetAddBackMultiplicity();y++)
            {
              TTigressHit *tigresshit = tigress->GetAddBackHit(y);
              
              if(tigresshit->GetCore()->GetEnergy()>10)
              {
                TH1D* gpointer = (TH1D*)outlist->FindObject("GammaLi");
                gpointer->Fill(tigresshit->GetCore()->GetEnergy()/1000.);
              }
            }
          }
        }
        if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form("pid_high_thick_7Li_%i_v1",hit->GetDetectorNumber()))))
        {
          if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()) && hit->GetEEnergy() > 10)
          {
            for(int y=0; y<tigress->GetAddBackMultiplicity();y++)
            {
              TTigressHit *tigresshit = tigress->GetAddBackHit(y);
              
              if(tigresshit->GetCore()->GetEnergy()>10)
              {
                TH1D* gpointer = (TH1D*)outlist->FindObject("GammaLi7");
                gpointer->Fill(tigresshit->GetCore()->GetEnergy()/1000.);
              }
            }
          }
        }
        if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form("pid_high_alpha_thickness_%i_v1",hit->GetDetectorNumber()))))
        {
          if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()) && hit->GetEEnergy() > 10)
          {
            for(int y=0; y<tigress->GetAddBackMultiplicity();y++)
            {
              TTigressHit *tigresshit = tigress->GetAddBackHit(y);
              
              if(tigresshit->GetCore()->GetEnergy()>10)
              {
                TH1D* gpointer = (TH1D*)outlist->FindObject("GammaAlpha");
                gpointer->Fill(tigresshit->GetCore()->GetEnergy()/1000.);
              }
            }
          }
        }
      }
      //***********************
      //looking for non id 12Be
      //***********************
      
      //if(csm->GetMultiplicity()>=2)
      {
        for(int i=0;i<csm->GetMultiplicity();i++)
        {
          TCSMHit *hit = csm->GetHit(i);
          if(hit->GetDetectorNumber()<3 && hit->GetEEnergy() < 10)
          {
            for(int y=0; y<tigress->GetAddBackMultiplicity();y++)
            {
              TTigressHit *tigresshit = tigress->GetAddBackHit(y);
              
              if(tigresshit->GetCore()->GetEnergy()>10)
              {
                TH1D* gpointer = (TH1D*)outlist->FindObject("Be12GammasNoID");
                gpointer->Fill(tigresshit->GetCore()->GetEnergy()/1000.);
                gpointer = (TH1D*)outlist->FindObject("Be12GammasDoppNoID");
                gpointer->Fill(Doppler(tigresshit,hit,12));
                
                if(tigresshit->GetCore()->GetEnergy()/1000. < 2.12 || tigresshit->GetCore()->GetEnergy()/1000. > 2.13)
                {
                  gpointer = (TH1D*)outlist->FindObject("Be12GammasDoppNoID_suppressed");
                  gpointer->Fill(Doppler(tigresshit,hit,12));
                }
              }
              if(tigresshit->GetCore()->GetEnergy()>2100 && tigresshit->GetCore()->GetEnergy()<2150)
              {
                TH2D* evtp = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE_gammaCutNoID",hit->GetDetectorNumber()));
                evtp->Fill(hit->GetThetaDeg(),hit->GetEnergyMeV());
              }
            }
            break;
          }
        }
      }
      
      //***********************
      //  looking for 2 alphas
      //***********************
      
      if(csm->GetMultiplicity()>=3)
      {
        vector<int> aloc;
        for(int i=0;i<csm->GetMultiplicity();i++)
        {
          if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form("pid_high_alpha_thickness_%i_v1",csm->GetHit(i)->GetDetectorNumber()))))
          {
            if(cut->IsInside(csm->GetHit(i)->GetEnergyMeV(),csm->GetHit(i)->GetDdE_dx()) && csm->GetHit(i)->GetEEnergy() > 10)
            {
              aloc.push_back(i);
            }
          }
        }
        
        if(aloc.size()>0)
        {
          TH1D* etotalphapointer = (TH1D*)outlist->FindObject(Form("ETot_%i_2alpha",csm->GetHit(aloc.at(0))->GetDetectorNumber()));
          double etot=0;
          for(int ee = 0; ee<csm->GetMultiplicity(); ee++)
          {
            etot += csm->GetHit(ee)->GetEnergyMeV();
          }
          etotalphapointer->Fill(etot);
          
          TH1I* numalphaptr = (TH1I*)outlist->FindObject("AlphaMult");
          numalphaptr->Fill(aloc.size());
        }
      }
      //***********************
      //      Fancy dE v E
      //***********************
      
      for(int xx = 0;xx<csm->GetMultiplicity();xx++)
      {
        TCSMHit *hit = csm->GetHit(xx);
        if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form("highblob_%i_v1",hit->GetDetectorNumber()))))
        {
          if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()) && hit->GetEEnergy() > 10)
          {
            TH1I* blobpointer = (TH1I*)outlist->FindObject("MultBlobHigh");
            blobpointer->Fill(csm->GetMultiplicity());
          }
        }
        if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form("lowblob_%i_v1",hit->GetDetectorNumber()))))
        {
          if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()) && hit->GetEEnergy() > 10)
          {
            TH1I* blobpointer = (TH1I*)outlist->FindObject("MultBlobLow");
            blobpointer->Fill(csm->GetMultiplicity());
          }
        }
      }
      
      if(csm->GetMultiplicity()>=2)
      {
        for(int aa=0;aa<csm->GetMultiplicity();aa++)
        {
          TCSMHit *hita = csm->GetHit(aa);
          if(hita->GetEEnergy()>0.)
          {
            for(int bb=aa+1;bb<csm->GetMultiplicity();bb++)
            {
              TCSMHit *hitb = csm->GetHit(bb);
              if(hitb->GetEEnergy()>0.)
              {
                if(hita->GetDetectorNumber() == hitb->GetDetectorNumber())
                {
                  TH2D *hit2histo = (TH2D*)outlist->FindObject(Form("pid_%i_summed_thickness_2hit",hita->GetDetectorNumber()));
                  hit2histo->Fill(hita->GetEnergyMeV(),hita->GetDdE_dx());
                  hit2histo->Fill(hitb->GetEnergyMeV(),hitb->GetDdE_dx());
                  if(hita->GetDVerticalStrip()==hitb->GetDVerticalStrip() && hita->GetDHorizontalStrip() == hitb->GetDHorizontalStrip())
                  {
                    TH2D *hit2histo = (TH2D*)outlist->FindObject(Form("pid_%i_summed_thickness_2hit_samepix",hita->GetDetectorNumber()));
                    hit2histo->Fill(hita->GetEnergyMeV(),hita->GetDdE_dx());
                    hit2histo->Fill(hitb->GetEnergyMeV(),hitb->GetDdE_dx());
                  }
                }
              }
            }
            if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form(Be12Cut,hita->GetDetectorNumber()))))
            {
              if(cut->IsInside(hita->GetEnergyMeV(),hita->GetDdE_dx()) && hita->GetEEnergy() > 10)
              {
                //we have a confirmed 12Be
                for(int bb=0;bb<csm->GetMultiplicity();bb++)
                {
                  TCSMHit *hitb = csm->GetHit(bb);
                  if(hitb==hita)
                    continue;
                  
                  if(hitb->GetEEnergy()>10. && hitb->GetDEnergy() > 10. && hita->GetDetectorNumber() != hitb->GetDetectorNumber())
                  {
                    TH2D *corrpointer = (TH2D*)outlist->FindObject(Form("pid_%i_summed_thickness_corr12",hita->GetDetectorNumber()));
                    corrpointer->Fill(hitb->GetEnergyMeV(),hitb->GetDdE_dx());
                  }
                }
              }
            }
          }
        }
      }
      
      
      //***********************
      //Looking for below PID 10Be
      //***********************
      if(csm->GetMultiplicity() == 2)
      {
        TCSMHit *hita = csm->GetHit(0);
        TCSMHit *hitb = csm->GetHit(1);
        //if(hita->GetEEnergy()<10. && hitb->GetEEnergy()<10.)
        {
          if(hita->GetDetectorNumber() == hitb->GetDetectorNumber())
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
                
                if(TCutG *cut = (TCutG*)(cutlist->FindObject("dual10be_exvt_reject_low")))
                {
                  TH1I *dualexr = (TH1I*)outlist->FindObject("DualBe10_ex_allcut_reject");
                  TH2I *dualacr = (TH2I*)outlist->FindObject("Dual10Be_allcut_corrected_reject");
                  
                  if(!cut->IsInside(excitecA,hita->GetThetaDeg()))
                  {
                    dualacr->Fill(hita->GetThetaDeg(),hita->GetCorrectedEnergyMeV("10be"));
                    dualexr->Fill(excitecA);
                  }
                  if(!cut->IsInside(excitecB,hitb->GetThetaDeg()))
                  {
                    dualacr->Fill(hitb->GetThetaDeg(),hitb->GetCorrectedEnergyMeV("10be"));
                    dualexr->Fill(excitecB);
                  }
                }
                
                TH1I *gamptr = (TH1I*)outlist->FindObject("DualBe10_allcut_gammas");
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
                
                for(int y=0; y<tigress->GetAddBackMultiplicity();y++)
                {
                  TTigressHit *tigresshit = tigress->GetAddBackHit(y);
                  
                  if(tigresshit->GetCore()->GetEnergy()>10)
                  {
                    gamptr->Fill(tigresshit->GetCore()->GetEnergy()/1000.);
                    gamptrh->Fill(Doppler(tigresshit,Hhit,10));
                    gamptrl->Fill(Doppler(tigresshit,Lhit,10));
                    
                    gamptreff->Fill(tigresshit->GetCore()->GetEnergy()/1000.,EfficiencyWeight(tigresshit));
                    gamptrheff->Fill(Doppler(tigresshit,Hhit,10),EfficiencyWeight(tigresshit));
                    gamptrleff->Fill(Doppler(tigresshit,Lhit,10),EfficiencyWeight(tigresshit));
                    
                    double dopp = Doppler(tigresshit,Hhit,10);
                    
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
                      TH1I* dualexgcut = (TH1I*)outlist->FindObject("DualBe10Ex_gcut_286-7");
                      dualexgcut->Fill(excitecA);
                      dualexgcut->Fill(excitecB);
                    }
                  }
                }         
              }
            }
          }
        }
      }
      //***********************
      //        Gammas
      //***********************
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
      
      if(tigress->GetAddBackMultiplicity()>1)
      {
        for(int y=1; y<tigress->GetAddBackMultiplicity();y++)
        {
          TTigressHit *tigresshit = tigress->GetAddBackHit(y);
          
          TH1I* difp = (TH1I*)outlist->FindObject("GvGTimeDiff");
          difp->Fill(tigress->GetAddBackHit(0)->GetTimeCFD()-tigresshit->GetTimeCFD());
        }
      }
      
//       for(int y=0; y<csm->GetMultiplicity();y++)
//       {
//         TCSMHit* hit = csm->GetHit(y);
//         for(int y=0; y<tigress->GetAddBackMultiplicity();y++)
//         {
//           TTigressHit *tigresshit = tigress->GetAddBackHit(y);
//           
//           TH1I* difp = (TH1I*)outlist->FindObject("TimeDiffAll");
//           difp->Fill(hit->GetDVerticalCFD()-tigresshit->GetTimeCFD());
//           TH2D* point = (TH2D*)outlist->FindObject("GEvT");
//           point->Fill(hit->GetDVerticalCFD()-tigresshit->GetTimeCFD(),tigresshit->GetCore()->GetEnergy()/1000.);
//         }
//         
//         if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form(Be12Cut,hit->GetDetectorNumber()))))
//         {
//           if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()) && hit->GetEEnergy() > 10)
//           {
//             for(int y=0; y<tigress->GetAddBackMultiplicity();y++)
//             {
//               TTigressHit *tigresshit = tigress->GetAddBackHit(y);
//               
//               TH1I* difp12 = (TH1I*)outlist->FindObject("TimeDiff12Be");
//               difp12->Fill(hit->GetDVerticalCFD()-tigresshit->GetTimeCFD());
//               TH2D* point12 = (TH2D*)outlist->FindObject("GEvT_12Be");
//               point12->Fill(hit->GetDVerticalCFD()-tigresshit->GetTimeCFD(),tigresshit->GetCore()->GetEnergy()/1000.);
//               
//               TH2D* matp = (TH2D*)outlist->FindObject("GammaMatrix_12Be");
//               for(int zz = y+1; zz<tigress->GetAddBackMultiplicity();zz++)
//               {
//                 matp->Fill(tigresshit->GetCore()->GetEnergy()/1000.,tigress->GetAddBackHit(zz)->GetCore()->GetEnergy()/1000.);
//               }
//             }
//           }
//         }
//       }
      
//       for(int y=0; y<tigress->GetAddBackMultiplicity();y++)
//       {
//         TTigressHit *tigresshit = tigress->GetAddBackHit(y);
//         TH2D* matp = (TH2D*)outlist->FindObject("GammaMatrix");
//         for(int zz = y+1; zz<tigress->GetAddBackMultiplicity();zz++)
//         {
//           matp->Fill(tigresshit->GetCore()->GetEnergy()/1000.,tigress->GetAddBackHit(zz)->GetCore()->GetEnergy()/1000.);
//         }
//       }
      
//       for(int y=0; y<tigress->GetAddBackMultiplicity();y++)
//       {
//         TTigressHit *tigresshit = tigress->GetAddBackHit(y);
//         
//         if(tigresshit->GetCore()->GetEnergy()>2120 && tigresshit->GetCore()->GetEnergy()<2130)
//         {
//           for(int q=0; q<csm->GetMultiplicity();q++)
//           {
//             TCSMHit* hit = csm->GetHit(q);
//             
//             if(hit->GetDetectorNumber()<3)
//             {
//               TH2D* tp = (TH2D*)outlist->FindObject(Form("pid_%i_summed_thickness_gcut",hit->GetDetectorNumber()));
//               tp->Fill(hit->GetEnergyMeV(),hit->GetDdE_dx());
//             }
//             TH1D* gcutp = (TH1D*)outlist->FindObject(Form("gCutE_%i",hit->GetDetectorNumber()));
//             gcutp->Fill(hit->GetEnergyMeV());
//           }
//         }
//       }
      
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
            //double* CorrVals = CorrParticle(hit->GetCorrectedEnergyMeV("10be")*1000., hit->GetPosition().Theta(), hit->GetPosition().Phi(),MASS_BE10);
            
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
                
                double excite = GetExciteE_Heavy_Corrected(hit,10);
                
                if(dopp>=2.577 && dopp<=2.612)
                {
                  TH1D* expg = (TH1D*)outlist->FindObject(Form("Be10Ex%i_gcut_2589_opp",hit->GetDetectorNumber()));
                  expg->Fill(excite);
                }
                else if(dopp>=2.876 && dopp<=2.913)
                {
                  TH1D* expg = (TH1D*)outlist->FindObject(Form("Be10Ex%i_gcut_2894_opp",hit->GetDetectorNumber()));
                  expg->Fill(excite);
                }
                else if(dopp>=3.337 && dopp<=3.402)
                {
                  TH1D* expg = (TH1D*)outlist->FindObject(Form("Be10Ex%i_gcut_3368_opp",hit->GetDetectorNumber()));
                  expg->Fill(excite);
                }
                else if(dopp>=5.951 && dopp<=5.986)
                {
                  TH1D* expg = (TH1D*)outlist->FindObject(Form("Be10Ex%i_gcut_5958_opp",hit->GetDetectorNumber()));
                  expg->Fill(excite);
                }
              }
            }
            
            for(int j = 0; j<csm->GetMultiplicity(); j++)
            {
              if(i==j)
                continue;
              
              TCSMHit *opphit = csm->GetHit(j);
              
              int conditions = 0;
              if(AlmostEqual(hit->GetEnergy(),CorrVals[0]))
                conditions++;
              if(AlmostEqual(hit->GetPosition().Theta(),CorrVals[1]))
                conditions++;
              if(AlmostEqual(hit->GetPosition().Phi(),CorrVals[2]))
                conditions++;
              
              if((conditions == 3 && int(BEAM_ENERGY)==55) || int(BEAM_ENERGY)==30)
              {
                for(int y=0; y<tigress->GetAddBackMultiplicity();y++)
                {
                  TTigressHit *tigresshit = tigress->GetAddBackHit(y);
                  
                  if(tigresshit->GetCore()->GetEnergy()>10)
                  {
                    TH1D* dopptr = (TH1D*)outlist->FindObject(Form("Be10_Gamma_%i_dopp_opp",hit->GetDetectorNumber()));
                    dopptr->Fill(Doppler(tigresshit,opphit,10));
                    TH1D* dopptreff = (TH1D*)outlist->FindObject(Form("Be10_Gamma_%i_dopp_opp_eff",hit->GetDetectorNumber()));
                    dopptreff->Fill(Doppler(tigresshit,opphit,10),EfficiencyWeight(tigresshit));
                  }
                }
                
                TH1D* exptr = (TH1D*)outlist->FindObject(Form("Be10Ex%i_corr_opp",hit->GetDetectorNumber()));
                exptr->Fill(GetExciteE_Heavy_Corrected(opphit,10));
                
                TH2D* evtptr = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE10_opp",hit->GetDetectorNumber()));
                evtptr->Fill(opphit->GetThetaDeg(),opphit->GetEnergyMeV());
                
                TH1D* supexptr = (TH1D*)outlist->FindObject(Form("Be10Ex%i_corr_supp",hit->GetDetectorNumber()));
                supexptr->Fill(GetExciteE_Heavy_Corrected(hit,10));
              }
            }
          }
          
        }
      }
      
      //***********************
      //  Multiplicity plots
      //***********************
      if(csm->GetMultiplicity()!=0 || tigress->GetAddBackMultiplicity()!=0)
      {
        bool be12=0;
        for(int aa=0;aa<csm->GetMultiplicity();aa++)
        {
          TCSMHit *hita = csm->GetHit(aa);
          if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form(Be12Cut,hita->GetDetectorNumber()))))
          {
            if(cut->IsInside(hita->GetEnergyMeV(),hita->GetDdE_dx()) && hita->GetEEnergy() > 10)
            {
              be12=1;
            }
          }
        }
        
        TH2I* cvgp = (TH2I*)outlist->FindObject("CvGmult");
        cvgp->Fill(csm->GetMultiplicity(),tigress->GetAddBackMultiplicity());
        
        if(be12)
        {
          TH2I* cvgpbe = (TH2I*)outlist->FindObject("CvGmult_12be");
          cvgpbe->Fill(csm->GetMultiplicity(),tigress->GetAddBackMultiplicity());
        }
      }
      
      //***********************
      //    One-off Low PID and High PID plots
      //***********************
      
      if(int(BEAM_ENERGY) == 55)
      {
        for(int xx = 0;xx<csm->GetMultiplicity();xx++)
        {
          TCSMHit *hit = csm->GetHit(xx);
          
          if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form("pid_high_thick_12Be_HighEnergyOnly_%i_v1",hit->GetDetectorNumber()))))
          {
            if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()) && hit->GetEEnergy() > 10)
            {
              TH2D* EvTPtr = (TH2D*)outlist->FindObject(Form("EvTheta_12Be_%i_high",hit->GetDetectorNumber()));
              EvTPtr->Fill( hit->GetThetaDeg(), hit->GetEnergyMeV());
              
              TH1D* geSpec = (TH1D*)outlist->FindObject("Be12Gammas_high");
              
              for(int y=0; y<tigress->GetAddBackMultiplicity();y++)
              {
                geSpec->Fill(tigress->GetAddBackHit(y)->GetCore()->GetEnergy()/1000.);
              }
            }
          }
          if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form("pid_high_thick_12Be_LowEnergyOnly_%i_v1",hit->GetDetectorNumber()))))
          {
            if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()) && hit->GetEEnergy() > 10)
            {
              TH2D* EvTPtr = (TH2D*)outlist->FindObject(Form("EvTheta_12Be_%i_low",hit->GetDetectorNumber()));
              EvTPtr->Fill( hit->GetThetaDeg(), hit->GetEnergyMeV());
              
              TH1D* geSpec = (TH1D*)outlist->FindObject("Be12Gammas_low");
              
              for(int y=0; y<tigress->GetAddBackMultiplicity();y++)
              {
                geSpec->Fill(tigress->GetAddBackHit(y)->GetCore()->GetEnergy()/1000.);
              }
            }
          }
        }
      }
      
      //***********************
      //         End
      //***********************
      
      ////////////////////////////////////////////////////////////////////////
      
    }
    else if(ANGULAR_DISTRIBUTION)
    {
      for(int i = 0;i<csm->GetMultiplicity();i++)
      {
        TCSMHit *hit = csm->GetHit(i);
        if(hit->GetDVerticalStrip()>0 && hit->GetDVerticalStrip()<15)//For now, suppress the perimeter strips, as there is a loss of solid angle there.
        {
          if(hit->GetDHorizontalStrip()>0 && hit->GetDHorizontalStrip()<15)
          {
            if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form(Be11Cut,hit->GetDetectorNumber()))))
            {
              if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()) && hit->GetEEnergy() > 10)
              {           
                double ex11c =GetExciteE_Heavy_Corrected(hit,11);
                
                if(ex11c >= -1 && ex11c<= 1)//We are looking at an elastic scattered 11Be
                {
                  
                  TH1I* tmpangdist = (TH1I*)outlist->FindObject("ang_dist_11be_0");
                  tmpangdist->Fill(CalcCOMThetaDeg(hit,11),1./hit->GetSolidAngleD());
                }
                else if(ex11c >= 1.8 && ex11c<= 3.4)//We are looking at an excited 9Be
                {
                  
                  TH1I* tmpangdist = (TH1I*)outlist->FindObject("ang_dist_11be_2.6");
                  tmpangdist->Fill(CalcCOMThetaDeg(hit,11),1./hit->GetSolidAngleD());
                }
              }
            }
            
            if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form(Be10Cut,hit->GetDetectorNumber()))))
            {
              if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()) && hit->GetEEnergy() > 10)
              {  
                // 0     -1 to 1.2     .1
                // 3.3  2.5 to 4.4     3.45
                // 6    5.5 to 7       6.25
                //9.3     9 to 10.5    9.75
                double ex10c =GetExciteE_Heavy_Corrected(hit,10);
                
                if(ex10c >= -1 && ex10c<= 1.2)
                {
                  TH1I* tmpangdist = (TH1I*)outlist->FindObject("ang_dist_10be_0_pid");
                  tmpangdist->Fill(CalcCOMThetaDeg(hit,10),1./hit->GetSolidAngleD());
                }
                
                else if(ex10c >= 2.5 && ex10c<= 4.4)
                {
                  TH1I* tmpangdist = (TH1I*)outlist->FindObject("ang_dist_10be_3.3_pid");
                  tmpangdist->Fill(CalcCOMThetaDeg(hit,10),1./hit->GetSolidAngleD());
                }
                
                else if(ex10c >= 5.5 && ex10c<= 7)
                {
                  TH1I* tmpangdist = (TH1I*)outlist->FindObject("ang_dist_10be_6_pid");
                  tmpangdist->Fill(CalcCOMThetaDeg(hit,10),1./hit->GetSolidAngleD());
                }
                
                else if(ex10c >= 9 && ex10c<= 10.5)
                {
                  TH1I* tmpangdist = (TH1I*)outlist->FindObject("ang_dist_10be_9.3_pid");
                  tmpangdist->Fill(CalcCOMThetaDeg(hit,10),1./hit->GetSolidAngleD());
                }
              }
            }
          }
        }
      }
      
      
      if(csm->GetMultiplicity() == 2)
      {
        TCSMHit *hita = csm->GetHit(0);
        TCSMHit *hitb = csm->GetHit(1);
        
        if(hita->GetDetectorNumber() == hitb->GetDetectorNumber())
          continue;
        
        double* CorrVals = CorrParticle(hita, 10);
        
        double energydiff = (hitb->GetEnergy() - CorrVals[0])/1000.; // MeV
        double thetadiff = (hitb->GetPosition().Theta() - CorrVals[1])*180./TMath::Pi(); // Degrees
        double phidiff = (hitb->GetPosition().Phi() - CorrVals[2])*180./TMath::Pi(); // Degrees
        if(phidiff >= -10 && phidiff <= 10)
        {
          if(energydiff >= -2.5 && energydiff <= .5)
          {
            if(thetadiff >= -3 && thetadiff <= 5)
            {
              double ex10cA =GetExciteE_Heavy_Corrected(hita,10);
              double ex10cB =GetExciteE_Heavy_Corrected(hitb,10);
              // 6    4.5 to 7.5
              // 9.3   7.5 to 10
              // 12   11.2 to 12.7
              
              if(ex10cA >= 4.5 && ex10cA <= 7.5)
              {
                TH1I* tmpangdist = (TH1I*)outlist->FindObject("ang_dist_10be_6_dual");
                tmpangdist->Fill(CalcCOMThetaDeg(hita,10),1./hita->GetSolidAngleD());
              }
              else if(ex10cA >= 7.5 && ex10cA <= 10)
              {
                TH1I* tmpangdist = (TH1I*)outlist->FindObject("ang_dist_10be_9.3_dual");
                tmpangdist->Fill(CalcCOMThetaDeg(hita,10),1./hita->GetSolidAngleD());
              }
              else if(ex10cA >= 11.2 && ex10cA <= 12.7)
              {
                TH1I* tmpangdist = (TH1I*)outlist->FindObject("ang_dist_10be_12_dual");
                tmpangdist->Fill(CalcCOMThetaDeg(hita,10),1./hita->GetSolidAngleD());
              }
              
              if(ex10cB >= 4.5 && ex10cB <= 7.5)
              {
                TH1I* tmpangdist = (TH1I*)outlist->FindObject("ang_dist_10be_6_dual");
                tmpangdist->Fill(CalcCOMThetaDeg(hitb,10),1./hitb->GetSolidAngleD());
              }
              else if(ex10cB >= 7.5 && ex10cB <= 10)
              {
                TH1I* tmpangdist = (TH1I*)outlist->FindObject("ang_dist_10be_9.3_dual");
                tmpangdist->Fill(CalcCOMThetaDeg(hitb,10),1./hitb->GetSolidAngleD());
              }
              else if(ex10cB >= 11.2 && ex10cB <= 12.7)
              {
                TH1I* tmpangdist = (TH1I*)outlist->FindObject("ang_dist_10be_12_dual");
                tmpangdist->Fill(CalcCOMThetaDeg(hitb,10),1./hitb->GetSolidAngleD());
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
  
  //   ofile<<"};"<<endl;
  //   ofile.close();
  
  //printf("\tprocessed " DYELLOW "%i" RESET_COLOR "/" DBLUE "%i" RESET_COLOR " entries in " DRED "%.02f" RESET_COLOR " seconds\n",x,nentries,w.RealTime());
  cout<<endl;
  return;
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
  
  
//   TFile cf2("thetacuts.root");
//   TIter *iter2 = new TIter(cf2.GetListOfKeys());
//   
//   while(TObject *obj = iter2->Next())
//   {
//     obj = ((TKey *)obj)->ReadObj();
//     
//     //printf("obj->ClassName() = %s\n", obj->ClassName());
//     if(strcmp(obj->ClassName(),"TCutG")!=0)
//     {
//       continue;
//     }
//     
//     cutlist->Add(obj);
//     //printf("found a cut! %s \n",((TNamed *)obj)->GetName());
//     ncuts++;
//   }
  
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
  
  
  printf("%i analysis trees added to chain.\n",i-4);
  if(!SIMULATED_DATA)
    chain->SetBranchAddress("TTigress",&tigress);
  chain->SetBranchAddress("TCSM",&csm);
  
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
  
  TString outputname;
  
  if(!ANGULAR_DISTRIBUTION)
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





