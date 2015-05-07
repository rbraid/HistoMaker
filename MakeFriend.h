#ifndef MakeFriend_h
#define MakeFriend_h

#include <TFile.h>
#include <TTree.h>
#include <iostream>
using namespace std;

class MakeFriend
{
public:
  MakeFriend();
  ~MakeFriend();

  void ResetFlags();
  void SetBranches();
  void SetAlpha();
  void SetBe();
  void Fill();
  void Write();

  TTree* GetFriendTree();
  
private:
  Bool_t AlphaFlag1;
  //Bool_t AlphaFlag2;
  Bool_t BeFlag;
  //Bool_t Be10Flag1;
  //Bool_t Be10Flag2;
  //Bool_t Be12Flag;
  
  TFile *fr;
  TTree *tf;


};

#endif
