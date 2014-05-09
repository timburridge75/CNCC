//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "Parameters.h"
#include "Splash.h"
//#include "Password.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm3 *Form3;
//---------------------------------------------------------------------------
__fastcall TForm3::TForm3(TComponent* Owner)
   : TForm(Owner)
{
    Label3->Caption = Label3->Caption + ver;
   // Label8->Caption = Label8->Caption + lisence;
}
//---------------------------------------------------------------------------
void __fastcall TForm3::Timer1Timer(TObject *Sender)
{
   if(ProgressBar1->Position >= 100)
   {
      Close();
   }
   else
   {
      ProgressBar1->Position++;
   }
}
//---------------------------------------------------------------------------



void __fastcall TForm3::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
        /*if(Key == 119)
        {
               Timer1->Enabled = false;
               TForm5 *f = new TForm5(this, cfg);
               f->ShowModal();
               Timer1->Enabled = true;
        } */
}
//---------------------------------------------------------------------------

