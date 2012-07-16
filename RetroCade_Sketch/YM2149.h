/*!
 *  @file		YM2149.h
 *  Project		YM2149 Library
 *	@brief		YM2149 Library for the ZPUino
 *	Version		1.0
 *  @author		Jack Gassett 
 *	@date		4/10/12
 *  License		GPL
 */

#ifndef LIB_YM2149_H_
#define LIB_YM2149_H_

#include <inttypes.h> 
#include <zpuino-types.h>
#include <zpuino.h>
#include "Arduino.h"


#define YM_ADDR_FREQ_A 0x00
#define YM_ADDR_FREQ_B 0x02
#define YM_ADDR_FREQ_C 0x04
#define YM_ADDR_NOISE 0x06
#define YM_ADDR_MIXER 0x07
#define YM_ADDR_LEVEL_A 0x08
#define YM_ADDR_LEVEL_B 0x09
#define YM_ADDR_LEVEL_C 0x0A
#define YM_ADDR_FREQ_E 0x0B
#define YM_ADDR_SHAPE_E 0x0D

#define YM2149BASE IO_SLOT(13)
#define YM2149REG(x) REGISTER(YM2149BASE,x)  

class YM2149
{
  public:
    YM2149();
    void writeData(unsigned char address, unsigned char data);
    void setNote(byte voice, int note, boolean active);
    void setNoise(byte voice, byte freq, boolean active);
    void setVolume(byte voice, byte volume);
    void setEnvelope(byte voice, int freq, boolean active);
    void setEnvelopeCONT(boolean active);
    void setEnvelopeATT(boolean active);
    void setEnvelopeALT(boolean active);
    void setEnvelopeHOLD(boolean active);    
    void reset();
  private:
    struct YM_REG_MIXER_STRUCT{
        unsigned int EMPTY : 2;
        unsigned int NOISEC : 1; 
        unsigned int NOISEB : 1;
        unsigned int NOISEA : 1;
        unsigned int TONEC : 1; 
        unsigned int TONEB : 1;
        unsigned int TONEA : 1;
    } ;
    struct YM_REG_ENVSHAPE_STRUCT{
        unsigned int EMPTY : 4;
        unsigned int CONT : 1; 
        unsigned int ATT : 1;
        unsigned int ALT : 1;
        unsigned int HOLD : 1;      
    } ;
    struct YM_REG_LEVEL_STRUCT{
        unsigned int EMPTY : 3;
        unsigned int MODE : 1; 
        unsigned int LEVEL : 4;
    } ;
    struct ymframe {
    	unsigned char regval[14];
    };  
    YM_REG_MIXER_STRUCT YM_REG_MIXER;
    YM_REG_ENVSHAPE_STRUCT YM_REG_ENVSHAPE;
    YM_REG_LEVEL_STRUCT YM_REG_VA_LEVEL;
    YM_REG_LEVEL_STRUCT YM_REG_VB_LEVEL;
    YM_REG_LEVEL_STRUCT YM_REG_VC_LEVEL; 
    static const int ym2149_note2MIDI[129];
};
    const int ym2149_note2MIDI[129] = {//MIDI note number
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

#endif // LIB_YM2149_H_
