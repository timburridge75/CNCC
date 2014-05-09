//---------------------------------------------------------------------------


#pragma hdrstop

#include "CNCControlKeyBoard.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


HANDLE hComm = NULL;
COMMTIMEOUTS ctmoNew = {0}, ctmoOld;
BYTE bt;
_fastcall CNCControlKeyBoard::CNCControlKeyBoard(bool CreateSuspended): TThread(CreateSuspended)
{
   if(openCommPort("COM1")){
      return;
   }else
      ;//error
}
void __fastcall CNCControlKeyBoard::Execute()
{

   DWORD dwBytesTransferred,dwCommModemStatus;

   // Specify a set of events to be monitored for the port.
   SetCommMask(hComm, EV_RXCHAR | PURGE_RXABORT |EV_RXFLAG );
   sendData(101);
   sendData(103);
   sendData(Form1->speedVal);
   while (hComm != INVALID_HANDLE_VALUE)
   {
      if(Terminated){
         CloseHandle(hComm);
         return;
      }
      // Wait for an event to occur for the port.
      WaitCommEvent (hComm, &dwCommModemStatus, 0);
      // Read the data from the serial port.
      ReadFile (hComm, &bt, 1, &dwBytesTransferred, NULL);
      PurgeComm(hComm,PURGE_RXCLEAR); // clear buffer
      Synchronize(sendByte);
   }
}

bool CNCControlKeyBoard::openCommPort(AnsiString s)
{
  DCB dcbCommPort;

  // OPEN THE COMM PORT.
  // REPLACE "COM2" WITH A STRING OR "COM1", "COM3", ETC. TO OPEN
  // ANOTHER PORT.

  hComm = CreateFile(s.c_str(),
                      GENERIC_READ | GENERIC_WRITE,
                      0,
                      0,
                      OPEN_EXISTING,
                      0,
                      NULL);

  // IF THE PORT CANNOT BE OPENED, BAIL OUT.

  if(hComm == INVALID_HANDLE_VALUE) return false;

  // SET THE COMM TIMEOUTS IN OUR EXAMPLE.

  GetCommTimeouts(hComm,&ctmoOld);
  ctmoNew.ReadTotalTimeoutConstant = 10;
  ctmoNew.ReadTotalTimeoutMultiplier = 10;
  ctmoNew.WriteTotalTimeoutMultiplier = 10;
  ctmoNew.WriteTotalTimeoutConstant = 10;
  ctmoNew.ReadIntervalTimeout = MAXDWORD;
  SetCommTimeouts(hComm, &ctmoNew);

  // SET BAUD RATE, PARITY, WORD SIZE, AND STOP BITS.
  // THERE ARE OTHER WAYS OF DOING SETTING THESE BUT THIS IS THE EASIEST.
  // IF YOU WANT TO LATER ADD CODE FOR OTHER BAUD RATES, REMEMBER
  // THAT THE ARGUMENT FOR BuildCommDCB MUST BE A POINTER TO A STRING.
  // ALSO NOTE THAT BuildCommDCB() DEFAULTS TO NO HANDSHAKING.

  dcbCommPort.DCBlength = sizeof(DCB);
  GetCommState(hComm, &dcbCommPort);
  BuildCommDCB("9600,N,8,1", &dcbCommPort);
  SetCommState(hComm, &dcbCommPort);
  return true;
}

void __fastcall CNCControlKeyBoard::sendByte(){
   
   Form1->aKeyPressed(bt);
}


void CNCControlKeyBoard::sendData(int val)
{
   DWORD dwBytesTransferred;
   WriteFile(hComm,&val,1, &dwBytesTransferred, NULL);
}
