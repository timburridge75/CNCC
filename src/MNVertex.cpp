//---------------------------------------------------------------------------


#pragma hdrstop

#include "MNVertex.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

void MNVertex::setX(double value)
{
   x=value;
}
double  MNVertex::getX()
{
   return x;
}
void MNVertex::setY(double value)
{
   y=value;
}
double  MNVertex::getY()
{
   return y;
}
void MNVertex::setBulge(double value)
{
   bulge=value;
}
double  MNVertex::getBulge()
{
   return bulge;
}

MNVertex::MNVertex(double x, double y, double bulge)
{
   this->x = x;
   this->y = y;
   this->bulge = bulge;
}

MNVertex* MNVertex::clone()
{
   return new MNVertex(x,y,bulge);
}

void MNVertex::setToGrid(double dx, double dy)
{
   x = floor(x / dx) * dx;
   y = floor(y / dy) * dy;
}
