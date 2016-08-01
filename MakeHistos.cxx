// g++ MakeHistos.cxx FunctionsForMakeHistos.cxx -Wl,--no-as-needed -o RunMe `grsi-config --cflags --all-libs --root`

#include "FunctionsForMakeHistos.hh"

double BEAM_ENERGY;

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

  TString Be12Cut;
  TString Be11Cut;
  TString Be10Cut;
  TString Be9Cut;
  
  if(int(BEAM_ENERGY) == 55)
  {
    Be12Cut = "pid_high_thick_12Be_%i_v3";
    Be11Cut = "pid_high_thick_11Be_%i_v2";//v1 is elastic only, v2 is everything
    Be10Cut = "pid_high_thick_10Be_%i_v1";
    Be9Cut = "pid_high_thick_9Be_%i_v1";
  }
  else if(int(BEAM_ENERGY) == 30)
  {
    Be12Cut = "pid_low_thick_12Be_%i_v2";
    Be11Cut = "pid_low_thick_11Be_%i_v2";//v1 is elastic only, v2 is everything
    Be10Cut = "pid_low_thick_10Be_%i_v2";
    Be9Cut = "pid_low_thick_9Be_%i_v1";
  }
  else
  {
    cerr<<"Something is wrong with setting cuts.  Beam energy unrecognized, reverting to high energy"<<endl;
    Be12Cut = "pid_high_thick_12Be_%i_v3";
    Be11Cut = "pid_high_thick_11Be_%i_v2";//v1 is elastic only, v2 is everything
    Be10Cut = "pid_high_thick_10Be_%i_v1";
    Be9Cut = "pid_high_thick_9Be_%i_v1";
  }

  
  
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

    /*TCSMHit *Be11Hit;
    TCSMHit *Alpha1Hit;
    TCSMHit *Alpha2Hit;
    TCSMHit *Be12Hit;

    bool Be11Flag = 0;
    bool Alpha1Flag = 0;
    bool Alpha2Flag = 0;
    bool Be12Flag = 0;

    if(csm->GetMultiplicity()==3)
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
	if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form(Be12Cut,csm->GetHit(y)->GetDetectorNumber()))))
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
*/
      int hits[4] = {0};
      for(int y=0; y<csm->GetMultiplicity(); y++)
      {
	hits[csm->GetHit(y)->GetDetectorNumber()-1]++;
      }

      if(hits[0]==2 && hits[1]==2)
      {
	continue;
      }

      else
      {
	double TotalEMeV = 0.;
	for(int eiter = 0; eiter<csm->GetMultiplicity();eiter++)
	{
	  TotalEMeV += csm->GetHit(eiter)->GetEnergyMeV();
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
		{
		  cerr<<"Too many correlated location hits"<<endl;
		  cout<<"  "<<hits[0]<<" "<<hits[1]<<endl;
		  cout<<" "<<hits[2]<<"   "<<hits[3]<<endl;
		}
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
	      diffpointer->Fill(csm->GetHit(loc1)->GetDPosition().Angle(csm->GetHit(loc2)->GetDPosition())*180/3.14159,csm->GetHit(loc1)->GetEnergyMeV()+csm->GetHit(loc2)->GetEnergyMeV());

	      if(csm->GetHit(loc1)->GetThetaDeg()==csm->GetHit(loc2)->GetThetaDeg())
	      {
		cout<<csm->GetMultiplicity()<<" "<<loc1<<" "<<loc2<<endl;
		csm->GetHit(loc1)->Print();
		csm->GetHit(loc2)->Print();
		cout<<endl;
	      }
	      
	      TString cut1name;
	      if(int(BEAM_ENERGY) == 55)
		cut1name = "sim_cone_high";
	      else if(int(BEAM_ENERGY) == 30)
		cut1name = "sim_cone";
	      
	      if(TCutG *cut = (TCutG*)(cutlist->FindObject(cut1name)))
	      {
		if(cut->IsInside(csm->GetHit(loc1)->GetEnergyMeV(),csm->GetHit(loc2)->GetEnergyMeV()))
		{
		  TH2D* diffpointer2 =(TH2D*)outlist->FindObject(Form("twohit_%i_thetadiff_onecut",det+1));
		  diffpointer2->Fill(csm->GetHit(loc1)->GetDPosition().Angle(csm->GetHit(loc2)->GetDPosition())*180/3.14159,csm->GetHit(loc1)->GetEnergyMeV()+csm->GetHit(loc2)->GetEnergyMeV());

		  TString cut2name;
		  if(int(BEAM_ENERGY) == 55)
		    cut2name = "sim_angle_high";
		  else if(int(BEAM_ENERGY) == 30)
		    cut2name = "sim_angle";
		  
		  if(TCutG *cut2 = (TCutG*)(cutlist->FindObject(cut2name)))
		  {
		    if(cut2->IsInside(csm->GetHit(loc1)->GetDPosition().Angle(csm->GetHit(loc2)->GetDPosition())*180/3.14159,
		      csm->GetHit(loc1)->GetEnergyMeV()+csm->GetHit(loc2)->GetEnergyMeV()))
		    {
		      TH2D* twocutpointer = (TH2D*)outlist->FindObject(Form("twohit_twocut_%i",csm->GetHit(loc1)->GetDetectorNumber()));
		      twocutpointer->Fill( csm->GetHit(loc1)->GetThetaDeg(), csm->GetHit(loc1)->GetEnergyMeV());
		      twocutpointer->Fill( csm->GetHit(loc2)->GetThetaDeg(), csm->GetHit(loc2)->GetEnergyMeV());

		      TH2D* twocone = (TH2D*)outlist->FindObject(Form("twohit_twocut_cone_%i",csm->GetHit(loc1)->GetDetectorNumber()));
		      twocone->Fill(csm->GetHit(loc1)->GetEnergyMeV(),csm->GetHit(loc2)->GetEnergyMeV());

		      double* Be8 = CalcBe8fromAlpha(csm->GetHit(loc1), csm->GetHit(loc2));
		      TH2D* be8pointer = (TH2D*)outlist->FindObject(Form("twohit_twocut_%i_Be8",csm->GetHit(loc1)->GetDetectorNumber()));
		      be8pointer->Fill(Be8[1]*180/3.14159,Be8[0]);

		      if(corrloc != -1)
		      {
			TH2D* corrpointer = (TH2D*)outlist->FindObject(Form("twohit_twocut_%i_corr",csm->GetHit(loc1)->GetDetectorNumber()));
			corrpointer->Fill(csm->GetHit(corrloc)->GetThetaDeg(),csm->GetHit(corrloc)->GetCorrectedEnergyMeV("12be"));

			TH1D* etotpointer = (TH1D*)outlist->FindObject(Form("twohit_twocut_%i_corr_Etot",csm->GetHit(loc1)->GetDetectorNumber()));
			etotpointer->Fill(csm->GetHit(corrloc)->GetEnergyMeV()+csm->GetHit(loc1)->GetEnergyMeV()+csm->GetHit(loc2)->GetEnergyMeV());

			if(csm->GetHit(corrloc)->GetEEnergy() > 1)
			{
			  bool lock = false;
			  TH2D* pidpointer = (TH2D*)outlist->FindObject(Form("twohit_twocut_%i_corr_PID",csm->GetHit(loc1)->GetDetectorNumber()));
			pidpointer->Fill(csm->GetHit(corrloc)->GetEnergyMeV(),csm->GetHit(corrloc)->GetDdE_dx());

			if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form("twohit_PID_%i_v1",csm->GetHit(loc1)->GetDetectorNumber()))))
			{
			  if(cut->IsInside(csm->GetHit(corrloc)->GetEnergyMeV(),csm->GetHit(corrloc)->GetDdE_dx()))
			  {
			    TH2D* evtpointercut = (TH2D*)outlist->FindObject(Form("twohit_twocut_%i_corr_cut",csm->GetHit(loc1)->GetDetectorNumber()));
			    evtpointercut->Fill( csm->GetHit(corrloc)->GetThetaDeg(), csm->GetHit(corrloc)->GetEnergyMeV());
			  }
			}
			}
		      }
		    }
		  }
		}
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
      }

      /*
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
    }*/

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
	if(hit->GetDEnergy()>1)
	  temp2->Fill(hit->GetDPosition().Theta()*180/TMath::Pi(),hit->GetDPosition().Phi()*180/TMath::Pi());
	if(hit->GetEEnergy()>1)
	  temp2->Fill(hit->GetEPosition().Theta()*180/TMath::Pi(),hit->GetEPosition().Phi()*180/TMath::Pi());
      }
//////////////////////////////////////////////////
      if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form(Be10Cut,hit->GetDetectorNumber()))))
      {
	if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()) && hit->GetEEnergy() > 10)
	{
	  temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE10",hit->GetDetectorNumber()));
	  temp2->Fill(hit->GetThetaDeg(),hit->GetEnergyMeV());

	  temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE10_corrected",hit->GetDetectorNumber()));
	  temp2->Fill(hit->GetThetaDeg(),hit->GetCorrectedEnergyMeV("10Be"));

	  TH1I* correctionpointer = (TH1I*)outlist->FindObject(Form("correction_straggling_%i",hit->GetDetectorNumber()));
	  correctionpointer->Fill(hit->GetCorrectedEnergyMeVTarget(hit->GetCorrectedEnergyMeVDetector("10Be"),"10Be") - hit->GetEnergyMeV());
	  correctionpointer = (TH1I*)outlist->FindObject(Form("correction_deadlayer_%i",hit->GetDetectorNumber()));
	  correctionpointer->Fill(hit->GetCorrectedEnergyMeVDetector("10Be") - hit->GetEnergyMeV());
	  
	  TH1I* multpointer = (TH1I*)outlist->FindObject(Form("Be10Mult_%i",hit->GetDetectorNumber()));
	  multpointer->Fill(csm->GetMultiplicity());

	  double excite = GetExciteE_Heavy(hit,10);
	  temp1 = (TH1D*)outlist->FindObject(Form("Be10Ex%i",hit->GetDetectorNumber()));
	  if(temp1) temp1->Fill(excite);

	  for(int y=0; y<tigress->GetAddBackMultiplicity();y++)
	  {
	    TTigressHit *tigresshit = tigress->GetAddBackHit(y);

	    if(tigresshit->GetCore()->GetEnergy()>10)
	    {
	      double dopp = Doppler(tigresshit,hit,10);
	      
	      if(dopp>=2.577 && dopp<=2.612)
	      {
		TH1D* expg = (TH1D*)outlist->FindObject(Form("Be10Ex%i_gcut_2589",hit->GetDetectorNumber()));
		expg->Fill(excite);
	      }
	      else if(dopp>=2.876 && dopp<=2.913)
	      {
		TH1D* expg = (TH1D*)outlist->FindObject(Form("Be10Ex%i_gcut_2894",hit->GetDetectorNumber()));
		expg->Fill(excite);
	      }
	      else if(dopp>=3.337 && dopp<=3.402)
	      {
		TH1D* expg = (TH1D*)outlist->FindObject(Form("Be10Ex%i_gcut_3368",hit->GetDetectorNumber()));
		expg->Fill(excite);
	      }
	      else if(dopp>=5.951 && dopp<=5.986)
	      {
		TH1D* expg = (TH1D*)outlist->FindObject(Form("Be10Ex%i_gcut_5958",hit->GetDetectorNumber()));
		expg->Fill(excite);
	      }
	    }
	  }

	  if(excite>-.5 && excite<1.5)
	  {
	    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE10_exCut_gs",hit->GetDetectorNumber()));
	    if(temp2) temp2->Fill(hit->GetThetaDeg(),hit->GetEnergyMeV());

	    TVector3 COMinfo = CalcCOMmomentum(hit, 10);
	    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE10_exCut_gs_COM",hit->GetDetectorNumber()));
	    if(temp2) temp2->Fill(COMinfo.Theta()*180./TMath::Pi(),COMinfo.Mag2()/(2.*MASS_BE10));
	    
	    temp2 = (TH2D*)outlist->FindObject(Form("Be10_TvTCOM_%i",hit->GetDetectorNumber()));
	    temp2->Fill(COMinfo.Theta()*180./TMath::Pi(),hit->GetThetaDeg());
	  }

	  temp1 = (TH1D*)outlist->FindObject(Form("Be10Ex%i_corr",hit->GetDetectorNumber()));
	  if(temp1) temp1->Fill(GetExciteE_Heavy_Corrected(hit,10));

	  temp2INT = (TH2I*)outlist->FindObject(Form("Be10Ex%i_mult",hit->GetDetectorNumber()));
	  temp2INT->Fill(GetExciteE_Heavy_Corrected(hit,10),csm->GetMultiplicity());

	  for(int y=0; y<tigress->GetAddBackMultiplicity();y++)
	  {
	    TTigressHit *tigresshit = tigress->GetAddBackHit(y);
	    
	    if(tigresshit->GetCore()->GetEnergy()>10)
	    {
	      temp1 = (TH1D*)outlist->FindObject(Form("Be10_Gamma_%i",hit->GetDetectorNumber()));
	      temp1->Fill(tigresshit->GetCore()->GetEnergy()/1000.);
	      temp1 = (TH1D*)outlist->FindObject(Form("Be10_Gamma_%i_dopp",hit->GetDetectorNumber()));
	      temp1->Fill(Doppler(tigresshit,hit,10));
	    }
	  }
	}
      }
//////////////////////////////////////////////////
      if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form(Be11Cut,hit->GetDetectorNumber()))))
      {
	if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()) && hit->GetEEnergy() > 10)
	{
	  temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_%i_Be11",hit->GetDetectorNumber()));
	  temp2->Fill(hit->GetThetaDeg(),hit->GetEnergyMeV());

	  temp1 = (TH1D*)outlist->FindObject(Form("Be11Ex%i",hit->GetDetectorNumber()));
	  if(temp1) temp1->Fill(GetExciteE_Heavy(hit,11));

	  temp1 = (TH1D*)outlist->FindObject(Form("Be11Ex%i_corr",hit->GetDetectorNumber()));
	  if(temp1) temp1->Fill(GetExciteE_Heavy_Corrected(hit,11));

	  temp2INT = (TH2I*)outlist->FindObject(Form("Be11Ex%i_mult",hit->GetDetectorNumber()));
	  temp2INT->Fill(GetExciteE_Heavy_Corrected(hit,11),csm->GetMultiplicity());
	}
      }
//////////////////////////////////////////////////
      
      if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form(Be12Cut,hit->GetDetectorNumber()))))
      {
	if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()) && hit->GetEEnergy() > 10)
	{
	  temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_12Be_%i",hit->GetDetectorNumber()));
	  temp2->Fill(hit->GetThetaDeg(),hit->GetEnergyMeV());

	  temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_12Be_%i_corrected",hit->GetDetectorNumber()));
	  temp2->Fill(hit->GetThetaDeg(),hit->GetCorrectedEnergyMeV("12Be"));

	  temp2 = (TH2D*)outlist->FindObject("EvTheta_12Be");
	  temp2->Fill(hit->GetThetaDeg(),hit->GetEnergyMeV());

	  if(csm->GetMultiplicity()==1)
	  {
	    temp2 = (TH2D*)outlist->FindObject(Form("EvTheta_12Be_%i_mult1",hit->GetDetectorNumber()));
	    temp2->Fill(hit->GetThetaDeg(),hit->GetEnergyMeV());
	  }

	  TH1I* multpointer = (TH1I*)outlist->FindObject(Form("Be12Mult_%i",hit->GetDetectorNumber()));
	  multpointer->Fill(csm->GetMultiplicity());

	  double excite = GetExciteE_Heavy(hit,12);
	  temp1 = (TH1D*)outlist->FindObject(Form("BeEx%i",hit->GetDetectorNumber()));
	  if(temp1) temp1->Fill(excite);

	  temp1 = (TH1D*)outlist->FindObject(Form("BeEx%i_corr",hit->GetDetectorNumber()));
	  if(temp1) temp1->Fill(GetExciteE_Heavy_Corrected(hit,12));

	  temp2INT = (TH2I*)outlist->FindObject(Form("BeEx%i_mult",hit->GetDetectorNumber()));
	  temp2INT->Fill(GetExciteE_Heavy_Corrected(hit,12),csm->GetMultiplicity());

// 	  TH1I* stat = (TH1I*)outlist->FindObject("counts");
// 	  stat->Fill(hit->GetDetectorNumber()+1);

	  for(int y=0; y<tigress->GetAddBackMultiplicity();y++)
	  {
	    TTigressHit *tigresshit = tigress->GetAddBackHit(y);
	    
	    if(tigresshit->GetCore()->GetEnergy()>10)
	    {
	      temp1 = (TH1D*)outlist->FindObject(Form("Be12_Gamma_%i",hit->GetDetectorNumber()));
	      temp1->Fill(tigresshit->GetCore()->GetEnergy()/1000.);
	      temp1 = (TH1D*)outlist->FindObject("Be12Gammas");
	      temp1->Fill(tigresshit->GetCore()->GetEnergy()/1000.);

	      if(hit->GetDVerticalCFD()-tigresshit->GetTimeCFD() > -140)
	      {
		temp1 = (TH1D*)outlist->FindObject("Be12Gammas_prompt");
		temp1->Fill(tigresshit->GetCore()->GetEnergy()/1000.);
		temp1 = (TH1D*)outlist->FindObject("Be12Gammas_prompt_doppler");
		temp1->Fill(Doppler(tigresshit,hit,12));
	      }

	      else if(hit->GetDVerticalCFD()-tigresshit->GetTimeCFD() < -150 && hit->GetDVerticalCFD()-tigresshit->GetTimeCFD() > -400 )
	      {
		temp1 = (TH1D*)outlist->FindObject("Be12Gammas_medium");
		temp1->Fill(tigresshit->GetCore()->GetEnergy()/1000.);
	      }

	      else if(hit->GetDVerticalCFD()-tigresshit->GetTimeCFD() < -500)
	      {
		temp1 = (TH1D*)outlist->FindObject("Be12Gammas_delayed");
		temp1->Fill(tigresshit->GetCore()->GetEnergy()/1000.);
	      }

	      double doppE = Doppler(tigresshit,hit,12);
	      
	      temp1 = (TH1D*)outlist->FindObject(Form("Be12_Gamma_%i_dopp",hit->GetDetectorNumber()));
	      temp1->Fill(doppE);
	      temp1 = (TH1D*)outlist->FindObject("Be12GammasDopp");
	      temp1->Fill(doppE);

	      if( (doppE > 2.09 && doppE < 2.14) ||
		(doppE > 2.70 && doppE < 2.76))
	      {
		TH2D* evtp = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE_gammaCut_dopp",hit->GetDetectorNumber()));
		evtp->Fill(hit->GetThetaDeg(),hit->GetEnergyMeV());

		TH2D* tp = (TH2D*)outlist->FindObject(Form("pid_%i_summed_thickness_gcut_dopp",hit->GetDetectorNumber()));
		tp->Fill(hit->GetEnergyMeV(),hit->GetDdE_dx());
	      }

	      
	      if(tigresshit->GetCore()->GetEnergy()>2100 && tigresshit->GetCore()->GetEnergy()<2140)
	      {
		TH1D* delta = (TH1D*)outlist->FindObject("deltaGamma");
		delta->Fill(hit->GetDVerticalCFD()-tigresshit->GetTimeCFD());
	      }
	      if(tigresshit->GetCore()->GetEnergy()>2100 && tigresshit->GetCore()->GetEnergy()<2150)
	      {
		TH2D* evtp = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE_gammaCut",hit->GetDetectorNumber()));
		evtp->Fill(hit->GetThetaDeg(),hit->GetEnergyMeV());
	      }
	    }
	  }
	}
      }
      
      if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form(Be12Cut,hit->GetDetectorNumber()))))
      {
	if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()) )//&& csm->GetMultiplicity()>=3)
	{
	  if(csm->GetMultiplicity()>=3)
	  {
	    TH1D* totalenergypointer=(TH1D*)outlist->FindObject(Form("TotalEnergy_%i_12Be",hit->GetDetectorNumber()));
	    double TotalE = 0;
	    for(int asdf=0;asdf<csm->GetMultiplicity();asdf++)
	    {
	      TotalE+=csm->GetHit(asdf)->GetEnergyMeV();
	    }
	    totalenergypointer->Fill(TotalE);
	  }
	}
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

	  //temp2 = (TH2D*)outlist->FindObject(Form("stripPID_det%i_strip%02i",hit->GetDetectorNumber(),hit->GetDVerticalStrip()));
	  //temp2->Fill(hit->GetEnergyMeV(),hit->GetDdE_dx());
	
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

	temp2 = (TH2D*)outlist->FindObject(Form("pid_%i_summed_thickness_mult%i",hit->GetDetectorNumber(),csm->GetMultiplicity()));
	temp2->Fill(hit->GetEnergyMeV(),hit->GetDdE_dx());
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
//    Gamma Plots
//***********************
    for(int I=0;I<csm->GetMultiplicity();I++)
    {
      TCSMHit *hit = csm->GetHit(I);
      if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form("pid_high_thick_allbe_%i_v1",hit->GetDetectorNumber()))))
      {
	if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()) && hit->GetEEnergy() > 10)
	{
	  for(int y=0; y<tigress->GetAddBackMultiplicity();y++)
	  {
	    TTigressHit *tigresshit = tigress->GetAddBackHit(y);

	    if(tigresshit->GetCore()->GetEnergy()>10)
	    {
	      TH1D* gpointer = (TH1D*)outlist->FindObject("GammaBe");
	      gpointer->Fill(tigresshit->GetCore()->GetEnergy()/1000.);
	    }
	  }
	}
      }

      if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form("pid_high_thick_allli_%i_v1",hit->GetDetectorNumber()))))
      {
	if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()) && hit->GetEEnergy() > 10)
	{
	  for(int y=0; y<tigress->GetAddBackMultiplicity();y++)
	  {
	    TTigressHit *tigresshit = tigress->GetAddBackHit(y);

	    if(tigresshit->GetCore()->GetEnergy()>10)
	    {
	      TH1D* gpointer = (TH1D*)outlist->FindObject("GammaLi");
	      gpointer->Fill(tigresshit->GetCore()->GetEnergy()/1000.);
	    }
	  }
	}
      }
      if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form("pid_high_thick_7Li_%i_v1",hit->GetDetectorNumber()))))
      {
	if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()) && hit->GetEEnergy() > 10)
	{
	  for(int y=0; y<tigress->GetAddBackMultiplicity();y++)
	  {
	    TTigressHit *tigresshit = tigress->GetAddBackHit(y);
	    
	    if(tigresshit->GetCore()->GetEnergy()>10)
	    {
	      TH1D* gpointer = (TH1D*)outlist->FindObject("GammaLi7");
	      gpointer->Fill(tigresshit->GetCore()->GetEnergy()/1000.);
	    }
	  }
	}
      }
      if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form("pid_high_alpha_thickness_%i_v1",hit->GetDetectorNumber()))))
      {
	if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()) && hit->GetEEnergy() > 10)
	{
	  for(int y=0; y<tigress->GetAddBackMultiplicity();y++)
	  {
	    TTigressHit *tigresshit = tigress->GetAddBackHit(y);
	    
	    if(tigresshit->GetCore()->GetEnergy()>10)
	    {
	      TH1D* gpointer = (TH1D*)outlist->FindObject("GammaAlpha");
	      gpointer->Fill(tigresshit->GetCore()->GetEnergy()/1000.);
	    }
	  }
	}
      }
    }
//***********************
//looking for non id 12Be
//***********************

    //if(csm->GetMultiplicity()>=2)
    {
      for(int i=0;i<csm->GetMultiplicity();i++)
      {
	TCSMHit *hit = csm->GetHit(i);
	if(hit->GetDetectorNumber()<3 && hit->GetEEnergy() < 10)
	{
	  for(int y=0; y<tigress->GetAddBackMultiplicity();y++)
	  {
	    TTigressHit *tigresshit = tigress->GetAddBackHit(y);

	    if(tigresshit->GetCore()->GetEnergy()>10)
	    {
	      TH1D* gpointer = (TH1D*)outlist->FindObject("Be12GammasNoID");
	      gpointer->Fill(tigresshit->GetCore()->GetEnergy()/1000.);
	      gpointer = (TH1D*)outlist->FindObject("Be12GammasDoppNoID");
	      gpointer->Fill(Doppler(tigresshit,hit,12));

	      if(tigresshit->GetCore()->GetEnergy()/1000. < 2.12 || tigresshit->GetCore()->GetEnergy()/1000. > 2.13)
	      {
		gpointer = (TH1D*)outlist->FindObject("Be12GammasDoppNoID_suppressed");
		gpointer->Fill(Doppler(tigresshit,hit,12));
	      }
	    }
	    if(tigresshit->GetCore()->GetEnergy()>2100 && tigresshit->GetCore()->GetEnergy()<2150)
	    {
	      TH2D* evtp = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE_gammaCutNoID",hit->GetDetectorNumber()));
	      evtp->Fill(hit->GetThetaDeg(),hit->GetEnergyMeV());
	    }
	  }
	  break;
	}
      }
    }
    
//***********************
//  looking for 2 alphas
//***********************

    if(csm->GetMultiplicity()>=3)
    {
      vector<int> aloc;
      for(int i=0;i<csm->GetMultiplicity();i++)
      {
	if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form("pid_high_alpha_thickness_%i_v1",csm->GetHit(i)->GetDetectorNumber()))))
	{
	  if(cut->IsInside(csm->GetHit(i)->GetEnergyMeV(),csm->GetHit(i)->GetDdE_dx()) && csm->GetHit(i)->GetEEnergy() > 10)
	  {
	    aloc.push_back(i);
	  }
	}
      }

      if(aloc.size()>0)
      {
	TH1D* etotalphapointer = (TH1D*)outlist->FindObject(Form("ETot_%i_2alpha",csm->GetHit(aloc.at(0))->GetDetectorNumber()));
	double etot=0;
	for(int ee = 0; ee<csm->GetMultiplicity(); ee++)
	{
	  etot += csm->GetHit(ee)->GetEnergyMeV();
	}
	etotalphapointer->Fill(etot);

	TH1I* numalphaptr = (TH1I*)outlist->FindObject("AlphaMult");
	numalphaptr->Fill(aloc.size());
      }
    }
//***********************
//      Fancy dE v E
//***********************

    for(int xx = 0;xx<csm->GetMultiplicity();xx++)
    {
      TCSMHit *hit = csm->GetHit(xx);
      if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form("highblob_%i_v1",hit->GetDetectorNumber()))))
      {
	if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()) && hit->GetEEnergy() > 10)
	{
	  TH1I* blobpointer = (TH1I*)outlist->FindObject("MultBlobHigh");
	  blobpointer->Fill(csm->GetMultiplicity());
	}
      }
      if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form("lowblob_%i_v1",hit->GetDetectorNumber()))))
      {
	if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()) && hit->GetEEnergy() > 10)
	{
	  TH1I* blobpointer = (TH1I*)outlist->FindObject("MultBlobLow");
	  blobpointer->Fill(csm->GetMultiplicity());
	}
      }
    }

    if(csm->GetMultiplicity()>=2)
    {
      for(int aa=0;aa<csm->GetMultiplicity();aa++)
      {
	TCSMHit *hita = csm->GetHit(aa);
	if(hita->GetEEnergy()>0.)
	{
	  for(int bb=aa+1;bb<csm->GetMultiplicity();bb++)
	  {
	    TCSMHit *hitb = csm->GetHit(bb);
	    if(hitb->GetEEnergy()>0.)
	    {
	      if(hita->GetDetectorNumber() == hitb->GetDetectorNumber())
	      {
		TH2D *hit2histo = (TH2D*)outlist->FindObject(Form("pid_%i_summed_thickness_2hit",hita->GetDetectorNumber()));
		hit2histo->Fill(hita->GetEnergyMeV(),hita->GetDdE_dx());
		hit2histo->Fill(hitb->GetEnergyMeV(),hitb->GetDdE_dx());
		if(hita->GetDVerticalStrip()==hitb->GetDVerticalStrip() && hita->GetDHorizontalStrip() == hitb->GetDHorizontalStrip())
		{
		  TH2D *hit2histo = (TH2D*)outlist->FindObject(Form("pid_%i_summed_thickness_2hit_samepix",hita->GetDetectorNumber()));
		  hit2histo->Fill(hita->GetEnergyMeV(),hita->GetDdE_dx());
		  hit2histo->Fill(hitb->GetEnergyMeV(),hitb->GetDdE_dx());
		}
	      }
	    }
	  }
	  if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form(Be12Cut,hita->GetDetectorNumber()))))
	  {
	    if(cut->IsInside(hita->GetEnergyMeV(),hita->GetDdE_dx()) && hita->GetEEnergy() > 10)
	    {
	      //we have a confirmed 12Be
	      for(int bb=0;bb<csm->GetMultiplicity();bb++)
	      {
		TCSMHit *hitb = csm->GetHit(bb);
		if(hitb==hita)
		  continue;
		
		if(hitb->GetEEnergy()>10. && hitb->GetDEnergy() > 10. && hita->GetDetectorNumber() != hitb->GetDetectorNumber())
		{
		  TH2D *corrpointer = (TH2D*)outlist->FindObject(Form("pid_%i_summed_thickness_corr12",hita->GetDetectorNumber()));
		  corrpointer->Fill(hitb->GetEnergyMeV(),hitb->GetDdE_dx());
		}
	      }
	    }
	  }
	}
      }
    }

    //***********************
    //      isotope plot
    //***********************
    for(int xx = 0;xx<csm->GetMultiplicity();xx++)
    {
      TCSMHit *hit = csm->GetHit(xx);

      TH1I *idptr = (TH1I*)outlist->FindObject(Form("NBe_%i",hit->GetDetectorNumber()));
      
      
      if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form(Be12Cut,hit->GetDetectorNumber()))))
      {
	if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()) && hit->GetEEnergy() > 10)
	{
	  idptr->Fill(12);
	}
      }
      
      if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form(Be10Cut,hit->GetDetectorNumber()))))
      {
	if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()) && hit->GetEEnergy() > 10)
	{
	  idptr->Fill(10);
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

    if(tigress->GetAddBackMultiplicity()>1)
    {
      for(int y=1; y<tigress->GetAddBackMultiplicity();y++)
      {
	TTigressHit *tigresshit = tigress->GetAddBackHit(y);

	TH1I* difp = (TH1I*)outlist->FindObject("GvGTimeDiff");
	difp->Fill(tigress->GetAddBackHit(0)->GetTimeCFD()-tigresshit->GetTimeCFD());
      }
    }

    for(int y=0; y<csm->GetMultiplicity();y++)
    {
      TCSMHit* hit = csm->GetHit(y);
      for(int y=0; y<tigress->GetAddBackMultiplicity();y++)
      {
	TTigressHit *tigresshit = tigress->GetAddBackHit(y);

	TH1I* difp = (TH1I*)outlist->FindObject("TimeDiffAll");
	difp->Fill(hit->GetDVerticalCFD()-tigresshit->GetTimeCFD());
	TH2D* point = (TH2D*)outlist->FindObject("GEvT");
	point->Fill(hit->GetDVerticalCFD()-tigresshit->GetTimeCFD(),tigresshit->GetCore()->GetEnergy()/1000.);
      }

      if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form(Be12Cut,hit->GetDetectorNumber()))))
      {
	if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()) && hit->GetEEnergy() > 10)
	{
	  for(int y=0; y<tigress->GetAddBackMultiplicity();y++)
	  {
	    TTigressHit *tigresshit = tigress->GetAddBackHit(y);
	    
	    TH1I* difp12 = (TH1I*)outlist->FindObject("TimeDiff12Be");
	    difp12->Fill(hit->GetDVerticalCFD()-tigresshit->GetTimeCFD());
	    TH2D* point12 = (TH2D*)outlist->FindObject("GEvT_12Be");
	    point12->Fill(hit->GetDVerticalCFD()-tigresshit->GetTimeCFD(),tigresshit->GetCore()->GetEnergy()/1000.);

	    TH2D* matp = (TH2D*)outlist->FindObject("GammaMatrix_12Be");
	    for(int zz = y+1; zz<tigress->GetAddBackMultiplicity();zz++)
	    {
	      matp->Fill(tigresshit->GetCore()->GetEnergy()/1000.,tigress->GetAddBackHit(zz)->GetCore()->GetEnergy()/1000.);
	    }
	  }
	}
      }
    }

    for(int y=0; y<tigress->GetAddBackMultiplicity();y++)
    {
      TTigressHit *tigresshit = tigress->GetAddBackHit(y);
      TH2D* matp = (TH2D*)outlist->FindObject("GammaMatrix");
      for(int zz = y+1; zz<tigress->GetAddBackMultiplicity();zz++)
      {
	matp->Fill(tigresshit->GetCore()->GetEnergy()/1000.,tigress->GetAddBackHit(zz)->GetCore()->GetEnergy()/1000.);
      }
    }

    for(int y=0; y<tigress->GetAddBackMultiplicity();y++)
    {
      TTigressHit *tigresshit = tigress->GetAddBackHit(y);
      
      if(tigresshit->GetCore()->GetEnergy()>2120 && tigresshit->GetCore()->GetEnergy()<2130)
      {
	for(int q=0; q<csm->GetMultiplicity();q++)
	{
	  TCSMHit* hit = csm->GetHit(q);

	  if(hit->GetDetectorNumber()<3)
	  {
	    TH2D* tp = (TH2D*)outlist->FindObject(Form("pid_%i_summed_thickness_gcut",hit->GetDetectorNumber()));
	    tp->Fill(hit->GetEnergyMeV(),hit->GetDdE_dx());
	  }
	  TH1D* gcutp = (TH1D*)outlist->FindObject(Form("gCutE_%i",hit->GetDetectorNumber()));
	  gcutp->Fill(hit->GetEnergyMeV());
	}
      }
    }
//***********************
//  Multiplicity plots
//***********************

for(int i=0;i<csm->GetMultiplicity();i++)
{
  TCSMHit *csmhit = csm->GetHit(i);
  for(int j=0;j<tigress->GetAddBackMultiplicity();j++)
  {
    TTigressHit *tigresshit = tigress->GetAddBackHit(j);
    
    if(tigresshit->GetCore()->GetEnergy()>10)
    {
      TH1D* temp1;
      temp1 = (TH1D*)outlist->FindObject("GammaBe10Brute");
      temp1->Fill(Doppler(tigresshit,csmhit,10));
      temp1 = (TH1D*)outlist->FindObject("GammaBe12Brute");
      temp1->Fill(Doppler(tigresshit,csmhit,12));
    }
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
      //double* CorrVals = CorrParticle(hit->GetCorrectedEnergyMeV("10be")*1000., hit->GetPosition().Theta(), hit->GetPosition().Phi(),MASS_BE10);
      
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
	  double excite = GetExciteE_Heavy(hit,10);
	  
	  if(dopp>=2.577 && dopp<=2.612)
	  {
	    TH1D* expg = (TH1D*)outlist->FindObject(Form("Be10Ex%i_gcut_2589_opp",hit->GetDetectorNumber()));
	    expg->Fill(excite);
	  }
	  else if(dopp>=2.876 && dopp<=2.913)
	  {
	    TH1D* expg = (TH1D*)outlist->FindObject(Form("Be10Ex%i_gcut_2894_opp",hit->GetDetectorNumber()));
	    expg->Fill(excite);
	  }
	  else if(dopp>=3.337 && dopp<=3.402)
	  {
	    TH1D* expg = (TH1D*)outlist->FindObject(Form("Be10Ex%i_gcut_3368_opp",hit->GetDetectorNumber()));
	    expg->Fill(excite);
	  }
	  else if(dopp>=5.951 && dopp<=5.986)
	  {
	    TH1D* expg = (TH1D*)outlist->FindObject(Form("Be10Ex%i_gcut_5958_opp",hit->GetDetectorNumber()));
	    expg->Fill(excite);
	  }
	}
      }
	
      for(int j = 0; j<csm->GetMultiplicity(); j++)
      {
	if(i==j)
	  continue;
	
	TCSMHit *opphit = csm->GetHit(j);
	
	int conditions = 0;
	if(AlmostEqual(hit->GetEnergy(),CorrVals[0]))
	  conditions++;
	if(AlmostEqual(hit->GetPosition().Theta(),CorrVals[1]))
	  conditions++;
	if(AlmostEqual(hit->GetPosition().Phi(),CorrVals[2]))
	  conditions++;

// 	if(conditions>=1)
// 	{
// 	  cout<<"Energy: "<<hit->GetEnergy()<<" "<<CorrVals[0]<<"   "<<AlmostEqual(hit->GetEnergy(),CorrVals[0])<<endl;
// 	  cout<<"Theta: "<<hit->GetPosition().Theta()<<" "<<CorrVals[1]<<"   "<<AlmostEqual(hit->GetPosition().Theta(),CorrVals[1])<<endl;
// 	  cout<<"Phi: "<<hit->GetPosition().Phi()<<" "<<CorrVals[2]<<"   "<<AlmostEqual(hit->GetPosition().Phi(),CorrVals[2])<<endl<<endl;;
// 	}

	TH3I* diagpointer = (TH3I*)outlist->FindObject("AlmostEqual_Diagnostic");
	diagpointer->Fill(int(AlmostEqual(hit->GetEnergy(),CorrVals[0])),int(AlmostEqual(hit->GetPosition().Theta(),CorrVals[1])),int(AlmostEqual(hit->GetPosition().Phi(),CorrVals[2])));

	if((conditions == 3 && int(BEAM_ENERGY)==55) || int(BEAM_ENERGY)==30)
	{
	  for(int y=0; y<tigress->GetAddBackMultiplicity();y++)
	  {
	    TTigressHit *tigresshit = tigress->GetAddBackHit(y);

	    if(tigresshit->GetCore()->GetEnergy()>10)
	    {
	      TH1D* dopptr = (TH1D*)outlist->FindObject(Form("Be10_Gamma_%i_dopp_opp",hit->GetDetectorNumber()));
	      dopptr->Fill(Doppler(tigresshit,opphit,10));
	    }
	  }

	  TH1D* exptr = (TH1D*)outlist->FindObject(Form("Be10Ex%i_corr_opp",hit->GetDetectorNumber()));
	  exptr->Fill(GetExciteE_Heavy_Corrected(opphit,10));

	  TH2D* evtptr = (TH2D*)outlist->FindObject(Form("EvTheta_%i_BE10_opp",hit->GetDetectorNumber()));
	  evtptr->Fill(opphit->GetThetaDeg(),opphit->GetEnergyMeV());

	  TH1D* supexptr = (TH1D*)outlist->FindObject(Form("Be10Ex%i_corr_supp",hit->GetDetectorNumber()));
	  supexptr->Fill(GetExciteE_Heavy_Corrected(hit,10));
	}
      }
    }

  }
}

//***********************
//  Multiplicity plots
//***********************
if(csm->GetMultiplicity()!=0 || tigress->GetAddBackMultiplicity()!=0)
{
  bool be12=0;
  for(int aa=0;aa<csm->GetMultiplicity();aa++)
  {
    TCSMHit *hita = csm->GetHit(aa);
    if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form(Be12Cut,hita->GetDetectorNumber()))))
    {
      if(cut->IsInside(hita->GetEnergyMeV(),hita->GetDdE_dx()) && hita->GetEEnergy() > 10)
      {
	be12=1;
      }
    }
  }

  TH2I* cvgp = (TH2I*)outlist->FindObject("CvGmult");
  cvgp->Fill(csm->GetMultiplicity(),tigress->GetAddBackMultiplicity());

  if(be12)
  {
    TH2I* cvgpbe = (TH2I*)outlist->FindObject("CvGmult_12be");
    cvgpbe->Fill(csm->GetMultiplicity(),tigress->GetAddBackMultiplicity());
  }
}

//***********************
//    One-off Low PID and High PID plots
//***********************

if(int(BEAM_ENERGY) == 55)
{
  for(int xx = 0;xx<csm->GetMultiplicity();xx++)
  {
    TCSMHit *hit = csm->GetHit(xx);

    if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form("pid_high_thick_12Be_HighEnergyOnly_%i_v1",hit->GetDetectorNumber()))))
    {
      if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()) && hit->GetEEnergy() > 10)
      {
	TH2D* EvTPtr = (TH2D*)outlist->FindObject(Form("EvTheta_12Be_%i_high",hit->GetDetectorNumber()));
	EvTPtr->Fill( hit->GetThetaDeg(), hit->GetEnergyMeV());

	TH1D* geSpec = (TH1D*)outlist->FindObject("Be12Gammas_high");

	for(int y=0; y<tigress->GetAddBackMultiplicity();y++)
	{
	  geSpec->Fill(tigress->GetAddBackHit(y)->GetCore()->GetEnergy()/1000.);
	}
      }
    }
    if(TCutG *cut = (TCutG*)(cutlist->FindObject(Form("pid_high_thick_12Be_LowEnergyOnly_%i_v1",hit->GetDetectorNumber()))))
    {
      if(cut->IsInside(hit->GetEnergyMeV(),hit->GetDdE_dx()) && hit->GetEEnergy() > 10)
      {
	TH2D* EvTPtr = (TH2D*)outlist->FindObject(Form("EvTheta_12Be_%i_low",hit->GetDetectorNumber()));
	EvTPtr->Fill( hit->GetThetaDeg(), hit->GetEnergyMeV());

	TH1D* geSpec = (TH1D*)outlist->FindObject("Be12Gammas_low");

	for(int y=0; y<tigress->GetAddBackMultiplicity();y++)
	{
	  geSpec->Fill(tigress->GetAddBackHit(y)->GetCore()->GetEnergy()/1000.);
	}
      }
    }
  }
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

  int i;

  if(strcmp(argv[1], "--low") == 0 || strcmp(argv[1], "-l") == 0)
  {
    i = 2;
    BEAM_ENERGY = 30.14;
  }
  else if(strcmp(argv[1], "--high") == 0 || strcmp(argv[1], "-h") == 0)
  {
    i = 2;
    BEAM_ENERGY = 55;
  }
  else
  {
    cerr<<"Undefined beam energy.  There will be strange behavior in energy dependent functions."<<endl;
    BEAM_ENERGY = 1;
  }
  
  
  TApplication *app = new TApplication("app",0,0);
  TFile cf("cuts.root");
  TIter *iter = new TIter(cf.GetListOfKeys());

  int ncuts = 0;

  while(TObject *obj = iter->Next())
  {
    obj = ((TKey *)obj)->ReadObj();

    //printf("obj->ClassName() = %s\n", obj->ClassName());
    if(strcmp(obj->ClassName(),"TCutG")!=0)
    {
      continue;
    }

    cutlist->Add(obj);
    //printf("found a cut! %s \n",((TNamed *)obj)->GetName());
    ncuts++;
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
    //printf("found a cut! %s \n",((TNamed *)obj)->GetName());
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


  printf("%i analysis trees added to chain.\n",i-2);
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

  TString outputname;

  if(int(BEAM_ENERGY) == 30)
    outputname = "outputlow.root";
  else if(int(BEAM_ENERGY) == 55)
    outputname = "outputhigh.root";
  else
    outputname = "output.root";

  TFile f(outputname,"recreate");
  f.cd();
  outlist->Write();
  f.Close();
  
  if(DEBUG)
  {
    cout<<"All done"<<endl;
  }

  return 0;
}





