//---------------------------------------------------------------------------


#pragma hdrstop

#include "CNCKeyBoardController.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


BYTE data;
_fastcall CNCKeyBoardController::CNCKeyBoardController(bool CreateSuspended): TThread(CreateSuspended)
{
/*   HRESULT	hr;
	CoInitialize(NULL);
	hr = CoCreateInstance(CLSID_ComPort, NULL, CLSCTX_INPROC_SERVER, IID_IComPort, (void**) &pComRC );
	if( ! SUCCEEDED( hr ) )
	{
		pComRC = NULL;
	}
	else
	{
		// Open port
		pComRC->put_PortID( 1 );  	// Use Com 1
		pComRC->put_BaudRate( 9600 );	// 9600 bps
		pComRC->Open();
	}                 //*/
}
void __fastcall CNCKeyBoardController::Execute()
{
/*  	if(pComRC==NULL)
		return;
   wchar_t *temp [5];
   pComRC->put_ComTimeout(100);
   while(1)
   {

      pComRC->ReadString(temp);
      if(temp[0][0] != '\0')
      {
         data = temp[0][0] - '0';
         if(temp[0][1] >= '0' && temp[0][1] <= '9')
            data = 10 *data + (temp[0][1] - '0');
         Synchronize(sendByte);
      }
   }                      //*/
}

void __fastcall CNCKeyBoardController::sendByte(){
/*   if(data <= 15 && data >= 0)
      Form1->aKeyPressed(data);   //*/
}

void CNCKeyBoardController::sendData(Byte b)
{
 /*  pComRC->WriteByte(b);   //*/
}