/*!
 *  @file		RetroCade.h
 *  Project		RetroCade Library
 *	@brief		RetroCade Library for the ZPUino
 *	Version		1.0
 *  @author		Jack Gassett 
 *	@date		9/11/12
 *  License		GPL
 */

#ifndef LIB_RetroCade_H_
#define LIB_RetroCade_H_

#include <inttypes.h> 
#include <zpuino-types.h>
#include <zpuino.h>
#include "Arduino.h"
#include "LiquidCrystal.h"
#include <SD.h>
#include "SmallFS.h"
#include "modplayer.h"
#include "ymplayer.h"
#include "sidplayer.h"
#include "SID.h"
#include "YM2149.h"

#define AUDIO_J1_L WING_B_1
#define AUDIO_J1_R WING_B_0

#define AUDIO_J2_L WING_B_3
#define AUDIO_J2_R WING_B_2

#define SERIAL1RXPIN WING_C_1
#define SERIAL1TXPIN WING_C_0

//Joystick
#define JSELECT WING_B_15
#define JDOWN WING_B_14
#define JUP WING_B_13
#define JRIGHT WING_B_12
#define JLEFT WING_B_11

//For SPI ADC1
#define SELPIN WING_C_9    //Selection Pin
#define DATAOUT WING_C_8   //MOSI
#define DATAIN  WING_C_7   //MISO
#define SPICLOCK WING_C_6  //Clock

//For SPI ADC2
#define SELPIN2 WING_C_5    //Selection Pin
#define DATAOUT2 WING_C_4   //MOSI
#define DATAIN2  WING_C_3   //MISO
#define SPICLOCK2 WING_C_2  //Clock

//SD Card
#define CSPIN  WING_C_13
#define SDIPIN WING_C_12
#define SCKPIN WING_C_11
#define SDOPIN WING_C_10

enum kButtonDirection {
	Left                = 0, 
	Right               = 1,  
	Up                  = 2,  
    Down                = 3,  
	Select              = 4,
    None                = 5
};

class RETROCADE
{ 
  public:
   YMPLAYER ymplayer;
   MODPLAYER modplayer;
   SIDPLAYER sidplayer;
   YM2149 ym2149;
   SID sid;   
   void setupMegaWing(); 
   void handleJoystick();
   void setTimeout();
   byte getActiveChannel();
   void printDirectory(File dir, int numTabs);
   void printFile(const char* ext);
   boolean sdFsActive();
   boolean smallFsActive();
   void spaceInvadersLCD();
  private:
   void initSD();
   int fileExtension(const char* name, const char* extension, size_t length);
   void smallfsModFileJoystick(byte type);
   void instrumentJoystick();
   void modFileJoystick();
   void ymFileJoystick(); 
   void sidFileJoystick(); 
   byte lcdMode;
   kButtonDirection buttonPressed;
   byte activeChannel;
   int activeInstrument;
   byte smallfsActiveTrack;
   unsigned long timeout;
   boolean smallFs;
   boolean sdFs;
   byte invadersCurLoc;
   byte invadersCurSeg;
   int invadersTimer; 
   File root; 
   File curFile;
   File curYMFile;
   File curMODFile;
   char * fileName;
};

#endif // LIB_RetroCade_H_
