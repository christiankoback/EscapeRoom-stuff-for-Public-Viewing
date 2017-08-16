
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

#endif
