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
