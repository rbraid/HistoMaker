#ifndef MakeFriend_h
#define MakeFriend_h

#include <TFile.h>
#include <TTree.h>
#include <TCSM.h>
#include <TCSMHit.h>
#include <iostream>
using namespace std;

class MakeFriend
{
public:
  MakeFriend(TCSM*);
  ~MakeFriend();

  void ResetFlags();
  void Reset();
  void SetBranches();
  void SetAlpha(int);
  void SetBe(int,int isotope=0);
  TCSMHit* GetAlpha(int);
  TCSMHit* GetBe();
  void Fill();
  void Write();

  TTree* GetFriendTree();
  
private:
  Bool_t AlphaFlag1;
  Bool_t AlphaFlag2;
  Bool_t BeFlag;
  Bool_t Be10Flag1;
  Bool_t Be10Flag2;
  Bool_t Be12Flag;
  Bool_t Be9Flag;

  Int_t Alpha_1;
  Int_t Alpha_2;
  Int_t Be;
  Int_t Be10_1;
  Int_t Be10_2;
  Int_t Be9;
  Int_t Be12;
  
  TFile *fr;
  TTree *tf;

  TCSM* csm;
};

#endif
