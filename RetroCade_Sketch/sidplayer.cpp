/*!
 *  @file		sidplayer.cpp
 *  Project		sidplayer Library
 *	@brief		sidplayer library for the ZPUino
 *	Version		1.0
 *  @author		Jack Gassett 
 *	@date		9/24/13
 *  License		GPL
 */
 
#include "sidplayer.h"
//#include "SID.h"
#include "tinysid.h"
#include "ramFS.h"

#define DEBUG

SIDPLAYER::SIDPLAYER(){
  
}
 
/*!
@par Description
Initialize the SIDPLAYER
@par Syntax
sidplayer.setup(newsid)
@param SID Pointer to a SID object

@par Example
~~~~~~~~{.c}
#include "SID.h"
#include "sidplayer.h"

SID sid;

void setup()
{
	sid.setup(&sid);
}

void loop() {}
~~~~~~~~
\n
*/
//void SIDPLAYER::setup(SID* newsid){    
void SIDPLAYER::setup(){ 
  //underruns = 0;
  // timerTicks = 0;
  // sidTimeStamp = 0;
  // resetSIDFlag = 0;
  // counter = 0;
   playing = false;
  // sid = newsid;
  // volumeAdjust = 4;
}

/*!
@par Description
Load a sid file from smallfs or a SD card. The provided file name will be searched for in the smallfs filesystem first and the SD file system second. If the filename exists in both locations the SD version will override the smallfs version.
@par Syntax
sidplayer.loadFile(name)
@param name filename of the sid file to open.

@par Example
~~~~~~~~{.c}
#include "SID.h"
#include "sidplayer.h"

SID sid;

void setup()
{
	sidplayer.setup(&sid);
	sidplayer.loadFile("track1.sid");
	sidplayer.play(true);
}

void loop() {}
~~~~~~~~
\n
*/
void SIDPLAYER::loadFile(const char* name)
{
	tinyLoadFile(name);
/*   sidSDfile.close();
  sidTimeStamp = 0;  
  sidSmallFSfile = SmallFS.open(name);
  if (!sidSmallFSfile.valid()) {
    #ifdef DEBUG  
      Serial.println("There is no smallfs File.");
    #endif     
  }
    
  sidSDfile = SD.open(name);    //TODO: Should check for SD filesystem init first.
  if (!sidSDfile){
    #ifdef DEBUG  
      Serial.println("There is no SD File.");
    #endif     
  } */
}

/*!
@par Description
Play a sid file after it has been loaded.
@par Syntax
sidplayer.play(play)
@param play Will play the file if set to 1 or true. 

@par Example
~~~~~~~~{.c}
#include "SID.h"
#include "sidplayer.h"

SID sid;

void setup()
{
	sidplayer.setup(&sid);
	sidplayer.loadFile("track1.sid");
	sidplayer.play(true);
}

void loop() {}
~~~~~~~~
\n
*/
void SIDPLAYER::play(boolean play)
{
  // boolean smallfscheck = sidSmallFSfile.valid();
  // if (!sidSDfile && !smallfscheck) {
    // play = false;
    // #ifdef DEBUG
      // Serial.println("Error: No SD or SmallFS File Available");
    // #endif  
    // return; 
  // }
  
  // if (smallfscheck)
   // fileType = SmallFSType;
  // if (sidSDfile)
   // fileType = SDFSType; 
  
  playing = play;
  if (play == true)
    resetSIDFlag = 1;
}

void SIDPLAYER::volume(int volume)
{
  volumeAdjust = volume;
}

void SIDPLAYER::audiofill()
{
	if (playing == true)
		tinyloop();
}

boolean SIDPLAYER::getPlaying()
{
  return playing;
}

void SIDPLAYER::zpu_interrupt()
{
	tinysid_zpu_interrupt();
}

