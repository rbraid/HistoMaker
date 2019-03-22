#include "FunctionsForMakeHistos.hh"



// TVector3 GetPositionsNew(TCSMHit* Hit, char pos)
// {
//   int hs = -1;
//   int vs = -1;
//   
//   if(pos == 'D' || pos == 'd')
//   {
//     hs = Hit->GetDHorizontalStrip();
//     vs = Hit->GetDVerticalStrip();
//   }
//   else if(pos == 'E' || pos == 'e')
//   {
//     hs = Hit->GetEHorizontalStrip();
//     vs = Hit->GetEVerticalStrip();
//   }
//   return GetPositionsNew(Hit->GetDetectorNumber(), pos, hs, vs);
// }
// 
// TVector3 GetPositionsNew(int detector,char pos, int horizontalstrip, int verticalstrip)
// {
//   //horizontal strips collect N charge!
//   //vertical strips collect P charge!
//   //verticalstrip -=1; //the loop that calls this works in 1-16, but here i work in 0-15
//   //horizontalstrip -=1;
//   //all dE detectors are flipped compared to the E detectors.
//   
//   //for side detectors, strip 0 has lowest theta in detector 3
//   //strip 15 has lowest theta for detector 4
//   
//   TVector3 Pos;
//   TVector3 horMove;
//   TVector3 vertMove;
//   TVector3 *RetPos = new TVector3[5];
//   
//   TRandom3 *rndm = new TRandom3(0);
//   
//   double detTheta = 31. * (TMath::Pi()/180.);
//   double SideX = 66.255;
//   double SideZ = 12.99;
//   double dER = 58.98-1.5;
//   double ER = 70.99-1.5;
//   double x = 0.0,y = 0.0,z = 0.0;
//   
//   double halfpixel = 50./32.;
//   double fullpixel = 50./16.;
//   
//   if(detector<3)
//   {
//     x = (verticalstrip-8) * fullpixel  +  halfpixel;
//     y = (horizontalstrip-8) * fullpixel  +  halfpixel;
//     
//     if(pos == 'D')
//       z = dER;
//     else if(pos == 'E')
//       z = ER;
//     else
//     {
//       cerr<<" Unrecognized position: "<<pos<<", reverting to dE to fail safe."<<endl;
//       z = dER;
//     }
//     if(detector==1)
//     {
//       x=-x;
//       detTheta = -detTheta;
//     }
//     
//     //randomize
//     x += rndm->Uniform(-halfpixel,halfpixel);
//     y += rndm->Uniform(-halfpixel,halfpixel);
//     Pos.SetXYZ(x,y,z);
//     
//     
//     vertMove.SetXYZ(0.,halfpixel,0.);
//     horMove.SetXYZ(halfpixel,0.,0.);
//     
//     RetPos[0] = Pos + vertMove + horMove;
//     RetPos[1] = Pos - vertMove + horMove;
//     RetPos[2] = Pos - vertMove - horMove;
//     RetPos[3] = Pos + vertMove - horMove;
//     
//     RetPos[4] = Pos;
//     
//     RetPos[0].RotateY(detTheta);
//     RetPos[1].RotateY(detTheta);
//     RetPos[2].RotateY(detTheta);
//     RetPos[3].RotateY(detTheta);
//     RetPos[4].RotateY(detTheta);
//   }
//   
//   else
//   {
//     //According to the SolidWorks diagram, the centerline for the side detectors should be 12.01 mm from rear edge of detector.
//     //12.01/fullpixel = 3.84
//     //center of 
//     if(detector==4)
//     {
//       x = SideX;
//       verticalstrip = -verticalstrip+15;
//     }
//     else if(detector==3)
//     {
//       x = -SideX;
//     }
//     y = (horizontalstrip-8) * fullpixel  +  halfpixel;
//     z = (verticalstrip-8) * fullpixel  +  halfpixel;
//     
//     z += SideZ;
//     z += rndm->Uniform(-halfpixel,halfpixel);
//     y += rndm->Uniform(-halfpixel,halfpixel);
//     
//     Pos.SetXYZ(x,y,z);
//     vertMove.SetXYZ(0.,halfpixel,0.);
//     horMove.SetXYZ(0.,0.,halfpixel);
//     
//     RetPos[0] = Pos + vertMove + horMove;
//     RetPos[1] = Pos - vertMove + horMove;
//     RetPos[2] = Pos - vertMove - horMove;
//     RetPos[3] = Pos + vertMove - horMove;
//     
//     RetPos[4] = Pos;
//   }
//   
//   delete rndm;
//   
//   return(RetPos[4]);
// }




