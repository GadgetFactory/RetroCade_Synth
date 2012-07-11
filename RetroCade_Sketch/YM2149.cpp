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

  //freqV4 = ym2149_note2MIDI[128];
  setFreq(YM_ADDR_FREQ_A,ym2149_note2MIDI[128]);   
  //freqV5 = ym2149_note2MIDI[128];
  setFreq(YM_ADDR_FREQ_B,ym2149_note2MIDI[128]);     
  //freqV6 = ym2149_note2MIDI[128];
  setFreq(YM_ADDR_FREQ_C,ym2149_note2MIDI[128]);   
}  


void YM2149::writeData(unsigned char address, unsigned char data)
{
  YM2149REG(address) = data;
}

void YM2149::setFreq(unsigned char address, int freq)
{
  int tmp;
  tmp = ym2149_note2MIDI[freq];
  writeData(address, tmp);
  writeData(address+1, (tmp >> 8));  
}

//YMZ294 related : set the volume between 0x00 (mute) and 0xff(full)
//For the simplicity it sets the volume of the 3 channels at the same time.
//Just make another signature if youwant to control channel per channel.
void YM2149::setVolume(byte pVolume){
  writeData(YM_ADDR_LEVEL_A, pVolume & 0x0f);
  writeData(YM_ADDR_LEVEL_B, pVolume & 0x0f);
  writeData(YM_ADDR_LEVEL_C, pVolume & 0x0f);  
}

//makes the setup for the Yamaha YMZ294 component
YM2149::YM2149(){

  //no noise 
  writeData(YM_ADDR_NOISE, 0x00);
  //mixer
  writeData(YM_ADDR_MIXER, 0x38);
  //set volume to MUTE
  setVolume(0xFF);  
}

void YM2149::controlChange(byte voice, byte number, byte value){

}
