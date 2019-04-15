#include "../include/Misc.hh"

bool AlmostEqual(double a, double b, double threshold)
{
  if( TMath::Abs(a-b) / ((a+b)/2) < threshold )
    return true;
  else
    return false;
}

double toDegrees(double ang)
{
  return ang*180./TMath::Pi();
}

double toRadians(double ang)
{
  return ang*TMath::Pi()/180.;
}

int SideAdjacent(int det)
{
  int retInt = -1;
  if(det == 1)
    retInt = 3;
  else if(det == 2)
    retInt = 4;
  
  else if(det == 3)
  {
    retInt = 1;
    cout<<"Error, I think you are calling SideAdjacent wrong, detector given: "<<det<<", returning "<<retInt<<endl;
  }
  else if(det == 4)
  {
    retInt = 2;
    cout<<"Error, I think you are calling SideAdjacent wrong, detector given: "<<det<<", returning "<<retInt<<endl;
  }
  else
  {
    cerr<<"Error! Fell through in SideAdjacent.  Detector number: "<<det<<endl;
  }
  return retInt;
}

int SideOpposite(int det)
{
  int retInt = -1;
  if(det == 1)
    retInt = 4;
  else if(det == 2)
    retInt = 3;
  
  else if(det == 3)
  {
    retInt = 2;
    cout<<"Error, I think you are calling SideOpposite wrong, detector given: "<<det<<", returning "<<retInt<<endl;
  }
  else if(det == 4)
  {
    retInt = 1;
    cout<<"Error, I think you are calling SideOpposite wrong, detector given: "<<det<<", returning "<<retInt<<endl;
  }
  else
  {
    cerr<<"Error! Fell through in SideOpposite.  Detector number: "<<det<<endl;
  }
  return retInt;
}

int SideOpposite(TCSMHit* hit)
{
  return SideOpposite(hit->GetDetectorNumber());
}

int SideAdjacent(TCSMHit* hit)
{
  return SideAdjacent(hit->GetDetectorNumber());
}