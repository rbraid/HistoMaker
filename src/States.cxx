#include "../include/States.hh"

int GetGamState(double doppVal, int nSigma, int iso)
{
  int doppI = -2;

  if(iso == 10)
  {
    if(InGaussian(doppVal, 2.58975, 9.12827e-03, nSigma))
    {
      if(doppI == -2)
        doppI = 2589;
      else
      {
        std::cout<<"Warning: reassignment in GetGamState. From "<<doppI<<" to "<<2589<<std::endl;
        doppI = 2589;
      }
    }
    if(InGaussian(doppVal, 2.89422, 5.18030e-03, nSigma))
    {
      if(doppI == -2)
        doppI = 2894;
      else
      {
        std::cout<<"Warning: reassignment in GetGamState. From "<<doppI<<" to "<<2894<<std::endl;
        doppI = 2894;
      }
    }
    if(InGaussian(doppVal, 3.37119, 1.22753e-02, nSigma))
    {
      if(doppI == -2)
        doppI = 3368;
      else
      {
        std::cout<<"Warning: reassignment in GetGamState. From "<<doppI<<" to "<<3368<<std::endl;
        doppI = 3368;
      }
    }
    
    if(doppVal >= 4.8)
    {
      if(doppI == -2)
        doppI = 5958;
      else
      {
        std::cout<<"Warning: reassignment in GetGamState. From "<<doppI<<" to "<<5958<<std::endl;
        doppI = 5958;
      }
    }

    // if(InGaussian(doppVal, 5.96114e+00, 3.13731e-02, nSigma)  || InGaussian(doppVal, 5.96114e+00-.511, 3.13731e-02, nSigma))
    // {
    //   if(doppI == -2)
    //     doppI = 5958;
    //   else
    //   {
    //     std::cout<<"Warning: reassignment in GetGamState. From "<<doppI<<" to "<<5958<<std::endl;
    //     doppI = 5958;
    //   }
    // }
  }
  else if(iso == 11)
  {
    if(InGaussian(doppVal, 0.3199, 0.0012, nSigma))
    {
      if(doppI == -2)
        doppI = 320;
      else
      {
        std::cout<<"Warning: reassignment in GetGamState. From "<<doppI<<" to "<<320<<std::endl;
        doppI = 320;
      }
    }
  }

  return doppI;
}

Bool_t InGaussian(double value, double median, double sigma, double nStdDev)
{
  Bool_t retVal = kFALSE;

  if(value <= median+nStdDev*sigma)
    if(value >= median-nStdDev*sigma)
      retVal = kTRUE;

  return retVal;
}

int GetExState(double exVal, int iso, bool sim)
{
  int exI = -1;

  if(iso == 10)
  {
    if(sim)
    {
      if(exVal >= -1.5 && exVal<= 1.2)
        exI = 0;
      else if(exVal >= 2. && exVal<= 4.)
        exI = 3;
      else if(exVal >= 4 && exVal <= 7.5) // Note there is a problem here with no exclusivity.  if you re-run 7, you need to comment out this line
        exI = 6;
      else if(exVal >= 5.50 && exVal <= 9){
//         std::cout<<"hack in place for state 7, this is broken"<<std::endl;
        exI = 7;
      }
      else if(exVal >= 5.5 && exVal <= 14)
        exI = 9;
      else if(exVal >= 10.7 && exVal <= 14)
        exI = 12;
    }
    else
    {
      if(exVal >= -1 && exVal<= 1.2)
        exI = 0;
      else if(exVal >= 2.5 && exVal<= 4.4)
        exI = 3;
      else if(exVal >= 4.5 && exVal<= 7.)
        exI = 6;
      else if(exVal >= 7 && exVal<= 8.4)
        exI = 7;
      else if(exVal >= 8.4 && exVal<= 10.5)
        exI = 9;
      else if(exVal >= 11.2 && exVal <= 12.7)
        exI = 12;
    }
  }

  else if(iso == 11 || iso == 9)
  {
    if(sim)
    {
      if(exVal >= -3 && exVal <= 1)
        exI = 0;
      else if(exVal >= 1.1 && exVal <= 4)
        exI=3;
    }

    else
    {
      if(exVal >= -1.5 && exVal <= 1)
        exI = 0;
      else if(exVal >= 1.5 && exVal <= 3.5)
        exI = 3;
    }
  }

  else
  {
    std::cerr<<"GetExState not defined for "<<iso<<"Be"<<std::endl;
    exI = -2;
  }

  return exI;
}
