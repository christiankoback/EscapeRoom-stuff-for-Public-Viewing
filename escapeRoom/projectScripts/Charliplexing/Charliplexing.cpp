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
   
   #include "Charlieplexing.h"
   #if defined(ARDUINO) && ARDUINO >= 100
   #include <Arduino.h>
   #else
   #include <WProgram.h>
   #endif
   #include <stdlib.h>
   #include <string.h>
   
   Charlieplex::Charlieplex(const uint8_t *pins, uint8_t numPins)
       : _count(((int)numPins) * (numPins - 1))
       , _lastTime(micros())
       , _currentIndex(-1)
       , _pwmPhase(0xC0)
   {
       // Determine the best hold time for 50 Hz refresh when all LED's
       // are lit.  Divide it again by 4 (to get 200 Hz) to manage the
       // simulated PWM in refresh().
       _holdTime = 20000 / _count / 4;
   
       // Allocate the pin arrays and populate them.  Doing this now makes
       // refresh() more efficient later, at the expense of some memory.
       _pins1 = (uint8_t *)malloc(_count);
       _pins2 = (uint8_t *)malloc(_count);
       int n = 0;
       for (uint8_t pass = 1; pass < numPins; ++pass) {
           for (uint8_t pin = 0; pin < (numPins - pass); ++pin) {
               _pins1[n] = _pins2[n + 1] = pins[pin];
               _pins2[n] = _pins1[n + 1] = pins[pin + pass];
               n += 2;
           }
       }
   
       // Allocate space for the LED value array and zero it.
       _values = (uint8_t *)malloc(_count);
       memset(_values, 0, _count);
   
       // Start with all pins configured as floating inputs (all LED's off).
       for (uint8_t pin = 0; pin < numPins; ++pin) {
           digitalWrite(pins[pin], LOW);
           pinMode(pins[pin], INPUT);
       }
   }
   
   Charlieplex::~Charlieplex()
   {
       free(_pins1);
       free(_pins2);
       free(_values);
   }
   
   void Charlieplex::loop()
   {
       unsigned long us = micros();
       if ((us - _lastTime) >= _holdTime) {
           _lastTime = us;
           refresh();
       }
   }
   
   void Charlieplex::refresh()
   {
       // Find the next LED to be lit.
       int prevIndex = _currentIndex;
       int limit = _count;
       while (limit >= 0) {
           _currentIndex = (_currentIndex + 1) % _count;
           if (_values[_currentIndex] != 0)
               break;
           --limit;
       }
       if (limit < 0) {
           // No LED's are lit.  Turn off the previous LED and exit.
           if (prevIndex != -1) {
               digitalWrite(_pins1[prevIndex], LOW);
               digitalWrite(_pins2[prevIndex], LOW);
               pinMode(_pins1[prevIndex], INPUT);
               pinMode(_pins2[prevIndex], INPUT);
           }
           _currentIndex = -1;
           return;
       }
   
       // Light the current LED.
       uint8_t value = _values[_currentIndex];
       uint8_t pin1 = _pins1[_currentIndex];
       uint8_t pin2 = _pins2[_currentIndex];
       _pwmPhase += 0x40;
       if (prevIndex != _currentIndex) {
           // Turn off the previous LED.
           if (prevIndex != -1) {
               digitalWrite(_pins1[prevIndex], LOW);
               digitalWrite(_pins2[prevIndex], LOW);
               pinMode(_pins1[prevIndex], INPUT);
               pinMode(_pins2[prevIndex], INPUT);
           }
   
           // We simulate PWM using a phase counter because analogWrite()
           // combined with holdTime() causes too much flickering if more
           // than one LED is lit.  This reduces the PWM resolution to 1 in 4.
           pinMode(pin1, OUTPUT);
           pinMode(pin2, OUTPUT);
           if (value > _pwmPhase)
               digitalWrite(pin1, HIGH);
           else
               digitalWrite(pin1, LOW);
       } else {
           // Same LED as previous.  Since there is only a single LED
           // that is lit, we can use analogWrite() to set the PWM state.
           if (value == 255)
               digitalWrite(pin1, HIGH);
           else
               analogWrite(pin1, value);
      }
  }
