/*!
 *  @file		YM2149.cpp
 *  Project		YM2149 Library
 *	@brief		YM2149 Library for the ZPUino
 *	Version		1.0
 *  @author		Jack Gassett 
 *	@date		4/10/12
 *  License		Creative Commons Atribution
 */
#include "YM2149.h"
//#include "WProgram.h"
//#include <inttypes.h> 
//#include <zpuino.h>
//#include <zpuino-types.h>

//makes the setup for the Yamaha YMZ294 component
YM2149::YM2149(){

  //no noise 
  writeData(YM_ADDR_NOISE, 0x00);
  //mixer
  writeData(YM_ADDR_MIXER, 0x38);
  reset();
}

void YM2149::writeData(unsigned char address, unsigned char data)
{
  YM2149REG(address) = data;
}

void YM2149::setFreq(byte voice, int freq, boolean active)
{
  int address;
  switch (voice) {
    case 1:
      address = YM_ADDR_FREQ_A;
      YM_REG_MIXER.TONEA = active;
      break;
    case 2:
      address = YM_ADDR_FREQ_B;
      YM_REG_MIXER.TONEB = active;
      break;
    case 3:
      address = YM_ADDR_FREQ_C;
      YM_REG_MIXER.TONEC = active;
      break;
  }
  writeData(address, ym2149_note2MIDI[freq]);
  writeData(address+1, (ym2149_note2MIDI[freq] >> 8));
  YM2149REG(YM_ADDR_MIXER) = *(char*)&YM_REG_MIXER;
}

void YM2149::setNoise(byte voice, byte freq, boolean active)
{
  int address;
  switch (voice) {
    case 1:
      YM_REG_MIXER.NOISEA = active;
      break;
    case 2:
      YM_REG_MIXER.NOISEA = active;
      break;
    case 3:
      YM_REG_MIXER.NOISEA = active;
      break;
  }  
  YM2149REG(YM_ADDR_MIXER) = *(char*)&YM_REG_MIXER;
  YM2149REG(YM_ADDR_NOISE) = freq;    //TODO There should be a way to just keep previous value. Make it an optional parameter?
}

void YM2149::setVolume(byte voice, byte volume)
{
  switch (voice) {
    case 1:
      writeData(YM_ADDR_LEVEL_A, volume & 0x0f);
      break;
    case 2:
      writeData(YM_ADDR_LEVEL_B, volume & 0x0f);
      break;
    case 3:
      writeData(YM_ADDR_LEVEL_C, volume & 0x0f);
      break;
  }    
}

void YM2149::setEnvelope(byte voice, int freq, boolean active)
{
  switch (voice) {
    case 1:
      YM_REG_VA_LEVEL.MODE = active;
      YM2149REG(YM_ADDR_LEVEL_A) = *(char*)&YM_REG_VA_LEVEL;
      break;
    case 2:
      YM_REG_VB_LEVEL.MODE = active;
      YM2149REG(YM_ADDR_LEVEL_B) = *(char*)&YM_REG_VB_LEVEL;
      break;
    case 3:
      YM_REG_VC_LEVEL.MODE = active;
      YM2149REG(YM_ADDR_LEVEL_C) = *(char*)&YM_REG_VC_LEVEL;
      break;
  }      
  writeData(YM_ADDR_FREQ_E, freq);
  writeData(YM_ADDR_FREQ_E+1, (freq >> 8));
}

void YM2149::setEnvelopeCONT(boolean active)
{
  YM_REG_ENVSHAPE.CONT = active; 
  YM2149REG(YM_ADDR_SHAPE_E) = *(char*)&YM_REG_ENVSHAPE;
}

void YM2149::setEnvelopeATT(boolean active)
{
  YM_REG_ENVSHAPE.ATT = active; 
  YM2149REG(YM_ADDR_SHAPE_E) = *(char*)&YM_REG_ENVSHAPE;
}

void YM2149::setEnvelopeALT(boolean active)
{
  YM_REG_ENVSHAPE.ALT = active; 
  YM2149REG(YM_ADDR_SHAPE_E) = *(char*)&YM_REG_ENVSHAPE;
}

void YM2149::setEnvelopeHOLD(boolean active)
{
  YM_REG_ENVSHAPE.HOLD = active; 
  YM2149REG(YM_ADDR_SHAPE_E) = *(char*)&YM_REG_ENVSHAPE;
}

void YM2149::reset(){
  YM_REG_ENVSHAPE.EMPTY = 0;
  YM_REG_ENVSHAPE.CONT = 1;
  YM_REG_ENVSHAPE.ATT = 1;
  YM_REG_ENVSHAPE.ALT = 1;
  YM_REG_ENVSHAPE.HOLD = 1;  
  
  YM_REG_MIXER.EMPTY = 0;
  YM_REG_MIXER.NOISEC = 1;
  YM_REG_MIXER.NOISEB = 1;
  YM_REG_MIXER.NOISEA = 1;
  YM_REG_MIXER.TONEC = 0;
  YM_REG_MIXER.TONEB = 0;
  YM_REG_MIXER.TONEA = 0;  
  
  YM_REG_VA_LEVEL.EMPTY = 0;  
  YM_REG_VA_LEVEL.MODE = 0;  
  YM_REG_VA_LEVEL.LEVEL = 7;   
  
  YM_REG_VB_LEVEL.EMPTY = 0;  
  YM_REG_VC_LEVEL.EMPTY = 0; 

  YM2149REG(YM_ADDR_LEVEL_A) = *(char*)&YM_REG_VA_LEVEL;
  YM2149REG(YM_ADDR_SHAPE_E) = *(char*)&YM_REG_ENVSHAPE;
  YM2149REG(YM_ADDR_MIXER) = *(char*)&YM_REG_MIXER; 

  setFreq(1,128,0);   
  setFreq(2,128,0);     
  setFreq(3,128,0);   
}  
