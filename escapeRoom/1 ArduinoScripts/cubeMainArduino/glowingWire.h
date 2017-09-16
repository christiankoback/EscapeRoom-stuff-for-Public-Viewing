#ifndef GLOWING_WIRE_H__
#define GLOWING_WIRE_H__

#include <Arduino.h>
class manageGlowingWire {
	public:
		void lightWire0 ();
		void lightWire1 ();
		void lightWire2 ();
		void lightWire3 (); 
		void lightWire4 (); 
		
		void turnOff0 (); 
		void turnOff1 ();
		void turnOff2 ();
		void turnOff3 (); 
		void turnOff4 (); 

    void blinkWire(int wire, int msdelay);
    void resetWire();
    void wireSetup();
	
   // typedef for class function
   typedef void (manageGlowingWire::*GeneralFunction) ();
   static const GeneralFunction lightWireAction [5];
   static const GeneralFunction turnOffAction [5];
   
};  // end of class manageGlowingWire

#endif
