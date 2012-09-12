/*!
 *  @file		YM2149.cpp
 *  Project		YM2149 Library
 *	@brief		YM2149 Library for the ZPUino
 *	Version		1.0
 *  @author		Jack Gassett 
 *	@date		4/10/12
 *  License		GPL
 */
#include "YM2149.h"
 
const int YM2149::MIDI2freq[129] = {//MIDI note number
  15289, 14431, 13621, 12856, 12135, 11454, 10811, 10204,//0-7
  9631, 9091, 8581, 8099, 7645, 7215, 6810, 6428,//8-15
  6067, 5727, 5405, 5102, 4816, 4545, 4290, 4050,//16-23
  3822, 3608, 3405, 3214, 3034, 2863, 2703, 2551,//24-31
  2408, 2273, 2145, 2025, 1911, 1804, 1703, 1607,//32-39
  1517, 1432, 1351, 1276, 1204, 1136, 1073, 1012,//40-47
  956, 902, 851, 804, 758, 716, 676, 638,//48-55
  602, 568, 536, 506, 478, 451, 426, 402,//56-63
  379, 358, 338, 319, 301, 284, 268, 253,//64-71
  239, 225, 213, 201, 190, 179, 169, 159,//72-79
  150, 142, 134, 127, 119, 113, 106, 100,//80-87
  95, 89, 84, 80, 75, 71, 67, 63,//88-95
  60, 56, 53, 50, 47, 45, 42, 40,//96-103
  38, 36, 34, 32, 30, 28, 27, 25,//104-111
  24, 22, 21, 20, 19, 18, 17, 16,//112-119
  15, 14, 13, 13, 12, 11, 11, 10,//120-127
  0//off
}; 

    //TODO: This is not the right place for this
    struct YM_REG_MIXER_STRUCT{
        unsigned int EMPTY : 2;
        unsigned int NOISEC : 1; 
        unsigned int NOISEB : 1;
        unsigned int NOISEA : 1;
        unsigned int TONEC : 1; 
        unsigned int TONEB : 1;
        unsigned int TONEA : 1;
    } ;    
    YM_REG_MIXER_STRUCT YM_REG_MIXER;

//const byte YM2149::YM_ADDR_FREQ_Array[4] = {
//   YM_ADDR_FREQ_A,
//   YM_ADDR_FREQ_A,
//   YM_ADDR_FREQ_B,
//   YM_ADDR_FREQ_C
//};


void YMVoice::setBase(int freqAddress, int volumeAddress)
{
  YM_ADDR_FREQ = freqAddress;
  YM_ADDR_LEVEL = volumeAddress; 
}

void YMVoice::setNote(int note, boolean active)
{
  switch (YM_ADDR_FREQ) {  //TODO figure more efficient way to do this. Want to avoid case statements.
    case 0:
      YM_REG_MIXER.TONEA = !active;
      break;
    case 2:
      YM_REG_MIXER.TONEB = !active;
      break;
    case 4:
      YM_REG_MIXER.TONEC = !active;
      break;
    default:
      return;
      break;       
  }
  YM2149REG(YM_ADDR_MIXER) = *(char*)&YM_REG_MIXER;  
  YM2149::writeData(YM_ADDR_FREQ, YM2149::MIDI2freq[note]);
  YM2149::writeData(YM_ADDR_FREQ+1, (YM2149::MIDI2freq[note] >> 8));  
  
//  switch (voice) {  //TODO figure more efficient way to do this. Want to avoid case statements.
//    case 1:
//      YM_REG_MIXER.TONEA = !active;
//      break;
//    case 2:
//      YM_REG_MIXER.TONEB = !active;
//      break;
//    case 3:
//      YM_REG_MIXER.TONEC = !active;
//      break;
//    default:
//      return;
//      break;       
//  }
//  YM2149REG(YM_ADDR_MIXER) = *(char*)&YM_REG_MIXER;
//
//  writeData(YM_ADDR_FREQ_Array[voice], MIDI2freq[MIDINote]);
//  writeData(YM_ADDR_FREQ_Array[voice]+1, (MIDI2freq[MIDINote] >> 8));
}

void YMVoice::setNoise(boolean active)
{
  switch (YM_ADDR_FREQ) {  //TODO figure more efficient way to do this. Want to avoid case statements.
    case 0:
      YM_REG_MIXER.NOISEA = !active;
      break;
    case 2:
      YM_REG_MIXER.NOISEB = !active;
      break;
    case 4:
      YM_REG_MIXER.NOISEC = !active;
      break;
  }  
  YM2149REG(YM_ADDR_MIXER) = *(char*)&YM_REG_MIXER;
//  YM2149REG(YM_ADDR_MIXER) = *(char*)&YM_REG_MIXER;
//  YM2149REG(YM_ADDR_NOISE) = freq;    //TODO There should be a way to just keep previous value. Make it an optional parameter?
}

void YMVoice::setEnvelope(boolean active)
{
//  switch (YM_ADDR_FREQ) {  //TODO figure more efficient way to do this. Want to avoid case statements.
//    case 0:
//      YM_REG_VA_LEVEL.MODE = active;
//      YM2149REG(YM_ADDR_LEVEL_A) = *(char*)&YM_REG_VA_LEVEL;
//      break;
//    case 2:
//      YM_REG_VB_LEVEL.MODE = active;
//      YM2149REG(YM_ADDR_LEVEL_B) = *(char*)&YM_REG_VB_LEVEL;
//      break;
//    case 4:
//      YM_REG_VC_LEVEL.MODE = active;
//      YM2149REG(YM_ADDR_LEVEL_C) = *(char*)&YM_REG_VC_LEVEL;
//      break;
//  }  
}

void YMVoice::setVolume(byte volume)
{
  YM2149::writeData(YM_ADDR_LEVEL, volume & 0x0f);
//  switch (voice) {
//    case 1:
//      writeData(YM_ADDR_LEVEL_A, volume & 0x0f);
//      break;
//    case 2:
//      writeData(YM_ADDR_LEVEL_B, volume & 0x0f);
//      break;
//    case 3:
//      writeData(YM_ADDR_LEVEL_C, volume & 0x0f);
//      break;
//  }    
}

YM2149::YM2149(){  
  //no noise 
  writeData(YM_ADDR_NOISE, 0x00);
  //mixer
  writeData(YM_ADDR_MIXER, 0x38);
  V1.setBase(YM_ADDR_FREQ_A, YM_ADDR_LEVEL_A);
  V2.setBase(YM_ADDR_FREQ_B, YM_ADDR_LEVEL_B);
  V3.setBase(YM_ADDR_FREQ_C, YM_ADDR_LEVEL_C);  
  reset();
}

void YM2149::writeData(unsigned char address, unsigned char data)
{
  YM2149REG(address) = data;
}


void YM2149::setEnvelopeFrequency(int freq)
{    
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
  
  //TODO: move these to voice reset
//  YM_REG_VA_LEVEL.EMPTY = 0;  
//  YM_REG_VA_LEVEL.MODE = 0;  
//  YM_REG_VA_LEVEL.LEVEL = 0;   
//  
//  YM_REG_VB_LEVEL.EMPTY = 0; 
//  YM_REG_VB_LEVEL.MODE = 0;  
//  YM_REG_VB_LEVEL.LEVEL = 0;  
// 
//  YM_REG_VC_LEVEL.EMPTY = 0;
//  YM_REG_VB_LEVEL.MODE = 0;  
//  YM_REG_VB_LEVEL.LEVEL = 0;  

  //YM2149REG(YM_ADDR_LEVEL_A) = *(char*)&YM_REG_VA_LEVEL;
  YM2149REG(YM_ADDR_SHAPE_E) = *(char*)&YM_REG_ENVSHAPE;
  YM2149REG(YM_ADDR_MIXER) = *(char*)&YM_REG_MIXER; 

//  setNote(1,128,true);   
//  setNote(2,128,true);     
//  setNote(3,128,true);   
}  
