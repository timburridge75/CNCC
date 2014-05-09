//---------------------------------------------------------------------------

#ifndef UtilityH
#define UtilityH

#include <Controls.hpp>
#include "CuttingModel.h"
#include "Parameters.h"
#include <math.hpp>

//---------------------------------------------------------------------------
struct ArrayChar{
   char * array;
   long long count;
   long long startIndex[1000];
   long long endIndex[1000];
   int polyCount;
};

struct ArrayDouble{
   double * array;
   double val;
   long long count;
};
struct Cartez{
   double x;
   double y;
};
struct Polar{
   double r;
   double theta;
};

double ghadreMotlagh(double x)
{
   if(x < 0)
      return -x;
   return x;
}
class  Utility
{
private:
   unsigned long long qpPerSecounds;
   unsigned long long qpPerMicroSecounds;
   unsigned long long qpPerMiliSecounds;

public:
   void sleep(unsigned int secounds);
   Utility();
   void miliSleep(unsigned long long miliSecounds);
   void microSleep(unsigned long long microSecounds);
   Graphics::TBitmap * drawModel(CuttingModel * model,int width, int height);
   void drawArc(double x1, double y1, double x2, double y2, double degRad, bool isClockwise,TColor col, Graphics::TBitmap *pic);
   void drawVertex(int x, int y,TColor col, Graphics::TBitmap *pic);
   void drawModelDesigner(CuttingModel * model,int width, int height,Graphics::TBitmap *pic);

};
#endif
 