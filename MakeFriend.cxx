#include "MakeFriend.h"
#define DEBUG 4

//ClassImp(AlphaFits)

MakeFriend::MakeFriend()
{
  if(DEBUG>0) cout<<"    MakeFriend constructor called"<<endl;
  fr = TFile::Open("friend.root","recreate");
  tf = new TTree("tf","a friend Tree");
  if(DEBUG>1) cout<<"    tf created"<<endl;
  SetBranches();
  ResetFlags();  
  if(DEBUG>1) cout<<"    End of MakeFriend constructor"<<endl;
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

void MakeFriend::ResetFlags()
{
  if(DEBUG>5) cout<<"    Resetting Flags"<<endl;
  AlphaFlag1 = 0;
  //AlphaFlag2 = 0;
  BeFlag = 0;
  //Be10Flag1 = 0;
  //Be10Flag2 = 0;
  //Be12Flag = 0;
  if(DEBUG>5) cout<<"    Flags Reset"<<endl;
}

void MakeFriend::SetBranches()
{
  if(DEBUG>1) cout<<"    Setting Branches"<<endl;
  if(DEBUG>3) tf->Print();
  
  tf->Branch("AlphaFlag1",&AlphaFlag1);
  tf->Branch("BeFlag",&BeFlag);
  if(DEBUG>2) tf->Print();
  
  if(DEBUG>0) cout<<"    Branches Set"<<endl;
}

void MakeFriend::SetAlpha()
{
  AlphaFlag1 = 1;
}

void MakeFriend::SetBe()
{
  BeFlag = 1;
}

void MakeFriend::Fill()
{
  tf->Fill();
}

TTree* MakeFriend::GetFriendTree()
{
  return(tf);
}