#include "FunctionsForMakeHistos.hh"

void SetupHistos(TList *outlist)
{
  if(DEBUG) cout<<"Setting up Histos"<<endl;
  TH1D *temp1 = 0;
  TH2D *temp2 = 0;
  TH2I *temp2INT = 0;
  TH3D *temp3 = 0;
  
  for(int id = 1; id<=2;id++)
  {
    
    for(int strip = 0; strip<16; strip++)
    {
      outlist->Add(new TH2D(Form("stripPID_det%i_strip%02i",id,strip),Form("Detector %i PID strip %i",id,strip),3200,0,40,800,0,800));
      temp2 = (TH2D*)outlist->FindObject(Form("stripPID_det%i_strip%02i",id,strip));
      temp2->GetXaxis()->SetTitle("Total Energy deposited in MeV");
      temp2->GetYaxis()->SetTitle("dE/dX in MeV/um");
      
      outlist->Add(new TH2D(Form("EvTheta_%i_BE_strip%02i",id,strip),Form("EvTheta %i, cut on Be Strip",id,strip),100,0,100,700,0,70));
      temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE_strip%02i",id,strip));
      temp2->GetXaxis()->SetTitle("Theta in Degrees");
      temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    }
    
    for(int theta=10;theta<55;theta++)
    {
      outlist->Add(new TH1D(Form("BeEx%i_theta%02i",id,theta),Form("Be-12 Excitation Energy Theta %i",id,theta),500,-10,25));
      temp1 = (TH1D*)outlist->FindObject(Form("BeEx%i_theta%02i",id,theta));
      temp1->GetXaxis()->SetTitle("Energy in MeV");
      temp1->GetYaxis()->SetTitle("Counts");
      
      outlist->Add(new TH1D(Form("BeEx%i_theta%02i_corr",id,theta),Form("Be-12 Excitation Energy Theta %i energy corrected",id,theta),500,-10,25));
      temp1 = (TH1D*)outlist->FindObject(Form("BeEx%i_theta%02i_corr",id,theta));
      temp1->GetXaxis()->SetTitle("Energy in MeV");
      temp1->GetYaxis()->SetTitle("Counts");
    }

    outlist->Add(new TH1D(Form("ETot_%i_2alpha",id),Form("Total energy with two detected alphas in detector %i",id),100,0,100));
    
    outlist->Add(new TH1I(Form("Be10Mult_%i",id),Form("Multiplicity of an identified 10Be in detector %i",id),20,0,20));
    outlist->Add(new TH1I(Form("Be12Mult_%i",id),Form("Multiplicity of an identified 12Be in detector %i",id),20,0,20));
    
    outlist->Add(new TH1D(Form("GammaCut_%i",id),Form("Gamma spectrum cut on Ex Spectrum",id),3000,0,30));
    temp1 = (TH1D*)outlist->FindObject(Form("GammaCut_%i",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");
    
    outlist->Add(new TH1D(Form("Be12_Gamma_%i",id),Form("Gamma spectrum",id),6000,0,30));
    temp1 = (TH1D*)outlist->FindObject(Form("Be12_Gamma_%i",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");

    outlist->Add(new TH1D(Form("Be12_Gamma_%i_dopp",id),Form("Gamma spectrum",id),6000,0,30));
    temp1 = (TH1D*)outlist->FindObject(Form("Be12_Gamma_%i",id));
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
    
    outlist->Add(new TH2D(Form("EvTheta_12Be_%i",id),Form("EvTheta %i",id),100,0,100,700,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_12Be_%i",id));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    
    outlist->Add(new TH1D(Form("GammaCut_dopp_%i",id),Form("Gamma spectrum cut on Ex Spectrum, doppler corrected",id),3000,0,30));
    temp1 = (TH1D*)outlist->FindObject(Form("GammaCut_dopp_%i",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");
    
    outlist->Add(new TH1D(Form("BeEx%i",id),Form("Be-12 Excitation Energy",id),350,-10,25));
    temp1 = (TH1D*)outlist->FindObject(Form("BeEx%i",id));
    temp1->GetXaxis()->SetTitle("Energy in MeV");
    temp1->GetYaxis()->SetTitle("Counts");
    
    outlist->Add(new TH1D(Form("BeEx%i_corr",id),Form("Be-12 Excitation Energy, with straggling correction",id),350,-10,25));
    temp1 = (TH1D*)outlist->FindObject(Form("BeEx%i_corr",id));
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
    
    outlist->Add(new TH2D(Form("pid_%i_fvb",id),Form("Particle ID, detector %i, cut on 1:1",id),700,0,70,700,0,70));//
    temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_fvb",id));
    temp2->GetXaxis()->SetTitle("E Energy deposited in MeV");
    temp2->GetYaxis()->SetTitle("dE Energy deposited in MeV");
    
    outlist->Add(new TH2D(Form("pid_%i_summed",id),Form("Particle ID, detector %i, summed",id),400,0,40,250,0,25));//
    temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_summed",id));
    temp2->GetXaxis()->SetTitle("Total Energy deposited in MeV");
    temp2->GetYaxis()->SetTitle("dE Energy deposited in MeV");
    
    outlist->Add(new TH2D(Form("pid_%i_summed_thickness",id),Form("Particle ID for Detector %i",id),3200,0,160,800,0,2000));//
    temp2->GetYaxis()->SetTitleOffset(1.5);
    temp2->SetContour(666);
    temp2->SetOption("colz");
    temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_summed_thickness",id));
    temp2->GetXaxis()->SetTitle("Total Energy deposited in MeV");
    temp2->GetYaxis()->SetTitle("dE/dX in MeV/um");

    outlist->Add(new TH2D(Form("pid_%i_summed_thickness_corr12",id),Form("Particle ID for Detector %i, detected with an identified 12Be",id),3200,0,160,800,0,2000));//
    temp2->SetOption("colz");
    temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_summed_thickness_corr12",id));
    temp2->GetXaxis()->SetTitle("Total Energy deposited in MeV");
    temp2->GetYaxis()->SetTitle("dE/dX in MeV/um");

    outlist->Add(new TH2D(Form("pid_%i_summed_thickness_2hit",id),Form("Particle ID for Detector %i 2 hits in same detector",id),3200,0,160,800,0,2000));//
    temp2->SetOption("colz");
    temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_summed_thickness_2hit",id));
    temp2->GetXaxis()->SetTitle("Total Energy deposited in MeV");
    temp2->GetYaxis()->SetTitle("dE/dX in MeV/um");
    
    outlist->Add(new TH2D(Form("pid_%i_summed_thickness_2hit_samepix",id),Form("Particle ID for Detector %i 2 hits in same detector",id),3200,0,160,800,0,2000));//
    temp2->SetOption("colz");
    temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_summed_thickness_2hit_samepix",id));
    temp2->GetXaxis()->SetTitle("Total Energy deposited in MeV");
    temp2->GetYaxis()->SetTitle("dE/dX in MeV/um");
    
    outlist->Add(new TH2D(Form("EvTheta_%iTotal",id),Form("Energy vs Theta for Detector %i",id),480,0,120,1400,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%iTotal",id));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    
    outlist->Add(new TH2D(Form("EvTheta_%i_BE8",id),Form("EvTheta %i of the reconstructed 8Be",id),400,0,100,1400,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE8",id));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");

    outlist->Add(new TH2D(Form("EvTheta_%i_BE10",id),Form("EvTheta %i of the identified 10Be",id),400,0,100,1400,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE10",id));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    
    outlist->Add(new TH2D(Form("EvTheta_%i_BE",id),Form("EvTheta %i, cut on Be",id),100,0,100,700,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE",id));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    
    outlist->Add(new TH2D(Form("EvTheta_%i_BE_correlated",id),Form("EvTheta %i, cut on Be",id),100,0,100,700,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE_correlated",id));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    
    outlist->Add(new TH2D(Form("EvTheta_%i_BE_exCut_gs",id),Form("EvTheta %i, cut on Be12 GS",id),100,0,100,700,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE_exCut_gs",id));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    
    outlist->Add(new TH2D(Form("EvTheta_%i_BE_exCut_1e",id),Form("EvTheta %i, cut on Be 2.24",id),100,0,100,700,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE_exCut_1e",id));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    
    outlist->Add(new TH2D(Form("EvTheta_%i_BE_exCut_sc",id),Form("EvTheta %i, cut on Be <5",id),100,0,100,700,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE_exCut_sc",id));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    
    outlist->Add(new TH2D(Form("CheckCalE_%i",id),Form("Front Energy vs Back Energy, Detector %i E",id),600,0,60,600,0,60));
    temp2 = (TH2D*)outlist->FindObject(Form("CheckCalE_%i",id));
    temp2->GetXaxis()->SetTitle("Energy deposited in Vertical (Front)");
    temp2->GetYaxis()->SetTitle("Energy deposited in Horizontal (Back)");
    
    outlist->Add(new TH2D(Form("twohitEVT_%i_side",id),Form("Corresponding hit from TwoHit in detector %i",id),400,0,100,1400,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("twohitEVT_%i_side",id));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    
    outlist->Add(new TH2D(Form("twohitEVT_%i_telescope",id),Form("Corresponding hit from TwoHit in detector %i",id),400,0,100,1400,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("twohitEVT_%i_telescope",id));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");

    outlist->Add(new TH1D(Form("TotalEnergy_%i_12Be",id),Form("Total energy of an event with 12Be in detector %i",id),100,0,100));
    
    
    for(int mid = 1; mid<=4;mid++)
    {
      outlist->Add(new TH2D(Form("pid_%i_mult%i",id,mid),Form("Particle ID, detector %i with Multiplicity %i",id,mid),700,0,70,700,0,70));
      temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_mult%i",id,mid));
      temp2->GetXaxis()->SetTitle("E Energy deposited in MeV");
      temp2->GetYaxis()->SetTitle("dE Energy deposited in MeV");
      
      outlist->Add(new TH2D(Form("EvTheta_%iTotal_mult%i",id,mid),Form("E vs Theta Detector %i with Multiplicity %i",id,mid),100,0,100,700,0,70));
      temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%iTotal_mult%i",id,mid));
      temp2->GetXaxis()->SetTitle("Theta in Degrees");
      temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");

      outlist->Add(new TH2D(Form("pid_%i_summed_thickness_mult%i",id,mid),Form("Particle ID for Detector %i, with Multiplicity %i",id,mid),3200,0,160,800,0,2000));//
      temp2->SetOption("colz");
      temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_summed_thickness_mult%i",id,mid));
      temp2->GetXaxis()->SetTitle("Total Energy deposited in MeV");
      temp2->GetYaxis()->SetTitle("dE/dX in MeV/um");
    }
  }
  
  for(int det=1;det<=4;det++)
  {    
    outlist->Add(new TH2D(Form("Alphacone_%i",det),Form("Alpha cone in detector %i",det),300,0,60,300,0,60));//
    temp2 = (TH2D*)outlist->FindObject(Form("Alphacone_%i",det));
    temp2->SetContour(666);
    temp2->SetOption("colz");
    temp2->GetXaxis()->SetTitle("Energy deposited in MeV");
    temp2->GetYaxis()->SetTitle("Energy deposited in MeV");
    
    outlist->Add(new TH1D(Form("AlphaEx%i",det),Form("AlphaEx Excitation Energy",det),350,-10,25));
    temp1 = (TH1D*)outlist->FindObject(Form("AlphaEx%i",det));
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

    outlist->Add(new TH2D(Form("twohit_twocut_%i",det),Form("2 Hits in detector %i, cut on cone and angle",det),100,0,100,700,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("twohit_twocut_%i",det));
    temp2->SetOption("colz");
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");

    outlist->Add(new TH2D(Form("twohit_twocut_%i_Be8",det),Form("2 Hits in detector %i, cut on cone and angle",det),100,0,100,700,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("twohit_twocut_%i_Be8",det));
    temp2->SetOption("colz");
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");

    outlist->Add(new TH2D(Form("twohit_twocut_%i_corr",det),Form("Correpsonding particle of 2 hits in detector %i",det),100,0,100,700,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("twohit_twocut_%i_corr",det));
    temp2->SetOption("colz");
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");

    outlist->Add(new TH2D(Form("twohit_twocut_%i_corr_cut",det),Form("Correpsonding particle of 2 hits in detector %i, cut on PID",det),100,0,100,700,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("twohit_twocut_%i_corr_cut",det));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");

    outlist->Add(new TH1D(Form("twohit_twocut_%i_corr_Etot",det),Form("Total energy under two cuts with corresponding particle, det %i",det),100,0,100));

    outlist->Add(new TH2D(Form("twohit_twocut_%i_corr_PID",det),Form("Correpsonding particle of 2 hits in detector %i, PID",det),3200,0,160,800,0,2000));
    temp2 = (TH2D*)outlist->FindObject(Form("twohit_twocut_%i_corr_PID",det));
    temp2->GetXaxis()->SetTitle("E Energy deposited in MeV");
    temp2->GetYaxis()->SetTitle("dE/dX in MeV/um");
    
    outlist->Add(new TH2D(Form("EvTheta_%i_HE",det),Form("EvTheta %i, cut on He",det),100,0,100,700,0,70));
    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_HE",det));
    temp2->GetXaxis()->SetTitle("Theta in Degrees");
    temp2->GetYaxis()->SetTitle("Total Energy deposited in MeV");
    
    outlist->Add(new TH2D(Form("CheckCalD_%i",det),Form("Front Energy vs Back Energy, Detector %i D",det),600,0,60,600,0,60));
    temp2 = (TH2D*)outlist->FindObject(Form("CheckCalD_%i",det));
    temp2->GetXaxis()->SetTitle("Energy deposited in Vertical (Back)");
    temp2->GetYaxis()->SetTitle("Energy deposited in Horizontal (Front)");
    for(char type='D';type<='E';type++) //Wow I can't believe this works.  I am glad they are in alphabetical order
    {
      if(det>2 && type=='E')//This skips 3 and 4 E, which don't exist
	continue;
      
      outlist->Add(new TH2D(Form("EvTheta_%i%c",det,type),Form("EvTheta, Detector %i, Position %c",det,type),100,0,100,700,0,70));//
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

  outlist->Add(new TH1D("fred1_1","Total energy of multiplicity 3",100,0,100));
  outlist->Add(new TH1D("fred2_1","Total energy of multiplicity 3, 2 hits in side",100,0,100));
  outlist->Add(new TH1D("fred3_1","Total energy of multiplicity 3, 2 hits in side and 12Be identified",100,0,100));
  
  outlist->Add(new TH2D("CSM_HP_Theta_Phi","Angular Coverage Map of Silicon Detectors",120,0,120,360,-180,180));
  temp2 = (TH2D*)outlist->FindObject("CSM_HP_Theta_Phi");
  temp2->GetXaxis()->SetTitle("Theta (Degrees)");
  temp2->GetYaxis()->SetTitle("Phi (Degrees)");
  
  outlist->Add(new TH1D("Multiplicity","Multiplicity",10,0,10));//

  outlist->Add(new TH1D("deltaGamma","Time difference between 12Be and 2.12 Gamma",1000,0,1000));

  outlist->Add(new TH1I("counts","counts",2,1,2));
  
  outlist->Add(new TH1D("GammaSum","Gamma Spectrum",2000,0,200));
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
  
  outlist->Add(new TH1I("StatCheck","Checking Cases",20,0,20));

  outlist->Add(new TH1I("MultBlobHigh","Multiplicity of the high energy blob in the dEve",20,0,20));
  outlist->Add(new TH1I("MultBlobLow","Multiplicity of the low energy blob in the dEvE",20,0,20));

  outlist->Add(new TH1I("AlphaMult","Number of identified alphas in an event",5,0,5));
  
  
  if(DEBUG)
  {
    cout<<"Histos Set"<<endl;
  }
}

double GetExciteE_Heavy(double be12E, double be12T)
{
  //   cout<<"BeamE: "<<BeamE<<endl;
  //   cout<<"BeE: "<<be12E<<endl;
  //   cout<<"BeT: "<<be12T<<endl;
  be12E=be12E/1000.;
  const double pi = TMath::Pi();
  
  const double M1 = MASS_BE11;
  const double M2 = MASS_BE9;
  const double M3 = MASS_BE8;
  const double M4 = MASS_BE12;
  double mQ = M1+M2-M3-M4;
  
  double V1 = sqrt(2*BEAM_ENERGY/M1);
  double COMV = ( M1 / ( M1 + M2 ) ) * V1;
  double V4 = sqrt(2*be12E/M4);
  double kPrimeM4 = COMV / V4;
  
  double COMTotalE = M2 / ( M1 + M2 ) * BEAM_ENERGY;
  double COMEnergyM4 = be12E * ( 1 + kPrimeM4*kPrimeM4 - 2*kPrimeM4*cos( be12T ) );
  double QVal =  ( COMEnergyM4*( M3 + M4 ) ) / M3 - COMTotalE;
  double ExcitedState = mQ - QVal;
  
  //   cout<<"EX: "<<ExcitedState<<endl<<endl;
  
  return(ExcitedState);
  
}

double GetExciteE_Heavy(TCSMHit* Hit)
{
  return(GetExciteE_Heavy(Hit->GetEnergy(),Hit->GetDPosition().Theta()));
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

double Doppler(TTigressHit* thit, TCSMHit* chit)
{
  double pi = TMath::Pi();
  double M4 = MASS_BE12;
  
  double LabEnergyHeavy = chit->GetEnergyMeV();
  
  double beta =  sqrt( (LabEnergyHeavy*(LabEnergyHeavy + 2.0*M4) )/( ( LabEnergyHeavy + M4 )*( LabEnergyHeavy + M4 ) ) );
  
  double CosTheta = cos( chit->GetPosition().Angle( thit->GetPosition() ) );
  
  double RelativisticCorr = (1.0 / ( sqrt( 1.0 - beta*beta ) ) );
  
  double EGammaDopplerCorr = ( (thit->GetCore()->GetEnergy()/1000. * RelativisticCorr) * ( (1.0 - beta*CosTheta) ) );
  
  return EGammaDopplerCorr;
}

double CalcCOMProductTheta(double theta, double energy)
{
  const double pi = TMath::Pi();
  const double BeamE = BEAM_ENERGY;
  
  const double M1 = MASS_BE11;
  const double M2 = MASS_BE9;
  const double M3 = MASS_BE8;
  const double M4 = MASS_BE12;
  
  double COMVelBeam = sqrt(2*BeamE/M1);
  double COMV = ( M1 / ( M1 + M2 ) ) * COMVelBeam;
  double productVel = sqrt(2*energy/M3);//to get the 12Be COM theta
  
  double kPrime = COMV / productVel;
  
  double thetaCOM = atan( sin(theta) / ( cos(theta)-kPrime ) );
  
  if( thetaCOM < 0 )
    thetaCOM += pi;
  
  return thetaCOM; //12Be
}

double* CorrParticle(double Energy, double Theta, double Phi, double Mass)
{
  bool debug = 0;
  const double pi = TMath::Pi();
  const double QVal = 1.50619;//from http://www.nndc.bnl.gov/qcalc/
  
  if(debug)
    cout<<"CORR PARTICLE DEBUG ACTIVE, E: "<<Energy<<" T: "<<Theta*180./pi<<" P: "<<Phi*180./pi<<" M: "<<Mass<<" EXPECTED MASS: "<<MASS_BE12<<" or "<<MASS_BE8<<endl;
  
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

double* CorrParticle(TCSMHit* Hit)
{
  return CorrParticle(Hit->GetEnergy(),Hit->GetDPosition().Theta(),Hit->GetDPosition().Phi(),Hit->GetMassMeV());
}

double* CorrParticleFromAlphas(TCSMHit* Hit1, TCSMHit* Hit2)
{
  double* be8vals;
  be8vals = CalcBe8fromAlpha(Hit1,Hit2);
  return CorrParticle(be8vals[0],be8vals[1],be8vals[2],MASS_BE8);
}
