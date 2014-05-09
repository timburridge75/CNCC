//---------------------------------------------------------------------------


#pragma hdrstop

#include "Utility.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

void Utility::sleep(unsigned int secounds)
{
   _LARGE_INTEGER lStart,l1 ;
   QueryPerformanceCounter(&lStart);
   lStart.QuadPart += secounds* qpPerSecounds;
   while(1)
   {
      QueryPerformanceCounter(&l1);
      if(l1.QuadPart >= lStart.QuadPart)
         return;
   }
}

Utility::Utility()
{
   _LARGE_INTEGER l1;
   QueryPerformanceFrequency(&l1);
   qpPerSecounds = l1.QuadPart;
   qpPerMiliSecounds = qpPerSecounds/ 1000;
   qpPerMicroSecounds = qpPerSecounds/1000000;
}

void Utility::miliSleep(unsigned long long miliSecounds)
{
   _LARGE_INTEGER lStart,l1 ;
   QueryPerformanceCounter(&lStart);
   lStart.QuadPart += miliSecounds* qpPerMiliSecounds;
   while(1)
   {
      QueryPerformanceCounter(&l1);
      if(l1.QuadPart >= lStart.QuadPart)
         return;
   }
}

void Utility::microSleep(unsigned long long microSecounds)
{
   _LARGE_INTEGER lStart,l1 ;
   QueryPerformanceCounter(&lStart);
   lStart.QuadPart += microSecounds* qpPerMicroSecounds;
   while(1)
   {
      QueryPerformanceCounter(&l1);
      if(l1.QuadPart >= lStart.QuadPart)
         return;
   }
}

Graphics::TBitmap * Utility::drawModel(CuttingModel * model,int width, int height)
{
   Graphics::TBitmap *pic = new Graphics::TBitmap();

   pic->Width = width;
   pic->Height =height;
   pic->Canvas->Pen->Color = COLOR_MODEL_MAP;

   int count = model->polyLines->Count;
   double x,y,r,X0,X1,X2,X3,Y0,Y1,Y2,Y3,vx,vy;

   for(int j=0;j<count;j++)
   {
      // Draw PolyLine
      MNPolyLine *polyLine = (MNPolyLine *)model->polyLines->Items[j];
      int s = polyLine->countOfVertexes();
      bool firstPoint = true;
      bool isArc = false;
      double vx1=0,vy1=0,vbulge=0,vx2=0,vy2=0;
      for(int i=0;i<s;i++){
         MNVertex * v = polyLine->getVertex(i);

         vx = v->getX();
         vy = v->getY();

         if(firstPoint){
            if(j != 0) //Not in first polyLine
            {
               pic->Canvas->Pen->Color = COLOR_FREE_LINES;
               pic->Canvas->LineTo(vx,height - vy);
            }
            firstPoint = false;
            pic->Canvas->MoveTo(vx,height - vy);
            if(v->getBulge() == 0)
               isArc = false;
            else{
               isArc = true;
               vx1 = vx;
               vy1 = vy;
               vbulge = atan(v->getBulge())*4;
            }
         }else{
            if(isArc){
               vx2 = vx;
               vy2 = vy;
               drawArc(vx1,vy1,vx2,vy2,vbulge,vbulge >= 0,COLOR_MODEL_MAP,pic);
               pic->Canvas->MoveTo(vx2,height - vy2);
            }else{
               pic->Canvas->Pen->Color = COLOR_MODEL_MAP;
               pic->Canvas->LineTo(vx,height - vy);
            }
            if(v->getBulge() == 0)
               isArc = false;
            else{
               isArc = true;
               vbulge = atan(v->getBulge())*4;
            }
            vx1 = vx;
            vy1 = vy;
         }
      }
   }
   return pic;
}

void Utility::drawModelDesigner(CuttingModel * model,int width, int height,Graphics::TBitmap *pic)
{

   pic->Width = width;
   pic->Height =height;
   pic->Canvas->FillRect(TRect(0,0,width,height));
   pic->Canvas->Pen->Color = clGreen;

   int count = model->polyLines->Count;
   for(int j=0;j<count;j++)
   {
      double x,y,r,X0,X1,X2,X3,Y0,Y1,Y2,Y3,vx,vy;
      // Draw PolyLine
      MNPolyLine *polyLine = (MNPolyLine *)model->polyLines->Items[j];
      int s = polyLine->countOfVertexes();
      bool firstPoint = true;
      bool isArc = false;
      double vx1,vy1,vbulge;
      for(int i=0;i<s;i++){
         MNVertex * v = polyLine->getVertex(i);

         vx = v->getX();
         vy = v->getY();

         drawVertex(vx,vy,clBlack,pic);
         if(firstPoint){
            if(j != 0) //Not in first polyLine
            {
               pic->Canvas->Pen->Color = COLOR_FREE_LINES;
               pic->Canvas->LineTo(vx,height - vy);
            }
            firstPoint = false;
            pic->Canvas->MoveTo(vx,height - vy);
            if(v->getBulge() == 0)
               isArc = false;
            else{
               isArc = true;
               vx1 = vx;
               vy1 = vy;
               vbulge = atan(v->getBulge())*4;
            }
         }else{
            if(isArc){
               double vx2 = vx;
               double vy2 = vy;

               drawArc(vx1,vy1,vx2,vy2,vbulge,vbulge >= 0,clGreen,pic);
               pic->Canvas->MoveTo(vx2,height - vy2);
            }else{
               pic->Canvas->Pen->Color = clBlack;
               pic->Canvas->LineTo(vx,height - vy);
            }
            if(v->getBulge() == 0)
               isArc = false;
            else{
               isArc = true;
               vbulge = atan(v->getBulge())*4;
            }
            vx1 = vx;
            vy1 = vy;
         }
      }
   }
}
void Utility::drawArc(double x1, double y1, double x2, double y2, double degRad,  bool isClockwise,TColor col,Graphics::TBitmap *pic)
{
   // distance between two V1 , V2
   double D = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
   double r = D/2.0/sin(degRad/2.0);

   // Center Point of Line between V1, V2   => vxm , vym
   double vxm = (x2+x1)/2.0;
   double vym = (y2+y1)/2.0;
   double al = atan2((y2-y1),(x2-x1));

   // center of Circle cx , cy
   double X  =  D/2/tan(degRad/2.0);
   double xc = -X*cos(M_PI_2-al) + vxm;
   double yc =  X*sin(M_PI_2-al) + vym;

   double MyDX, MyDY;

   double x,y;
   x = x1 - xc;
   y = y1 - yc;

   double rSquard = r*r;

   double locationDegree=0;
   double vbulge_180 = ghadreMotlagh(degRad* 180 / M_PI);

   double startPointDegree = atan2(y,x)*180/M_PI;

   double locationX = x+xc,locationY=y+yc;

   while(locationDegree < vbulge_180)
   {
      if(ghadreMotlagh(y) <= ghadreMotlagh(x))    // for y inc
      {
         for (; ghadreMotlagh(y) <= ghadreMotlagh(x) && locationDegree < vbulge_180 ; )
         {
            pic->Canvas->Pixels[locationX][pic->Height - locationY]=col;
            if(y >= 0)
               MyDX = -1;
            else
               MyDX = 1;

            if(x >= 0)
               MyDY = 1;
            else
               MyDY = -1;
            if(!isClockwise)
            {
               MyDY = -MyDY;
               MyDX = -MyDX;
            }

            y += MyDY;

            double xTemp = sqrt(ghadreMotlagh(rSquard - y*y));
            double a = ghadreMotlagh(ghadreMotlagh(x+MyDX) -  xTemp ); //Cost For Do
            double b = ghadreMotlagh( ghadreMotlagh(x) -  xTemp ); //Cost For Not Do
            if( b > a)
               x += MyDX;

            // compute traced degree
            double localDegree = atan2(y,x)*180/M_PI;
            locationDegree = ghadreMotlagh(localDegree - startPointDegree);
            if(localDegree < startPointDegree && isClockwise)
               locationDegree = 360 - locationDegree;
            if(localDegree > startPointDegree && !isClockwise)
               locationDegree = 360 - locationDegree;

            locationX = x + xc;
            locationY = y + yc;

            locationDegree = RoundTo(locationDegree,-10);
         }
      }
      else          // for x inc
      {
         for (; ghadreMotlagh(y) > ghadreMotlagh(x)  && locationDegree < vbulge_180; )
         {
            pic->Canvas->Pixels[locationX][pic->Height - locationY]=col;

            // Set dx and dy
            if(y >= 0)
               MyDX = -1;
            else
               MyDX = 1;

            if(x >= 0)
               MyDY = 1;
            else
               MyDY = -1;

            if(!isClockwise)
            {
               MyDY = -MyDY;
               MyDX = -MyDX;
            }
            x += MyDX;

            double yTemp = sqrt(ghadreMotlagh(rSquard - x*x));
            double a = ghadreMotlagh(ghadreMotlagh(y+MyDY) -  yTemp ); //Cost For Do
            double b = ghadreMotlagh(ghadreMotlagh(y) -  yTemp ); //Cost For Not Do
            if( b > a)
               y += MyDY;
            // compute traced degree
            double localDegree = atan2(y,x)*180/M_PI;
            locationDegree = ghadreMotlagh(localDegree - startPointDegree);
            if(localDegree < startPointDegree && isClockwise)
               locationDegree = 360 - locationDegree;
            if(localDegree > startPointDegree && !isClockwise)
               locationDegree = 360 - locationDegree;

            locationX = x + xc ;
            locationY = y + yc ;
            
            locationDegree = RoundTo(locationDegree,-10);
         }
      }
   }
}

void Utility::drawVertex(int x, int y,TColor col, Graphics::TBitmap *pic)
{
   double height = pic->Height;
   pic->Canvas->Pen->Color = col;
   pic->Canvas->Rectangle(x-2,height-y-2,x+2,height-y+2);
}
