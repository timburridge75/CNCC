//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TConfiguration.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TConfiguration *Configuration;
//---------------------------------------------------------------------------
__fastcall TConfiguration::TConfiguration(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------
 