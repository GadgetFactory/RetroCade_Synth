/* 
  This Space Invaders alien crawling along the RetroCade LCD was created by JO3RI and adapted to the RetroCade by Jack Gassett.
  Please take a look at JO3RI's website for more cool hacks!
  
  Demo by JO3RI
  
  http://www.JO3RI.be/arduino/arduino-projects/lcd-16x2-demo
 
*/

#ifndef LIB_SPACEINVADERS_H_
#define LIB_SPACEINVADERS_H_

 #include "binary.h"

// set for space invader animation speed:
int y=50;

// drawing the pixels for each 8x5 part of the LCD (SPACE INVADER)
// --START SPACE INVADER--
// --EMPTY START--
byte charSP0[8] = {
 B00000,
 B00000,
 B00000,
 B00000,
 B00000,
 B00000,
 B00000,
 B00000
};
// --EMPTY END--

// --MOVE STEP A START--
byte charSP1a[8] = {
 B00000,
 B00000,
 B00000,
 B10000,
 B10000,
 B00000,
 B00000,
 B10000
};

byte charSP2a[8] = {
 B00000,
 B00000,
 B10000,
 B11000,
 B11000,
 B00000,
 B10000,
 B01000
};

byte charSP3a[8] = {
 B00000,
 B10000,
 B11000,
 B01100,
 B11100,
 B10000,
 B01000,
 B10100
};

byte charSP4a[8] = {
 B10000,
 B11000,
 B11100,
 B10110,
 B11110,
 B01000,
 B10100,
 B01010
};

byte charSP5a[8] = {
 B11000,
 B11100,
 B11110,
 B11011,
 B11111,
 B00100,
 B11010,
 B00101
};

byte charSP6a[8] = {
 B01100,
 B11110,
 B11111,
 B01101,
 B11111,
 B10010,
 B01101,
 B10010
};


byte charSP7a[8] = {
 B00110,
 B01111,
 B11111,
 B10110,
 B11111,
 B01001,
 B10110,
 B01001
};

byte charSP8a[8] = {
 B00011,
 B00111,
 B01111,
 B11011,
 B11111,
 B00100,
 B01011,
 B10100
};

byte charSP9a[8] = {
 B00001,
 B00011,
 B00111,
 B01101,
 B01111,
 B00001,
 B00010,
 B00101
};

byte charSP10a[8] = {
 B00000,
 B00001,
 B00011,
 B00110,
 B00111,
 B00001,
 B00010,
 B00101
};

byte charSP11a[8] = {
 B00000,
 B00000,
 B00001,
 B00011,
 B00011,
 B00000,
 B00001,
 B00010
};

byte charSP12a[8] = {
 B00000,
 B00000,
 B00000,
 B00001,
 B00001,
 B00000,
 B00000,
 B00001
};
// --MOVE STEP A END--

// --MOVE STEP B START--
byte charSP1b[8] = {
 B00000,
 B00000,
 B00000,
 B10000,
 B10000,
 B00000,
 B10000,
 B00000
};

byte charSP2b[8] = {
 B00000,
 B00000,
 B10000,
 B11000,
 B11000,
 B10000,
 B01000,
 B10000
};

byte charSP3b[8] = {
 B00000,
 B10000,
 B11000,
 B01100,
 B11100,
 B01000,
 B00100,
 B01000
};

byte charSP4b[8] = {
 B10000,
 B11000,
 B11100,
 B10110,
 B11110,
 B10100,
 B00010,
 B00100
};

byte charSP5b[8] = {
 B11000,
 B11100,
 B11110,
 B11011,
 B11111,
 B11010,
 B00001,
 B00010
};

byte charSP6b[8] = {
 B01100,
 B11110,
 B11111,
 B01101,
 B11111,
 B01101,
 B00000,
 B00001
};

byte charSP7b[8] = {
 B00110,
 B01111,
 B11111,
 B10110,
 B11111,
 B10110,
 B00000,
 B10000
};

byte charSP8b[8] = {
 B00011,
 B00111,
 B01111,
 B11011,
 B11111,
 B01011,
 B10000,
 B01000
};

byte charSP9b[8] = {
 B00001,
 B00011,
 B00111,
 B01101,
 B01111,
 B00101,
 B01000,
 B00100
};

byte charSP10b[8] = {
 B00000,
 B00001,
 B00011,
 B00110,
 B00111,
 B00010,
 B00100,
 B00010
};

byte charSP11b[8] = {
 B00000,
 B00000,
 B00001,
 B00011,
 B00011,
 B00001,
 B00010,
 B00001
};

byte charSP12b[8] = {
 B00000,
 B00000,
 B00000,
 B00001,
 B00001,
 B00000,
 B00001,
 B00000
};
// --MOVE STEP B END--
// --END SPACE INVADER--


#endif // LIB_SPACEINVADERS_H_
