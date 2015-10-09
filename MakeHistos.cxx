// g++ MakeHistos.cxx FunctionsForMakeHistos.cxx -Wl,--no-as-needed -o RunMe `grsi-config --cflags --all-libs --root`

#include "FunctionsForMakeHistos.hh"

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

  if(DEBUG)
  {
    cout<<"Starting ProcessChain"<<endl;
  }

  for(int x=0; x<nentries; x++)
  {
    chain->GetEntry(x);

    bool IsInteresting = 0;
    
    if(DEBUG)
    {
      cout<<"nentries: "<<nentries<<endl;
    }

    if(csm->GetMultiplicity()==0)
      continue;

    ((TH1D *)outlist->FindObject("Multiplicity"))->Fill(csm->GetMultiplicity());

    TCSMHit *Be11Hit;
    TCSMHit *Alpha1Hit;
    TCSMHit *Alpha2Hit;
    TCSMHit *Be12Hit;

    bool Be11Flag = 0;
    bool Alpha1Flag = 0;
    bool Alpha2Flag = 0;
    bool Be12Flag = 0;

    if(csm->GetMultiplicity()>=2)
    {

      int hits[4] = {0};
      int Ahits[4] = {0};
      int Bhits[4] = {0};

      for(int y=0; y<csm->GetMultiplicity(); y++)
      {
	if(DEBUG)
	{
	  cout<<"Be's Get Multiplicity()"<<endl;
	}

	if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form("thickness_alpha_%i_1",csm->GetHit(y)->GetDetectorNumber()))))
	{
	  if(cut->IsInside(csm->GetHit(y)->GetEnergyMeV(),csm->GetHit(y)->GetDdE_dx()) && csm->GetHit(y)->GetEEnergy() > 10)
	  {
	    if(!Alpha1Flag)
	    {
	      csm->GetHit(y)->SetIsotope(4,"He");
	      Alpha1Hit = csm->GetHit(y);
	      Alpha1Flag = 1;
	      Ahits[csm->GetHit(y)->GetDetectorNumber()-1]++;
	    }
	    else if(!Alpha2Flag)
	    {
	      csm->GetHit(y)->SetIsotope(4,"He");
	      Alpha2Hit = csm->GetHit(y);
	      Alpha2Flag = 1;
	      Ahits[csm->GetHit(y)->GetDetectorNumber()-1]++;
	    }
	    else
	    {
	      cout<<"Too Many Alphas!"<<endl;
	      //IsInteresting = 1;
	    }
	  }
	}
	//if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form("theta_Be_%i_%02i",hit->GetDetectorNumber(),int(hit->GetThetaDeg())))))
	//{
	//  if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()))
	 // {
	  //}
	//}
	if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form("Be12_thick_%i_v1",csm->GetHit(y)->GetDetectorNumber()))))
	{
	  if(cut->IsInside(csm->GetHit(y)->GetEnergyMeV(),csm->GetHit(y)->GetDdE_dx()) && csm->GetHit(y)->GetEEnergy() > 10)
	  {
	    if(!Be12Flag)
	    {
	      csm->GetHit(y)->SetIsotope(12,"Be");
	      Be12Hit = csm->GetHit(y);
	      Be12Flag = 1;
	      Bhits[csm->GetHit(y)->GetDetectorNumber()-1]++;
	    }
	    else
	    {
	      cout<<"Too Many Be!"<<endl;
	      //IsInteresting = 1;
	    }
	  }
	}

	hits[csm->GetHit(y)->GetDetectorNumber()-1]++;
      }

      for(int det=0;det<4;det++)
      {
	int corrdet;
	if(det==0)
	  corrdet=1;
	else if(det==1)
	  corrdet=0;
	else if(det==2)
	  corrdet=1;
	else if(det==3)
	  corrdet=0;
	else
	  corrdet=-1;
	
	if(hits[det]>=2)
	{
	  int loc1 = -1;
	  int loc2 = -1;
	  int corrloc = -1;
	  
	  for(int search=0; search<csm->GetMultiplicity();search++)
	  {
	    if(csm->GetHit(search)->GetDetectorNumber() == det+1 &&
	      csm->GetHit(search)->GetDEnergy()>1)
	    {
	      if(loc1==-1)
		loc1=search;
	      else if(loc2==-1)
		loc2=search;
	      else
		cerr<<"Too many hits in one detector"<<endl;
	    }
	    if(csm->GetHit(search)->GetDetectorNumber() == corrdet+1 &&
	      csm->GetHit(search)->GetDEnergy()>1)
	    {
	      if(corrloc == -1)
		corrloc = search;
	      else
		cerr<<"Too many correlated location hits"<<endl;
	    }
	  }
	  if(loc2!=-1)
	  {
	    TRandom *rnd = new TRandom(x);
	    if(rnd->Uniform(1)>.5)
	    {
	      int temp;
	      temp = loc1;
	      loc1 = loc2;
	      loc2 = temp;
	    }
	    TH2D* conepointer = (TH2D*)outlist->FindObject(Form("twohit_%i",det+1));
	    conepointer->Fill(csm->GetHit(loc1)->GetEnergyMeV(),csm->GetHit(loc2)->GetEnergyMeV());
	    TH2D* diffpointer =(TH2D*)outlist->FindObject(Form("twohit_%i_thetadiff",det+1));
	    diffpointer->Fill(csm->GetHit(loc1)->GetThetaDeg()-csm->GetHit(loc2)->GetThetaDeg(),csm->GetHit(loc1)->GetEnergyMeV()+csm->GetHit(loc2)->GetEnergyMeV());

	    if(csm->GetHit(loc1)->GetThetaDeg()==csm->GetHit(loc2)->GetThetaDeg())
	    {
	      cout<<csm->GetMultiplicity()<<" "<<loc1<<" "<<loc2<<endl;
	      csm->GetHit(loc1)->Print();
	      csm->GetHit(loc2)->Print();
	      cout<<endl;
	    }
	  }
	  if(corrloc != -1)
	  {
	    TH2D* evtpointer;
	    if(det<2)
	      evtpointer = (TH2D*)outlist->FindObject(Form("twohitEVT_%i_telescope",det+1));
	    else
	      evtpointer = (TH2D*)outlist->FindObject(Form("twohitEVT_%i_side",det-1));
	    
	    evtpointer->Fill(csm->GetHit(corrloc)->GetThetaDeg(),csm->GetHit(corrloc)->GetEnergyMeV());
	  }
	}
      }

       int AlphaSum = Ahits[0]+Ahits[1]+Ahits[2]+Ahits[3];
       int BeSum = Bhits[0]+Bhits[1]+Bhits[2]+Bhits[3];
//       if(BeSum >=1)
//       {
// 	cout<<"Alpha Sum: "<<AlphaSum<<endl;
// 	cout<<"Be12 Sum: "<<BeSum<<endl;
//       }

      double be8thetawindow = 15./180*TMath::Pi();//Simulations indicate the max spread of the Be8 should be 15 degrees.
      double be12thetawindow = 20./180*TMath::Pi();//This is a guess
      double be12energywindow = 2500; //This is a guess in keV

      if(Ahits[0]==2&&hits[1]==1)
      {
	((TH1I *)outlist->FindObject("StatCheck"))->Fill(2);
	
	for(int iter=0; iter<csm->GetMultiplicity(); iter++)
	{
	  if(csm->GetHit(iter)->GetDetectorNumber()==1+1)
	  {
	    csm->GetHit(iter)->SetIsotope(12,"Be");
	    Be12Hit = csm->GetHit(iter);
	    Be12Flag = 1;
	    break;
	  } 
	}
      }
      else if(Ahits[1]==2&&hits[0]==1)
      {
	((TH1I *)outlist->FindObject("StatCheck"))->Fill(3);
	
	for(int iter=0; iter<csm->GetMultiplicity(); iter++)
	{
	  if(csm->GetHit(iter)->GetDetectorNumber()==0+1)
	  {
	    csm->GetHit(iter)->SetIsotope(12,"Be");
	    Be12Hit = csm->GetHit(iter);
	    Be12Flag = 1;
	    break;
	  }
	}
      }
      else if(Bhits[0]==1&&hits[1]==2)
      {
	((TH1I *)outlist->FindObject("StatCheck"))->Fill(4);
	int loc1=-1;
	int loc2=-1;
	for(int iter=0; iter<csm->GetMultiplicity(); iter++)
	{
	  if(csm->GetHit(iter)->GetDetectorNumber()==1+1)
	  {
	    if(loc1==-1)
	      loc1=iter;
	    else if(loc2==-1)
	      loc2=iter;
	    else
	      cerr<<"Too many alphas!"<<endl;
	  }
	}
	if(abs(csm->GetHit(loc1)->GetDPosition().Theta()-csm->GetHit(loc2)->GetDPosition().Theta()) < be8thetawindow)
	{
	  csm->GetHit(loc1)->SetIsotope(4,"He");
	  Alpha1Hit = csm->GetHit(loc1);
	  Alpha1Flag = 1;
	  csm->GetHit(loc2)->SetIsotope(4,"He");
	  Alpha2Hit = csm->GetHit(loc2);
	  Alpha2Flag = 1;
	}
      }
      else if(Bhits[1]==1&&hits[0]==2)
      {
	((TH1I *)outlist->FindObject("StatCheck"))->Fill(5);
	int loc1=-1;
	int loc2=-1;
	for(int iter=0; iter<csm->GetMultiplicity(); iter++)
	{
	  if(csm->GetHit(iter)->GetDetectorNumber()==0+1)
	  {
	    if(loc1==-1)
	      loc1=iter;
	    else if(loc2==-1)
	      loc2=iter;
	    else
	      cerr<<"Too many alphas!"<<endl;
	  }
	}
	if(abs(csm->GetHit(loc1)->GetDPosition().Theta()-csm->GetHit(loc2)->GetDPosition().Theta()) < be8thetawindow)
	{
	  csm->GetHit(loc1)->SetIsotope(4,"He");
	  Alpha1Hit = csm->GetHit(loc1);
	  Alpha1Flag = 1;
	  csm->GetHit(loc2)->SetIsotope(4,"He");
	  Alpha2Hit = csm->GetHit(loc2);
	  Alpha2Flag = 1;
	}
      }
      else if(Ahits[0]==1&&hits[0]>=2)
      {
	((TH1I *)outlist->FindObject("StatCheck"))->Fill(6);
	
	for(int iter=0; iter<csm->GetMultiplicity(); iter++)
	{
	  if(csm->GetHit(iter)->GetDetectorNumber()==0+1)
	  {
	    if(csm->GetHit(iter)->GetIsotope()=="default" && abs(csm->GetHit(iter)->GetDPosition().Theta() - Alpha1Hit->GetDPosition().Theta()) < be8thetawindow)
	    {
	      csm->GetHit(iter)->SetIsotope(4,"He");
	      Alpha2Hit = csm->GetHit(iter);
	      Alpha2Flag = 1;
	    }
	  }
	}
      }
      else if(Ahits[1]==1&&hits[1]>=2)
      {
	((TH1I *)outlist->FindObject("StatCheck"))->Fill(7);
	
	for(int iter=0; iter<csm->GetMultiplicity(); iter++)
	{
	  if(csm->GetHit(iter)->GetDetectorNumber()==1+1)
	  {
	    if(csm->GetHit(iter)->GetIsotope()=="default" && abs(csm->GetHit(iter)->GetDPosition().Theta() - Alpha1Hit->GetDPosition().Theta()) < be8thetawindow)
	    {
	      csm->GetHit(iter)->SetIsotope(4,"He");
	      Alpha2Hit = csm->GetHit(iter);
	      Alpha2Flag = 1;
	    }
	  }
	}
      }
      else if(hits[0]>=1&&hits[1]>=2&&Bhits[1]==0&&Ahits[0]==0)
      {
	((TH1I *)outlist->FindObject("StatCheck"))->Fill(8);

	int aloc1 = -1;
	int aloc2 = -1;
	int alocOverflow = -1;
	int beloc = -1;
	int belocOverflow = -1;
	
	for(int y=0; y<csm->GetMultiplicity(); y++)
	{
	  if(csm->GetHit(y)->GetDetectorNumber()==1+1 && csm->GetHit(y)->GetEnergy()>1 )
	  {
	    if(aloc1==-1)
	      aloc1=y;
	    else if(aloc2==-1)
	      aloc2=y;
	    else if(alocOverflow==-1)
	      alocOverflow=y;
	    else
	      cerr<<" Error, alpha overflow exceeded.  You should probably write a vector-based version of this..."<<endl;
	  }
	  else if(csm->GetHit(y)->GetDetectorNumber()==0+1 && csm->GetHit(y)->GetEnergy()>1)
	  {
	    if(beloc = -1)
	      beloc=y;
	    else if(belocOverflow = -1)
	      belocOverflow=y;
	    else
	      cerr<<" Error, Be overflow exceeded.  You should probably write a vector-based version of this..."<<endl;
	  }
	}
	TCutG *cut = (TCutG*)(cutlist->FindObject("alphacone_2_2"));
	  
	if(aloc2!=-1 &&
	  abs(csm->GetHit(aloc1)->GetDPosition().Theta() - csm->GetHit(aloc2)->GetDPosition().Theta()) < be8thetawindow &&
	  cut->IsInside(csm->GetHit(aloc1)->GetEnergyMeV(),csm->GetHit(aloc2)->GetEnergyMeV()))
	{
	  csm->GetHit(aloc1)->SetIsotope(4,"He");
	  Alpha1Hit = csm->GetHit(aloc1);
	  Alpha1Flag = 1;
	  csm->GetHit(aloc2)->SetIsotope(4,"He");
	  Alpha2Hit = csm->GetHit(aloc2);
	  Alpha2Flag = 1;
	}

	else if(alocOverflow!=-1 &&
	  abs(csm->GetHit(aloc1)->GetDPosition().Theta() - csm->GetHit(alocOverflow)->GetDPosition().Theta()) < be8thetawindow &&
	  cut->IsInside(csm->GetHit(aloc1)->GetEnergyMeV(),csm->GetHit(alocOverflow)->GetEnergyMeV()))
	{
	  csm->GetHit(aloc1)->SetIsotope(4,"He");
	  Alpha1Hit = csm->GetHit(aloc1);
	  Alpha1Flag = 1;
	  csm->GetHit(alocOverflow)->SetIsotope(4,"He");
	  Alpha2Hit = csm->GetHit(alocOverflow);
	  Alpha2Flag = 1;
	}

	else if(aloc2!=-1 && alocOverflow!=-1 &&
	  abs(csm->GetHit(aloc2)->GetDPosition().Theta() - csm->GetHit(alocOverflow)->GetDPosition().Theta()) < be8thetawindow &&
	  cut->IsInside(csm->GetHit(aloc2)->GetEnergyMeV(),csm->GetHit(alocOverflow)->GetEnergyMeV()))
	{
	  csm->GetHit(aloc2)->SetIsotope(4,"He");
	  Alpha1Hit = csm->GetHit(aloc2);
	  Alpha1Flag = 1;
	  csm->GetHit(alocOverflow)->SetIsotope(4,"He");
	  Alpha2Hit = csm->GetHit(alocOverflow);
	  Alpha2Flag = 1;
	}
	
	if(Alpha2Flag && Alpha1Flag)
	{
	  double* corvals;
	  corvals = CorrParticleFromAlphas(Alpha1Hit,Alpha2Hit);
	  if(beloc!=-1 && abs(csm->GetHit(beloc)->GetPosition().Theta() - corvals[1]) < be12thetawindow && abs(csm->GetHit(beloc)->GetEnergy() - corvals[0]) < be12energywindow)
	  {
	    csm->GetHit(beloc)->SetIsotope("12Be");
	    Be12Hit = csm->GetHit(beloc);
	    Be12Flag = 1;
	  }
	  else if(belocOverflow!=-1 && abs(csm->GetHit(belocOverflow)->GetPosition().Theta() - corvals[1]) < be12thetawindow && abs(csm->GetHit(belocOverflow)->GetEnergy() - corvals[0]) < be12energywindow)
	  {
	    csm->GetHit(belocOverflow)->SetIsotope("12Be");
	    Be12Hit = csm->GetHit(belocOverflow);
	    Be12Flag = 1;
	  }
	}
      }
      else if(hits[1]>=1&&hits[0]>=2&&Bhits[0]==0&&Ahits[1]==0)
      {
	((TH1I *)outlist->FindObject("StatCheck"))->Fill(9);
	
	int aloc1 = -1;
	int aloc2 = -1;
	int alocOverflow = -1;
	int beloc = -1;
	int belocOverflow = -1;
	
	for(int y=0; y<csm->GetMultiplicity(); y++)
	{
	  if(csm->GetHit(y)->GetDetectorNumber()==0+1 && csm->GetHit(y)->GetEnergy()>1 )
	  {
	    if(aloc1==-1)
	      aloc1=y;
	    else if(aloc2==-1)
	      aloc2=y;
	    else if(alocOverflow==-1)
	      alocOverflow=y;
	    else
	      cerr<<" Error, alpha overflow exceeded.  You should probably write a vector-based version of this..."<<endl;
	  }
	  else if(csm->GetHit(y)->GetDetectorNumber()==1+1 && csm->GetHit(y)->GetEnergy()>1)
	  {
	    if(beloc = -1)
	      beloc=y;
	    else if(belocOverflow = -1)
	      belocOverflow=y;
	    else
	      cerr<<" Error, Be overflow exceeded.  You should probably write a vector-based version of this..."<<endl;
	  }
	}
	TCutG *cut = (TCutG*)(cutlist->FindObject("alphacone_1_2"));
	
	if(aloc2!=-1 &&
	  abs(csm->GetHit(aloc1)->GetDPosition().Theta() - csm->GetHit(aloc2)->GetDPosition().Theta()) < be8thetawindow &&
	  cut->IsInside(csm->GetHit(aloc1)->GetEnergyMeV(),csm->GetHit(aloc2)->GetEnergyMeV()))
	{
	  csm->GetHit(aloc1)->SetIsotope(4,"He");
	  Alpha1Hit = csm->GetHit(aloc1);
	  Alpha1Flag = 1;
	  csm->GetHit(aloc2)->SetIsotope(4,"He");
	  Alpha2Hit = csm->GetHit(aloc2);
	  Alpha2Flag = 1;
	}
	
	else if(alocOverflow!=-1 &&
	  abs(csm->GetHit(aloc1)->GetDPosition().Theta() - csm->GetHit(alocOverflow)->GetDPosition().Theta()) < be8thetawindow &&
	  cut->IsInside(csm->GetHit(aloc1)->GetEnergyMeV(),csm->GetHit(alocOverflow)->GetEnergyMeV()))
	{
	  csm->GetHit(aloc1)->SetIsotope(4,"He");
	  Alpha1Hit = csm->GetHit(aloc1);
	  Alpha1Flag = 1;
	  csm->GetHit(alocOverflow)->SetIsotope(4,"He");
	  Alpha2Hit = csm->GetHit(alocOverflow);
	  Alpha2Flag = 1;
	}
	
	else if(aloc2!=-1 && alocOverflow!=-1 &&
	  abs(csm->GetHit(aloc2)->GetDPosition().Theta() - csm->GetHit(alocOverflow)->GetDPosition().Theta()) < be8thetawindow &&
	  cut->IsInside(csm->GetHit(aloc2)->GetEnergyMeV(),csm->GetHit(alocOverflow)->GetEnergyMeV()))
	{
	  csm->GetHit(aloc2)->SetIsotope(4,"He");
	  Alpha1Hit = csm->GetHit(aloc2);
	  Alpha1Flag = 1;
	  csm->GetHit(alocOverflow)->SetIsotope(4,"He");
	  Alpha2Hit = csm->GetHit(alocOverflow);
	  Alpha2Flag = 1;
	}
	
	if(Alpha2Flag && Alpha1Flag)
	{
	  double* corvals;
	  corvals = CorrParticleFromAlphas(Alpha1Hit,Alpha2Hit);
	  if(beloc!=-1 && abs(csm->GetHit(beloc)->GetPosition().Theta() - corvals[1]) < be12thetawindow && abs(csm->GetHit(beloc)->GetEnergy() - corvals[0]) < be12energywindow)
	  {
	    csm->GetHit(beloc)->SetIsotope("12Be");
	    Be12Hit = csm->GetHit(beloc);
	    Be12Flag = 1;
	  }
	  else if(belocOverflow!=-1 && abs(csm->GetHit(belocOverflow)->GetPosition().Theta() - corvals[1]) < be12thetawindow && abs(csm->GetHit(belocOverflow)->GetEnergy() - corvals[0]) < be12energywindow)
	  {
	    csm->GetHit(belocOverflow)->SetIsotope("12Be");
	    Be12Hit = csm->GetHit(belocOverflow);
	    Be12Flag = 1;
	  }
	}
      }
      else if(hits[0]==1&&hits[3]==2&&Ahits[0]==0)
      {
	((TH1I *)outlist->FindObject("StatCheck"))->Fill(10);
	
	for(int y=0; y<csm->GetMultiplicity(); y++)
	{
	  if(csm->GetHit(y)->GetDetectorNumber()==3+1 && csm->GetHit(y)->GetEnergy()>1 )
	  {
	    if(!Alpha1Flag)
	    {
	      csm->GetHit(y)->SetIsotope(4,"He");
	      Alpha1Hit = csm->GetHit(y);
	      Alpha1Flag = 1;
	    }
	    else if(!Alpha2Flag)
	    {
	      csm->GetHit(y)->SetIsotope(4,"He");
	      Alpha2Hit = csm->GetHit(y);
	      Alpha2Flag = 1;
	    }
	    else
	      cerr<<" Too many alphas"<<endl;
	  }
	}
      }
      else if(hits[1]==1&&hits[2]==2&&Ahits[1]==0)
      {
	((TH1I *)outlist->FindObject("StatCheck"))->Fill(11);
	
	for(int y=0; y<csm->GetMultiplicity(); y++)
	{
	  if(csm->GetHit(y)->GetDetectorNumber()==2+1 && csm->GetHit(y)->GetEnergy()>1 )
	  {
	    if(!Alpha1Flag)
	    {
	      csm->GetHit(y)->SetIsotope(4,"He");
	      Alpha1Hit = csm->GetHit(y);
	      Alpha1Flag = 1;
	    }
	    else if(!Alpha2Flag)
	    {
	      csm->GetHit(y)->SetIsotope(4,"He");
	      Alpha2Hit = csm->GetHit(y);
	      Alpha2Flag = 1;
	    }
	    else
	      cerr<<" Too many alphas"<<endl;
	  }
	}
      }

//     if(IsInteresting)
//       if(TCutG *cut = (TCutG*)(cutlist->FindObject("alphaCone_Interesting")))
//       {
// 	if(Alpha2Flag)
// 	{
// 	  if(cut->IsInside(Alpha1Hit->GetEnergyMeV(),Alpha2Hit->GetEnergyMeV()))
// 	  {
// 	    for(int iter=0;iter<csm->GetMultiplicity();iter++)
// 	    {
// 	      csm->GetHit(iter)->Print();
// 	    }
// 	    cout<<GREEN<<"*******"<<RESET_COLOR<<endl;
// 	  }
// 	}
//       }	
    }

    if(Alpha2Flag && Alpha1Flag)//This randomizes who is alpha1 and who is alpha 2
    {
      TRandom *rnd = new TRandom(x);
      if(rnd->Uniform(1)>.5)
      {
	TCSMHit *temp;
	temp = Alpha1Hit;
	Alpha1Hit = Alpha2Hit;
	Alpha2Hit = temp;
      }
    }

    if(Alpha1Flag && !Alpha2Flag)
    {
      TH2D* evtpointer = (TH2D*)outlist->FindObject(Form("EvTheta_%i_HE",Alpha1Hit->GetDetectorNumber()));
      evtpointer->Fill(Alpha1Hit->GetThetaDeg(),Alpha1Hit->GetEnergyMeV());
    }

    if(Alpha2Flag && Alpha1Flag)
    {
      TH2D* evtpointer = (TH2D*)outlist->FindObject(Form("EvTheta_%i_HE",Alpha1Hit->GetDetectorNumber()));
      evtpointer->Fill(Alpha1Hit->GetThetaDeg(),Alpha1Hit->GetEnergyMeV());
      TH2D* evtpointer2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_HE",Alpha2Hit->GetDetectorNumber()));
      evtpointer2->Fill(Alpha2Hit->GetThetaDeg(),Alpha2Hit->GetEnergyMeV());

      if(Alpha1Hit->GetDetectorNumber() == Alpha2Hit->GetDetectorNumber())
      {
	TH2D* alphaconepointer = (TH2D*)outlist->FindObject(Form("Alphacone_%i",Alpha1Hit->GetDetectorNumber()));
	alphaconepointer->Fill(Alpha1Hit->GetEnergyMeV(),Alpha2Hit->GetEnergyMeV());
	
	if(Alpha1Hit->GetDetectorNumber()<3)
	{
	  if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form("alphacone_%i_1",Alpha1Hit->GetDetectorNumber()))))
	  {
	    if(cut->IsInside(Alpha1Hit->GetEnergyMeV(),Alpha2Hit->GetEnergyMeV()))
	    {
	      double* Be8 = CalcBe8fromAlpha(Alpha1Hit, Alpha2Hit);
	      TH2D* be8pointer = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE8",Alpha1Hit->GetDetectorNumber()));
	      be8pointer->Fill(Be8[1]*180/3.14159,Be8[0]);
	      TH1D* alphaEXpointer = (TH1D*)outlist->FindObject(Form("AlphaEx%i",Alpha1Hit->GetDetectorNumber()));
	      alphaEXpointer->Fill(GetExciteE_Light(Alpha1Hit,Alpha2Hit));
	    }
	  }
	}
	else
	{
	  double* Be8 = CalcBe8fromAlpha(Alpha1Hit, Alpha2Hit);
	  
	  int sidetostack=-1;
	  if(Alpha1Hit->GetDetectorNumber()==3)
	    sidetostack=2;
	  else if(Alpha1Hit->GetDetectorNumber()==4)
	    sidetostack=1;
	  
	  TH2D* be8pointer = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE8",sidetostack));
	  be8pointer->Fill(Be8[1]*180/3.14159,Be8[0]);
	  TH1D* alphaEXpointer = (TH1D*)outlist->FindObject(Form("AlphaEx%i",Alpha1Hit->GetDetectorNumber()));
	  alphaEXpointer->Fill(GetExciteE_Light(Alpha1Hit,Alpha2Hit));
	}
      }
    }

    if(Be12Flag)
    {
      TH1D *temp1 = 0;
      TH2D *temp2 = 0;

      double excite = GetExciteE_Heavy(Be12Hit);
      temp1 = (TH1D*)outlist->FindObject(Form("BeEx%i",Be12Hit->GetDetectorNumber()));
      if(temp1) temp1->Fill(excite);
      temp1 = (TH1D*)outlist->FindObject(Form("BeEx%i_theta%02i",Be12Hit->GetDetectorNumber(),int(Be12Hit->GetThetaDeg())));
      if(temp1) temp1->Fill(excite);
      if(excite<3.2 && excite>1.9)
      {
	for(int y=0; y<tigress->GetAddBackMultiplicity();y++)
	{
	  TTigressHit *tigresshit = tigress->GetAddBackHit(y);

	  if(tigresshit->GetCore()->GetEnergy()>10)
	  {
	    temp1 = (TH1D*)outlist->FindObject(Form("GammaCut_%i",Be12Hit->GetDetectorNumber()));
	    temp1->Fill(tigresshit->GetCore()->GetEnergy()/1000.);

	    temp1 = (TH1D*)outlist->FindObject(Form("GammaCut_dopp_%i",Be12Hit->GetDetectorNumber()));
	    temp1->Fill(Doppler(tigresshit, Be12Hit));
	  }
	}
      }

      temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE_correlated",Be12Hit->GetDetectorNumber()));
      double *tmpValues = CorrParticle(Be12Hit);
      temp2->Fill(tmpValues[1]*180/3.14159,tmpValues[0]/1000.);

      temp1 = (TH1D*)outlist->FindObject(Form("BeEx%i_theta%02i_corr",Be12Hit->GetDetectorNumber(),int(Be12Hit->GetThetaDeg())));
      double excite2 = GetExciteE_Heavy(Be12Hit->GetCorrectedEnergy(),Be12Hit->GetDPosition().Theta());
      if(temp1) temp1->Fill(excite2);
      temp1 = (TH1D*)outlist->FindObject(Form("BeEx%i_corr",Be12Hit->GetDetectorNumber()));
      if(temp1) temp1->Fill(excite2);

      if(excite<3.8 && excite>1.8)
      {
	temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE_exCut_1e",Be12Hit->GetDetectorNumber()));
	if(temp2) temp2->Fill(Be12Hit->GetThetaDeg(),Be12Hit->GetEnergyMeV());
      }
      if(excite<1.3 && excite>0)
      {
	temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE_exCut_gs",Be12Hit->GetDetectorNumber()));
	if(temp2) temp2->Fill(Be12Hit->GetThetaDeg(),Be12Hit->GetEnergyMeV());
      }
      if(excite<5)
      {
	temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE_exCut_sc",Be12Hit->GetDetectorNumber()));
	if(temp2) temp2->Fill(Be12Hit->GetThetaDeg(),Be12Hit->GetEnergyMeV());
      }
      temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE",Be12Hit->GetDetectorNumber()));
      if(temp2) temp2->Fill(Be12Hit->GetThetaDeg(),Be12Hit->GetEnergyMeV());
      temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE_strip%02i",Be12Hit->GetDetectorNumber(),Be12Hit->GetDVerticalStrip()));
      if(temp2) temp2->Fill(Be12Hit->GetThetaDeg(),Be12Hit->GetEnergyMeV());
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
      1DV = 0
      1DH = 20
      1EV = 40
      1EH = 60
      2DV = 80
      2DH = 100
      2EV = 120
      2EH = 140
      3DV = 160
      3DH = 180
      4DV = 200
      4DH = 220
      */

      int offset = -1;
      if(hit->GetDetectorNumber()==1)
	offset=0;
      else if(hit->GetDetectorNumber()==2)
	offset=80;
      else if(hit->GetDetectorNumber()==3)
	offset=160;
      else if(hit->GetDetectorNumber()==4)
	offset=200;
      
      temp2 = (TH2D*)outlist->FindObject("EnergyCheck");
      if(hit->GetDVerticalEnergy()>10.)
	temp2->Fill(hit->GetDVerticalStrip()+offset,hit->GetDVerticalEnergy()/1000.);
      if(hit->GetDHorizontalEnergy()>10.)
	temp2->Fill(hit->GetDHorizontalStrip()+offset+20,hit->GetDHorizontalEnergy()/1000.);
      
      if(hit->GetDetectorNumber()<3)
      {
	if(hit->GetEVerticalEnergy()>10.)
	  temp2->Fill(hit->GetEVerticalStrip()+offset+40,hit->GetEVerticalEnergy()/1000.);
	if(hit->GetEHorizontalEnergy()>10.)
	  temp2->Fill(hit->GetEHorizontalStrip()+offset+60,hit->GetEHorizontalEnergy()/1000.);
      }
      
      temp2 = (TH2D*)outlist->FindObject("ChargeCheck");
      if(hit->GetDVerticalCharge() != 0)
	temp2->Fill(hit->GetDVerticalStrip()+offset,hit->GetDVerticalCharge()/125.);
      if(hit->GetDHorizontalCharge() != 0)
	temp2->Fill(hit->GetDHorizontalStrip()+offset+20,hit->GetDHorizontalCharge()/125.);
      
      if(hit->GetDetectorNumber()<3)
      {
	if(hit->GetEVerticalCharge() != 0)
	  temp2->Fill(hit->GetEVerticalStrip()+offset+40,hit->GetEVerticalCharge()/125.);
	if(hit->GetEHorizontalCharge() != 0)
	  temp2->Fill(hit->GetEHorizontalStrip()+offset+60,hit->GetEHorizontalCharge()/125.);
      }

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
	temp2->Fill(hit->GetEPosition().Theta()*180/TMath::Pi(),hit->GetEPosition().Phi()*180/TMath::Pi());
      }

      if(DEBUG) cout<<"EVTheta"<<endl;
      temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%iD",hit->GetDetectorNumber()));
      temp2->Fill(hit->GetThetaDeg(),hit->GetDEnergy()/1000.);

      temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%iE",hit->GetDetectorNumber()));
      if(temp2) temp2->Fill(hit->GetEPosition().Theta()*180/TMath::Pi(),hit->GetEEnergy()/1000.);
      if(DEBUG) cout<<"HitPattern"<<endl;

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
	  temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_summed_thickness",hit->GetDetectorNumber()));
	  temp2->Fill(hit->GetEnergyMeV(),hit->GetDdE_dx());

	  temp2 = (TH2D*)outlist->FindObject(Form("stripPID_det%i_strip%02i",hit->GetDetectorNumber(),hit->GetDVerticalStrip()));
	  temp2->Fill(hit->GetEnergyMeV(),hit->GetDdE_dx());
	
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
//        Gammas
//***********************
    for(int y=0; y<tigress->GetAddBackMultiplicity();y++)
    {
      TTigressHit *hit = tigress->GetAddBackHit(y);
      TH1D* temp = 0;

      temp = (TH1D*)outlist->FindObject("GammaSum");
      temp->Fill(hit->GetCore()->GetEnergy()/1000.);
    }
//***********************
//         End
//***********************

////////////////////////////////////////////////////////////////////////

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

  TApplication *app = new TApplication("app",0,0);
  TFile cf("cuts.root");
  TIter *iter = new TIter(cf.GetListOfKeys());

  while(TObject *obj = iter->Next())
  {
    obj = ((TKey *)obj)->ReadObj();

    //printf("obj->ClassName() = %s\n", obj->ClassName());
    if(strcmp(obj->ClassName(),"TCutG")!=0)
    {
      continue;
    }

    cutlist->Add(obj);
    printf("found a cut! %s \n",((TNamed *)obj)->GetName());
  }

  TFile cf2("thetacuts.root");
  TIter *iter2 = new TIter(cf2.GetListOfKeys());

  while(TObject *obj = iter2->Next())
  {
    obj = ((TKey *)obj)->ReadObj();

    //printf("obj->ClassName() = %s\n", obj->ClassName());
    if(strcmp(obj->ClassName(),"TCutG")!=0)
    {
      continue;
    }

    cutlist->Add(obj);
    printf("found a cut! %s \n",((TNamed *)obj)->GetName());
  }
  
  TChain *chain = new TChain("AnalysisTree");
  int i =1;

  while(i<argc)
  {
    chain->Add(argv[i++]);
  }


  printf("%i analysis trees added to chain.\n",i-1);
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

  TFile f("output.root","recreate");
  f.cd();
  outlist->Write();
  f.Close();
  
  if(DEBUG)
  {
    cout<<"All done"<<endl;
  }

  return 0;
}





