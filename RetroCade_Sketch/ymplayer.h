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

class YMPLAYER
{
  public: 
    YMPLAYER();
    void setup(YM2149* ym);
    void loadFile(const char* name);
    void play(boolean play);    
    boolean getPlaying();
    void audiofill();  
    void _zpu_interrupt(); 
    void volume(int volume);   
  private:
    struct ymframe {        
    	unsigned char regval[14];
    };      
    CircularBuffer<ymframe,2> YMaudioBuffer; 
    unsigned int timerTicks;  
    int counter;
    int ymTimeStamp;
    int resetYMFlag;
    YM2149* ym2149;
    //unsigned underruns;
    //pt_mod_s *pt_init_smallfs(SmallFSFile &file);
    SmallFSFile ymaudiofile;
    //char buf[128];
    //pt_mod_s * mod;
    boolean playing;    
    //unsigned char __end__;
  
};


#endif // LIB_YMPLAYER_H_
