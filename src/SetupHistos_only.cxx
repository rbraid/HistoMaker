#include "../include/SetupHistos_only.hh"

void SetupHistos(TList *outlist)
{
  TH1D *temp1 = 0;
  TH1I *temp1INT = 0;
  TH2D *temp2 = 0;
  TH2I *temp2INT = 0;
//   TH3D *temp3 = 0;
//   TH3I *temp3INT = 0;
  
  for(int id = 1; id<=4;id++)
  {
    outlist->Add(new TH2D(Form("EvTheta_%i_BE10_Fred",id),Form("EvTheta %i of particle associated with the identified 10Be",id),100,0,100,350,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE10_Fred",id));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
  }
  
  
  for(int id = 1; id<=2;id++)
  {
    outlist->Add(new TH1D(Form("Be10_Gamma_%i",id),"Gamma spectrum associated with 10Be PID",30000,0,30));
    temp1 = (TH1D*)outlist->FindObject(Form("Be10_Gamma_%i",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");
    
    outlist->Add(new TH1D(Form("Be10_Gamma_%i_dopp",id),"Gamma spectrum, doppler corrected on 10Be PID",30000,0,30));
    temp1 = (TH1D*)outlist->FindObject(Form("Be10_Gamma_%i_dopp",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");
    
    outlist->Add(new TH1D(Form("Be11_Gamma_%i",id),"Gamma spectrum assocaited with a 11Be PID",30000,0,30));
    temp1 = (TH1D*)outlist->FindObject(Form("Be11_Gamma_%i",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");
    
    outlist->Add(new TH1D(Form("Be11_Gamma_%i_dopp",id),"Gamma spectrum, doppler corrected on 11Be PID",30000,0,30));
    temp1 = (TH1D*)outlist->FindObject(Form("Be11_Gamma_%i_dopp",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");
    
    outlist->Add(new TH1D(Form("Be10_Gamma_%i_eff",id),"Gamma spectrum",30000,0,30));
    temp1 = (TH1D*)outlist->FindObject(Form("Be10_Gamma_%i_eff",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");
    
    outlist->Add(new TH1D(Form("Be10_Gamma_%i_dopp_eff",id),"Gamma spectrum",30000,0,30));
    temp1 = (TH1D*)outlist->FindObject(Form("Be10_Gamma_%i_eff",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");
    
    outlist->Add(new TH1D(Form("GammaCut_dopp_%i",id),"Gamma spectrum cut on Ex Spectrum, doppler corrected",3000,0,30));
    temp1 = (TH1D*)outlist->FindObject(Form("GammaCut_dopp_%i",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");
    
    outlist->Add(new TH1D(Form("BeEx%i",id),"Be-12 Excitation Energy",1400,-10,60));
    temp1 = (TH1D*)outlist->FindObject(Form("BeEx%i",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");
    
    outlist->Add(new TH1D(Form("Be10Ex%i",id),"Be-10 Excitation Energy",1400,-10,60));
    temp1 = (TH1D*)outlist->FindObject(Form("Be10Ex%i",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");
    
    outlist->Add(new TH2I(Form("Be10Ex_Vs_Theta_%i",id),Form("Corrected Excitation Energy vs Theta in Detector %i",id),1400,-10,60,70,0,70));
    temp2INT = (TH2I*)outlist->FindObject(Form("Be10Ex_Vs_Theta_%i",id));
    temp2INT->GetXaxis()->SetTitle("Excitation Energy in MeV");
    temp2INT->GetYaxis()->SetTitle("Theta in Degrees");
    
    outlist->Add(new TH1D(Form("Be11Ex%i",id),"Be-11 Excitation Energy",1400,-10,60));
    temp1 = (TH1D*)outlist->FindObject(Form("Be11Ex%i",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");
    
    outlist->Add(new TH1D(Form("Be9Ex%i",id),"Be-9 Excitation Energy",1400,-10,60));
    temp1 = (TH1D*)outlist->FindObject(Form("Be9Ex%i",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");
    
    outlist->Add(new TH1D(Form("Be11Ex%i_corr",id),"Be-11 Excitation Energy, with energy corrections",1400,-10,60));
    temp1 = (TH1D*)outlist->FindObject(Form("Be11Ex%i_corr",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");
    
    outlist->Add(new TH1D(Form("Be9Ex%i_corr",id),"Be-9 Excitation Energy, with energy corrections",1400,-10,60));
    temp1 = (TH1D*)outlist->FindObject(Form("Be9Ex%i_corr",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");
    
    outlist->Add(new TH2I(Form("Be11_%i_EvTCOM",id),Form("Be-11 COM Theta vs Energy in Detector %i",id),1400,-10,60,70,0,70));
    
    outlist->Add(new TH2I(Form("Be11_TvTCOM_%i",id),Form("Theta vs ThetaCOM for 11Be, detector %i",id),1440,0,360,400,0,100));
    temp2INT = (TH2I*)outlist->FindObject(Form("Be11_TvTCOM_%i",id));
    temp2INT->GetXaxis()->SetTitle("Theta COM");
    temp2INT->GetYaxis()->SetTitle("Theta Lab");
    
    outlist->Add(new TH1D(Form("Be10Ex%i_corr",id),"Be-10 Excitation Energy, with straggling correction",1400,-10,60));
    temp1 = (TH1D*)outlist->FindObject(Form("Be10Ex%i_corr",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");
    
    outlist->Add(new TH2D(Form("Be10Ex%i_corr_v_tigressMult",id),"Be-10 Excitation Energy, with straggling correction, vs Tigress Multiplicity",1400,-10,60,10,0,10));
    temp2 = (TH2D*)outlist->FindObject(Form("Be10Ex%i_corr_v_tigressMult",id));
    temp2->GetXaxis()->SetTitle("Energy in MeV");
    temp2->GetYaxis()->SetTitle("Tigress Multiplicity");
    
    outlist->Add(new TH2D(Form("Be10Ex%i_corr_v_tigressMult_opp",id),"Be-10 Excitation Energy, with straggling correction, vs Tigress Multiplicity",1400,-10,60,10,0,10));
    temp2 = (TH2D*)outlist->FindObject(Form("Be10Ex%i_corr_v_tigressMult_opp",id));
    temp2->GetXaxis()->SetTitle("Energy in MeV");
    temp2->GetYaxis()->SetTitle("Tigress Multiplicity");
    
    outlist->Add(new TH2D(Form("Be10Ex%i_corr_v_csmMult",id),"Be-10 Excitation Energy, with straggling correction, vs CSM Multiplicity",1400,-10,60,5,0,5));
    temp2 = (TH2D*)outlist->FindObject(Form("Be10Ex%i_corr_v_csmMult",id));
    temp2->GetXaxis()->SetTitle("Energy in MeV");
    temp2->GetYaxis()->SetTitle("CSM Multiplicity");
    
    outlist->Add(new TH1D(Form("Be10Ex%i_corr_supp",id),"Be-10 Excitation Energy, with straggling correction kinematically suppressed",1400,-10,60));
    temp1 = (TH1D*)outlist->FindObject(Form("Be10Ex%i_corr_supp",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");
    
    outlist->Add(new TH1D(Form("BeEx%i_corr",id),"Be-12 Excitation Energy, with straggling correction",1400,-10,60));
    temp1 = (TH1D*)outlist->FindObject(Form("BeEx%i_corr",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");
    
    outlist->Add(new TH2D(Form("pid_%i",id),Form("Particle ID, detector %i",id),700,0,70,700,0,70));//
    temp2 = (TH2D*)outlist->FindObject(Form("pid_%i",id));
    temp2->GetXaxis()->SetTitle("E Energy deposited in MeV");
    temp2->GetYaxis()->SetTitle("dE Energy deposited in MeV");
    
    outlist->Add(new TH2D(Form("pid_%i_summed_thickness",id),Form("Particle ID for Detector %i",id),3200,0,160,1600,0,2000));//
    temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_summed_thickness",id));
    temp2->GetYaxis()->SetTitleOffset(1.5);
    temp2->SetContour(666);
    temp2->SetOption("colz");
    temp2->GetXaxis()->SetTitle("Total Energy deposited in MeV");
    temp2->GetYaxis()->SetTitle("dE/dX in keV/um");
    
    outlist->Add(new TH2D(Form("pid_%i_summed_thickness_isolate",id),Form("Particle ID for Detector %i",id),3200,0,160,1600,0,2000));//
    temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_summed_thickness_isolate",id));
    temp2->GetYaxis()->SetTitleOffset(1.5);
    temp2->SetContour(666);
    temp2->SetOption("colz");
    temp2->GetXaxis()->SetTitle("Total Energy deposited in MeV");
    temp2->GetYaxis()->SetTitle("dE/dX in keV/um");
    
    outlist->Add(new TH2D(Form("EvTheta_%iTotal",id),Form("Energy vs Theta for Detector %i",id),480,0,120,1400,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%iTotal",id));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    
    outlist->Add(new TH2D(Form("EvTheta_%i_BE10",id),Form("EvTheta %i of the identified 10Be",id),100,0,100,350,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE10",id));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    
    outlist->Add(new TH2D(Form("EvTheta_%i_Be11",id),Form("EvTheta %i of the identified 11Be",id),100,0,100,350,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_Be11",id));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    
    outlist->Add(new TH2D(Form("EvTheta_%i_Be9",id),Form("EvTheta %i of the identified 9Be",id),100,0,100,350,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_Be9",id));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    
    outlist->Add(new TH2D(Form("EvTheta_%i_BE10_corrected",id),Form("EvTheta %i of the identified 10Be, with straggling correction",id),100,0,100,350,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE10",id));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    
    outlist->Add(new TH2D(Form("Be10_TvTCOM_%i",id),Form("Theta vs ThetaCOM for 10Be, detector %i",id),1440,0,360,400,0,100));
    temp2 = (TH2D*)outlist->FindObject(Form("Be10_TvTCOM_%i",id));
    temp2->GetXaxis()->SetTitle("Theta COM");
    temp2->GetYaxis()->SetTitle("Theta Lab");
    
    outlist->Add(new TH1D(Form("Be10Ex%i_corr_opp",id),"Oppostie Be-10 Excitation Energy, with straggling correction",1400,-10,60));
    temp1 = (TH1D*)outlist->FindObject(Form("Be10Ex%i_corr_opp",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");
    
    outlist->Add(new TH2D(Form("EvTheta_%i_BE10_opp_math",id),Form("EvTheta %i of the mathematical opposite 10Be",id),100,0,100,350,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE10_opp_math",id));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    
    outlist->Add(new TH2D(Form("EvTheta_%i_BE10_opp",id),Form("EvTheta %i of the opposite 10Be",id),100,0,100,350,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE10_opp",id));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    
    for(int iso = 9; iso <=11;iso +=2)
    {
      outlist->Add(new TH2D(Form("EvTheta_%i_%iBe_corr",id,iso),Form("EvTheta %i of the below PID %iBe",id,iso),100,0,100,350,0,70));
      temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_%iBe_corr",id,iso));
      temp2->GetXaxis()->SetTitle("Theta in Degrees");
      temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
      
      outlist->Add(new TH2D(Form("EvTheta_%i_%iBe_corr_high",id,iso),Form("EvTheta %i of the below PID 11Be and 9Be, highest energy only",id),100,0,100,350,0,70));
      temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_%iBe_corr_high",id,iso));
      temp2->GetXaxis()->SetTitle("Theta in Degrees");
      temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    }
    outlist->Add(new TH1D(Form("Be10_Gamma_%i_dopp_opp",id),"Gamma spectrum for opposite particle",30000,0,30));
    temp1 = (TH1D*)outlist->FindObject(Form("Be10_Gamma_%i_dopp_opp",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");
    
    outlist->Add(new TH1D(Form("Be10_Gamma_%i_dopp_opp_math",id),"Gamma spectrum for theoretical opposite particle",30000,0,30));
    temp1 = (TH1D*)outlist->FindObject(Form("Be10_Gamma_%i_dopp_opp_math",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");
    
    outlist->Add(new TH1D(Form("Be10_Gamma_%i_dopp_opp_eff",id),"Gamma spectrum for opposite particle, Efficiency Corrected",30000,0,30));
    temp1 = (TH1D*)outlist->FindObject(Form("Be10_Gamma_%i_dopp_opp_eff",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");
    
    outlist->Add(new TH1D(Form("Be10_Gamma_%i_dopp_opp_math_eff",id),"Gamma spectrum for theoretical opposite particle, Efficiency Corrected",30000,0,30));
    temp1 = (TH1D*)outlist->FindObject(Form("Be10_Gamma_%i_dopp_opp_math_eff",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");
  }
  
  outlist->Add(new TH1D("Multiplicity","Multiplicity",10,0,10));//
  
  outlist->Add(new TH1D("GammaSum","Gamma Spectrum",30000,0,30));
  temp1 = (TH1D*)outlist->FindObject("GammaSum");
  temp1->GetXaxis()->SetTitle("Energy of Gamma in MeV");
  temp1->GetYaxis()->SetTitle("Counts per .1 MeV bin");
  
  outlist->Add(new TH1D("GammaSum_supp","Gamma Spectrum BGO Suppressed",30000,0,30));
  temp1 = (TH1D*)outlist->FindObject("GammaSum_supp");
  temp1->GetXaxis()->SetTitle("Energy of Gamma in MeV");
  temp1->GetYaxis()->SetTitle("Counts per .1 MeV bin");
  
  outlist->Add(new TH1D("GammaSum_eff","Efficiency Corrected Gamma Spectrum",30000,0,30));
  temp1 = (TH1D*)outlist->FindObject("GammaSum_eff");
  temp1->GetXaxis()->SetTitle("Energy of Gamma in MeV");
  temp1->GetYaxis()->SetTitle("Counts per .1 MeV bin");
  
  outlist->Add(new TH2I("Dual10Be_nocut","EvTheta of 2 particles",100,0,100,350,0,70));
  temp2INT = (TH2I*)outlist->FindObject("Dual10Be_nocut");
  temp2INT->GetXaxis()->SetTitle("Theta in Degrees");
  temp2INT->GetYaxis()->SetTitle("Total Energy deposited in MeV");
  
  outlist->Add(new TH2I("Dual10Be_phicut","EvTheta of 2 particles, cut on Phi",100,0,100,350,0,70));
  temp2INT = (TH2I*)outlist->FindObject("Dual10Be_phicut");
  temp2INT->GetXaxis()->SetTitle("Theta in Degrees");
  temp2INT->GetYaxis()->SetTitle("Total Energy deposited in MeV");
  
  outlist->Add(new TH2I("Dual10Be_encut","EvTheta of 2 particles, cut on Energy and Phi",100,0,100,350,0,70));
  temp2INT = (TH2I*)outlist->FindObject("Dual10Be_encut");
  temp2INT->GetXaxis()->SetTitle("Theta in Degrees");
  temp2INT->GetYaxis()->SetTitle("Total Energy deposited in MeV");
  
  outlist->Add(new TH2I("Dual10Be_thetacut","EvTheta of 2 particles, cut on Theta and Phi",100,0,100,350,0,70));
  temp2INT = (TH2I*)outlist->FindObject("Dual10Be_thetacut");
  temp2INT->GetXaxis()->SetTitle("Theta in Degrees");
  temp2INT->GetYaxis()->SetTitle("Total Energy deposited in MeV");
  
  outlist->Add(new TH2I("Dual10Be_allcut","EvTheta of 2 particles, cut on Theta, Phi, and Energy",100,0,100,350,0,70));
  temp2INT = (TH2I*)outlist->FindObject("Dual10Be_allcut");
  temp2INT->GetXaxis()->SetTitle("Theta in Degrees");
  temp2INT->GetYaxis()->SetTitle("Total Energy deposited in MeV");
  
  
  outlist->Add(new TH2I("Be10Ex_Vs_Theta_%i_Dual","Corrected Excitation Energy vs Theta, from dual kinematics cuts",1400,-10,60,70,0,70));
  temp2INT = (TH2I*)outlist->FindObject("Be10Ex_Vs_Theta_%i_Dual");
  temp2INT->GetXaxis()->SetTitle("Excitation Energy in MeV");
  temp2INT->GetYaxis()->SetTitle("Theta in Degrees");
  
  outlist->Add(new TH1I("DualBe10_ex_allcut","Be-10 Excitation Energy non-PID cut on Theta, Phi and Energy",1400,-10,60));
  temp1INT = (TH1I*)outlist->FindObject("DualBe10_ex_allcut");
  temp1INT->GetXaxis()->SetTitle("Energy in MeV");
  temp1INT->GetYaxis()->SetTitle("Counts");
  
  outlist->Add(new TH1I("DualBe11_ex_allcut","Be-11 Excitation Energy non-PID cut on Theta, Phi and Energy, and energy corrected",1400,-10,60));
  temp1INT = (TH1I*)outlist->FindObject("DualBe11_ex_allcut");
  temp1INT->GetXaxis()->SetTitle("Energy in MeV");
  temp1INT->GetYaxis()->SetTitle("Counts");
  
  outlist->Add(new TH1I("DualBe10_ex_allcut_highonly","Be-10 Excitation Energy non-PID cut on Theta, Phi and Energy, Higher energy hit only",1400,-10,60));
  temp1INT = (TH1I*)outlist->FindObject("DualBe10_ex_allcut_highonly");
  temp1INT->GetXaxis()->SetTitle("Energy in MeV");
  temp1INT->GetYaxis()->SetTitle("Counts");
  
  outlist->Add(new TH1I("DualBe10_ex_allcut_lowonly","Be-10 Excitation Energy non-PID cut on Theta, Phi and Energy, Lower energy hit only",1400,-10,60));
  temp1INT = (TH1I*)outlist->FindObject("DualBe10_ex_allcut_lowonly");
  temp1INT->GetXaxis()->SetTitle("Energy in MeV");
  temp1INT->GetYaxis()->SetTitle("Counts");
  
  outlist->Add(new TH2I("Dual10Be_allcut_corrected","EvTheta of 2 particles, cut on Theta, Phi, and Energy, energy corrected",100,0,100,350,0,70));
  temp2INT = (TH2I*)outlist->FindObject("Dual10Be_allcut_corrected");
  temp2INT->GetXaxis()->SetTitle("Theta in Degrees");
  temp2INT->GetYaxis()->SetTitle("Total Energy deposited in MeV");
  
  outlist->Add(new TH2I("Dual10Be_allcut_corrected_highonly","EvTheta of 2 particles, cut on Theta, Phi, and Energy, energy corrected higher energy hit only",100,0,100,350,0,70));
  temp2INT = (TH2I*)outlist->FindObject("Dual10Be_allcut_corrected_highonly");
  temp2INT->GetXaxis()->SetTitle("Theta in Degrees");
  temp2INT->GetYaxis()->SetTitle("Total Energy deposited in MeV");
  
  outlist->Add(new TH2I("Dual10Be_allcut_corrected_lowonly","EvTheta of 2 particles, cut on Theta, Phi, and Energy, energy corrected lower energy hit only",100,0,100,350,0,70));
  temp2INT = (TH2I*)outlist->FindObject("Dual10Be_allcut_corrected_lowonly");
  temp2INT->GetXaxis()->SetTitle("Theta in Degrees");
  temp2INT->GetYaxis()->SetTitle("Total Energy deposited in MeV");
  
  outlist->Add(new TH1I("DualBe10_allcut_gammas_dopp_low","Gamma Spectrum with 2 non-PID 10Be, Doppler Corrected on Low E 10Be",30000,0,30));
  outlist->Add(new TH1I("DualBe10_allcut_gammas_dopp_high","Gamma Spectrum with 2 non-PID 10Be, Doppler Corrected on High E 10Be",30000,0,30));
  outlist->Add(new TH1I("DualBe10_allcut_gammas","Gamma Spectrum with 2 non-PID 10Be",30000,0,30));
  outlist->Add(new TH1I("DualBe10_allcut_gammas_supp","Gamma Spectrum with 2 non-PID 10Be, BGO suppressed",30000,0,30));
  
  outlist->Add(new TH1I("DualBe10_allcut_gammas_dopp_low_eff","Gamma Spectrum with 2 non-PID 10Be, Doppler Corrected on Low E 10Be, with Efficiency",30000,0,30));
  outlist->Add(new TH1I("DualBe10_allcut_gammas_dopp_high_eff","Gamma Spectrum with 2 non-PID 10Be, Doppler Corrected on High E 10Be, with Efficiency",30000,0,30));
  outlist->Add(new TH1I("DualBe10_allcut_gammas_eff","Gamma Spectrum with 2 non-PID 10Be, with Efficiency",30000,0,30));
  
  for(int det = 1; det<3;det++)
  {
    for(int state = 0; state<13; state +=3)
    {
      outlist->Add(new TH1D(Form("RingCounts_s%i_d%i_dual",state,det),Form("Counts per ring for state %i, detector %i, dual detection",state,det),100,0,100));
      temp1 = (TH1D*)outlist->FindObject(Form("RingCounts_s%i_d%i_dual",state,det));
      temp1->GetXaxis()->SetTitle("Ring Number");
      temp1->GetYaxis()->SetTitle("Counts");
      
      outlist->Add(new TH1D(Form("RingCounts_s%i_d%i_pid",state,det),Form("Counts per ring for state %i, detector %i, pid detection",state,det),100,0,100));
      temp1 = (TH1D*)outlist->FindObject(Form("RingCounts_s%i_d%i_pid",state,det));
      temp1->GetXaxis()->SetTitle("Ring Number");
      temp1->GetYaxis()->SetTitle("Counts");
      
      outlist->Add(new TH1D(Form("RingCounts_s%i_d%i_11Be",state,det),Form("11Be counts per ring for state %i, detector %i, pid detection",state,det),100,0,100));
      temp1 = (TH1D*)outlist->FindObject(Form("RingCounts_s%i_d%i_11Be",state,det));
      temp1->GetXaxis()->SetTitle("Ring Number");
      temp1->GetYaxis()->SetTitle("Counts");
      
      outlist->Add(new TH1D(Form("RingCounts_s%i_d%i_9Be",state,det),Form("9Be counts per ring for state %i, detector %i, pid detection",state,det),100,0,100));
      temp1 = (TH1D*)outlist->FindObject(Form("RingCounts_s%i_d%i_9Be",state,det));
      temp1->GetXaxis()->SetTitle("Ring Number");
      temp1->GetYaxis()->SetTitle("Counts");
      
      outlist->Add(new TH1D(Form("RingCounts_s%i_d%i_11Be_corr",state,det),Form("11Be counts per ring for state %i, detector %i, corr detection",state,det),100,0,100));
      temp1 = (TH1D*)outlist->FindObject(Form("RingCounts_s%i_d%i_11Be_corr",state,det));
      temp1->GetXaxis()->SetTitle("Ring Number");
      temp1->GetYaxis()->SetTitle("Counts");
      
      outlist->Add(new TH1D(Form("RingCounts_s%i_d%i_9Be_corr",state,det),Form("9Be counts per ring for state %i, detector %i, corr detection",state,det),100,0,100));
      temp1 = (TH1D*)outlist->FindObject(Form("RingCounts_s%i_d%i_9Be_corr",state,det));
      temp1->GetXaxis()->SetTitle("Ring Number");
      temp1->GetYaxis()->SetTitle("Counts");
    }
  }
  int gammas[5] = {3368,2589,2894,2867,5958};
  for(int gammaiter = 0; gammaiter<5; gammaiter++)
  {
    for(int det = 1; det<3;det++)
    {
      outlist->Add(new TH1D(Form("RingCounts_d%i_10Be_opp_%i",det,gammas[gammaiter]),
                            Form("10Be counts per ring for detector %i, pid detection cut on opposite doppler corrected %i gamma ray",det,gammas[gammaiter]),
                            100,0,100));
      temp1 = (TH1D*)outlist->FindObject(Form("RingCounts_d%i_10Be_opp_%i",det,gammas[gammaiter]));
      temp1->GetXaxis()->SetTitle("Ring Number");
      temp1->GetYaxis()->SetTitle("Counts");        
      
      outlist->Add(new TH1D(Form("Be10Ex%i_gcut_%i",det,gammas[gammaiter]),Form("Be-10 Excitation Energy, cut on %i gamma ray",gammas[gammaiter]),1400,-10,60));
      temp1 = (TH1D*)outlist->FindObject(Form("Be10Ex%i_gcut_%i",det,gammas[gammaiter]));
      temp1->GetXaxis()->SetTitle("Energy in MeV");
      temp1->GetYaxis()->SetTitle("Counts");
      
      outlist->Add(new TH1D(Form("Be10Ex%i_gcut_%i_opp",det,gammas[gammaiter]),Form("Be-10 Excitation Energy, cut on %i gamma ray",gammas[gammaiter]),1400,-10,60));
      temp1 = (TH1D*)outlist->FindObject(Form("Be10Ex%i_gcut_%i_opp",det,gammas[gammaiter]));
      temp1->GetXaxis()->SetTitle("Energy in MeV");
      temp1->GetYaxis()->SetTitle("Counts");
    }
    
    outlist->Add(new TH1I(Form("DualBe10Ex_gcut_%i",gammas[gammaiter]),Form("Be-10 Excitation Energy dual detection cut on %i #gamma ray",gammas[gammaiter]),1400,-10,60));
    temp1INT = (TH1I*)outlist->FindObject(Form("DualBe10Ex_gcut_%i",gammas[gammaiter]));
    temp1INT->GetXaxis()->SetTitle("Energy in MeV");
    temp1INT->GetYaxis()->SetTitle("Counts");
    
    outlist->Add(new TH1I(Form("DualBe10Ex_gcut_%i_high",gammas[gammaiter]),Form("Be-10 Excitation Energy dual detection (high energy particle) cut on %i #gamma ray",gammas[gammaiter]),1400,-10,60));
    temp1INT = (TH1I*)outlist->FindObject(Form("DualBe10Ex_gcut_%i_high",gammas[gammaiter]));
    temp1INT->GetXaxis()->SetTitle("Energy in MeV");
    temp1INT->GetYaxis()->SetTitle("Counts");
    
    outlist->Add(new TH1I(Form("DualBe10Ex_gcut_%i_low",gammas[gammaiter]),Form("Be-10 Excitation Energy dual detection (low energy particle) cut on %i #gamma ray",gammas[gammaiter]),1400,-10,60));
    temp1INT = (TH1I*)outlist->FindObject(Form("DualBe10Ex_gcut_%i_low",gammas[gammaiter]));
    temp1INT->GetXaxis()->SetTitle("Energy in MeV");
    temp1INT->GetYaxis()->SetTitle("Counts");
  }
  
  int gammas11[1] = {320};
  for(int gammaiter = 0; gammaiter<1; gammaiter++)
  {
    for(int det = 1; det<3;det++)
    {
      outlist->Add(new TH1D(Form("Be11Ex%i_gcut_%i",det,gammas11[gammaiter]),Form("Be-11 Excitation Energy, cut on %i gamma ray",gammas11[gammaiter]),1400,-10,60));
      temp1 = (TH1D*)outlist->FindObject(Form("Be11Ex%i_gcut_%i",det,gammas11[gammaiter]));
      temp1->GetXaxis()->SetTitle("Energy in MeV");
      temp1->GetYaxis()->SetTitle("Counts");
      
      outlist->Add(new TH1D(Form("RingCounts_d%i_11Be_gtag_%i",det,gammas11[gammaiter]),Form("11Be counts per ring for detector %i, pid detection, tagged on %i gamma",det,gammas11[gammaiter]),100,0,100));
      temp1 = (TH1D*)outlist->FindObject(Form("RingCounts_d%i_11Be_gtag_%i",det,gammas11[gammaiter]));
      temp1->GetXaxis()->SetTitle("Ring Number");
      temp1->GetYaxis()->SetTitle("Counts");
    }
  }
}

void SetupHistosDualAndPID(TList *outlist)
{
  TH2D *temp2 = 0;
  TH1D *temp1 = 0;
  
  for(int id = 1; id<=4;id++)
  {
    outlist->Add(new TH2D(Form("EvTheta_%i_BE10_DualAndPID",id),Form("EvTheta in detector %i of the particle passing dual id cuts, that is associated with a PID 10Be",id),100,0,100,350,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE10_DualAndPID",id));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    
    outlist->Add(new TH1D(Form("Be10Ex%i_corr_DualAndPID",id),"Excitation Energy of the particle passing dual id cuts, that is associated with a PID 10Be, with straggling correction",1400,-10,60));
    temp1 = (TH1D*)outlist->FindObject(Form("Be10Ex%i_corr_DualAndPID",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");
    
    outlist->Add(new TH2D(Form("EvTheta_%i_BE10_not_DualAndPID",id),Form("EvTheta in detector %i of the particle NOT passing dual id cuts, that is associated with a PID 10Be",id),100,0,100,350,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE10_not_DualAndPID",id));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    
    outlist->Add(new TH1D(Form("Be10Ex%i_corr_not_DualAndPID",id),"Excitation Energy of the particle NOT passing dual id cuts, that is associated with a PID 10Be, with straggling correction",1400,-10,60));
    temp1 = (TH1D*)outlist->FindObject(Form("Be10Ex%i_corr_not_DualAndPID",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");
  }
}