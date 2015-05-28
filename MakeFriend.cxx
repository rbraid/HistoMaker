#include "MakeFriend.h"
#define DEBUG 4

//ClassImp(AlphaFits)

MakeFriend::MakeFriend(TCSM* csmstart)
{
  if(DEBUG>0) cout<<"    MakeFriend constructor called"<<endl;
  fr = TFile::Open("friend.root","recreate");
  tf = new TTree("tf","a friend Tree");
  if(DEBUG>1) cout<<"    tf created"<<endl;
  SetBranches();
  ResetFlags();  
  if(DEBUG>1) cout<<"    End of MakeFriend constructor"<<endl;
  csm=csmstart;
  cout<<"MULTIPLICITY:"<<csmstart->GetMultiplicity()<<endl;
}

MakeFriend::~MakeFriend()
{
  if(DEBUG>0) cout<<"     MakeFriend Destructor called"<<endl;
  fr->Close();
  if(DEBUG>1) cout<<"     End of MakeFriend destructor"<<endl;
}

void MakeFriend::Write()
{
  if(DEBUG>2) tf->Print();
  
  fr->cd();
  if(DEBUG>1) cout<<"     Writing"<<endl;
  tf->Write();
}

void MakeFriend::Reset()
{
  ResetFlags();

  Alpha_1 = 0;
  Alpha_2 = 0;
  Be = 0;
  Be10_1 = 0;
  Be10_2 = 0;
  Be9 = 0;
  Be12 = 0;
}

void MakeFriend::ResetFlags()
{
  if(DEBUG>5) cout<<"    Resetting Flags"<<endl;
  AlphaFlag1 = 0;
  AlphaFlag2 = 0;
  BeFlag = 0;
  Be10Flag1 = 0;
  Be10Flag2 = 0;
  Be12Flag = 0;
  Be9Flag = 0;
  if(DEBUG>5) cout<<"    Flags Reset"<<endl;
}

void MakeFriend::SetBranches()
{
  if(DEBUG>1) cout<<"    Setting Branches"<<endl;
  if(DEBUG>3) tf->Print();
  
  tf->Branch("AlphaFlag1",&AlphaFlag1);
  tf->Branch("AlphaFlag2",&AlphaFlag2);
  
  tf->Branch("BeFlag",&BeFlag);
  tf->Branch("Be10Flag1",&Be10Flag1);
  tf->Branch("Be10Flag2",&Be10Flag2);
  tf->Branch("Be12Flag",&Be12Flag);
  tf->Branch("Be9Flag",&Be9Flag);

  if(DEBUG>1) cout<<"    Settting CSMHits"<<endl;
  
  tf->Branch("Alpha_1",&Alpha_1);
  tf->Branch("Alpha_2",&Alpha_2);
  tf->Branch("Be",&Be);
  tf->Branch("Be10_1",&Be10_1);
  tf->Branch("Be10_2",&Be10_2);
  tf->Branch("Be9",&Be9);
  tf->Branch("Be12",&Be12);
  
  if(DEBUG>2) tf->Print();
  
  if(DEBUG>0) cout<<"    Branches Set"<<endl;
}

void MakeFriend::SetAlpha(int num)
{
  if(Alpha_1 == 0)
  {
    if(DEBUG >4) cout<<"Setting first alpha at "<<num<<endl;
    Alpha_1 = num;
  }
  else if(Alpha_2 == 0)
  {
    if(DEBUG >4) cout<<"Setting second alpha at "<<num<<endl;
    Alpha_2 = num;
  }
  else
  {
    cerr<<DRED<<"   Error, SetAlpha is full.  3 or more alphas detected"<<RESET_COLOR<<endl;
  }
}

void MakeFriend::SetBe(int num, int isotope)
{
  //cout<<"Before Be"<<endl;
  //cout<<Be<<endl;
}

TCSMHit* GetAlpha(int alphanumber)
{
  //return(csm->GetHit(alphanumber));
}

TCSMHit* GetBe()
{

}

void MakeFriend::Fill()
{
  tf->Fill();
}

TTree* MakeFriend::GetFriendTree()
{
  return(tf);
}