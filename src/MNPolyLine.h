//---------------------------------------------------------------------------

#ifndef MNPolyLineH
#define MNPolyLineH
//---------------------------------------------------------------------------

#include "MNVertex.h"
#include <math.h>
#include <classes.hpp>
struct MNPolyLineNode{
   MNPolyLineNode* llink;
   MNPolyLineNode* rlink;
   MNVertex * value;
};

class MNPolyLine{

public:
   MNPolyLine();
   bool isEmpty() const;
   MNPolyLine* insertAtFirst(MNVertex * v);
   int countOfVertexes() const;
   void removeAtFirst();
   ~MNPolyLine();
   MNVertex * getVertex(int index) const;
   MNPolyLine* addVertex(MNVertex* v);
   MNPolyLine * clone();
   double getTotalLenght();
   void setToGrid(double dx, double dy);
   void replacement(double x0, double y0, double scale);
   __fastcall rotaion(double angle,TPoint startPoint);
   TRect getBound();
   TPoint getStartPoint();
   void mirror(bool vertical, double baseLine);
private:
   MNPolyLineNode* first;

};

#endif
