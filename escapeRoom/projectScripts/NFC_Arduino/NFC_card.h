#ifndef NFC_CARD_H__
#define NFC_CARD_H__

#include <Arduino.h>
class manageCards {
  public:
    void doCard0 ();
    void doCard1 ();
    void doCard2 ();
    void doCard3 (); 
    void resetCards();
    void cardSetup();
   // typedef for class function
   typedef void (manageCards::*GeneralFunction) ();
   static const GeneralFunction doCardAction [4];
};  // end of class manageCards

//bluetooth stuff
#ifndef BT_SETUP__
#define BT_SETUP__
const int btCodeLength = 3;
byte BTCode[btCodeLength];

void sendBTCode(char * strWord,int strLength){
  Serial1.write( "0" ); //garbage
  for (int i = 0; i < strLength; i++){
    Serial1.write( strWord[i] );
  }
}
#endif


#endif
