//---------------------------------------------------------------------------

#ifndef ConfigFormH
#define ConfigFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TForm6 : public TForm
{
__published:	// IDE-managed Components
   TButton *Button1;
   TPageControl *tabControl;
   TTabSheet *TabSheet1;
   TTabSheet *TabSheet2;
   TLabel *Label1;
   TLabel *Label2;
   TLabel *Label3;
   TLabel *Label4;
   TLabel *Label5;
   TLabel *Label6;
   TEdit *txtX;
   TEdit *txtY;
   TEdit *txtErrorX;
   TEdit *txtBaseX;
   TComboBox *cmbX;
   TButton *btnX;
   TEdit *txtErrorY;
   TEdit *txtBaseY;
   TComboBox *cmbY;
   TButton *btnY;
   TButton *btnCancel;
   TButton *btnChangX;
   TButton *btnChangeY;
   TImage *imgYU;
   TImage *imgXR;
   TImage *imgXL;
   TImage *imgYD;
   void __fastcall Button1Click(TObject *Sender);
   void __fastcall FormCreate(TObject *Sender);
   void __fastcall btnXClick(TObject *Sender);
   void __fastcall btnYClick(TObject *Sender);
   void __fastcall btnCancelClick(TObject *Sender);
   void __fastcall btnChangXClick(TObject *Sender);
   void __fastcall btnChangeYClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
   __fastcall TForm6(TComponent* Owner, TFileStream *cfg2);
   void UpdateShow();
};
//---------------------------------------------------------------------------
//extern PACKAGE TForm6 *Form6;
//---------------------------------------------------------------------------
#endif
