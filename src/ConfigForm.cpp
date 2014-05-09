//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ConfigForm.h"
#include "Parameters.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

Parameters pars;
TFileStream *cfg;

//---------------------------------------------------------------------------
__fastcall TForm6::TForm6(TComponent* Owner, TFileStream *cfg2)
   : TForm(Owner)
{

   cfg = cfg2;
   cfg->Seek(0,0);
   unsigned char buff[] = { 5, 28, 2};
   cfg->Read(buff,3);
   cfg->Read(&(pars.dx),sizeof(pars.dx));
   cfg->Read(&(pars.dy),sizeof(pars.dy));
   cfg->Read(&(pars.forwardCommandX),sizeof(pars.forwardCommandX));
   cfg->Read(&(pars.forwardCommandY),sizeof(pars.forwardCommandY));
}
//---------------------------------------------------------------------------
void __fastcall TForm6::Button1Click(TObject *Sender)
{
      cfg->Seek(0,0);
      unsigned char buff[] = { 5, 28, 2};
      cfg->Read(buff,3);
      pars.dx = StrToFloat(txtX->Text);
      pars.dy = StrToFloat(txtY->Text);
      cfg->Write(&(pars.dx),sizeof(pars.dx));
      cfg->Write(&(pars.dy),sizeof(pars.dy));
      cfg->Write(&(pars.forwardCommandX),sizeof(pars.forwardCommandX));
      cfg->Write(&(pars.forwardCommandY),sizeof(pars.forwardCommandY));

      Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm6::FormCreate(TObject *Sender)
{
   cmbX->ItemIndex = 0;
   cmbY->ItemIndex = 0;
   txtX->Text = FloatToStr(pars.dx);
   txtY->Text = FloatToStr(pars.dy);
   UpdateShow();
}
//---------------------------------------------------------------------------
void __fastcall TForm6::btnXClick(TObject *Sender)
{
   double error = StrToFloat(txtErrorX->Text);
   double base = StrToFloat(txtBaseX->Text);

   if(cmbX->ItemIndex == 0)
   {
      pars.dx += error/base * pars.dx;
   }
   else
   {
      pars.dx -= error/base * pars.dx;
   }

   txtX->Text = FloatToStr(pars.dx);
   txtY->Text = FloatToStr(pars.dy);

   txtErrorX ->Text = "";
   txtBaseX->Text = "";
}
//---------------------------------------------------------------------------
void __fastcall TForm6::btnYClick(TObject *Sender)
{
   double error = StrToFloat(txtErrorY->Text);
   double base = StrToFloat(txtBaseY->Text);

   if(cmbY->ItemIndex == 0)
   {
      pars.dy += error/base * pars.dy;
   }
   else
   {
      pars.dy -= error/base * pars.dy;
   }

   txtX->Text = FloatToStr(pars.dx);
   txtY->Text = FloatToStr(pars.dy);

   txtErrorY ->Text = "";
   txtBaseY->Text = "";
}
//---------------------------------------------------------------------------
void __fastcall TForm6::btnCancelClick(TObject *Sender)
{
   Close();   
}
//---------------------------------------------------------------------------




void __fastcall TForm6::btnChangXClick(TObject *Sender)
{
   pars.forwardCommandX = -pars.forwardCommandX;
   UpdateShow();
}
//---------------------------------------------------------------------------

void __fastcall TForm6::btnChangeYClick(TObject *Sender)
{
   pars.forwardCommandY = -pars.forwardCommandY;
   UpdateShow();
}
//---------------------------------------------------------------------------


void TForm6::UpdateShow()
{
   imgXL->Visible = (pars.forwardCommandX == 1);
   imgXR->Visible = (pars.forwardCommandX == -1);
   imgYU->Visible = (pars.forwardCommandY == 1);
   imgYD->Visible = (pars.forwardCommandY == -1);

}
