//---------------------------------------------------------------------------

#ifndef CuttingModelH
#define CuttingModelH

#include "MNPolyLine.h"
#include <classes.hpp>

//---------------------------------------------------------------------------
class CuttingModel{

private:


public:
   TList *polyLines;
   CuttingModel();
   ~CuttingModel();
   void addPolyLine(); // Create and add a polyline
   void __fastcall addPolyLine(MNPolyLine *pol);
   double getTotalLenght(); // Copy by value, delete pol after work
   void __fastcall setToGrid(double dx, double dy);
   void __fastcall replacement(double x0, double y0, double scale);
   CuttingModel* clone();
   TRect getBound();
   void __fastcall rotaion(double angle,TPoint startPoint);
   TPoint startPoint;
   TPoint getStartPoint();
   void mirror(bool vertical, double line);
};
#endif
 