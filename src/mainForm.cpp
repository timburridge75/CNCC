//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "mainForm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Joystick"
#pragma link "Joystick"
#pragma resource "*.dfm"
TForm1 *Form1;
TForm6 *Form6;
MachineNavigator * machineNavigator;
CNCControlKeyBoard* cncControlKeyBoard;

//---------------------------------------------------------------------------

CuttingModel *cuttingModel;
CuttingModel *viewingModel;
RealTimeActions * realTimeActions;
Devices *devices;
bool gasIsOn = false;

Utility u;
double scale=0,xCenter=0,yCenter=0;
#define  BUTTON_NUMBER  10
bool __oldButtonState[BUTTON_NUMBER], isOldButtonPressed = false;
int __oldPOV = 65535;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
   PanelImage->DoubleBuffered = true;
   showMapX = 0;
   showMapY = 0;
   delay = initialDelay;
   speedVal = (MIN_SPEED - (double)delay)/(MIN_SPEED - MAX_SPEED) *100;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormDestroy(TObject *Sender)
{
   
   devices->shutDownPort();
   if(machineNavigator != NULL)
   {
      machineNavigator->FreeOnTerminate = true;
      machineNavigator->Terminate();
     // delete machineNavigator;
   }
   if(realTimeActions != NULL)
   {
      realTimeActions->FreeOnTerminate = true;
      realTimeActions->Terminate();
     // delete machineNavigator;
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::setStartPoint(double x, double y)
{
   int height = ImageMap->Height;
   ImageMap->Canvas->MoveTo(x*scale + xCenter,height - (y*scale + yCenter));
}

void __fastcall TForm1::FormCreate(TObject *Sender)
{
   TFileStream *cfg = new TFileStream("WinDev.dll",fmOpenReadWrite);

#ifndef DEBUG_MODE
   Form3 = new TForm3(this);
   Form3->ShowModal();
#endif
   Form6 = new TForm6(this, cfg);

   //Load Configuration

   cfg->Seek(0,0);
   unsigned char buff[] = { 3, 10, 9};
   cfg->Read(buff,3);

   cfg->Read(&(params.dx),sizeof(params.dx));
   cfg->Read(&(params.dy),sizeof(params.dy));
   cfg->Read(&(params.forwardCommandX),sizeof(params.forwardCommandX));
   cfg->Read(&(params.forwardCommandY),sizeof(params.forwardCommandY));
   
   //Splash
   try{
  /*
      TFileStream *f = new TFileStream("C:\\WINDOWS\\system32\\winlog.dll",fmOpenReadWrite);
      char buffer[6];
      int limit;

      TDateTime now = Now();
      int nY = now.FormatString("YY").ToInt();
      int nM = now.FormatString("MM").ToInt();
      int nD = now.FormatString("DD").ToInt();

      int dY, dM, dD;
      f->Seek(0,0);
      f->Read(&dY,4);
      f->Read(&dM,4);
      f->Read(&dD,4);

      f->Read(&limit,4);
      if(nY != dY || nM != dM || nD != dD )
      {
         if(nY > dY || nM > dM || nD > dD )
         {
            int diff = nD - dD;
            diff += (nM - dM)*31;
            diff += (nY - dY)*256;
            if(diff > 10)
               diff = 10;
            limit-= diff;
         }
         else if (nY < dY || nM < dM || nD < dD )
         {
            limit--;
         }
//         limit = 97;
         f->Seek(0,0);
         f->Write(&nY, 4);
         f->Write(&nM, 4);
         f->Write(&nD, 4);
         f->Write(&limit, 4);
      }
    */
      int limit = 1;
      if(limit > 0)
      {
         //Actual Works
         devices = new Devices();
         devices->openPort();
         isForward = true;
         cncControlKeyBoard = new CNCControlKeyBoard(false);
         realTimeActions = new RealTimeActions(true,devices);

      }
      else
      {
          ShowMessage("Progrm Expired, Call with: +98-912-2274122");
      }
   }
   catch(...)
   {
      ShowMessage("An error in Execution, Call with: +98-912-2274122");
      return;
   }
   for(int i =0; i< BUTTON_NUMBER; i++)
      __oldButtonState[i] = false;

   #ifndef DEBUG_MODE
   try
   {
        gamepad->Active = true;
        TimerGamePad->Enabled = true;
   }
   catch(...)
   {
        gamepad->Active = false;
        TimerGamePad->Enabled = false;
   }
   #endif
}
//---------------------------------------------------------------------------


void __fastcall TForm1::aKeyPressed(BYTE b)
{
   switch(b)
   {
      case 200: //Get Status Request
         cncControlKeyBoard->sendData(speedVal);
         if(devices->gasIsOn())
            cncControlKeyBoard->sendData(100);
         else
            cncControlKeyBoard->sendData(101);

         if(realTimeActions->highSpeed)
                cncControlKeyBoard->sendData(102);
         else
                cncControlKeyBoard->sendData(103);
         break;

      case 20:
         devices->torchStop();
         break;
      case 7: //3 Forward
         BtnForward->Click();
         break;

      case 6: // Gas
         BtnGas->Click();
         if(devices->gasIsOn())
                cncControlKeyBoard->sendData(100);
         else
                cncControlKeyBoard->sendData(101);
         break;

      case 4: //9  Backward
         BtnBackward->Click();
         break;

      case 10: //6  Pause
         BtnStop->Click();
         cncControlKeyBoard->sendData(101);
         cncControlKeyBoard->sendData(103);
         cncControlKeyBoard->sendData(speedVal);
         devices->torchStop();
         break;

      case 12: //torch Up
        devices->torchStop();
         devices->torchUp();
         break;

      case 8:  //torch Down
         devices->torchStop();
         devices->torchDown();
         break;

//--------------------------- Arrow
      case 2: //Arrow Up
         if(machineNavigator != NULL)
            if(!(machineNavigator->Suspended))
               return;

         if(realTimeActions->yDir == 1)
            realTimeActions->yDir = 0;
         else
            realTimeActions->yDir = 1;

         if(realTimeActions->Suspended)
                realTimeActions->Resume();

         break;

      case 0:  //Arrow Down
         if(machineNavigator != NULL)
            if(!(machineNavigator->Suspended))
               return;

         if(realTimeActions->yDir == -1)
            realTimeActions->yDir = 0;
         else
            realTimeActions->yDir = -1;

         if(realTimeActions->Suspended)
                realTimeActions->Resume();
         break;
         
      case 3: //Arrow Left
         if(machineNavigator != NULL)
            if(!(machineNavigator->Suspended))
               return;

         if(realTimeActions->xDir == -1)
            realTimeActions->xDir = 0;
         else
            realTimeActions->xDir = -1;

         if(realTimeActions->Suspended)
                realTimeActions->Resume();
         break;
      case 1:  //Arrow Right
         if(machineNavigator != NULL)
            if(!(machineNavigator->Suspended))
              return;

         if(realTimeActions->xDir == 1)
            realTimeActions->xDir = 0;
         else
            realTimeActions->xDir = 1;

         if(realTimeActions->Suspended)
                realTimeActions->Resume();
         break;

//--------------------------- Speed 
      case 15: //Speed Up
         BtnSpeedUp->Click();
         cncControlKeyBoard->sendData(speedVal);
         break;
      case 11:  //Speed Down
         BtnSpeedDown->Click();
         cncControlKeyBoard->sendData(speedVal);
         break;
      case 14:  //High Speed
         BtnHighSpeed->Click();
         if(realTimeActions->highSpeed)
                cncControlKeyBoard->sendData(102);
         else
                cncControlKeyBoard->sendData(103);
         break;

//--------------------------- Stop
      case 16:
         devices->torchStop();
         devices->motorFree();
         if(!(realTimeActions->Suspended))
            realTimeActions->Suspend();
   }
   showStatus();
}

void __fastcall TForm1::BtnImportClick(TObject *Sender)
{
   BtnStop->Click();
   Form2->Show();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BtnForwardClick(TObject *Sender)
{
   if(!realTimeActions->Suspended)
      realTimeActions->Suspend();
      
   if (machineNavigator == NULL)
      return;

   if(machineNavigator->Suspended)
   {
      machineNavigator->setForward();
      machineNavigator->Resume();
   }
   else
   {
      if(!(machineNavigator->isForward()))
      {
         machineNavigator->doStop();
         machineNavigator->setForward();
         machineNavigator->Resume();
      }
      else
      {
         machineNavigator->doStop();
      }
   }
   ShowCuttingMapTimer->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BtnBackwardClick(TObject *Sender)
{
   if(!realTimeActions->Suspended)
      realTimeActions->Suspend();
      
   if (machineNavigator == NULL)
      return;

   if(machineNavigator->Suspended)
   {
      machineNavigator->setBackward();
      machineNavigator->Resume();
   }
   else
   {
      if(machineNavigator->isForward())
      {
         machineNavigator->doStop();
         machineNavigator->setBackward();
         machineNavigator->Resume();
      }
      else
      {
         machineNavigator->doStop();
      }
   }
   ShowCuttingMapTimer->Enabled = true;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::BtnStopClick(TObject *Sender)
{
   ShowCuttingMapTimer->Enabled = false;
   if(machineNavigator != NULL)
   {
      if(!(machineNavigator->isInFreeLine()))
         machineNavigator->highSpeed = false;
      machineNavigator->doStop();
   }
   if(!(realTimeActions->Suspended))
   {
      realTimeActions->Suspend();
   }
   if(devices->motorIsFree())
      devices->motorPause();
   else
      devices->motorFree();
   devices->stopGas();
   showStatus();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BtnGasClick(TObject *Sender)
{
   if(devices->gasIsOn())
   {
      devices->stopGas();
   }
   else
   {
      devices->startGas();
   }
   showStatus();
}

//---------------------------------------------------------------------------


void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
   realTimeActions->Suspend();
   if(machineNavigator != NULL)
      machineNavigator->doStop();
}
//---------------------------------------------------------------------------




void __fastcall TForm1::BtnSpeedUpClick(TObject *Sender)
{
   if(machineNavigator != NULL)
      if(machineNavigator->highSpeed)
         return;
         
   if(delay < MAX_SPEED + SPEED_STEP )
      return;

   delay = delay - SPEED_STEP;

   if(machineNavigator != NULL)
      machineNavigator->mustBeDelay = delay ;

   speedVal = (MIN_SPEED - (double)delay)/(MIN_SPEED - MAX_SPEED) *100;
   LabelSpeed->Caption = IntToStr(speedVal);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BtnSpeedDownClick(TObject *Sender)
{
   if(machineNavigator != NULL)
      if(machineNavigator->highSpeed)
         return;

   if(delay > MIN_SPEED - SPEED_STEP )
      return;

   delay = delay + SPEED_STEP;
   if(machineNavigator != NULL)
      machineNavigator->mustBeDelay = delay ;

   speedVal = (MIN_SPEED - (double)delay)/(MIN_SPEED - MAX_SPEED) *100;
   LabelSpeed->Caption = IntToStr(speedVal);
   
}
//---------------------------------------------------------------------------


void __fastcall TForm1::BtnHighSpeedClick(TObject *Sender)
{
   if(machineNavigator == NULL)
   {
      realTimeActions->highSpeed = !realTimeActions->highSpeed;
      showStatus();
      return;
   }

   if(machineNavigator->isInFreeLine())
      return;
   devices->stopGas();
   machineNavigator->highSpeed= !machineNavigator->highSpeed;
   realTimeActions->highSpeed = machineNavigator->highSpeed;
   showStatus();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::setModel(CuttingModel * cm, CuttingModel* vm, double sc, AnsiString s)
{
   showMapX = showMapY = 0;
   Caption = "CNCC -- File Name: "+s;
   scale = sc;

   cuttingModel = cm;
   viewingModel = vm;
   
   TPoint cPoint = cuttingModel->getStartPoint();
   TPoint vPoint = viewingModel->getStartPoint();
   xCenter = vPoint.x - cPoint.x * scale;
   yCenter = vPoint.y - cPoint.y * scale;
   
   double dv = (params.dx+params.dy) /4;
   int K = (1.0 / scale) / dv;
   if(K<=0) K = 1;
   ImageMap->Picture->Graphic = u.drawModel(viewingModel,ImageMap->Width , ImageMap->Height);

   if(machineNavigator != NULL)
   {
      machineNavigator->freeMemory();
   }
   machineNavigator = new MachineNavigator(true,devices);

   BtnBackward->Enabled = true;
   BtnForward->Enabled = true;
   BtnStop->Enabled = true;

   if(devices->validate())
   {
      machineNavigator->importModel(cuttingModel);
      machineNavigator->setImageForm(K);
      delay = machineNavigator->mustBeDelay;
      int val2 = (MIN_SPEED - (double)delay)/(MIN_SPEED - MAX_SPEED) *100;
      LabelSpeed->Caption = IntToStr(val2);
      return;
   }
   ShowMessage("Please call with this number: +98-912-2274122");
}

void TForm1::resetImageMap(bool isForwrd)
{
   ImageMap->Picture->Graphic = u.drawModel(viewingModel,ImageMap->Width , ImageMap->Height);
   ImageMap->Canvas->Pen->Width = 2;
   isForward = isForwrd;
   if(machineNavigator != NULL)
   {
      machineNavigator->highSpeed = false;
      if(isForward)
         machineNavigator->setForward();
      else
         machineNavigator->setBackward();
   }
}

void __fastcall TForm1::stopGas()
{
   devices->stopGas();
}

void __fastcall TForm1::startGas()
{
   devices->startGas();
}

void __fastcall TForm1::highSpeed()
{
   machineNavigator->highSpeed = true;
}
void __fastcall TForm1::normalSpeed()
{
   machineNavigator->highSpeed = false;
}

//---------------------------------------------------------------------------

void __fastcall TForm1::BtnTorchUpMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    devices->torchDown();
}

//---------------------------------------------------------------------------
void __fastcall TForm1::BtnTorchDownMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    devices->torchUp();
}

//---------------------------------------------------------------------------
void __fastcall TForm1::BtnTorchUpMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   devices->torchStop();
}

//---------------------------------------------------------------------------

void __fastcall TForm1::BtnArrowUpMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if(machineNavigator != NULL)
      if(!(machineNavigator->Suspended))
         return;
   if(realTimeActions->yDir == 1)
      realTimeActions->yDir = 0;
   else
      realTimeActions->yDir = 1;

   realTimeActions->Resume();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BtnArrowRightMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if(machineNavigator != NULL)
      if(!(machineNavigator->Suspended))
         return;

   if(realTimeActions->xDir == -1)
      realTimeActions->xDir = 0;
   else
      realTimeActions->xDir = -1;
   if(realTimeActions->Suspended)
        realTimeActions->Resume();

}
//---------------------------------------------------------------------------

void __fastcall TForm1::BtnArrowDownMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if(machineNavigator != NULL)
      if(!(machineNavigator->Suspended))
         return;
   if(realTimeActions->yDir == -1)
      realTimeActions->yDir = 0;
   else
      realTimeActions->yDir = -1;

   realTimeActions->Resume();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BtnArrowLeftMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if(machineNavigator != NULL)
      if(!(machineNavigator->Suspended))
         return;

   if(realTimeActions->xDir == 1)
      realTimeActions->xDir = 0;
   else
      realTimeActions->xDir = 1;

   realTimeActions->Resume();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BtnArrowLeftMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   realTimeActions->Suspend();
   realTimeActions->xDir = 0;
   realTimeActions->yDir = 0;
   devices->motorFree();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------


void __fastcall TForm1::ShowCuttingMapTimerTimer(TObject *Sender)
{
   showStatus();
   if(showMapX == 0 && showMapY == 0)
      return;
   int height = ImageMap->Height;
   ImageMap->Canvas->Pen->Color = showMapcol;
   ImageMap->Canvas->LineTo(showMapX*scale + xCenter,height - (showMapY*scale + yCenter));
}
//---------------------------------------------------------------------------


void TForm1::StopShow()
{
   ShowCuttingMapTimer->Enabled = false;
}
void __fastcall TForm1::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{

   switch(Key)
   {
      case  109: //Torch Up
      case 45:
         devices->torchDown();

      break;
      case  107: //Torch Down
      case 46:
         devices->torchUp();
      break;
      case  106: //Gas
      case 35:
         BtnGas->Click();
      break;
      case  96: //Stop
      case 32:
      case 88:

         BtnStop->Click();
      break;
      case  104: //UP
      case 83:
          if(machineNavigator != NULL)
            if(!(machineNavigator->Suspended))
               return;
         realTimeActions->yDir = -1;
         realTimeActions->Resume();

      break;
      case  102: //Right
      case 68:
         if(machineNavigator != NULL)
            if(!(machineNavigator->Suspended))
               return;

         realTimeActions->xDir = -1;

         realTimeActions->Resume();
      break;
      case  98: //Down
      case 87:
         if(machineNavigator != NULL)
            if(!(machineNavigator->Suspended))
               return;
         realTimeActions->yDir = 1;
         realTimeActions->Resume();
      break;
      case  100: //Left
      case 65:
         if(machineNavigator != NULL)
            if(!(machineNavigator->Suspended))
               return;

         realTimeActions->xDir = 1;

         realTimeActions->Resume();
      break;
      case 33: //Speed+
         BtnSpeedUp->Click();
      break;
      case 34: //Speed-
         BtnSpeedDown->Click();
      break;
      case 103: //Backward
      case 188:
      case 90:
         if(BtnBackward->Enabled)
            BtnBackward->Click();
      break;
      case 105: //Forward
      case 190:
      case 67:
         if(BtnForward->Enabled)
            BtnForward->Click();
      break;
      case 111: //High Speed
      case 36:
         BtnHighSpeed->Click();
      break;
   }
}


void __fastcall TForm1::FormKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
   switch(Key)
   {
      case 45:
      case 46:
      case  109: //Torch Up
      case  107: //Torch Down
         devices->torchStop();
      break;
      case  104: //UP
      case  102: //Right
      case  98: //Down
      case  100: //Left
      case 87:
      case 68:
      case 83:
      case 65:
         realTimeActions->SetDirection(DIRECTION_STOP);
         devices->motorFree();
      break;


   }
}
//---------------------------------------------------------------------------
void TForm1::showStatus()
{
   if(realTimeActions != NULL)
   {
      if(realTimeActions->highSpeed)
         pnlSpeed->Color = clBlue;
      else
         pnlSpeed->Color = clSilver;
   }
   if(devices != NULL)
   {
      if(devices->gasIsOn())
         pnlGas->Color = clRed;
      else
         pnlGas->Color = clSilver;

      if(devices->motorIsFree())
         pnlMotorFree->Color = clSilver;
      else
         pnlMotorFree->Color = clRed;
   }
}
int yDirection = 0 , timer = 0, torchStatus = 0;
void __fastcall TForm1::TimerGamePadTimer(TObject *Sender)
{
   //Check Buttons
   if(__oldButtonState[4] != gamepad->PressedButtons.Contains(joButton5))
   {
      //backward
      if(__oldButtonState[4])
      {
          __oldButtonState[4] = false;
      }
      else
      {
         //Pressed
         __oldButtonState[4] = true;
          if(BtnBackward->Enabled)
            BtnBackward->Click();

      }
   }
   if(__oldButtonState[5] != gamepad->PressedButtons.Contains(joButton6))
   {
      //Forward
      if(__oldButtonState[5])
      {
          __oldButtonState[5] = false;
      }
      else
      {
         //Pressed
          __oldButtonState[5] = true;
          if(BtnForward->Enabled)
            BtnForward->Click();
      }
   }
   //Check POV
   if(__oldPOV != gamepad->PointOfView)
   {
      __oldPOV = gamepad->PointOfView;
      Navigate4Way(__oldPOV);
   }
   
   //Check Axis
   switch(yDirection)
   {
      case 1:
         if(timer++>10)
         {
            BtnSpeedUp->Click();
            timer = 0;
         }
         break;
      case -1:
         if(timer++>10)
         {
            BtnSpeedDown->Click();
            timer = 0;
         }
         break;
   }
   int u = gamepad->PositionU ;
   if( u > 65000)
   {
      if(torchStatus != -1)
      {
         devices->torchDown();
         torchStatus = -1;
      }
   }
   else if( u < 500)
   {
      if(torchStatus != 1)
      {
         devices->torchUp();
         torchStatus = 1;
      }
   }
   else
   {
      if(torchStatus != 0)
      {
         devices->torchStop();
         torchStatus = 0;
      }
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Navigate4Way(int degree)
{
   if(machineNavigator != NULL)
      if(!(machineNavigator->Suspended))
         return;
         
   realTimeActions->SetDirection(degree);
   if(degree == DIRECTION_STOP)
      realTimeActions->Suspend();
   else
      realTimeActions->Resume();
}

void __fastcall TForm1::gamepadMove(TObject *Sender, TButtons Pressed,
      int X, int Y)
{
   if(Y == 0 )
   {
      if(yDirection != 1)
      {
         BtnSpeedUp->Click();
         yDirection = 1;
         timer = 0;
      }
   }
   else if(Y == 65535)
   {
      if(yDirection != -1)
      {
         BtnSpeedDown->Click();
         yDirection = -1;
         timer = 0;
      }
   }
   else
       yDirection = 0;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::gamepadButtonDown(TObject *Sender,
      TButtons Changed, TButtons Pressed, int X, int Y)
{
   if(Pressed.Contains(joButton1))
   {
         __oldButtonState[0] = true;

         ShowCuttingMapTimer->Enabled = false;
         if(machineNavigator != NULL)
         {
            if(!(machineNavigator->isInFreeLine()))
               machineNavigator->highSpeed = false;
            machineNavigator->doStop();
         }
         realTimeActions->SetDirection(DIRECTION_STOP);
         devices->motorPause();
         showStatus();
   }
   
   if(Pressed.Contains(joButton2))
   {
      BtnGas->Click();
   }
   if(Pressed.Contains(joButton3))
   {
      BtnHighSpeed->Click();
   }
   if(Pressed.Contains(joButton4))
   {
         ShowCuttingMapTimer->Enabled = false;
         if(machineNavigator != NULL)
         {
            if(!(machineNavigator->isInFreeLine()))
               machineNavigator->highSpeed = false;
            machineNavigator->doStop();
         }
         realTimeActions->SetDirection(DIRECTION_STOP);
         devices->motorFree();
         showStatus();
   }
}
//-------------------------------------------------------------------


void __fastcall TForm1::Open1Click(TObject *Sender)
{
   BtnImport->Click();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Exit1Click(TObject *Sender)
{
   Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Mode1Click(TObject *Sender)
{
   Menu1->Items[0].Checked = true;   
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Setting1Click(TObject *Sender)
{
   Form6->Show();
}
//---------------------------------------------------------------------------

