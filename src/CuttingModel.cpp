//---------------------------------------------------------------------------
// A class for having Cutting plane.
//
// Producer name: Saeed Shariati
// e-mail address: Saeed_Shariati@comp.iust.ac.ir
//---------------------------------------------------------------------------

#pragma hdrstop

#include "CuttingModel.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

CuttingModel::CuttingModel()
{
   polyLines = new TList();
}

CuttingModel::~CuttingModel()
{
   polyLines->Clear();
   delete polyLines;
}

void CuttingModel::addPolyLine()
{
   polyLines->Add(new MNPolyLine());
}

void __fastcall CuttingModel::addPolyLine(MNPolyLine *pol)
{
   polyLines->Add(pol->clone());
}

double CuttingModel::getTotalLenght()
{
   double modelTotalLenght = 0;
   int pCount = polyLines->Count;
   for (int pIndex = 0; pIndex < pCount ; pIndex++)  // For Each PolyLine
   {
      MNPolyLine* pTemp = (MNPolyLine*) polyLines->Items[pIndex];
      modelTotalLenght += pTemp->getTotalLenght();
   }
   return modelTotalLenght;
}

void __fastcall CuttingModel::setToGrid(double dx, double dy)
{
   int pCount = polyLines->Count;
   for (int pIndex = 0; pIndex < pCount ; pIndex++)  // For Each PolyLine
   {
      MNPolyLine* pTemp = (MNPolyLine*) polyLines->Items[pIndex];
      pTemp->setToGrid(dx,dy);
   }
}

void __fastcall CuttingModel::replacement(double x0, double y0, double scale)
{
   int pCount = polyLines->Count;
   for (int pIndex = 0; pIndex < pCount ; pIndex++)  // For Each PolyLine
   {
      MNPolyLine* pTemp = (MNPolyLine*) polyLines->Items[pIndex];
      pTemp->replacement(x0,y0,scale);
   }
}

CuttingModel*  CuttingModel::clone()
{
   CuttingModel * model = new CuttingModel();

   int pCount = polyLines->Count;
   for (int pIndex = 0; pIndex < pCount ; pIndex++)  // For Each PolyLine
   {
      MNPolyLine* pTemp = (MNPolyLine*) polyLines->Items[pIndex];
      MNPolyLine* pTemp2 = pTemp->clone();
      model->addPolyLine(pTemp2);
   }
   return model;
}

TRect CuttingModel::getBound()
{
   TRect rec;
   int pCount = polyLines->Count;
   if(pCount > 0)
   {
      MNPolyLine* pTemp = (MNPolyLine*) polyLines->Items[0];
      rec = pTemp->getBound();
   }
   for (int pIndex = 1; pIndex < pCount ; pIndex++)  // For Each PolyLine
   {
      MNPolyLine* pTemp = (MNPolyLine*) polyLines->Items[pIndex];
      TRect recTemp = pTemp->getBound();

      if(rec.left > recTemp.left)
         rec.left = recTemp.left;
      if(rec.right < recTemp.right)
         rec.right = recTemp.right;
      if(rec.top > recTemp.top)
         rec.top = recTemp.top;
      if(rec.bottom < recTemp.bottom)
         rec.bottom = recTemp.bottom;
   }
   return rec;
}

void __fastcall CuttingModel::rotaion(double angle,TPoint startPoint)
{
   int pCount = polyLines->Count;
   for (int pIndex = 0; pIndex < pCount ; pIndex++)  // For Each PolyLine
   {
      MNPolyLine* pTemp = (MNPolyLine*) polyLines->Items[pIndex];
      pTemp->rotaion(angle,startPoint);
   }
}


TPoint CuttingModel::getStartPoint()
{
   int pCount = polyLines->Count;
   if ( pCount > 0)
   {
      MNPolyLine* pTemp = (MNPolyLine*) polyLines->Items[0];
      return pTemp->getStartPoint();
   }
   return TPoint(0, 0);
}

void CuttingModel::mirror(bool vertical, double baseLine)
{
   int pCount = polyLines->Count;
   for (int pIndex = 0; pIndex < pCount ; pIndex++)  // For Each PolyLine
   {
      MNPolyLine* pTemp = (MNPolyLine*) polyLines->Items[pIndex];
      pTemp->mirror(vertical,baseLine);
   }
}
