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

#define ADDR_FREQ_A 0x00
#define ADDR_FREQ_B 0x02
#define ADDR_FREQ_C 0x04
#define ADDR_NOISE 0x06
#define ADDR_MIXER 0x07
#define ADDR_LEVEL_A 0x08
#define ADDR_LEVEL_B 0x09
#define ADDR_LEVEL_C 0x0A
#define ADDR_FREQ_E 0x0B
#define ADDR_SHAPE_E 0x0D

#define YM2149BASE IO_SLOT(13)
#define YM2149REG(x) REGISTER(YM2149BASE,x)

void write_data(unsigned char address, unsigned char data);
void set_ch(unsigned char address, int i);
void setVolumeYM2149(byte pVolume);
void setupYM2149();

#endif // LIB_YM2149_H_
