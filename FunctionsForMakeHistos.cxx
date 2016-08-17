#include "FunctionsForMakeHistos.hh"

void SetupHistos(TList *outlist)
{
  if(DEBUG) cout<<"Setting up Histos"<<endl;
  TH1D *temp1 = 0;
  TH2D *temp2 = 0;
  TH2I *temp2INT = 0;
  TH3D *temp3 = 0;
  TH3I *temp3INT = 0;
  
  for(int id = 1; id<=2;id++)
  {
    
    /*for(int strip = 0; strip<16; strip++)
    {
      outlist->Add(new TH2D(Form("stripPID_det%i_strip%02i",id,strip),Form("Detector %i PID strip %i",id,strip),3200,0,40,800,0,800));
      temp2 = (TH2D*)outlist->FindObject(Form("stripPID_det%i_strip%02i",id,strip));
      temp2->GetXaxis()->SetTitle("Total Energy deposited in MeV");
      temp2->GetYaxis()->SetTitle("dE/dX in MeV/um");
      
      outlist->Add(new TH2D(Form("EvTheta_%i_BE_strip%02i",id,strip),Form("EvTheta %i, cut on Be Strip",id,strip),100,0,100,350,0,70));
      temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE_strip%02i",id,strip));
      temp2->GetXaxis()->SetTitle("Theta in Degrees");
      temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    }*/
    
    /*for(int theta=10;theta<55;theta++)
    {
      outlist->Add(new TH1D(Form("BeEx%i_theta%02i",id,theta),Form("Be-12 Excitation Energy Theta %i",id,theta),500,-10,25));
      temp1 = (TH1D*)outlist->FindObject(Form("BeEx%i_theta%02i",id,theta));
      temp1->GetXaxis()->SetTitle("Energy in MeV");
      temp1->GetYaxis()->SetTitle("Counts");
      
      outlist->Add(new TH1D(Form("BeEx%i_theta%02i_corr",id,theta),Form("Be-12 Excitation Energy Theta %i energy corrected",id,theta),500,-10,25));
      temp1 = (TH1D*)outlist->FindObject(Form("BeEx%i_theta%02i_corr",id,theta));
      temp1->GetXaxis()->SetTitle("Energy in MeV");
      temp1->GetYaxis()->SetTitle("Counts");
    }*/

    outlist->Add(new TH1I(Form("correction_straggling_%i",id),Form("Energy added by a straggling correction in detector %i",id),500,0,10));
    outlist->Add(new TH1I(Form("correction_deadlayer_%i",id),Form("Energy added by a dead layer correction in detector %i",id),500,0,10));
    
    outlist->Add(new TH1D(Form("ETot_%i_2alpha",id),Form("Total energy with two detected alphas in detector %i",id),100,0,100));

    outlist->Add(new TH1I(Form("NBe_%i",id),Form("Number of Be of each isotope",id),6,8,14));
    outlist->Add(new TH1I(Form("NHe_%i",id),Form("Number of He of each isotope",id),4,3,7));

    
    outlist->Add(new TH1I(Form("Be10Mult_%i",id),Form("Multiplicity of an identified 10Be in detector %i",id),20,0,20));
    outlist->Add(new TH1I(Form("Be12Mult_%i",id),Form("Multiplicity of an identified 12Be in detector %i",id),20,0,20));
    
//     outlist->Add(new TH1D(Form("GammaCut_%i",id),Form("Gamma spectrum cut on Ex Spectrum",id),3000,0,30));
//     temp1 = (TH1D*)outlist->FindObject(Form("GammaCut_%i",id));
//     temp1->GetXaxis()->SetTitle("Energy in MeV");
//     temp1->GetYaxis()->SetTitle("Counts");
    
    outlist->Add(new TH1D(Form("Be12_Gamma_%i",id),Form("Gamma spectrum",id),6000,0,30));
    temp1 = (TH1D*)outlist->FindObject(Form("Be12_Gamma_%i",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");

    outlist->Add(new TH1D(Form("Be12_Gamma_%i_dopp",id),Form("Gamma spectrum",id),6000,0,30));
    temp1 = (TH1D*)outlist->FindObject(Form("Be12_Gamma_%i_dopp",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");

    outlist->Add(new TH1D(Form("Be12_Gamma_%i_dopp_gt8",id),Form("Gamma spectrum, with excitation > 8 MeV",id),1500,0,30));
    temp1 = (TH1D*)outlist->FindObject(Form("Be12_Gamma_%i_dopp_gt8",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");

    outlist->Add(new TH1D(Form("Be12_Gamma_%i_dopp_lt8",id),Form("Gamma spectrum, with excitation < 8 MeV",id),1500,0,30));
    temp1 = (TH1D*)outlist->FindObject(Form("Be12_Gamma_%i_dopp_lt8",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");
    
    outlist->Add(new TH1D(Form("Be10_Gamma_%i",id),Form("Gamma spectrum",id),6000,0,30));
    temp1 = (TH1D*)outlist->FindObject(Form("Be10_Gamma_%i",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");

    outlist->Add(new TH1D(Form("Be10_Gamma_%i_dopp",id),Form("Gamma spectrum",id),6000,0,30));
    temp1 = (TH1D*)outlist->FindObject(Form("Be10_Gamma_%i",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");
    
    outlist->Add(new TH2D(Form("EvTheta_12Be_%i",id),Form("EvTheta %i",id),100,0,100,350,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_12Be_%i",id));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");

    outlist->Add(new TH2D(Form("EvTheta_12Be_%i_corrected",id),Form("EvTheta %i, 12Be, with straggling correction",id),100,0,100,350,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_12Be_%i_corrected",id));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    
    outlist->Add(new TH2D(Form("EvTheta_12Be_%i_low",id),Form("EvTheta %i Low Energy PID only",id),100,0,100,350,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_12Be_%i_low",id));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");

    outlist->Add(new TH2D(Form("EvTheta_12Be_%i_high",id),Form("EvTheta %i High Energy PID only",id),100,0,100,350,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_12Be_%i_high",id));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");

    outlist->Add(new TH2D(Form("EvTheta_12Be_%i_mult1",id),Form("EvTheta %i with multiplicity 1",id),100,0,100,350,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_12Be_%i_mult1",id));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    
    outlist->Add(new TH1D(Form("GammaCut_dopp_%i",id),Form("Gamma spectrum cut on Ex Spectrum, doppler corrected",id),3000,0,30));
    temp1 = (TH1D*)outlist->FindObject(Form("GammaCut_dopp_%i",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");
    
    outlist->Add(new TH1D(Form("BeEx%i",id),Form("Be-12 Excitation Energy",id),450,-10,45));
    temp1 = (TH1D*)outlist->FindObject(Form("BeEx%i",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");

    outlist->Add(new TH2I(Form("BeEx%i_mult",id),Form("Be-12 Excitation Energy vs multiplicity",id),450,-10,45,5,0,4));

    outlist->Add(new TH1D(Form("Be10Ex%i",id),Form("Be-10 Excitation Energy",id),450,-10,45));
    temp1 = (TH1D*)outlist->FindObject(Form("Be10Ex%i",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");

    outlist->Add(new TH2I(Form("Be10Ex%i_mult",id),Form("Be-10 Excitation Energy vs multiplicity",id),450,-10,45,5,0,4));

    outlist->Add(new TH1D(Form("Be10Ex%i_gcut_2589",id),Form("Be-10 Excitation Energy, cut on gamma rays",id),450,-10,45));
    temp1 = (TH1D*)outlist->FindObject(Form("Be10Ex%i_gcut_2589",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");
    
    outlist->Add(new TH1D(Form("Be10Ex%i_gcut_2894",id),Form("Be-10 Excitation Energy, cut on gamma rays",id),450,-10,45));
    temp1 = (TH1D*)outlist->FindObject(Form("Be10Ex%i_gcut_2894",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");
    
    outlist->Add(new TH1D(Form("Be10Ex%i_gcut_3368",id),Form("Be-10 Excitation Energy, cut on gamma rays",id),450,-10,45));
    temp1 = (TH1D*)outlist->FindObject(Form("Be10Ex%i_gcut_3368",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");

    outlist->Add(new TH1D(Form("Be10Ex%i_gcut_5958",id),Form("Be-10 Excitation Energy, cut on gamma rays",id),450,-10,45));
    temp1 = (TH1D*)outlist->FindObject(Form("Be10Ex%i_gcut_5958",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");

    outlist->Add(new TH1D(Form("Be10Ex%i_gcut_2589_opp",id),Form("Be-10 Excitation Energy, cut on gamma rays",id),450,-10,45));
    temp1 = (TH1D*)outlist->FindObject(Form("Be10Ex%i_gcut_2589",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");
    
    outlist->Add(new TH1D(Form("Be10Ex%i_gcut_2894_opp",id),Form("Be-10 Excitation Energy, cut on gamma rays",id),450,-10,45));
    temp1 = (TH1D*)outlist->FindObject(Form("Be10Ex%i_gcut_2894",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");
    
    outlist->Add(new TH1D(Form("Be10Ex%i_gcut_3368_opp",id),Form("Be-10 Excitation Energy, cut on gamma rays",id),450,-10,45));
    temp1 = (TH1D*)outlist->FindObject(Form("Be10Ex%i_gcut_3368",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");
    
    outlist->Add(new TH1D(Form("Be10Ex%i_gcut_5958_opp",id),Form("Be-10 Excitation Energy, cut on gamma rays",id),450,-10,45));
    temp1 = (TH1D*)outlist->FindObject(Form("Be10Ex%i_gcut_5958",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");

    outlist->Add(new TH2I(Form("Be10Ex_Vs_Theta_%i",id),Form("Corrected Excitation Energy vs Theta in Detector %i",id),350,-5,25,28,0,56));
    temp2INT = (TH2I*)outlist->FindObject(Form("Be10Ex_Vs_Theta_%i",id));
    temp2INT->GetXaxis()->SetTitle("Excitation Energy in MeV");
    temp2INT->GetYaxis()->SetTitle("Theta in Degrees");

    outlist->Add(new TH1D(Form("Be11Ex%i",id),Form("Be-11 Excitation Energy",id),450,-10,45));
    temp1 = (TH1D*)outlist->FindObject(Form("Be11Ex%i",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");

    outlist->Add(new TH2I(Form("Be11Ex%i_mult",id),Form("Be-11 Excitation Energy vs multiplicity",id),450,-10,45,4,0,4));

    outlist->Add(new TH1D(Form("Be11Ex%i_corr",id),Form("Be-11 Excitation Energy, with energy corrections",id),450,-10,45));
    temp1 = (TH1D*)outlist->FindObject(Form("Be11Ex%i_corr",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");
    
    outlist->Add(new TH1D(Form("Be10Ex%i_corr",id),Form("Be-10 Excitation Energy, with straggling correction",id),450,-10,45));
    temp1 = (TH1D*)outlist->FindObject(Form("Be10Ex%i_corr",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");

    outlist->Add(new TH1D(Form("Be10Ex%i_corr_supp",id),Form("Be-10 Excitation Energy, with straggling correction kinematically suppressed",id),450,-10,45));
    temp1 = (TH1D*)outlist->FindObject(Form("Be10Ex%i_corr_supp",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");
    
    outlist->Add(new TH1D(Form("BeEx%i_corr",id),Form("Be-12 Excitation Energy, with straggling correction",id),450,-10,45));
    temp1 = (TH1D*)outlist->FindObject(Form("BeEx%i_corr",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");

    outlist->Add(new TH2I(Form("Be12Ex_Vs_Theta_%i",id),Form("Corrected Excitation Energy vs Theta in Detector %i",id),350,-5,25,28,0,56));
    temp2INT = (TH2I*)outlist->FindObject(Form("Be12Ex_Vs_Theta_%i",id));
    temp2INT->GetXaxis()->SetTitle("Excitation Energy in MeV");
    temp2INT->GetYaxis()->SetTitle("Theta in Degrees");
    
    outlist->Add(new TH1D(Form("Be12Ex%i_gcut_2102",id),Form("Be-12 Excitation Energy, cut on gamma rays",id),450,-10,45));
    temp1 = (TH1D*)outlist->FindObject(Form("Be12Ex%i_gcut_2102",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");

    outlist->Add(new TH1D(Form("Be12Ex%i_gcut_2702",id),Form("Be-12 Excitation Energy, cut on gamma rays",id),450,-10,45));
    temp1 = (TH1D*)outlist->FindObject(Form("Be12Ex%i_gcut_2702",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");
    
    outlist->Add(new TH2D(Form("pid_%i",id),Form("Particle ID, detector %i",id),700,0,70,700,0,70));//
    temp2 = (TH2D*)outlist->FindObject(Form("pid_%i",id));
    temp2->GetXaxis()->SetTitle("E Energy deposited in MeV");
    temp2->GetYaxis()->SetTitle("dE Energy deposited in MeV");
    
    outlist->Add(new TH2D(Form("pid_%i_thickness",id),Form("Particle ID, detector %i, with thickness correction",id),3200,0,160,800,0,2000));//
    temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_thickness",id));
    temp2->GetXaxis()->SetTitle("E Energy deposited in MeV");
    temp2->GetYaxis()->SetTitle("dE/dX in MeV/um");
    
//     outlist->Add(new TH2D(Form("pid_%i_fvb",id),Form("Particle ID, detector %i, cut on 1:1",id),700,0,70,700,0,70));//
//     temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_fvb",id));
//     temp2->GetXaxis()->SetTitle("E Energy deposited in MeV");
//     temp2->GetYaxis()->SetTitle("dE Energy deposited in MeV");
    
    outlist->Add(new TH2D(Form("pid_%i_summed",id),Form("Particle ID, detector %i, summed",id),700,0,70,500,0,50));//
    temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_summed",id));
    temp2->GetXaxis()->SetTitle("Total Energy deposited in MeV");
    temp2->GetYaxis()->SetTitle("dE Energy deposited in MeV");
    
    outlist->Add(new TH2D(Form("pid_%i_summed_thickness",id),Form("Particle ID for Detector %i",id),3200,0,160,800,0,2000));//
    temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_summed_thickness",id));
    temp2->GetYaxis()->SetTitleOffset(1.5);
    temp2->SetContour(666);
    temp2->SetOption("colz");
    temp2->GetXaxis()->SetTitle("Total Energy deposited in MeV");
    temp2->GetYaxis()->SetTitle("dE/dX in MeV/um");

    outlist->Add(new TH2D(Form("pid_%i_summed_thickness_gcut",id),Form("Particle ID for Detector %i, cut on 2.1 MeV gamma",id),3200,0,160,800,0,2000));//
    temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_summed_thickness_gcut",id));
    temp2->SetOption("colz");
    temp2->GetXaxis()->SetTitle("Total Energy deposited in MeV");
    temp2->GetYaxis()->SetTitle("dE/dX in MeV/um");

    outlist->Add(new TH2D(Form("pid_%i_summed_thickness_gcut_dopp",id),Form("Particle ID for Detector %i, cut on 2.1 or 2.7 MeV gamma",id),3200,0,160,800,0,2000));//
    temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_summed_thickness_gcut_dopp",id));
    temp2->GetXaxis()->SetTitle("Total Energy deposited in MeV");
    temp2->GetYaxis()->SetTitle("dE/dX in MeV/um");

    outlist->Add(new TH2D(Form("pid_%i_summed_thickness_corr12",id),Form("Particle ID for Detector %i, detected with an identified 12Be",id),3200,0,160,800,0,2000));//
    temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_summed_thickness_corr12",id));
    temp2->SetOption("colz");
    temp2->GetXaxis()->SetTitle("Total Energy deposited in MeV");
    temp2->GetYaxis()->SetTitle("dE/dX in MeV/um");

    outlist->Add(new TH2D(Form("pid_%i_summed_thickness_2hit",id),Form("Particle ID for Detector %i 2 hits in same detector",id),3200,0,160,800,0,2000));//
        temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_summed_thickness_2hit",id));

    temp2->SetOption("colz");
    temp2->GetXaxis()->SetTitle("Total Energy deposited in MeV");
    temp2->GetYaxis()->SetTitle("dE/dX in MeV/um");
    
    outlist->Add(new TH2D(Form("pid_%i_summed_thickness_2hit_samepix",id),Form("Particle ID for Detector %i 2 hits in same detector",id),3200,0,160,800,0,2000));//
        temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_summed_thickness_2hit_samepix",id));

    temp2->SetOption("colz");
    temp2->GetXaxis()->SetTitle("Total Energy deposited in MeV");
    temp2->GetYaxis()->SetTitle("dE/dX in MeV/um");
    
    outlist->Add(new TH2D(Form("EvTheta_%iTotal",id),Form("Energy vs Theta for Detector %i",id),480,0,120,1400,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%iTotal",id));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    
//     outlist->Add(new TH2D(Form("EvTheta_%i_BE8",id),Form("EvTheta %i of the reconstructed 8Be",id),100,0,100,350,0,70));
//     temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE8",id));
//     temp2->GetXaxis()->SetTitle("Theta in Degrees");
//     temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");

    outlist->Add(new TH2D(Form("EvTheta_%i_BE10",id),Form("EvTheta %i of the identified 10Be",id),100,0,100,350,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE10",id));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");

    outlist->Add(new TH2D(Form("EvTheta_%i_Be11",id),Form("EvTheta %i of the identified 11Be",id),100,0,100,350,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_Be11",id));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    
    outlist->Add(new TH2D(Form("EvTheta_%i_BE10_corrected",id),Form("EvTheta %i of the identified 10Be, with straggling correction",id),100,0,100,350,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE10",id));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    
//     outlist->Add(new TH2D(Form("EvTheta_%i_BE",id),Form("EvTheta %i, cut on Be",id),100,0,100,350,0,70));
//     temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE",id));
//     temp2->GetXaxis()->SetTitle("Theta in Degrees");
//     temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");

    outlist->Add(new TH2D(Form("EvTheta_%i_BE_gammaCut",id),Form("EvTheta %i, cut on gammas",id),100,0,100,350,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE_gammaCut",id));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");

    outlist->Add(new TH2D(Form("EvTheta_%i_BE_gammaCut_dopp",id),Form("EvTheta %i, cut on doppler corrected 2.1 and 2.7 gammas",id),100,0,100,350,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE_gammaCut_dopp",id));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    
    outlist->Add(new TH2D(Form("EvTheta_%i_BE_gammaCutNoID",id),Form("EvTheta %i, cut on gammas",id),100,0,100,350,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE_gammaCutNoID",id));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    
    outlist->Add(new TH2D(Form("EvTheta_%i_BE_correlated",id),Form("EvTheta %i, cut on Be",id),100,0,100,350,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE_correlated",id));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    
    outlist->Add(new TH2D(Form("EvTheta_%i_BE10_exCut_gs",id),Form("EvTheta %i, cut on Be10 GS",id),100,0,100,350,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE10_exCut_gs",id));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");

    outlist->Add(new TH2D(Form("EvTheta_%i_BE10_exCut_gs_COM",id),Form("EvThetaCOM %i, cut on Be10 GS",id),1440,0,360,1400,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE10_exCut_gs_COM",id));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    
    outlist->Add(new TH2D(Form("EvTheta_%i_BE_exCut_1e",id),Form("EvTheta %i, cut on Be 2.24",id),100,0,100,350,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE_exCut_1e",id));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    
    outlist->Add(new TH2D(Form("EvTheta_%i_BE_exCut_sc",id),Form("EvTheta %i, cut on Be <5",id),100,0,100,350,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE_exCut_sc",id));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    
    outlist->Add(new TH2D(Form("CheckCalE_%i",id),Form("Front Energy vs Back Energy, Detector %i E",id),600,0,60,600,0,60));
    temp2 = (TH2D*)outlist->FindObject(Form("CheckCalE_%i",id));
    temp2->GetXaxis()->SetTitle("Energy deposited in Vertical (Front)");
    temp2->GetYaxis()->SetTitle("Energy deposited in Horizontal (Back)");
    
    outlist->Add(new TH2D(Form("twohitEVT_%i_side",id),Form("Corresponding hit from TwoHit in detector %i",id),100,0,100,350,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("twohitEVT_%i_side",id));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    
    outlist->Add(new TH2D(Form("twohitEVT_%i_telescope",id),Form("Corresponding hit from TwoHit in detector %i",id),100,0,100,350,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("twohitEVT_%i_telescope",id));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");

    outlist->Add(new TH1D(Form("TotalEnergy_%i_12Be",id),Form("Total energy of an event with 12Be in detector %i",id),100,0,100));

    outlist->Add(new TH2D(Form("Be10_TvTCOM_%i",id),Form("Theta vs ThetaCOM for 10Be, detector %i",id),1440,0,360,400,0,100));
    temp2 = (TH2D*)outlist->FindObject(Form("Be10_TvTCOM_%i",id));
    temp2->GetXaxis()->SetTitle("Theta COM");
    temp2->GetYaxis()->SetTitle("Theta Lab");

    outlist->Add(new TH1D(Form("Be10Ex%i_corr_opp",id),Form("Oppostie Be-10 Excitation Energy, with straggling correction",id),450,-10,45));
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
    
    outlist->Add(new TH1D(Form("Be10_Gamma_%i_dopp_opp",id),Form("Gamma spectrum for opposite particle",id),6000,0,30));
    temp1 = (TH1D*)outlist->FindObject(Form("Be10_Gamma_%i_dopp_opp",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");

    outlist->Add(new TH1D(Form("Be10_Gamma_%i_dopp_opp_math",id),Form("Gamma spectrum for theoretical opposite particle",id),6000,0,30));
    temp1 = (TH1D*)outlist->FindObject(Form("Be10_Gamma_%i_dopp_opp_math",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");
    
    
    for(int mid = 1; mid<=4;mid++)
    {
      outlist->Add(new TH2D(Form("pid_%i_mult%i",id,mid),Form("Particle ID, detector %i with Multiplicity %i",id,mid),700,0,70,700,0,70));
      temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_mult%i",id,mid));
      temp2->GetXaxis()->SetTitle("E Energy deposited in MeV");
      temp2->GetYaxis()->SetTitle("dE Energy deposited in MeV");
      
      outlist->Add(new TH2D(Form("EvTheta_%iTotal_mult%i",id,mid),Form("E vs Theta Detector %i with Multiplicity %i",id,mid),100,0,100,350,0,70));
      temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%iTotal_mult%i",id,mid));
      temp2->GetXaxis()->SetTitle("Theta in Degrees");
      temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");

      outlist->Add(new TH2D(Form("pid_%i_summed_thickness_mult%i",id,mid),Form("Particle ID for Detector %i, with Multiplicity %i",id,mid),3200,0,160,800,0,2000));//
            temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_summed_thickness_mult%i",id,mid));

      temp2->SetOption("colz");
      temp2->GetXaxis()->SetTitle("Total Energy deposited in MeV");
      temp2->GetYaxis()->SetTitle("dE/dX in MeV/um");
    }
  }
  
  for(int det=1;det<=4;det++)
  {

//     outlist->Add(new TH2D(Form("Alphacone_%i",det),Form("Alpha cone in detector %i",det),300,0,60,300,0,60));//
//     temp2 = (TH2D*)outlist->FindObject(Form("Alphacone_%i",det));
//     temp2->SetContour(666);
//     temp2->SetOption("colz");
//     temp2->GetXaxis()->SetTitle("Energy deposited in MeV");
//     temp2->GetYaxis()->SetTitle("Energy deposited in MeV");
    
//     outlist->Add(new TH1D(Form("AlphaEx%i",det),Form("AlphaEx Excitation Energy",det),450,-10,45));
//     temp1 = (TH1D*)outlist->FindObject(Form("AlphaEx%i",det));
//     temp1->GetXaxis()->SetTitle("Energy in MeV");
//     temp1->GetYaxis()->SetTitle("Counts");
    
    outlist->Add(new TH1D(Form("gCutE_%i",det),Form("Total E, cut on 2.125 MeV gamma ray",det),6500,0,65));
    temp1 = (TH1D*)outlist->FindObject(Form("gCutE_%i",det));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");
    
    outlist->Add(new TH2D(Form("twohit_%i",det),Form("2 Hits in detector %i",det),400,0,40,400,0,40));//
    temp2 = (TH2D*)outlist->FindObject(Form("twohit_%i",det));
    temp2->SetContour(666);
    temp2->SetOption("colz");
    temp2->GetXaxis()->SetTitle("Energy deposited in MeV");
    temp2->GetYaxis()->SetTitle("Energy deposited in MeV");

    outlist->Add(new TH2D(Form("twohit_twocut_cone_%i",det),Form("2 Hits in detector %i, cut on both Cone and Angle",det),400,0,40,400,0,40));//
    temp2 = (TH2D*)outlist->FindObject(Form("twohit_twocut_cone_%i",det));
    temp2->SetContour(666);
    temp2->SetOption("colz");
    temp2->GetXaxis()->SetTitle("Energy deposited in MeV");
    temp2->GetYaxis()->SetTitle("Energy deposited in MeV");
    
    outlist->Add(new TH2D(Form("twohit_%i_thetadiff",det),Form("Angle difference vs total energy for 2 hits in detector %i",det),200,-100,100,120,0,60));//
    temp2 = (TH2D*)outlist->FindObject(Form("twohit_%i_thetadiff",det));
    temp2->SetContour(666);
    temp2->SetOption("colz");
    temp2->GetXaxis()->SetTitle("Difference in Angle in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    
    outlist->Add(new TH2D(Form("twohit_%i_thetadiff_onecut",det),Form("2 Hits in detector %i, cut on the alpha cone",det),200,-100,100,120,0,60));//
    temp2 = (TH2D*)outlist->FindObject(Form("twohit_%i_thetadiff_onecut",det));
    temp2->SetContour(666);
    temp2->SetOption("colz");
    temp2->GetXaxis()->SetTitle("Difference in Angle in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");

    outlist->Add(new TH2D(Form("twohit_twocut_%i",det),Form("2 Hits in detector %i, cut on cone and angle",det),100,0,100,350,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("twohit_twocut_%i",det));
    temp2->SetOption("colz");
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");

    outlist->Add(new TH2D(Form("twohit_twocut_%i_Be8",det),Form("2 Hits in detector %i, cut on cone and angle",det),100,0,100,350,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("twohit_twocut_%i_Be8",det));
    temp2->SetOption("colz");
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");

    outlist->Add(new TH2D(Form("twohit_twocut_%i_corr",det),Form("Correpsonding particle of 2 hits in detector %i",det),100,0,100,350,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("twohit_twocut_%i_corr",det));
    temp2->SetOption("colz");
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");

    outlist->Add(new TH2D(Form("twohit_twocut_%i_corr_cut",det),Form("Correpsonding particle of 2 hits in detector %i, cut on PID",det),100,0,100,350,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("twohit_twocut_%i_corr_cut",det));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");

    outlist->Add(new TH1D(Form("twohit_twocut_%i_corr_Etot",det),Form("Total energy under two cuts with corresponding particle, det %i",det),100,0,100));

    outlist->Add(new TH2D(Form("twohit_twocut_%i_corr_PID",det),Form("Correpsonding particle of 2 hits in detector %i, PID",det),3200,0,160,800,0,2000));
    temp2 = (TH2D*)outlist->FindObject(Form("twohit_twocut_%i_corr_PID",det));
    temp2->GetXaxis()->SetTitle("E Energy deposited in MeV");
    temp2->GetYaxis()->SetTitle("dE/dX in MeV/um");
    
//     outlist->Add(new TH2D(Form("EvTheta_%i_HE",det),Form("EvTheta %i, cut on He",det),100,0,100,350,0,70));
//     temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_HE",det));
//     temp2->GetXaxis()->SetTitle("Theta in Degrees");
//     temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    
    outlist->Add(new TH2D(Form("CheckCalD_%i",det),Form("Front Energy vs Back Energy, Detector %i D",det),600,0,60,600,0,60));
    temp2 = (TH2D*)outlist->FindObject(Form("CheckCalD_%i",det));
    temp2->GetXaxis()->SetTitle("Energy deposited in Vertical (Back)");
    temp2->GetYaxis()->SetTitle("Energy deposited in Horizontal (Front)");
    for(char type='D';type<='E';type++) //Wow I can't believe this works.  I am glad they are in alphabetical order
    {
      if(det>2 && type=='E')//This skips 3 and 4 E, which don't exist
	continue;
      
      outlist->Add(new TH2D(Form("EvTheta_%i%c",det,type),Form("EvTheta, Detector %i, Position %c",det,type),100,0,100,350,0,70));//
      temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i%c",det,type));
      temp2->GetXaxis()->SetTitle("Theta in Degrees");
      temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
      
      outlist->Add(new TH2I(Form("CSM_HitPattern_%i%c",det,type),Form("HitPattern, Detector %i, Position %c",det,type),16,0,16,16,0,16));//
      temp2INT = (TH2I*)outlist->FindObject(Form("CSM_HitPattern_%i%c",det,type));
      temp2INT->GetXaxis()->SetTitle("Vertical Strip Number");
      temp2INT->GetYaxis()->SetTitle("Horizontal Strip Number");
    }
  }
  
  
  outlist->Add(new TH3D("positions","positions",200,0,100,120,-30,30,400,-100,100));
  outlist->Add(new TH2D("positions_proj","positions_proj",220,-20,100,400,-100,100));
  temp2 = (TH2D*)outlist->FindObject("positions_proj");
  temp2->SetOption("colz");
  temp2->GetXaxis()->SetTitle("Z");
  temp2->GetYaxis()->SetTitle("X");

  outlist->Add(new TH2I("nBe10_vs_nHe6","Number of 10Be vs Number of 6He",5,0,5,5,0,5));
  temp2INT = (TH2I*)outlist->FindObject("nBe10_vs_nHe6");
  temp2INT->GetXaxis()->SetTitle("nBe10");
  temp2INT->GetYaxis()->SetTitle("nHe6");

  outlist->Add(new TH2I("nBe10_vs_nHe4","Number of 10Be vs Number of 4He",5,0,5,5,0,5));
  temp2INT = (TH2I*)outlist->FindObject("nBe10_vs_nHe4");
  temp2INT->GetXaxis()->SetTitle("nBe10");
  temp2INT->GetYaxis()->SetTitle("nHe4");

  outlist->Add(new TH2I("nHe6_vs_nHe4","Number of 6He vs Number of 4He",5,0,5,5,0,5));
  temp2INT = (TH2I*)outlist->FindObject("nHe6_vs_nHe4");
  temp2INT->GetXaxis()->SetTitle("nHe6");
  temp2INT->GetYaxis()->SetTitle("nHe4");

//   outlist->Add(new TH1D("fred1_1","Total energy of multiplicity 3",100,0,100));
//   outlist->Add(new TH1D("fred2_1","Total energy of multiplicity 3, 2 hits in side",100,0,100));
//   outlist->Add(new TH1D("fred3_1","Total energy of multiplicity 3, 2 hits in side and 12Be identified",100,0,100));
  
  outlist->Add(new TH2D("CSM_HP_Theta_Phi","Angular Coverage Map of Silicon Detectors",120,0,120,360,-180,180));
  temp2 = (TH2D*)outlist->FindObject("CSM_HP_Theta_Phi");
  temp2->GetXaxis()->SetTitle("Theta (Degrees)");
  temp2->GetYaxis()->SetTitle("Phi (Degrees)");
  
  outlist->Add(new TH1D("Multiplicity","Multiplicity",10,0,10));//

  outlist->Add(new TH1D("deltaGamma","Time difference between 12Be and 2.12 Gamma",2000,-10000,10000));

  //outlist->Add(new TH1I("counts","counts",2,1,2));
  
  outlist->Add(new TH1D("GammaSum","Gamma Spectrum",6000,0,30));
  temp1 = (TH1D*)outlist->FindObject("GammaSum");
  temp1->GetXaxis()->SetTitle("Energy of Gamma in MeV");
  temp1->GetYaxis()->SetTitle("Counts per .1 MeV bin");
  
  outlist->Add(new TH2D("EnergyCheck","Energy for each strip",240,0,240,7000,0,70));//
  temp2 = (TH2D*)outlist->FindObject("EnergyCheck");
  temp2->GetXaxis()->SetTitle("Strip Number");
  temp2->GetYaxis()->SetTitle("Energy");
  
  outlist->Add(new TH2D("ChargeCheck","Charge for each strip",240,0,240,3000,0,6000));//
  temp2 = (TH2D*)outlist->FindObject("ChargeCheck");
  temp2->GetXaxis()->SetTitle("Strip Number");
  temp2->GetYaxis()->SetTitle("Charge");

  outlist->Add(new TH2D("EvTheta_12Be","EvTheta",100,0,100,350,0,70));
  temp2 = (TH2D*)outlist->FindObject("EvTheta_12Be");
  temp2->GetXaxis()->SetTitle("Theta in Degrees");
  temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
  //outlist->Add(new TH1I("StatCheck","Checking Cases",20,0,20));

  outlist->Add(new TH1I("MultBlobHigh","Multiplicity of the high energy blob in the dEve",20,0,20));
  outlist->Add(new TH1I("MultBlobLow","Multiplicity of the low energy blob in the dEvE",20,0,20));

  outlist->Add(new TH1I("AlphaMult","Number of identified alphas in an event",5,0,5));

  outlist->Add(new TH3I("AlmostEqual_Diagnostic","Checking what conditions are most strict",4,0,2,4,0,2,4,0,2));
  temp3INT = (TH3I*)outlist->FindObject("AlmostEqual_Diagnostic");
  temp3INT->GetXaxis()->SetTitle("EnergyBool");
  temp3INT->GetYaxis()->SetTitle("ThetaBool");
  temp3INT->GetZaxis()->SetTitle("PhiBool");

  outlist->Add(new TH1D("GammaBe","Gamma spec for broad Be cut",6000,0,30));
  outlist->Add(new TH1D("GammaLi","Gamma spec for broad Li cut",6000,0,30));
  outlist->Add(new TH1D("GammaLi7","Gamma spec for 7Li cut",6000,0,30));
  outlist->Add(new TH1D("GammaAlpha","Gamma spec for Alpha cut",6000,0,30));
  outlist->Add(new TH1D("Be12Gammas","Gamma spec for 12Be cut",6000,0,30));
  outlist->Add(new TH1D("Be12Gammas_prompt","Gamma spec for 12Be cut, prompt only",6000,0,30));
  outlist->Add(new TH1D("Be12Gammas_prompt_doppler","Gamma spec for 12Be cut, prompt only, with doppler correction",6000,0,30));
  outlist->Add(new TH1D("Be12Gammas_medium","Gamma spec for 12Be cut, medium cut",6000,0,30));
  outlist->Add(new TH1D("Be12Gammas_delayed","Gamma spec for 12Be cut, delayed only",6000,0,30));
  outlist->Add(new TH1D("Be12GammasNoID","Gamma spec for 12Be cut",6000,0,30));
  outlist->Add(new TH1D("Be12GammasDopp","Gamma spec for 12Be cut doppler corrected",6000,0,30));
  outlist->Add(new TH1D("Be12GammasDoppNoID","Gamma spec for 12Be cut doppler corrected",6000,0,30));
  outlist->Add(new TH1D("Be12GammasDoppNoID_suppressed","Gamma spec for 12Be cut doppler corrected, with 2.1 MeV stop line suppressed",6000,0,30));

  outlist->Add(new TH1D("Be12Gammas_low","Gamma spec for 12Be cut Low Energy PID only",6000,0,30));
  outlist->Add(new TH1D("Be12Gammas_high","Gamma spec for 12Be cut High Energy PID only",6000,0,30));

  outlist->Add(new TH1D("GammaBe10Brute","Gamma spec for all, assuming 10Be doppler correction",6000,0,30));
  outlist->Add(new TH1D("GammaBe12Brute","Gamma spec for all, assuming 12Be doppler correction",6000,0,30));

  outlist->Add(new TH2D("GEvT","Gamma energy vs time difference from charged particle",4100,-4000,100,5000,0,50));
  temp2 = (TH2D*)outlist->FindObject("GEvT");
  temp2->GetXaxis()->SetTitle("Time difference (Arb. Units)");
  temp2->GetYaxis()->SetTitle("Energy in MeV");

  outlist->Add(new TH2D("GEvT_12Be","Gamma energy vs time difference from 12Be",4100,-4000,100,5000,0,50));
  temp2 = (TH2D*)outlist->FindObject("GEvT_12Be");
  temp2->GetXaxis()->SetTitle("Time difference (Arb. Units)");
  temp2->GetYaxis()->SetTitle("Energy in MeV");
  
  outlist->Add(new TH2D("GammaMatrix","Gamma energy vs gamma energy",1200,0,30,1200,0,30));
  temp2 = (TH2D*)outlist->FindObject("GammaMatrix");
  temp2->GetXaxis()->SetTitle("Energy in MeV");
  temp2->GetYaxis()->SetTitle("Energy in MeV");

  outlist->Add(new TH2D("GammaMatrix_12Be","Gamma energy vs gamma energy, cut on 12Be",1200,0,30,1200,0,30));
  temp2 = (TH2D*)outlist->FindObject("GammaMatrix_12Be");
  temp2->GetXaxis()->SetTitle("Energy in MeV");
  temp2->GetYaxis()->SetTitle("Energy in MeV");
  
  outlist->Add(new TH1I("TimeDiffAll","Difference between charged particle and gamma",20000,-10000,10000));
  outlist->Add(new TH1I("TimeDiff12Be","Difference between 12Be and gamma",20000,-10000,10000));
  
  outlist->Add(new TH1I("GvGTimeDiff","Difference in time between gammas",20000,-10000,10000));

  outlist->Add(new TH2I("CvGmult","Charged particle multiplicity vs Gamma multiplicity",20,0,20,20,0,20));
  outlist->Add(new TH2I("CvGmult_12be","Charged particle multiplicity vs Gamma multiplicity, cut on 12Be",20,0,20,20,0,20));
  
  if(DEBUG)
  {
    cout<<"Histos Set"<<endl;
  }
}

double GetExciteE_Heavy(double energy, double theta, double mass)
{
  //   cout<<"BeamE: "<<BeamE<<endl;
  //   cout<<"BeE: "<<energy<<endl;
  //   cout<<"BeT: "<<theta<<endl;
  energy=energy/1000.;
  const double pi = TMath::Pi();

  double othermass;

  switch(int(mass))
  {
    case 7456: //be8
      othermass = MASS_BE12;
      break;
    case 8394: //be9
      othermass = MASS_BE11;
      break;
    case 9327: //be10
      othermass = MASS_BE10;
      break; 
    case 10266: //be11
      othermass = MASS_BE9;
      break;
    case 11203: //be12
      othermass = MASS_BE8;
      break;
    default:
      cerr<<"Unknown mass in GetExciteE_Heavy: "<<mass<<endl;
      return(-1);
  }
  
  const double M1 = MASS_BE11;
  const double M2 = MASS_BE9;
  const double M3 = othermass;
  const double M4 = mass;
  double mQ = M1+M2-M3-M4;
  
  double V1 = sqrt(2*BEAM_ENERGY/M1);
  double COMV = ( M1 / ( M1 + M2 ) ) * V1;
  double V4 = sqrt(2*energy/M4);
  double kPrimeM4 = COMV / V4;
  
  double COMTotalE = M2 / ( M1 + M2 ) * BEAM_ENERGY;
  double COMEnergyM4 = energy * ( 1 + kPrimeM4*kPrimeM4 - 2*kPrimeM4*cos( theta ) );
  double QVal =  ( COMEnergyM4*( M3 + M4 ) ) / M3 - COMTotalE;
  double ExcitedState = mQ - QVal;
  
  //   cout<<"EX: "<<ExcitedState<<endl<<endl;
  
  return(ExcitedState);
  
}

double GetExciteE_Heavy(TCSMHit* Hit, int Z)
{
  double MASS = 0.;
  
  switch(Z)
  {
    case 10:
      MASS = MASS_BE10;
      break;
    case 12:
      MASS = MASS_BE12;
      break;
    case 8:
      MASS = MASS_BE8;
      break;
    case 11:
      MASS = MASS_BE11;
      break;
    case 0:
      MASS = Hit->GetMassMeV();
      break;
    default:
      cerr<<"unrecognized Z in Corr Particle: "<<Z<<endl;
      MASS = Z;
  }
  
  return(GetExciteE_Heavy(Hit->GetEnergy(),Hit->GetDPosition().Theta(),MASS));
}

double GetExciteE_Heavy_Corrected(TCSMHit* Hit, int Z)
{
  double MASS = 0.;
  TString isotope;
  
  switch(Z)
  {
    case 10:
      MASS = MASS_BE10;
      isotope = "10be";
      break;
    case 12:
      MASS = MASS_BE12;
      isotope = "12be";
      break;
    case 11:
      MASS = MASS_BE11;
      isotope = "11be";
      break;
    case 8:
      MASS = MASS_BE8;
      isotope = "8be";
      break;
    case 0:
      MASS = Hit->GetMassMeV();
      isotope = Hit->GetIsotope();
      break;
    default:
      cerr<<"unrecognized Z in Corr Particle: "<<Z<<endl;
      MASS = Z;
      isotope = "12be";
  }
  
  return(GetExciteE_Heavy(Hit->GetCorrectedEnergyMeV(isotope)*1000.,Hit->GetDPosition().Theta(),MASS));
}

double* CalcBe8fromAlpha(TCSMHit *A1H,TCSMHit *A2H)
{
  const double pi=TMath::Pi();
  
  double *Be8Values = new double[3];
  
  //Make the masses for the 8Be and 4He
  const double mBe8 = 8.0*931.494027 + 4.9416;
  const double mAlpha = 4.0*931.494027 + 2.4249156;
  
  vector<double> PVecAlpha1, PVecAlpha2, pBe;
  //Convert from energy to momentum
  double PAlpha1 = sqrt( 2.0*mAlpha*A1H->GetEnergyMeV() );
  double PAlpha2 = sqrt( 2.0*mAlpha*A2H->GetEnergyMeV() );
  
  //fill the momentum vector for the first alpha
  PVecAlpha1.push_back( PAlpha1*sin( A1H->GetDPosition().Theta() )*cos( A1H->GetDPosition().Phi() ) );
  PVecAlpha1.push_back( PAlpha1*sin( A1H->GetDPosition().Theta() )*sin( A1H->GetDPosition().Phi() ) );
  PVecAlpha1.push_back( PAlpha1*cos( A1H->GetDPosition().Theta() ) );
  
  //fill the momentum vector for the second alpha
  PVecAlpha2.push_back( PAlpha2*sin( A2H->GetDPosition().Theta() )*cos( A1H->GetDPosition().Phi() ) );
  PVecAlpha2.push_back( PAlpha2*sin( A2H->GetDPosition().Theta() )*sin( A1H->GetDPosition().Phi() ) );
  PVecAlpha2.push_back( PAlpha2*cos( A2H->GetDPosition().Theta() ) );
  
  //fill the 8Be vector
  pBe.push_back( ( PVecAlpha1[0]+PVecAlpha2[0] ) );
  pBe.push_back( ( PVecAlpha1[1]+PVecAlpha2[1] ) );
  pBe.push_back( ( PVecAlpha1[2]+PVecAlpha2[2] ) );
  
  //make the 8Be physical parameters, energy, theta, phi
  Be8Values[0] = ( (pBe[0]*pBe[0] + pBe[1]*pBe[1] + pBe[2]*pBe[2]) )/ (2.*mBe8 );  //Energy, from E=p^2/2m
  Be8Values[1] = acos( pBe[2]/ ( sqrt( pBe[0]*pBe[0] + pBe[1]*pBe[1] + pBe[2]*pBe[2] ) ) );  //Theta
  //Be8Values[2] = atan( pBe[1]/pBe[0] ); //Phi
  
  if( pBe[0] > 0.0 && pBe[1] > 0.0 )
    Be8Values[2] = atan( pBe[1] / pBe[0] );
  else if( pBe[0] < 0.0 && pBe[1] > 0.0 )
    Be8Values[2] = atan( pBe[1] / pBe[0] ) + pi;
  else if( pBe[0] < 0.0 && pBe[1] < 0.0 )
    Be8Values[2] = atan( pBe[1] / pBe[0] ) + pi;
  else if( pBe[0] > 0.0 && pBe[1] < 0.0 )
    Be8Values[2] = atan( pBe[1] / pBe[0] ) + 2*pi;
  
  return Be8Values;
}

double GetExciteE_Light(TCSMHit *A1H, TCSMHit *A2H)
{
  
  double *Be8Values = new double[3];
  
  Be8Values = CalcBe8fromAlpha(A1H,A2H);
  
  //11Be(9Be,8Be)12Be*
  const double M1 = MASS_BE11;
  const double M2 = MASS_BE9;
  const double M3 = MASS_BE8;
  const double M4 = MASS_BE12;
  double mQ = M1+M2-M3-M4;
  
  double VelBeam = sqrt(2*BEAM_ENERGY/M1);
  double COMV = ( M1 / ( M1 + M2 ) ) * VelBeam;
  double VelocityM3 = sqrt(2 * (Be8Values[0]) / M3);
  double kPrimeM3 = COMV / VelocityM3;
  
  double COMTotalE = M2 / ( M1 + M2 ) * BEAM_ENERGY;
  double COMEnergyM3 = Be8Values[0] * ( 1 + kPrimeM3*kPrimeM3 - 2*kPrimeM3*cos( Be8Values[1] ) );
  double QVal =  ( COMEnergyM3*( M3 + M4 ) ) / M4 - COMTotalE;
  double ExcitedState = mQ - QVal;
  
  return(ExcitedState);
  
}

double Doppler(double tenergy, double ttheta, double tphi, double cenergy, double ctheta, double cphi, int mass)
{
  double pi = TMath::Pi();

  TVector3 tVec = TVector3(tenergy/1000.,0,0);
  tVec.SetTheta(ttheta);
  tVec.SetPhi(tphi);

  TVector3 cVec = TVector3(cenergy/1000.,0,0);
  cVec.SetTheta(ctheta);
  cVec.SetPhi(cphi);
  
  double M4;
  
  switch(mass)
  {
    case 12:
      M4 = MASS_BE12;
      break;
    case 10:
      M4 = MASS_BE10;
      break;
    default:
      cerr<<"Doppler correcting for unknown mass, reverting to 12Be.  Note Doppler assumes a Be isotope for now."<<endl;
      M4 = MASS_BE12;
  }
  
  double LabEnergyHeavy = cVec.Mag();
  
  double beta =  sqrt( (LabEnergyHeavy*(LabEnergyHeavy + 2.0*M4) )/( ( LabEnergyHeavy + M4 )*( LabEnergyHeavy + M4 ) ) );
  
  double CosTheta = cos( cVec.Angle( tVec) );
  
  double RelativisticCorr = (1.0 / ( sqrt( 1.0 - beta*beta ) ) );
  
  double EGammaDopplerCorr = ( (tVec.Mag() * RelativisticCorr) * ( (1.0 - beta*CosTheta) ) );
  
  return EGammaDopplerCorr;
}

double Doppler(TTigressHit* thit, TCSMHit* chit, int mass)
{
  return Doppler(thit->GetCore()->GetEnergy(), thit->GetPosition().Theta(), thit->GetPosition().Phi(), chit->GetEnergy(), chit->GetPosition().Theta(), chit->GetPosition().Phi(), mass);
}

double Doppler(TTigressHit* thit, double cenergy, double ctheta, double cphi, int mass)
{
  return Doppler(thit->GetCore()->GetEnergy(), thit->GetPosition().Theta(), thit->GetPosition().Phi(), cenergy, ctheta, cphi, mass);
}

TVector3 CalcCOMmomentum(TVector3 pos, double energy, double mass)
{
  bool debug = 0;

  energy=energy/1000.;
  
  double vParticleMag = sqrt((2.*energy)/mass);
  TVector3 vParticle = pos;
  vParticle.SetMag(vParticleMag);

  double vBeam = sqrt((2.*BEAM_ENERGY)/MASS_BE11);

  double vCOMMag = (mass*vBeam)/(mass+MASS_BE11);
  TVector3 vCOM(0.,0.,vCOMMag);

  if(debug)
  {
    cout<<"Calculated Theta COM: "<<((vParticle-vCOM)*mass).Theta()*180./TMath::Pi()<<endl;
  }
  
  return((vParticle-vCOM)*mass);
}

TVector3 CalcCOMmomentum(TCSMHit* Hit, int Z)
{
  double MASS = 0.;
  
  switch(Z)
  {
    case 10:
      MASS = MASS_BE10;
      break;
    case 12:
      MASS = MASS_BE12;
      break;
    case 8:
      MASS = MASS_BE8;
      break;
    case 0:
      MASS = Hit->GetMassMeV();
      break;
    default:
      cerr<<"unrecognized Z in Corr Particle: "<<Z<<endl;
      MASS = Z;
  }
  
  return CalcCOMmomentum(Hit->GetPosition(),Hit->GetEnergy(),MASS);
  
}

double* CorrParticle(double Energy, double Theta, double Phi, double Mass)
{
  bool debug = 0;
  const double pi = TMath::Pi();
  double QVal = 0.;

  if(int(Mass) == int(MASS_BE12))
    QVal = 1.50619;//from http://www.nndc.bnl.gov/qcalc/;
  else if(int(Mass) == int(MASS_BE8))
    QVal = 1.50619;
  else if(int(Mass) == int(MASS_BE10))
    QVal = 6.310645;
  else
  {
    cerr<<"Error in Corr Particle, I don't recognize the mass"<<endl;
    QVal = 0;
  }
  
  if(debug)
    cout<<"CORR PARTICLE DEBUG ACTIVE, E: "<<Energy<<" T: "<<Theta*180./pi<<" P: "<<Phi*180./pi<<" M: "<<Mass<<" EXPECTED MASS: "<<MASS_BE12<<" or "<<MASS_BE8<<" or "<<MASS_BE10<<endl;
  
  Energy = Energy/1000.;
  
  double *Values = new double[3];
  
  double pParticleMag = sqrt( 2. * Mass * Energy);
  
  TVector3 pParticle = TVector3(pParticleMag,0.,0.);
  pParticle.SetTheta(Theta);
  pParticle.SetPhi(Phi);
  
  if(debug)
    cout<<"MASS_BE11: "<<MASS_BE11<<", BEAM_ENERGY: "<<BEAM_ENERGY<<" sqrt: "<<sqrt( 2. * MASS_BE11 * BEAM_ENERGY)<<", prod: "<<double(MASS_BE11) * double(BEAM_ENERGY)<<endl;
  double pBeamMag = sqrt( 2. * MASS_BE11 * BEAM_ENERGY); //This is all in the z direction
  
  if(debug)
    cout<<"MASS_BE11: "<<MASS_BE11<<", BEAM_ENERGY: "<<BEAM_ENERGY<<", pBeamMag: "<<pBeamMag<<endl;
  
  if(debug)
  {
    cout<<"PARTICLE X: "<<pParticle.X()<<" Y: "<<pParticle.Y()<<" Z: "<<pParticle.Z()<<endl;
  }
  
  double CorrMass = 1.;
  
  if(int(Mass) == int(MASS_BE12))
    CorrMass = MASS_BE8;
  else if(int(Mass) == int(MASS_BE8))
    CorrMass = MASS_BE12;
  else if(int(Mass) == int(MASS_HE4))
    cerr<<"Error in Corr Particle, I can't use a helium, it has to be Be8"<<endl;
  else if(int(Mass) == int(MASS_BE10))
    CorrMass = MASS_BE10;
  else
    cerr<<"Error in Corr Particle, I don't recognize the mass"<<endl;
  
  double ECorr = BEAM_ENERGY - Energy;
  
  double pCorrMag = pBeamMag*pBeamMag/MASS_BE11 - pParticle.Mag2()/Mass;
  
  TVector3 pCorr;
  pCorr.SetX(-pParticle.X());
  pCorr.SetY(-pParticle.Y());
  pCorr.SetZ(pBeamMag-pParticle.Z());
  
  if(debug)
  {
    cout<<"CORR X: "<<pCorr.X()<<" Y: "<<pCorr.Y()<<" Z: "<<pCorr.Z()<<endl;
    cout<<"Magnitudes    Particle: "<<pParticle.Mag()<<", Beam: "<<pBeamMag<<", Corr: "<<pCorr.Mag()<<", other way: "<<pCorrMag*pCorrMag<<endl;
    cout<<"Energies from Mag  Particle: "<<pParticle.Mag2()/(2*Mass)<<", Beam: "<<pBeamMag*pBeamMag/(2*MASS_BE11)<<", Corr: "<<pCorr.Mag2()/(2*CorrMass)<<endl;
  }
  
  Values[0] = pCorr.Mag2() / (2.*CorrMass )*1000.;  //Energy, from E=p^2/2m
  Values[1] = pCorr.Theta();
  Values[2] = pCorr.Phi();
  
  if(debug)
    cout<<"Energy: "<<Values[0]<<" Theta: "<<Values[1]*180/pi<<" Phi: "<<Values[2]*180/pi<<endl;
  
  if(debug)
    cout<<"Checking energy: "<<"Particle E: "<<Energy<<", Corresponding Energy: "<<Values[0]<<", "<<BEAM_ENERGY<<" MeV - those two: "<<BEAM_ENERGY - (Energy + Values[0])<<endl;
  
  if(debug)
    cout<<endl;
  
  return Values;
  
}

double* CorrParticle(TCSMHit* Hit, int Z)
{
  double MASS = 0.;

    switch(Z)
    {
      case 10:
	MASS = MASS_BE10;
	break;
      case 12:
	MASS = MASS_BE12;
	break;
      case 8:
	MASS = MASS_BE8;
	break;
      case 0:
	MASS = Hit->GetMassMeV();
	break;
      default:
	cerr<<"unrecognized Z in Corr Particle: "<<Z<<endl;
	MASS = Z;
    }

  return CorrParticle(Hit->GetEnergy(),Hit->GetDPosition().Theta(),Hit->GetDPosition().Phi(),MASS);
  
}

double* CorrParticleFromAlphas(TCSMHit* Hit1, TCSMHit* Hit2)
{
  double* be8vals;
  be8vals = CalcBe8fromAlpha(Hit1,Hit2);
  return CorrParticle(be8vals[0],be8vals[1],be8vals[2],MASS_BE8);
}

bool AlmostEqual(double a, double b, double threshold)
{
  if( abs(a-b) / ((a+b)/2) < threshold )
    return true;
  else
    return false;
}