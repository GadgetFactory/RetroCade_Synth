/*!
 *  @file		modplayer.h
 *  Project		modplayer Library
 *	@brief		modplayer wrapper for the ptplay library for the ZPUino, ported by Alvaro Lopes
 *	Version		1.0
 *  @author		Jack Gassett 
 *	@date		4/10/12
 *  License		GPL
 */

#ifndef LIB_MODPLAYER_H_
#define LIB_MODPLAYER_H_

#include <inttypes.h> 
#include <zpuino-types.h>
#include <zpuino.h>
#include "Arduino.h"
#include "SmallFS.h"
#include "cbuffer.h"
#include "ptplay.h"

class MODPLAYER
{
  public: 
    MODPLAYER();
    void setup();
    //void loadFile(String name);
    void play();    
    void stop(); 
    boolean getPlaying();
    void audiofill();  
    CircularBuffer<unsigned,7> audioBuffer;  //TODO: make this private  
    unsigned underruns;    //TODO: make private
    void _zpu_interrupt();    
  private:

    pt_mod_s *pt_init_smallfs(SmallFSFile &file);
    SmallFSFile modfile;
    char buf[128];
    pt_mod_s * mod;
    boolean playing;    
    //unsigned char __end__;
};


#endif // LIB_MODPLAYER_H_
