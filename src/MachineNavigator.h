//---------------------------------------------------------------------------
// A class for controlling CNC Controller Board. This class uses CuttingModel
// class for direction and we can get position at each time.
// Position must be durable.
//
// Producer name: Saeed Shariati
// e-mail address: Saeed_Shariati@comp.iust.ac.ir
//---------------------------------------------------------------------------
#ifndef MachineNavigatorH
#define MachineNavigatorH

#include <vcl\vcl.h>
#include "CuttingModel.h"
#include "NavigationArray.h"
#include "mainForm.h"
#include "Utility.h"
#include "Devices.h"
#include "Parameters.h"
#include <math.hpp>
#include <math.h>
#include<mmsystem.h>
//#pragma comment(lib, "winmm.lib")

//---------------------------------------------------------------------------
struct MotorCommand{
   int x;  // 1 Up    -1 Down 0 none
   int y;  // 1 Right -1 Left 0 none
};

class MachineNavigator:public TThread{
private:
   CuttingModel *model;
   double xLocation,yLocation;
   int K;
   ArrayChar navArray;
   Utility utility;
   double startMapX, startMapY, endMapX,endMapY;
   int vx,vy;
   Devices *devices;
   double getDistFromPoint(double x1, double y1, double x2, double y2);
   double getDistFromCircle(double x1, double y1, double x2, double y2, double r);
   double getDegreeDiffFromLine(double x1, double y1, double x2, double y2, double r, double currentLocX , double currentLocY);
   double getDistFromLine(double x , double y, double x1, double y1, double x2, double y2);
   Polar cartezToPolar(double x, double y, double xo, double yo);
   double ghadreMotlagh(double x);

   void __fastcall sendToForm();
   void makeArcNavigation(double x1, double y1, double x2, double y2, double degRad,  bool isClockwise, NavigationArray *navTemp);
   bool inline __fastcall InPolyLine(long long i);
   bool inFreeLine;
   bool forward;
   unsigned long long delay ;

   //Timer 

protected:
   void __fastcall Execute();
public:


   unsigned long long mustBeDelay;
   
   __fastcall MachineNavigator(bool CreateSuspended,Devices * dev);
   bool highSpeed;
   void setImageForm(int k);
   void setValToPort(int val);

   void importModel(CuttingModel *cm);
   void freeMemory();
   void makeLineNavigation(double x1, double y1, double x2, double y2, NavigationArray * navTemp);
   void __fastcall addItem(int x);

   void __fastcall toFreeLine();
   void __fastcall toPolyLine();
   void __fastcall setForward();
   void __fastcall setBackward();
   bool __fastcall isForward();
   bool __fastcall isInFreeLine();
   void doStop();

};
#endif
 
