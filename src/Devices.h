//---------------------------------------------------------------------------

#ifndef DevicesH
#define DevicesH
//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#include "winio.h"
#include "Utility.h"

#define PORTADDRESS 0x378
#define BIT1 1
#define BIT2 2
#define BIT3 4
#define BIT4 8
#define BIT5 16
#define BIT6 32
#define BIT7 64
#define BIT8 128

#define NOT_BIT1 0xFE
#define NOT_BIT2 0xFD
#define NOT_BIT3 0xFB
#define NOT_BIT4 0xF7
#define NOT_BIT5 0xEF
#define NOT_BIT6 0xDF
#define NOT_BIT7 0xBF
#define NOT_BIT8 0x7F

#define TORCH_DELAY 200
class Devices{


public:
   Devices();
   bool openPort();
   void startGas();

   void torchStop();
   void torchUp();
   void torchDown();

   void motorFree();
   bool motorIsFree();
   void motorPause();
   void SendToMotor(unsigned long val);
   void shutDownPort();
   bool validate();
   void stopGas();
   bool gasIsOn();

private:
   unsigned long state;
   bool gasStatus;
};
#endif
 