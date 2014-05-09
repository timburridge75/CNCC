//---------------------------------------------------------------------------

#ifndef NavigationArrayH
#define NavigationArrayH
//---------------------------------------------------------------------------
#define ARRAY_MAX_COUNT 1000
class NavigationArray
{
private:
   char ** arrays;
   unsigned long long defSize;
   unsigned long long itemsIndex;
   unsigned int cardsIndex;
public:
   NavigationArray(unsigned long long defaultSize);
   char * getItemsList();
   unsigned __int64 getSize();
   ~NavigationArray();
   void addItem(char item);

};
#endif
