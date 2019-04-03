#include "../include/MakeHistos.hh"

bool SIMULATED_DATA;

TFile* ringFile;
// TFile* edgeFile;

TTigress *tigress =  new TTigress;
TCSM *csm =  new TCSM;
TList *cutlist = new TList;

void ProcessChain(TChain *chain,TList *outlist)
{
  int nentries = chain->GetEntries();
  TStopwatch w;
  w.Start();
  TString Be10Cut;
  
  Be10Cut = "pid_low_thick_10Be_%i_v2";
  if(SIMULATED_DATA)
    Be10Cut = "pid_low_thick_10Be_%i_sim";

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

    for(int y=0; y<csm->GetMultiplicity(); y++)
    {
      if(DEBUG)
      {
        cout<<"Main Get Multiplicity()"<<endl;
      }

      TH2D *temp2 = 0;
      TCSMHit *hit = csm->GetHit(y);
      
      if(DEBUG)
      {
        cout<<"General"<<endl;
      }
      
      if(DEBUG) hit->Print();
      
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
                if(tp) tp->Fill(RingNumber(hit,ringFile));
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
  TFile cf("inputRootFiles/cuts.root");
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
  
  ringFile = TFile::Open("inputRootFiles/DumbRings.root","read");
//   edgeFile = TFile::Open("inputRootFiles/edge.root","read");
//   SAFile = TFile::Open("inputRootFiles/solidAngleDiag.root","read");
  
  TList *outlist = new TList;
  cout<<"*Beginning"<<endl;
  SetupHistos(outlist);
  cout<<"*Histos Set"<<endl;
  ProcessChain(chain,outlist);
  cout<<"*ProcessChain Done."<<endl;
  
  Process11BePID(chain,outlist,cutlist,ringFile,SIMULATED_DATA);
  cout<<"*Process11BePID Done."<<endl;
  
  Process9BePID(chain,outlist,cutlist,ringFile,SIMULATED_DATA);
  cout<<"*Process9BePID Done."<<endl;
  
  Process10BePID(chain,outlist,cutlist,ringFile,SIMULATED_DATA);
  cout<<"*Process10BePID Done."<<endl;
  
  ProcessDual10Be(chain,outlist,ringFile,SIMULATED_DATA);
  cout<<"*ProcessDual10Be Done."<<endl;
  
  ProcessDualElastic(chain,outlist,ringFile,SIMULATED_DATA);
  cout<<"*ProcessDualElastic Done."<<endl;
  
  ProcessGammas(chain,outlist);
  cout<<"*ProcessGammas Done."<<endl;
  
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
  
  TString loc = Form("outputRootFiles/%s",outputname.Data());
  if(DEBUG) cout<<"Saving to "<<loc<<endl;
  TFile f(loc,"recreate");
  f.cd();
  outlist->Write();
  f.Close();
  
  if(DEBUG)
  {
    cout<<"All done"<<endl;
  }
  
  return 0;
}





