//---------------------------------------------------------------------------


#pragma hdrstop

#include "Devices.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


Devices::Devices()
{
   state = BIT3;
   gasStatus = false;
}

bool Devices::openPort()
{
   for (int i=0;i<20; i++)
   {
      if(InitializeWinIo()){
         SetPortVal(PORTADDRESS,state,1);
         return true;
      }
   }
   return false;
}
bool Devices::gasIsOn()
{
   return gasStatus;
}
void Devices::startGas()
{
   gasStatus = true;
   state = state | BIT6;
   SetPortVal(PORTADDRESS,state,1);
}

void Devices::torchUp()
{
   state = state & 0xbf;
   state = state | BIT8;
   SetPortVal(PORTADDRESS,state,1);
}

void Devices::torchDown()
{
   state = state & 0x7f;
   state = state | BIT7;
   SetPortVal(PORTADDRESS,state,1);
}

void Devices::motorFree()
{
   state = state | BIT3;
   SetPortVal(PORTADDRESS,state,1);
}
void Devices::motorPause()
{
   state = 0;
   SetPortVal(PORTADDRESS,state,1);
}

bool Devices::motorIsFree()
{
   return (state & BIT3);
}

void  Devices::SendToMotor(unsigned long val)
{
   state = (state & 0xe0)|(val&0x1f);
   SetPortVal(PORTADDRESS,state,1);
}

void Devices::shutDownPort()
{
   ShutdownWinIo();
}

bool Devices::validate()
{
   unsigned char * address;
   address = (unsigned char *)0xffff5;
   unsigned long val = 1;
   int day,month,year;
   if(GetPhysLong(address,&val))
   {
      day = ((val & 0x00ff)-0x30) * 10 + (((val & 0x00ff00)>>8)-0x30);
      address = (unsigned char *)0xffff8;
      if(GetPhysLong(address,&val))
      {
         month = ((val & 0x00ff)-0x30) * 10 + (((val & 0x00ff00)>>8)-0x30);
         address = (unsigned char *)0xffffb;
         if(GetPhysLong(address,&val))
         {
            year = ((val & 0x00ff)-0x30) * 10 + (((val & 0x00ff00)>>8)-0x30);
            if((day == 4 && month == 8 & year == 9)|| //Saeed Work
               (day == 11 && month == 6 & year == 8)  //Client 008
               )  // ?

               return true;
         }
      }
   }
   return true;   
}

void Devices::stopGas()
{
   gasStatus = false;
   state = state & 0xdf;
   SetPortVal(PORTADDRESS,state,1);
}

void Devices::torchStop()
{
   state = state & 0x3f;
   SetPortVal(PORTADDRESS,state,1);
}

