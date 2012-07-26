#include "YM2149.h"
#include <inttypes.h> 

int ym2149_note2MIDI[] = {//MIDI note number
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

void write_data(unsigned char address, unsigned char data)
{
  YM2149REG(address) = data;
}

void set_ch(unsigned char address, int i)
{
  write_data(address, ym2149_note2MIDI[i]);
  write_data(address+1, (ym2149_note2MIDI[i] >> 8));  
}

//YMZ294 related : set the volume between 0x00 (mute) and 0xff(full)
//For the simplicity it sets the volume of the 3 channels at the same time.
//Just make another signature if youwant to control channel per channel.
void setVolumeYM2149(byte pVolume){
  write_data(ADDR_LEVEL_A, pVolume & 0x0f);
  write_data(ADDR_LEVEL_B, pVolume & 0x0f);
  write_data(ADDR_LEVEL_C, pVolume & 0x0f);  
}

//makes the setup for the Yamaha YMZ294 component
void setupYM2149(){
  //no noise 
  write_data(ADDR_NOISE, 0x00);
  //mixer
  write_data(ADDR_MIXER, 0x38);
  //set volume to MUTE
  setVolumeYM2149(0x00);  
}
