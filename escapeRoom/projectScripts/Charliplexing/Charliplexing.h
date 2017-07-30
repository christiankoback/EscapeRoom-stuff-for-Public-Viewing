/*
    2  * Copyright (C) 2012 Southern Storm Software, Pty Ltd.
    3  *
    4  * Permission is hereby granted, free of charge, to any person obtaining a
    5  * copy of this software and associated documentation files (the "Software"),
    6  * to deal in the Software without restriction, including without limitation
    7  * the rights to use, copy, modify, merge, publish, distribute, sublicense,
    8  * and/or sell copies of the Software, and to permit persons to whom the
    9  * Software is furnished to do so, subject to the following conditions:
   10  *
   11  * The above copyright notice and this permission notice shall be included
   12  * in all copies or substantial portions of the Software.
   13  *
   14  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   15  * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   16  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   17  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   18  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   19  * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   20  * DEALINGS IN THE SOFTWARE.
   21  */
  
   #ifndef Charlieplexing_h
   #define Charlieplexing_h
  
   #include <inttypes.h>
   
   class Charlieplex
   {
   public:
       Charlieplex(const uint8_t *pins, uint8_t numPins);
       ~Charlieplex();
   
       int count() const { return _count; }
   
       bool led(int index) const { return _values[index] != 0; }
       void setLed(int index, bool value) { _values[index] = (value ? 255 : 0); }
   
       uint8_t pwmLed(int index) const { return _values[index]; }
       void setPwmLed(int index, uint8_t value) { _values[index] = value; }
   
       unsigned long holdTime() const { return _holdTime; }
       void setHoldTime(unsigned long us) { _holdTime = us; }
   
       void loop();
       void refresh();
   
   private:
       int _count;
       uint8_t *_pins1;
       uint8_t *_pins2;
       uint8_t *_values;
       unsigned long _holdTime;
       unsigned long _lastTime;
       int _currentIndex;
       uint8_t _pwmPhase;
   };
   
   #endif