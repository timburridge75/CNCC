//---------------------------------------------------------------------------

#ifndef mainFormH
#define mainFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include "PropertyForm.h"
#include "Utility.h"
#include "CNCControlKeyBoard.h"
#include <Buttons.hpp>
#include <Graphics.hpp>
#include "MachineNavigator.h"
#include "RealTimeActions.h"
#include "Devices.h"
#include "Splash.h"
#include "Parameters.h"
#include "ConfigForm.h"
#include <classes.hpp>
#include "Joystick.hpp"

//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
   TPanel *PanelImage;
   TImage *ImageMap;
   TBitBtn *BtnImport;
   TPanel *PanelKeys;
   TBitBtn *BtnSpeedUp;
   TBitBtn *BtnSpeedDown;
   TBitBtn *BtnTorchUp;
   TBitBtn *BtnTorchDown;
   TPanel *Panel1;
   TBitBtn *BtnArrowDown;
   TBitBtn *BtnArrowRight;
   TBitBtn *BtnArrowUp;
   TBitBtn *BtnArrowLeft;
   TPanel *Panel2;
   TBitBtn *BtnForward;
   TBitBtn *BtnStop;
   TBitBtn *BtnBackward;
   TImage *Image1;
   TImage *Image2;
   TPanel *Panel3;
   TImage *Image3;
   TLabel *LabelSpeed;
   TTimer *ShowCuttingMapTimer;
   TPanel *pnlSpeed;
   TBitBtn *BtnHighSpeed;
   TPanel *pnlGas;
   TBitBtn *BtnGas;
   TJoystick *gamepad;
   TTimer *TimerGamePad;
   TPanel *pnlMotorFree;
   TMainMenu *Menu1;
   TMenuItem *File1;
   TMenuItem *Open1;
   TMenuItem *Exit1;
   TMenuItem *Device1;
   TMenuItem *Setting1;
   TMenuItem *NavigationMode1;
   TMenuItem *Mode1;
   TMenuItem *Config1;
   void __fastcall FormDestroy(TObject *Sender);
   void __fastcall FormCreate(TObject *Sender);
   void __fastcall BtnImportClick(TObject *Sender);
   void __fastcall BtnForwardClick(TObject *Sender);
   void __fastcall BtnBackwardClick(TObject *Sender);
   void __fastcall BtnStopClick(TObject *Sender);
   void __fastcall BtnGasClick(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall BtnSpeedUpClick(TObject *Sender);
   void __fastcall BtnSpeedDownClick(TObject *Sender);
   void __fastcall BtnHighSpeedClick(TObject *Sender);
   void __fastcall BtnTorchUpMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
   void __fastcall BtnTorchDownMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
   void __fastcall BtnTorchUpMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall BtnArrowUpMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
   void __fastcall BtnArrowRightMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
   void __fastcall BtnArrowDownMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
   void __fastcall BtnArrowLeftMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
   void __fastcall BtnArrowLeftMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
   void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
   void __fastcall ShowCuttingMapTimerTimer(TObject *Sender);
   void __fastcall FormKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
   void __fastcall TimerGamePadTimer(TObject *Sender);
   void __fastcall gamepadMove(TObject *Sender, TButtons Pressed, int X,
          int Y);
   void __fastcall gamepadButtonDown(TObject *Sender, TButtons Changed,
          TButtons Pressed, int X, int Y);
   void __fastcall Open1Click(TObject *Sender);
   void __fastcall Exit1Click(TObject *Sender);
   void __fastcall Mode1Click(TObject *Sender);
   void __fastcall Setting1Click(TObject *Sender);

private:
   void showStatus();
   void __fastcall Navigate4Way(int degree);

   	// User declarations
public:		// User declarations
   unsigned long long delay;
   double showMapX, showMapY;
   TColor showMapcol;
   __fastcall TForm1(TComponent* Owner);
   int speedVal;
   void __fastcall setStartPoint(double x, double y);
   void __fastcall drawVertex(int x, int y);
   void __fastcall reportBugs(String b);
   void __fastcall aKeyPressed(BYTE b);
   unsigned long long speed;
   bool isForward;
   void __fastcall setModel(CuttingModel * cm, CuttingModel* vm, double sc, AnsiString s);
   void resetImageMap(bool isForward);
   void __fastcall stopGas();
   void __fastcall startGas();
   void __fastcall normalSpeed();
   void __fastcall highSpeed();
   void StopShow();
   Parameters params;

};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
