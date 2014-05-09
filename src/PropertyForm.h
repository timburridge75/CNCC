//---------------------------------------------------------------------------

#ifndef PropertyFormH
#define PropertyFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>

#include "VectorGraphic.h"
#include "Parameters.h"
#include "mainForm.h"
#include "Utility.h"
#include <ComCtrls.hpp>

//---------------------------------------------------------------------------
class TForm2 : public TForm
{
__published:	// IDE-managed Components
   TOpenDialog *OpenDialog1;
   TPanel *PanelImage;
   TImage *ImageMap;
   TGroupBox *GroupBox1;
   TBitBtn *BitBtn2;
   TBitBtn *BitBtn1;
   TGroupBox *GroupBox2;
        TButton *btnZoomIN;
        TButton *btnZoomOut;
        TButton *btnFit;
   TBitBtn *btnRotateManual;
   TGroupBox *GroupBox3;
   TBitBtn *btnHorMirror;
   TBitBtn *btnVerMirror;
   TBitBtn *btnOpen;
   TBitBtn *btnOK;
   TBitBtn *btnExit;
   TLabel *lblGage;
   TLabel *lblScale;
   TLabel *lblLenght;
   TLabel *Label1;
   void __fastcall FormDestroy(TObject *Sender);
   void __fastcall ImageMapMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall ImageMapMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall ImageMapMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
   void __fastcall BitBtn1Click(TObject *Sender);
   void __fastcall FormCreate(TObject *Sender);
   void __fastcall UpDown1Click(TObject *Sender, TUDBtnType Button);
   void __fastcall btnFitClick(TObject *Sender);
   void __fastcall btnZoomOutClick(TObject *Sender);
   void __fastcall BitBtn2Click(TObject *Sender);
   void __fastcall btnRotateManualClick(TObject *Sender);
   void __fastcall btnHorMirrorClick(TObject *Sender);
   void __fastcall btnVerMirrorClick(TObject *Sender);
   void __fastcall btnZoomINClick(TObject *Sender);
   void __fastcall btnOpenClick(TObject *Sender);
   void __fastcall btnOKClick(TObject *Sender);
   void __fastcall btnExitClick(TObject *Sender);

private:
   void __fastcall Zoom(double scale);
   void __fastcall ShowFit();
   CuttingModel *cuttingModel;
   CuttingModel *viewModel;
   CuttingModel *viewForm1Model;

   VectorGraphic *VG;
   double scale;
   TPoint startPoint;
   int k;
   Utility u;

   // 0 -> NO
   // 1 -> Move
   // 2 -> Rotate Manually Button pressed
   // 3 -> Rotate Manually
   int mouseDownMode;

public:		// User declarations
   __fastcall TForm2(TComponent* Owner);
   void setNewScale();
        void ShowFitViewModel();


};
//---------------------------------------------------------------------------
extern PACKAGE TForm2 *Form2;
//---------------------------------------------------------------------------
#endif
