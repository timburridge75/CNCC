//---------------------------------------------------------------------------


#pragma hdrstop

#include "RealTimeActions.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//Timer Initialization
UINT uDelay = 1;
UINT uResolution = 1;
DWORD dwUser = NULL;
UINT fuEvent = TIME_PERIODIC; 
MMRESULT FTimerID;

void CALLBACK TimeProc(UINT uTimerID, UINT uMsg,
                        DWORD dwUser, DWORD dw1, DWORD dw2)
{
   //Timer Interrupt
}
void __fastcall RealTimeActions::Execute()
{
   unsigned long val = 0, clock = 0;

   while(1)
   {
      if(xDir == 0 && yDir == 0 )
        Sleep(1);
      val = 0;
      clock = 0;
      if(xDir == Form1->params.forwardCommandX)
         val = BIT5;
      if(yDir == Form1->params.forwardCommandY)
         val = val | BIT2;
      if(xDir != 0)
         clock =  BIT4;
      if(yDir != 0)
         clock = clock | BIT1;
      
      devices->SendToMotor(val);
      //utility.microSleep(100);
      Sleep(1);
      val = val | clock;
      devices->SendToMotor(val);
      if(highSpeed)
         Sleep(1);
      else
      {
        if(Form1->delay != 10000)
                Form1->delay = 10000;
        Sleep(Form1->delay / 1000);
      }
   }
}

__fastcall RealTimeActions::RealTimeActions(bool CreateSuspended,Devices * dev)
: TThread(CreateSuspended)
{
   Priority = tpTimeCritical;
   devices = dev;
   xDir = 0;
   yDir = 0;
   highSpeed = false;
   timeBeginPeriod(1);
   FTimerID = timeSetEvent(uDelay, uResolution, TimeProc, dwUser, fuEvent);
   if(FTimerID==NULL){
      ShowMessage("Failed to generate multimedia timer.\n");
   }
}

void RealTimeActions::SetDirection(int degree)
{
   switch(degree)
   {
      case 0:
         xDir = 0;
         yDir = 1;
         break;
      case 4500:
         xDir = -1;
         yDir = 1;
         break;
      case 9000:
         xDir = -1;
         yDir = 0;
         break;
      case 13500:
         xDir = -1;
         yDir = -1;
         break;
      case 18000:
         xDir = 0;
         yDir = -1;
         break;
      case 22500:
         xDir = 1;
         yDir = -1;
         break;
      case 27000:
         xDir = 1;
         yDir = 0;
         break;
      case 31500:
         xDir = 1;
         yDir = 1;
         break;
      default:
         xDir = 0;
         yDir = 0;
         break;
   }
}
