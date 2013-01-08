/*!
 *  @file		ymplayer.h
 *  Project		ymplayer Library
 *	@brief		ymplayer library for the ZPUino
 *	Version		1.0
 *  @author		Jack Gassett 
 *	@date		4/10/12
 *  License		GPL
 */

#ifndef LIB_YMPLAYER_H_
#define LIB_YMPLAYER_H_

#include <inttypes.h> 
#include <zpuino-types.h>
#include <zpuino.h>
#include "Arduino.h"
#include "SmallFS.h"
#include "cbuffer.h"
#include "YM2149.h"
#include <SD.h>
#include "retrocade_defs.h"

class YMPLAYER
{
  public: 
    YMPLAYER();
    void setup(YM2149* ym);
    void loadFile(const char* name);
    void play(boolean play);    
    boolean getPlaying();
    void audiofill();  
    void zpu_interrupt(); 
    void volume(int volume);   
  private:
    struct ymframe {        
    	unsigned char regval[14];
    };      
    CircularBuffer<ymframe,2> YMaudioBuffer; 
    unsigned int timerTicks;  
    int counter;
    byte volumeAdjust;
    int ymTimeStamp;
    int resetYMFlag;
    YM2149* ym2149;
    SmallFSFile ymSmallFSfile;
    File ymSDfile;
    boolean playing; 
    kFileType fileType;  
   
};


#endif // LIB_YMPLAYER_H_
