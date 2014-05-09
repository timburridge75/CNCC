//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PropertyForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm2 *Form2;
//---------------------------------------------------------------------------
AnsiString fileName ="";
__fastcall TForm2::TForm2(TComponent* Owner)
   : TForm(Owner)
{
   k = 0;
   startPoint.x = 0;
   startPoint.y = 0;
   PanelImage->DoubleBuffered = true;
   mouseDownMode = 0;
}
//---------------------------------------------------------------------------

void __fastcall TForm2::FormDestroy(TObject *Sender)
{
   delete VG;
}
//---------------------------------------------------------------------------

void __fastcall TForm2::ShowFit()
{
     if(viewModel == NULL)
         return;
     delete viewModel;
     viewModel = cuttingModel->clone();

     TRect bound = cuttingModel->getBound();

     double hScale = (bound.Right - bound.Left );
     if(hScale != 0)
         hScale = (double)(ImageMap->Width - 10) / hScale;
     else
         hScale = 1;

     double rScale = (bound.Bottom - bound.Top);
     if(rScale != 0)
         rScale = (double)(ImageMap->Height - 10) / rScale;
     else
         rScale = 1;
     scale = hScale<rScale?hScale:rScale;
     viewModel->replacement(0,0,scale);

     bound =  viewModel->getBound();
     int tx = - (bound.Right + bound.Left)/2 + (ImageMap->Width) /2 ;
     int ty = - (bound.Bottom + bound.Top)/2 + (ImageMap->Height) /2;
     viewModel->replacement(tx,ty,1);
     ImageMap->Picture->Graphic = u.drawModel(viewModel,ImageMap->Width , ImageMap->Height);
     setNewScale();
}


int xPress,yPress,xDown,yDown;
void __fastcall TForm2::ImageMapMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if(mouseDownMode == 0)
      mouseDownMode = 1;
   else if(mouseDownMode == 2)
      mouseDownMode = 3;

   xDown = xPress = X;
   yDown = yPress = Y;
}
//---------------------------------------------------------------------------

void __fastcall TForm2::ImageMapMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if(mouseDownMode == 1){
      startPoint.x += X - xDown;
      startPoint.y += yDown -Y ;
   }
   mouseDownMode = 0;
   ImageMap->Cursor = crSizeAll;
}
//---------------------------------------------------------------------------

void __fastcall TForm2::ImageMapMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
   if(mouseDownMode == 1)
   {
      if(viewModel == NULL)
         return;
      viewModel->replacement(X - xPress ,yPress - Y,1);
      u.drawModelDesigner(viewModel,ImageMap->Width , ImageMap->Height,(Graphics::TBitmap *)ImageMap->Picture->Graphic);
      xPress = X;
      yPress = Y;
   }
   else if (mouseDownMode ==3)
   {
      if(viewModel == NULL)
         return;
      double degree = (yDown - Y)/100.0;
      viewModel->rotaion(degree,TPoint(xPress,yPress));
      cuttingModel->rotaion(degree,TPoint(xPress,yPress));
      u.drawModelDesigner(viewModel,ImageMap->Width , ImageMap->Height,(Graphics::TBitmap *)ImageMap->Picture->Graphic);
      xDown = X;
      yDown = Y;
   }
}
//---------------------------------------------------------------------------


void __fastcall TForm2::BitBtn1Click(TObject *Sender)
{
   if(viewModel == NULL)
         return;
   viewModel->rotaion(M_PI_2,startPoint);
   cuttingModel->rotaion(M_PI_2,TPoint(0,0));

   TRect bound = viewModel->getBound();
   int tx = - (bound.Right + bound.Left)/2 + (ImageMap->Width) /2 ;
   int ty = - (bound.Bottom + bound.Top)/2 + (ImageMap->Height) /2;
   viewModel->replacement(tx,ty,1);
   ImageMap->Picture->Graphic = u.drawModel(viewModel,ImageMap->Width , ImageMap->Height);
}
//---------------------------------------------------------------------------

void __fastcall TForm2::FormCreate(TObject *Sender)
{
   k = DEFAULT_K;
}
//---------------------------------------------------------------------------

void __fastcall TForm2::UpDown1Click(TObject *Sender, TUDBtnType Button)
{
   Zoom(1);
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Zoom(double myScale)
{
     if(viewModel == NULL)
         return;
     viewModel->replacement(0,0,myScale);

     scale *= myScale;
     
     TRect bound =  viewModel->getBound();
     int tx = - (bound.Right + bound.Left)/2 + (ImageMap->Width) /2 ;
     int ty = - (bound.Bottom + bound.Top)/2 + (ImageMap->Height) /2;
     viewModel->replacement(tx,ty,1);
     ImageMap->Picture->Graphic = u.drawModel(viewModel,ImageMap->Width , ImageMap->Height);
     setNewScale();
}


void __fastcall TForm2::btnFitClick(TObject *Sender)
{
   ShowFit();
}
//---------------------------------------------------------------------------

void __fastcall TForm2::btnZoomOutClick(TObject *Sender)
{
   Zoom(.9);
}
//---------------------------------------------------------------------------


void __fastcall TForm2::BitBtn2Click(TObject *Sender)
{
   if(viewModel == NULL)
         return;
   viewModel->rotaion(-M_PI_2,startPoint);
   cuttingModel->rotaion(-M_PI_2,TPoint(0,0));
   TRect bound = viewModel->getBound();
   int tx = - (bound.Right + bound.Left)/2 + (ImageMap->Width) /2 ;
   int ty = - (bound.Bottom + bound.Top)/2 + (ImageMap->Height) /2;
   viewModel->replacement(tx,ty,1);
   ImageMap->Picture->Graphic = u.drawModel(viewModel,ImageMap->Width , ImageMap->Height);
}
//---------------------------------------------------------------------------


void __fastcall TForm2::btnRotateManualClick(TObject *Sender)
{
   mouseDownMode = 2;
   ImageMap->Cursor = crCross;   
}
//---------------------------------------------------------------------------

void __fastcall TForm2::btnHorMirrorClick(TObject *Sender)
{
   if(viewModel == NULL)
         return;
   cuttingModel->mirror(false, 0);
   ShowFit();
}
//---------------------------------------------------------------------------

void __fastcall TForm2::btnVerMirrorClick(TObject *Sender)
{
   if(viewModel == NULL)
         return;
   cuttingModel->mirror(true, 0);
   ShowFit();
}
//---------------------------------------------------------------------------


void __fastcall TForm2::btnZoomINClick(TObject *Sender)
{
   Zoom(1.1);   
}
//---------------------------------------------------------------------------

void __fastcall TForm2::btnOpenClick(TObject *Sender)
{
   if(VG != 0){
      delete VG;
   }
   try{
      if(OpenDialog1->Execute()){
         fileName = OpenDialog1->FileName;
         VG = new VectorGraphic(OpenDialog1->FileName);
         if(VG->model->polyLines->Count == 0)
         {
            ShowMessage("There is no polyLine");
            return;
         }
         cuttingModel = VG->model;
         //cuttingModel->setToGrid(dx,dy);   //?????????????????
         viewModel = cuttingModel->clone();
         ShowFit();
      }
   }catch(...){
      ShowMessage("Couldn't open file");
   }   
}
//---------------------------------------------------------------------------

void __fastcall TForm2::btnOKClick(TObject *Sender)
{
   if(k != 0 && viewModel != NULL)
   {
      Form1->setModel(cuttingModel, viewModel->clone(),scale,fileName);
   }
   Close();   
}
//---------------------------------------------------------------------------

void __fastcall TForm2::btnExitClick(TObject *Sender)
{
   Close();      
}
//---------------------------------------------------------------------------


void TForm2::setNewScale()
{
 //  int scaleNumer =(int)(lblGage->Width/(double)scale*10);
  // lblScale->Caption = IntToStr(scaleNumer)+ " mm";
   double lenght = cuttingModel->getTotalLenght();
   lblLenght->Caption = IntToStr((int)(lenght/1000))+ " mm";

}

void TForm2::ShowFitViewModel()
{
   
}
