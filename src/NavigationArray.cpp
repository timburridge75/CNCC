//---------------------------------------------------------------------------


#pragma hdrstop

#include "NavigationArray.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

NavigationArray::NavigationArray(unsigned long long defaultSize)
{
   defSize = defaultSize;

   arrays = new char*[ARRAY_MAX_COUNT];
   itemsIndex = -1;
   cardsIndex = 0;
   arrays[cardsIndex] = new char[defSize];
}

char* NavigationArray::getItemsList()
{
   //make result array
   unsigned long long len = getSize();
   char * result = new char[len];
   if( len < 1)
      return '\0';

   // copy
   unsigned long long index = 0;
   for (unsigned int ci=0 ; ci < cardsIndex ; ci++ )
   {
      for (unsigned long long cl=0 ; cl < defSize ; cl++)
      {
         result[index++] = arrays[ci][cl];
      }
   }
   for (unsigned long long i = 0; i<=itemsIndex; i++)
   {
      result[index++] = arrays[cardsIndex][i];
   }
   
   return result;
}

unsigned __int64 NavigationArray::getSize()
{
   return (cardsIndex)*defSize + itemsIndex + 1;
}

NavigationArray::~NavigationArray()
{
   for (unsigned int i = 0; i<= cardsIndex ;i++)
   {
      delete arrays[i];
   }
   delete[] arrays;
}

void NavigationArray::addItem(char item)
{
   itemsIndex++;
   if(itemsIndex == defSize)
   {
      itemsIndex = 0;
      cardsIndex++;
      arrays[cardsIndex] = new char[defSize];
   }
   arrays[cardsIndex][itemsIndex] = item;
}

