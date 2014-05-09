//---------------------------------------------------------------------------


#pragma hdrstop

#include "MNPolyLine.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


MNPolyLine::MNPolyLine()
{
   first = new MNPolyLineNode;
   first->value = 0;
   first->llink = 0;
   first->rlink = 0;

}

bool MNPolyLine::isEmpty() const
{
   return (first->llink == 0);
}

MNPolyLine* MNPolyLine::insertAtFirst(MNVertex * v)
{
   if(isEmpty())
      {
         MNPolyLineNode* p=new MNPolyLineNode;
         p->value = v->clone();
         p->llink = first;
         p->rlink = first;
         first->llink = p;
         first->rlink = p;
         return this;
      }else{
         MNPolyLineNode* p=new MNPolyLineNode;
         p->value = v->clone();
         p->llink = first;
         p->rlink = first->llink;
         first->llink->llink = p;
         first->llink = p;
         return this;
    }
}

int MNPolyLine::countOfVertexes() const
{
    MNPolyLineNode* A = first->llink;
    if(A == 0)
      return 0;
    int i=0;
    for(;A!=first;)
    {
   	i++;
      A=A->rlink;
    }
    return i;
}

void MNPolyLine::removeAtFirst()
{
   if(countOfVertexes()== 1)
   {
      delete first->llink ;
      first->llink = 0;
      first->rlink = 0;
      return ;
   }
   MNPolyLineNode* p = first->llink;
   MNPolyLineNode* next = p->rlink;
   first->llink = next;
   next-> llink = first;
   delete p;
   return;
}

MNPolyLine::~MNPolyLine()
{
   int s = countOfVertexes();
   for(int i=0;i<s;i++){
      removeAtFirst();
   }
}

MNVertex * MNPolyLine::getVertex(int index) const
{
   if(index >= countOfVertexes() || index < 0) return 0;
   MNPolyLineNode* temp = first->llink;
   for(int i=0;i<index;i++){
      temp = temp ->rlink;
   }
   return temp->value;
}

MNPolyLine* MNPolyLine::addVertex(MNVertex* v)
{
   if(isEmpty())
      {
         MNPolyLineNode* p=new MNPolyLineNode;
         p->value = v->clone();
         p->llink = first;
         p->rlink = first;
         first->llink = p;
         first->rlink = p;
         return this;
      }else{

         MNPolyLineNode* p=new MNPolyLineNode;
         p->value = v->clone();

         p->rlink = first;
         p->llink = first->rlink;
         first->rlink->rlink = p;
         first->rlink = p;
         return this;
    }
}

MNPolyLine * MNPolyLine::clone()
{
   MNPolyLine* p = new MNPolyLine();
   for(int i=0;i<countOfVertexes();i++){
      p->addVertex(getVertex(i));
   }
   return p;
}

TRect MNPolyLine::getBound()
{
   TRect rec;
   int count = countOfVertexes();
   MNVertex * v = NULL;
   if(count > 0)
   {
      v =  getVertex(0);
      rec.left = rec.right = v->getX();
      rec.top = rec.bottom = v->getY();
      if(v->getBulge() != 0)
      {
         int r  = 100;
      }
      for(int i=1;i<count;i++){
         v = getVertex(i);
         int x = v->getX();
         int y = v->getY();
         if(v->getBulge() != 0)
      {
         int r  = 100;
      }
         if(rec.left > x)
            rec.left = x;
         if(rec.right < x)
            rec.right = x;
         if(rec.top > y)
            rec.top = y;
         if(rec.bottom < y)
            rec.bottom = y;
      }
   }
   return rec;
}

double MNPolyLine::getTotalLenght()
{
   // Compute Total ModelLenght
   double modelTotalLenght = 0;

   int vCount = countOfVertexes();
   if(vCount < 2)
      return 0;

   MNVertex* vTemp1 = getVertex(0);
   MNVertex* vTemp2;
   for (int vIndex=1 ; vIndex < vCount ;vIndex++) // For each Vertex
   {
      vTemp2 = getVertex(vIndex);
      double x1 = vTemp1->getX();
      double y1 = vTemp1->getY();
      double x2 = vTemp2->getX();
      double y2 = vTemp2->getY();

      if(vTemp1->getBulge() == 0)     // It's a Line
      {
         double len = sqrt((x2 - x1)*(x2 - x1) +(y2 - y1)*(y2 - y1));
         modelTotalLenght += len;
      }
      else //It's an Arc
      {
         double vbulge = atan(vTemp1->getBulge())*4;
         
         // distance between two V1 , V2
         double D = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
         double r = D/2.0/sin(vbulge/2.0);

         double len = r * vbulge;
         if (len < 0)
         {
            len = -len;
         }
         modelTotalLenght += len;
      }
      vTemp1 = vTemp2;
   }
   return  modelTotalLenght;
}

void MNPolyLine::setToGrid(double dx, double dy)
{
   int vCount = countOfVertexes();
   for (int vIndex = 0; vIndex < vCount ; vIndex++)  // For Each Vertex
   {
      MNVertex* vTemp = getVertex(vIndex);
      vTemp->setToGrid(dx,dy);
   }
}

void MNPolyLine::replacement(double x0, double y0, double scale)
{
   int l =countOfVertexes();
   for(int i=0;i<l;i++){
      MNVertex * v = getVertex(i);
      v->setX(v->getX() * scale + x0);
      v->setY(v->getY() * scale + y0);
      l =countOfVertexes();
   }
}

__fastcall MNPolyLine::rotaion(double angle,TPoint startPoint)
{
   double cx = startPoint.x;
   double cy = startPoint.y;
   int l =countOfVertexes();
   for(int i=0;i<l;i++){
      MNVertex * v = getVertex(i);

      double xpl = cx + (v->getX()-cx)*cos(angle) + (v->getY()-cy)*sin(angle);
		double ypl = cy - (v->getX()-cx)*sin(angle) + (v->getY()-cy)*cos(angle);

      v->setX(xpl);
      v->setY(ypl);
  //    if(v->getBulge() !=0)
 //     {
 //        v->setBulge(- v->getBulge());
 //     }

   }
}

TPoint MNPolyLine::getStartPoint()
{
   if(countOfVertexes() > 0)
      return TPoint(getVertex(0)->getX(),getVertex(0)->getY());
   return TPoint(0,0);
}

void MNPolyLine::mirror(bool vertical, double baseLine)
{
   int l =countOfVertexes();
   for(int i=0;i<l;i++){
      MNVertex * v = getVertex(i);
      if(vertical)
         v->setY(baseLine - v->getY());
      else
         v->setX(baseLine - v->getX());
      if(v->getBulge() !=0)
      {
         v->setBulge(- v->getBulge());
      }
   }
}
