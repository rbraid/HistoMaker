#include "../include/MakeHistos.hh"


int main(int argc, char **argv)
{
  bool SIMULATED_DATA;
  
  TStopwatch wGlobal;
  wGlobal.Start();
  
  TTigress *tigress =  new TTigress;
  TCSM *csm =  new TCSM;
  
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
  
  TList* cutlist = new TList;
  
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
    
  TList *suppList = new TList;
  
  TFile* ringFile = TFile::Open("inputRootFiles/DumbRings.root","read");
  suppList->Add(ringFile);
  
  TFile* gammaFile = TFile::Open("inputRootFiles/GammaInfo.root","read");
  suppList->Add(gammaFile);
  
  
  TList *outlist = new TList;
  cout<<"*Beginning"<<endl;
  SetupHistos(outlist);
  cout<<"*Histos Set"<<endl;

  SetupBasicHistos(outlist);
  ProcessBasic(chain,outlist);
  
  Setup11BePIDHistos(outlist);
  Process11BePID(chain,outlist,cutlist,suppList,SIMULATED_DATA);
  
  Process9BePID(chain,outlist,cutlist,suppList,SIMULATED_DATA);
  
  Process10BePID(chain,outlist,cutlist,suppList,SIMULATED_DATA);
  
  ProcessOpposite(chain,outlist,cutlist,suppList,SIMULATED_DATA);
  
  SetupDualHistos(outlist);
  ProcessDual10Be(chain,outlist,cutlist,suppList,SIMULATED_DATA);
  
  SetupDualElasticHistos(outlist);
  ProcessDualElastic(chain,outlist,cutlist,suppList,SIMULATED_DATA);
  
  SetupHistosDualAndPID(outlist);
  ProcessPIDandDual(chain,outlist,cutlist,suppList,SIMULATED_DATA);
  
//   if(!SIMULATED_DATA)
//     ProcessGammas(chain,outlist,suppList);
  
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
  
  delete csm;
  delete tigress;
  
  cout<<endl<<"Total Real Time: "<<DGREEN<<wGlobal.RealTime()/60.<<" minutes."<<RESET_COLOR<<endl;
  
  return 0;
}





