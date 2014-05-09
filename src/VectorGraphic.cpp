//---------------------------------------------------------------------------


#pragma hdrstop

#include "VectorGraphic.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


VectorGraphic::VectorGraphic(AnsiString FileName)
{
     f = new TFileStream(FileName,fmOpenRead);
     iToken =0;
     model = new CuttingModel();
     scale = 1;
     XAxesDistance = 0;
     YAxesDistance = 0;
     do{
        token=getNextToken();
        if( token == "0"){
         token=getNextToken();
         if(token.AnsiCompareIC("SECTION")==0){
            token=getNextToken();
            if(token == "2"){
               token=getNextToken();
               if(token.AnsiCompareIC("HEADER")==0){
                makeHeaders();
               }
               else if(token.AnsiCompareIC("TABLES")==0){
                makeTables();
               }
               else if(token.AnsiCompareIC("BLOCKS")==0){
                findPolylines();
               }
               else if(token.AnsiCompareIC("ENTITIES")==0){
                findPolylines();
               }
            }
         }
         if(token.AnsiCompareIC("EOF")==0){
            return;
         }
        }
     }while(!token.IsEmpty());
}

AnsiString VectorGraphic::getNextToken()
{
       AnsiString s="";
       for(;iToken<f->Size;){
         char p[1];
         f->Read(p,1);
         iToken++;
         if(p[0] == ' ' || p[0] =='\n'){
            if(s.IsEmpty())
               continue;
            else
               break;
         }
         if(p[0] == '\r'){
            if(s.IsEmpty())
               continue;
            else
               break;
         }
         s = s+p[0];
       }
       return s;
}

VectorGraphic::~VectorGraphic()
{
   try{
      if(f != NULL){
         delete f;
      }
   }catch(...){}
}

void VectorGraphic::makeHeaders()
{
   token=getNextToken();
   while(!token.IsEmpty()){
    
      if(token == "0"){ //          return when  0 EndSec finded
multipleZero1:
         token=getNextToken();
         if(token.AnsiCompareIC("ENDSEC")==0){
            return;
         }
         if(token == "0")
            goto multipleZero1;
      }

      token=getNextToken();
   }
}

void VectorGraphic::findPolylines()
{
   token=getNextToken();
   while(!token.IsEmpty()){

      if(token == "0"){ //          return when  0 EndSec finded
multipleZero4:
         token=getNextToken();
multipleZero_withoutNextToken:
         //-----------------------------------------------------
         if(token.AnsiCompareIC("ENDSEC")==0){
            return;
         }
         //-----------------------------------------------------
         if(token == "0")
            goto multipleZero4;
         //------------------------------------***** SEE A PolyLine
         if(token.AnsiCompareIC("POLYLINE")==0)
         {
            MNPolyLine *polyLine = new MNPolyLine();

            while(1){
            token=getNextToken();
            if(token  == "10" || token  == "20" || token  == "30"){
               token=getNextToken();
               continue;
            }
            else if(token == "0")
            {
               token=getNextToken();
               if(token.AnsiCompareIC("VERTEX")==0)
               {
                  while(1)
                  {
                     MNVertex* v = new MNVertex(0,0,0);
                     while(1)
                     {
                        token=getNextToken();
                        if(token == "10")
                        {
                           token=getNextToken();
                           v->setX(StrToFloat(token)*1000);
                        }
                        else if(token == "20")
                        {
                           token=getNextToken();
                           v->setY(StrToFloat(token)*1000);
                        }
                        else if(token == "42")
                        {
                           token=getNextToken();
                           double b = StrToFloat(token);
                           v->setBulge(b);
                        }
                        else if(token == "0")
                        {
                           token=getNextToken();
                           if(token.AnsiCompareIC("VERTEX")==0)
                              break;
                           else
                           {
                              polyLine->addVertex(v);
                              delete v;
                              model->addPolyLine(polyLine);
                              goto multipleZero_withoutNextToken;
                           }
                        }
                        else
                        {
                           token=getNextToken();
                        }
                     }
                     polyLine->addVertex(v);
                     delete v;
                  }
               }
               model->addPolyLine(polyLine);
               goto multipleZero_withoutNextToken;
            }
            else
               token=getNextToken();
            }
         }
      }
      token=getNextToken();
   }
}

void VectorGraphic::makeTables()
{
   token=getNextToken();
   while(!token.IsEmpty()){

      if(token == "0"){ //          return when  0 EndSec finded
multipleZero3:
         token=getNextToken();
         if(token.AnsiCompareIC("ENDSEC")==0){
            return;
         }
         if(token == "0")
            goto multipleZero3;
      }

      token=getNextToken();
   }
}

int VectorGraphic::gerdKardan(double x)
{
   int xInt = (int)x;
   double diff = x - xInt;
   if(diff > 0 )
      if(diff >= .5)
         return  (int)x+1;
      else
         return (int)x;
   else
      if(-diff >= .5)
         return  (int)x-1;
      else
         return (int)x;
}
