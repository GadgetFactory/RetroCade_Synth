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
#include <SD.h>
#include "ramFS.h"
#include "ptplay.h"
#include "retrocade_defs.h"

class MODPLAYER
{
  public: 
    MODPLAYER();
    void setup();
    void loadFile(const char* name);
    void play(boolean play);    
    boolean getPlaying();
    void audiofill();  
    void zpu_interrupt(); 
    void volume(int volume);   
  private:
    CircularBuffer<unsigned,7> audioBuffer;   
    unsigned underruns;
    pt_mod_s *pt_init_smallfs();
    SmallFSFile modSmallFSfile;
    File modSDfile;
    RamFSFile modRAMfile;
    char buf[128];
    pt_mod_s * mod;
    boolean playing;
    boolean fileLoaded;
    kFileType fileType;    
};


#endif // LIB_MODPLAYER_H_
