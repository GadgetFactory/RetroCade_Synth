/*!
 *  @file		sidplayer.h
 *  Project		sidplayer Library
 *	@brief		sidplayer library for the ZPUino
 *	Version		1.0
 *  @author		Jack Gassett 
 *	@date		9/24/13
 *  License		GPL
 */

#ifndef LIB_SIDPLAYER_H_
#define LIB_SIDPLAYER_H_

#include <inttypes.h> 
#include <zpuino-types.h>
#include <zpuino.h>
#include "Arduino.h"
 #include "SmallFS.h"
// #include "cbuffer.h"
// #include "SID.h"
 #include <SD.h>
 #include "ramFS.h"
 //#include "retrocade_defs.h"

// enum kFileType {
	// SmallFSType               = 0,  
	// SDFSType                  = 1,  
// };

class SIDPLAYER
{
  public: 
    SIDPLAYER();
    //void setup(SID* newsid);
	void setup();
    void loadFile(const char* name);
    void play(boolean play);    
    boolean getPlaying();
    void audiofill();  
    void zpu_interrupt(); 
    void volume(int volume);   
  private:
    // struct ymframe {        
    	// unsigned char regval[14];
    // };      
    // CircularBuffer<ymframe,2> YMaudioBuffer; 
    unsigned int timerTicks;  
    int counter;
    byte volumeAdjust;
    int sidTimeStamp;
    int resetSIDFlag;
    //SID* sid;
    SmallFSFile sidSmallFSfile;
    File sidSDfile;
    boolean playing; 
	boolean fileLoaded;
    //kFileType fileType;  
    SmallFSFile modSmallFSfile;
    File modSDfile;
    RamFSFile modRAMfile;	
   
};


#endif // LIB_SIDPLAYER_H_
