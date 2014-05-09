//---------------------------------------------------------------------------

#ifndef CNCControlKeyBoardH
#define CNCControlKeyBoardH
//---------------------------------------------------------------------------
#include "mainForm.h"

#include <vcl\vcl.h>

class CNCControlKeyBoard:public TThread{
protected:
   void __fastcall Execute();

public:
   __fastcall CNCControlKeyBoard(bool CreateSuspended  );
   void sendData(int val);

private:
   bool openCommPort(AnsiString s);
   void __fastcall sendByte();
};

#endif
