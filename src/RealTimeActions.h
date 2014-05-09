//---------------------------------------------------------------------------

#ifndef RealTimeActionsH
#define RealTimeActionsH
//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#include "Utility.h"
#include "Devices.h"
#include "mainForm.h"

#define DIRECTION_STOP  65535

class RealTimeActions:public TThread{
private:
   Devices *devices;
   Utility utility;

protected:
   void __fastcall Execute();

public:
   void SetDirection(int degree);
   __fastcall RealTimeActions(bool CreateSuspended,Devices * dev);
   int xDir,yDir;
   bool highSpeed;

};
#endif
 