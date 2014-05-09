//---------------------------------------------------------------------------
// A class for analysing Vector files such as DXF and DWG files and create
// CuttingModel.
//
// Producer name: Saeed Shariati
// e-mail address: Saeed_Shariati@comp.iust.ac.ir
//---------------------------------------------------------------------------

#ifndef VectorGraphicH
#define VectorGraphicH

#include "CuttingModel.h"

#include <Classes.hpp>
#include <StdCtrls.hpp>
#include <math.h>
//---------------------------------------------------------------------------

class VectorGraphic{
private:
   
   int iToken;                  // used for indexing
   TFileStream * f;
   AnsiString token;
   
   AnsiString getNextToken();
   void makeHeaders();
   void makeTables();
   void findPolylines();

   double scale,XAxesDistance,YAxesDistance;
   
   int gerdKardan(double x);

public:
   CuttingModel *model;
   
   // constractor
   VectorGraphic(AnsiString FileName);
   // destructor
   ~VectorGraphic();

};


#endif
 