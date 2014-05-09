//---------------------------------------------------------------------------

#pragma hdrstop

#include "MachineNavigator.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

int counterK = 1;
unsigned long val = 0;

//Timer Initialization
UINT uDelay = 1;
UINT uResolution = 1;
DWORD dwUser = NULL;
UINT fuEvent = TIME_PERIODIC; 
MMRESULT FTimerID;
double __dx,__dy;
char __commandX , __commandY;
void CALLBACK TimeProc(UINT uTimerID, UINT uMsg,
                        DWORD dwUser, DWORD dw1, DWORD dw2)
{
   //Timer Interrupt
}

__fastcall MachineNavigator::MachineNavigator(bool CreateSuspended,Devices * dev
                     ): TThread(CreateSuspended)
{
   Priority = tpTimeCritical;
   delay = NO_FLICK_START_SPEED;
   mustBeDelay = initialDelay;
   forward = true;
   devices = dev;
   inFreeLine = false;
   highSpeed = false;

   timeBeginPeriod(1);
   FTimerID = timeSetEvent(uDelay, uResolution, TimeProc, dwUser, fuEvent);
   if(FTimerID==NULL){
      ShowMessage("Failed to generate multimedia timer.\n");
   }
   __dx = Form1->params.dx;
   __dy = Form1->params.dy;
   __commandX = Form1->params.forwardCommandX;
   __commandY = Form1->params.forwardCommandY;
}

void __fastcall MachineNavigator::Execute()
{
   double x,y,r,X0,X1,X2,X3,Y0,Y1,Y2,Y3;

   while(1)
   {
      int polyLineIndex = 0;
      signed long long i = 0;
      MotorCommand motorCommand;
      Form1->resetImageMap(forward);
      if(forward)
      {
         xLocation = startMapX;
         yLocation = startMapY;
      }
      else
      {
         xLocation = endMapX;
         yLocation = endMapY;
         if(i ==0)
            i = navArray.count-1;
      }

      Form1->setStartPoint(xLocation,yLocation);

      // Do Command

      for (; i<navArray.count && i>= 0 ; )
      {
         if(highSpeed){
            Sleep(2);
         }
         else{
            // Change Speed
            Sleep(delay/1000);
            if( delay != mustBeDelay)
            {
               //if(delay + NO_FLICK_SPEED_STEP <= mustBeDelay )
               //   delay += NO_FLICK_SPEED_STEP;
               //else if(delay - NO_FLICK_SPEED_STEP >= mustBeDelay )
               //   delay -= NO_FLICK_SPEED_STEP;
               //else
                  delay = mustBeDelay;
            }
         }
         char command = navArray.array[i];

         command --;

         if(forward){
            i++;
         }else{
            command = (command + 4)%8;
            i--;
         }
         // Make Command & Set New Location
         switch(command)
         {
            case 0:
               xLocation += __dx;
               motorCommand.x = 1;
               motorCommand.y = 0;
               break;

            case 1:
               xLocation +=  __dx;
               yLocation -=  __dy;
               motorCommand.x = 1;
               motorCommand.y = -1;
               break;

            case 2:
               yLocation -=  __dy;
               motorCommand.x = 0;
               motorCommand.y = -1;
               break;

            case 3:
               xLocation -=  __dx;
               yLocation -=  __dy;
               motorCommand.x = -1;
               motorCommand.y = -1;
               break;

            case 4:
               xLocation -=  __dx;
               motorCommand.x = -1;
               motorCommand.y = 0;
               break;

            case 5:
               xLocation -=  __dx;
               yLocation +=  __dy;
               motorCommand.x = -1;
               motorCommand.y = 1;
               break;

            case 6:
               yLocation +=  __dy;
               motorCommand.x = 0;
               motorCommand.y = 1;
               break;

            case 7:
               xLocation +=  __dx;
               yLocation +=  __dy;
               motorCommand.x = 1;
               motorCommand.y = 1;
               break;
         }

         // Off Clock must be High
         if(motorCommand.x == 0)
            val = val & NOT_BIT4; // Not Clocked
         else
         {
            val = val | BIT4;
            //Set Direction
            if(motorCommand.x == __commandX)
               val = val | BIT5;
            else
               val = val & NOT_BIT5;
         }
         if(motorCommand.y == 0)
            val = val & NOT_BIT1; // Not Clocked
         else
         {
            val = val | BIT1;
            if(motorCommand.y == __commandY)
               val = val | BIT2;
            else
               val = val & NOT_BIT2;
         }

         devices->SendToMotor(val);

         if( motorCommand.y != 0 && motorCommand.x != 0)
            utility.microSleep(4000);   // Two axis navigation
         else
            utility.microSleep(400);

         val = val & NOT_BIT1;
         val = val & NOT_BIT4;
         
         devices->SendToMotor(val);

         if(K <= counterK)
         {
            counterK = 1;
            if(inFreeLine)
            {
               Form1->showMapcol  = COLOR_FREE_LINES;
            }
            else
            {
               if(!(devices->gasIsOn()))
                  Form1->showMapcol = COLOR_GAS_OFF;
               else
               {
                  if(forward)
                     Form1->showMapcol = COLOR_FORWARD;
                  else
                     Form1->showMapcol = COLOR_BACKWARD;
               }
            }
            Form1->showMapX = xLocation;
            Form1->showMapY = yLocation;
         }
         counterK++;
                      
         // Control Free Lines
         bool currentIsInPolyLine = InPolyLine(i);

         if(!inFreeLine)
         {
            if(!currentIsInPolyLine)
            {
               Form1->stopGas();
               Form1->highSpeed();
               toFreeLine();
            }
        }else{
            if(currentIsInPolyLine)
            {
               Form1->stopGas();
               Form1->normalSpeed();
               Suspend();

               bool nextIsInPolyLine = false;
               if(forward)
                  nextIsInPolyLine = InPolyLine(i+10);
               else
                  nextIsInPolyLine = InPolyLine(i-10);
               if(nextIsInPolyLine)
               {
                  toPolyLine();
                  Form1->normalSpeed();
               }
            }
         }
      }
      //devices->motorFree();
      Form1->showMapX = xLocation;
      Form1->showMapY = yLocation;
      delay = NO_FLICK_START_SPEED;
      devices->stopGas();
      Form1->StopShow();
      Suspend();
   }
   timeKillEvent(FTimerID);
   timeEndPeriod(1);
}

void MachineNavigator::importModel(CuttingModel *cm)
{
   model = cm;

   NavigationArray *navTemp = new NavigationArray(initialNavigationArray);
   //---------------------------------------- make navTemp

   int pCount = model->polyLines->Count;

   bool firstPoly = true;
   int sectionIndex=0;
   double x1 = 0, x2 = 0, y1 = 0, y2 = 0;
   
   for (int pIndex = 0; pIndex < pCount ; pIndex++)  // For Each PolyLine
   {
      MNPolyLine* pTemp = (MNPolyLine*) model->polyLines->Items[pIndex];
      int vCount = pTemp->countOfVertexes();
      if(vCount < 2)
         return;
      MNVertex* vTemp1 = pTemp->getVertex(0);
      MNVertex* vTemp2;
      vx  = vTemp1->getX();
      vy  = vTemp1->getY();

      if(firstPoly){
         // Set Start point
         startMapX = vx ;
         startMapY = vy ;
         firstPoly = false;
      }else{

         // Make Speed Line
         makeLineNavigation(x2,y2,vx,vy,navTemp);
         //----------------
      }

      navArray.startIndex[sectionIndex] = navTemp->getSize();
      navArray.polyCount = sectionIndex +1;
      
      for (int vIndex=1 ; vIndex < vCount ;vIndex++) // For each Vertex
      {
         vTemp2 = pTemp->getVertex(vIndex);

         x1 = vTemp1->getX();
         y1 = vTemp1->getY();
         x2 = vTemp2->getX();
         y2 = vTemp2->getY();

         vx = x2;
         vy = y2;

         // Start point is x1,y1 and Destination is x2,y2
         // +__dx  -->      p0     command  1
         // +__dx -__dy  -->  p1     command  2
         // -__dy  -->      p2     command  3
         // -__dx -__dy -->   p3     command  4
         // -__dx  -->      p4     command  5
         // -__dx +__dy -->   p5     command  6
         // +__dy  -->      p6     command  7
         // +__dx +__dy -->   p7     command  8

         if(vTemp1->getBulge() != 0)     // It's an Arc
         {
            double vbulge = atan(vTemp1->getBulge())*4 ;  //Main Arc Degree
            makeArcNavigation(x1,y1,x2,y2,(vbulge) ,vbulge >= 0,navTemp);
         }
         else //It's a Line
         {
            makeLineNavigation(x1,y1,x2,y2,navTemp);
         }

         vTemp1 = vTemp2;
         endMapX = x2;
         endMapY = y2;
      }
      navArray.endIndex[sectionIndex] = navTemp->getSize()-1;
      sectionIndex ++;
   }
   navArray.count = navTemp->getSize();
   navArray.array = navTemp->getItemsList();
   delete navTemp;
}

double MachineNavigator::getDistFromPoint(double x1, double y1, double x2, double y2)
{
   return sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
}

double MachineNavigator::getDistFromCircle(double x1, double y1, double x2, double y2, double r)
{
   double dist = sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
   double res = dist - r;
   if(res < 0)
      res = -res;
   return res;
}

double MachineNavigator::getDegreeDiffFromLine(double x1, double y1, double x2, double y2, double r, double currentLocX , double currentLocY)
{
   double L = getDistFromLine(currentLocX,currentLocY,x1,y1,x2,y2);
   return asin(L/r);
}

double MachineNavigator::getDistFromLine(double x , double y, double x1, double y1, double x2, double y2)
{
   double A,B,C;
   if(x1!=x2)
	 {
		 A = (y2-y1)/(x2-x1);
		 B = -1;
		 C = y2 - A*x2;
	 }
	 else
	 {
		 B=0;
		 A=1;
		 C=-x2;
    }
    double temp = A*x+B*y+C;
    if(temp < 0)
      temp = - temp;
   return temp/sqrt(A*A + B*B);
}

void MachineNavigator::setImageForm(int k)
{
   K = k;
}

Polar MachineNavigator::cartezToPolar(double x, double y, double xo, double yo)
{
   Polar p;
   x = x - xo;
   y = y - yo;
   p.r = sqrt(x*x+y*y);
   p.theta = atan2(y,x);
   if(p.theta < 0)
      p.theta += M_PI * 2;
   p.theta = p.theta*180 / M_PI;
   return p;
}

double MachineNavigator::ghadreMotlagh(double x)
{
   if(x > 0)
      return x;
   return -x;
}
void MachineNavigator::freeMemory()
{
   delete[] navArray.array;
   FreeOnTerminate = true;
   Terminate();
}

void MachineNavigator::makeArcNavigation(double x1, double y1, double x2, double y2, double degRad, bool isClockwise, NavigationArray *navTemp)
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
   // x , y is related from c
   x = x1 - xc;
   y = y1 - yc;

   double rSquard = r*r;

   double locationDegree=0;
   double vbulge_180 = ghadreMotlagh(degRad* 180 / M_PI);

   double startPointDegree = atan2(y,x)*180/M_PI;

   double locationX = x+xc,locationY=y+yc;

   locationDegree = RoundTo(locationDegree,-10);
   vbulge_180 = RoundTo(vbulge_180,-10);

   double locationDegreeGrad = atan2(y,x);
   double circleCuttingBound =  __dx / __dy;
   double temp ;
   while(locationDegree < vbulge_180)
   {    
      if( ghadreMotlagh(temp = tan(locationDegreeGrad)) <= circleCuttingBound )// ghadreMotlagh(y) <= ghadreMotlagh(x))    // for y inc
      {
         for (; ghadreMotlagh(tan(locationDegreeGrad)) <= circleCuttingBound && locationDegree < vbulge_180 ; )
         {
            // Set __dx and __dy
            if(y >= 0)
               MyDX = - __dx;
            else
               MyDX =  __dx;

            if(x >= 0)
               MyDY =  __dy;
            else
               MyDY = - __dy;
            if(!isClockwise)
            {
               MyDY = -MyDY;
               MyDX = -MyDX;
            }


            y += MyDY;

            double xTemp = sqrt(rSquard - y*y);
            double a = ghadreMotlagh(ghadreMotlagh(x+MyDX) -  xTemp ); //Cost For Do
            double b = ghadreMotlagh( ghadreMotlagh(x) -  xTemp ); //Cost For Not Do

            if( b > a)
            {              //Do
              // do{
                  if(isClockwise)
                  {
                     if(x >= 0 && y >= 0)
                        navTemp->addItem(6);
                     else if(x < 0 && y >= 0)
                        navTemp->addItem(4);
                     else if(x < 0 && y < 0)
                        navTemp->addItem(2);
                     else
                        navTemp->addItem(8);
                  }else{
                     if(x >= 0 && y >= 0)
                        navTemp->addItem(2);
                     else if(x < 0 && y >= 0)
                        navTemp->addItem(8);
                     else if(x < 0 && y < 0)
                        navTemp->addItem(6);
                     else
                        navTemp->addItem(4);
                  }
                  x += MyDX;
             //     a = ghadreMotlagh(ghadreMotlagh(x+MyDX) -  xTemp ); //Cost For Do
             //  }while(a );

            }
            else
            {              //Do Nothing
               if(isClockwise)
               {
                  if(x >= 0)
                      navTemp->addItem(7);
                  else
                      navTemp->addItem(3);
               }else{
                  if(x >= 0)
                      navTemp->addItem(3);
                  else
                      navTemp->addItem(7);
               }

            }

            // compute traced degree
            locationDegreeGrad = atan2(y,x);
            double localDegree = locationDegreeGrad*180/M_PI;
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
         for (; ghadreMotlagh(tan(locationDegreeGrad)) > circleCuttingBound  && locationDegree < vbulge_180; )
         {
            // Set __dx and __dy
            if(y >= 0)
               MyDX = - __dx;
            else
               MyDX =  __dx;

            if(x >= 0)
               MyDY =  __dy;
            else
               MyDY = - __dy;

            if(!isClockwise)
            {
               MyDY = -MyDY;
               MyDX = -MyDX;
            }
            x += MyDX;

            double yTemp = sqrt(rSquard - x*x);
            double a = ghadreMotlagh(ghadreMotlagh(y+MyDY) -  yTemp ); //Cost For Do
            double a2 = ghadreMotlagh(ghadreMotlagh(y+2*MyDY) -  yTemp ); //Cost For Do
            double b = ghadreMotlagh(ghadreMotlagh(y) -  yTemp ); //Cost For Not Do
            if( b > a)
            {              //Do
               if(isClockwise)
               {
                  if(x >= 0 && y >= 0)
                      navTemp->addItem(6);
                  else if(x < 0 && y >= 0)
                      navTemp->addItem(4);
                  else if(x < 0 && y < 0)
                      navTemp->addItem(2);
                  else
                      navTemp->addItem(8);
               }else{
                  if(x >= 0 && y >= 0)
                      navTemp->addItem(2);
                  else if(x < 0 && y >= 0)
                      navTemp->addItem(8);
                  else if(x < 0 && y < 0)
                      navTemp->addItem(6);
                  else
                      navTemp->addItem(4);
               }
               y += MyDY;
            }
            else
            {              //Do Nothing
               if(isClockwise)
               {
                  if( y >= 0)
                      navTemp->addItem(5);
                  else
                      navTemp->addItem(1);
               }else{
                  if( y >= 0)
                      navTemp->addItem(1);
                  else
                      navTemp->addItem(5);
               }          
            }

            // compute traced degree
            locationDegreeGrad = atan2(y,x);
            double localDegree = locationDegreeGrad*180/M_PI;
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

void MachineNavigator::makeLineNavigation(double x1, double y1, double x2, double y2, NavigationArray * navTemp)
{
     double Dx = x2 - x1;
     double Dy = y2 - y1;
     double locationY = y1,locationX = x1; 
     if(Dx == 0)    // Vertical Line
     {
        if(y1 < y2)
        {
           for(locationY = y1+ __dy; locationY <= y2 ;locationY +=  __dy )
                navTemp->addItem(7);
        }
        else
        {
           for(locationY = y1- __dy; locationY >= y2 ;locationY -=  __dy )
               navTemp->addItem(3);
        }
        return;
     }

     double m = Dy/Dx;
     double a1,a2,b1,b2;
     if(ghadreMotlagh(Dy) <= ghadreMotlagh(Dx) ) // |m|<=1
     {
        double MyDX =  __dx,MyDY =  __dy;

        double* lowLimit = &locationX;
        double* highLimit = &x2;
        if( x2 < x1 )
        {
           MyDX = -MyDX;
           lowLimit = &x2;
           highLimit = &locationX;
        }
        if( y2 < y1 )
           MyDY = -MyDY;

        for(locationX = x1+MyDX ; *lowLimit <= *highLimit ;locationX += MyDX )
        {
           double relativeLocXNew = locationX - x1;
           double relativeLocY = locationY - y1;

           double a = ghadreMotlagh((relativeLocY+MyDY) - m * relativeLocXNew );
           double b = ghadreMotlagh(m * relativeLocXNew - relativeLocY );
           if( b > a)
           {
              if(MyDY > 0)
                 if(MyDX > 0)
                     navTemp->addItem(8);
                 else
                     navTemp->addItem(6);
              else
                 if(MyDX > 0)
                     navTemp->addItem(2);
                 else
                     navTemp->addItem(4);
              locationY += MyDY;
           }else{
              if(MyDX > 0)
                  navTemp->addItem(1);
              else
                  navTemp->addItem(5);
           }
        }
     }
     else   // |m|>1
     {
        double MyDX =  __dx,MyDY =  __dy;

        double* lowLimit = &locationY;
        double* highLimit = &y2;
        if( y2 < y1 )
        {
           MyDY = -MyDY;
           lowLimit = &y2;
           highLimit = &locationY;
        }
        if( x2 < x1 )
           MyDX = -MyDX;

        for(locationY = y1+MyDY; *lowLimit <= *highLimit ;locationY += MyDY )
        {
           double relativeLocYNew = locationY - y1;
           double relativeLocX = locationX - x1;

           double a = ghadreMotlagh((relativeLocX+MyDX) - relativeLocYNew / m ); //Cost For Do
           double b = ghadreMotlagh( relativeLocYNew / m - relativeLocX );       // Cost For Not Do
           if( b > a)
           {           // Do
              if(MyDX > 0)
                 if(MyDY > 0)
                     navTemp->addItem(8);
                 else
                     navTemp->addItem(2);
              else
                 if(MyDY > 0)
                     navTemp->addItem(6);
                 else
                     navTemp->addItem(4);
              locationX += MyDX;
           }else{        // Do Nothing
              if(MyDY > 0)
                  navTemp->addItem(7);
              else
                  navTemp->addItem(3);
         }
      }
   }
}

bool inline __fastcall MachineNavigator::InPolyLine(long long i)
{
   bool isInPolyLine = false;
   for (int polyIndx = 0; polyIndx < navArray.polyCount ; polyIndx++)
   {
      if(i >= navArray.startIndex[polyIndx] && i <= navArray.endIndex[polyIndx])
      {
         isInPolyLine = true;
         break;
      }
   }
   return isInPolyLine;
}


void __fastcall MachineNavigator::toFreeLine()
{
   inFreeLine = true;
}

void __fastcall MachineNavigator::toPolyLine()
{
   inFreeLine = false;
}


void __fastcall MachineNavigator::setForward()
{
   forward = true;
}

void __fastcall MachineNavigator::setBackward()
{
   forward = false;
}

bool __fastcall MachineNavigator::isForward()
{
   return forward;
}

bool __fastcall MachineNavigator::isInFreeLine()
{
   return inFreeLine;
}


void MachineNavigator::doStop()
{
   delay = NO_FLICK_START_SPEED;
   if(!(this->Suspended))
         this->Suspend();
}

