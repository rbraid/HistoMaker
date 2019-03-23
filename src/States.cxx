#include "../include/States.hh"

int GetGamState(double doppVal)
{
  int doppI = -2;
  
  if(doppVal>=3.337 && doppVal<=3.402)
    doppI = 3368;
  else if(doppVal>=2.577 && doppVal<=2.612)
    doppI = 2589;
  else if(doppVal>=2.876 && doppVal<=2.913)
    doppI = 2894;
  else if(doppVal>=2.86 && doppVal<=2.87)
    doppI = 2867;
  else if(doppVal>=5.951 && doppVal<=5.986)
    doppI = 5958;
  
  //3368,2589,2894,2867,5958
  
  return doppI;
}

int GetExState(double exVal, int iso)
{
  int exI = -1;
  
  if(iso == 10)
  {
    if(SIMULATED_DATA)
    {
      if(exVal >= -1.5 && exVal<= 1.2)
        exI = 0;
      else if(exVal >= 2. && exVal<= 4.)
        exI = 3;      
      else if(exVal >= 4.5 && exVal <= 8)
        exI = 6;
      else if(exVal >= 7.7 && exVal <= 10.7)
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
      else if(exVal >= 4.5 && exVal<= 7.5)
        exI = 6;
      else if(exVal >= 7.5 && exVal<= 10.5)
        exI = 9;
      else if(exVal >= 11.2 && exVal <= 12.7)
        exI = 12;
    }
  }
  
  else if(iso == 11 || iso == 9)
  {
    if(SIMULATED_DATA)
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
    cerr<<"GetExState not defined for "<<iso<<"Be"<<endl;
    exI = -2;
  }
  
  return exI;
}
