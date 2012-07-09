/*!
 *  @file		YM2149.h
 *  Project		YM2149 Library
 *	@brief		YM2149 Library for the ZPUino
 *	Version		1.0
 *  @author		Jack Gassett 
 *	@date		4/10/12
 *  License		Creative Commons Atribution
 */

#ifndef LIB_YM2149_H_
#define LIB_YM2149_H_

#include <inttypes.h> 
#include <zpuino.h>
#include <zpuino-types.h>

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

void ym_write_data(unsigned char address, unsigned char data);
void ym_set_freq(unsigned char address, int freq);
//void set_ch(unsigned char address, int i);
void setVolumeYM2149(byte pVolume);
void setupYM2149();

#endif // LIB_YM2149_H_
