//---------------------------------------------------------------------------

#ifndef MNVertexH
#define MNVertexH

#include <math.h>
//---------------------------------------------------------------------------

class MNVertex{

private:
   double x;
   double y;
   double bulge;  /*
                        angle= atan(|bulge|)*4
                        if(bulge<0)
                           clockwise
                        else
                           anticlockwise
                        bulge value is for an arc between this vertex
                        and next vertex
                  */
public:
   void setX(double value);
   double getX();
   void setY(double value);
   double getY();
   void setBulge(double value);
   double getBulge();
   MNVertex(double x, double y, double bulge);
   MNVertex* clone();
   void setToGrid(double dx, double dy);
};

#endif
