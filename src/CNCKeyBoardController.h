//---------------------------------------------------------------------------

#ifndef CNCKeyBoardControllerH
#define CNCKeyBoardControllerH
//---------------------------------------------------------------------------
//#include "mainForm.h"

#include "AComportConstants.h"
#include "AComport.h"
#include "AComport_i.c"
#include <vcl/vcl.h>

class CNCKeyBoardController:public TThread{
protected:
   void __fastcall Execute();


public:
   __fastcall CNCKeyBoardController(bool CreateSuspended  );
   void sendData(Byte b);

private:
//   IComPort *pComRC ;
   void __fastcall sendByte();
};

#endif
