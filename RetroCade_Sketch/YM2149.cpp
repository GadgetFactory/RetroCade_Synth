#include "YM2149.h"
#include <inttypes.h> 



void ym_write_data(unsigned char address, unsigned char data)
{
  YM2149REG(address) = data;
}

//void set_ch(unsigned char address, int i)
//{
//  ym_write_data(address, ym2149_note2MIDI[i]);
//  ym_write_data(address+1, (ym2149_note2MIDI[i] >> 8));  
//}

void ym_set_freq(unsigned char address, int freq)
{
  ym_write_data(address, freq);
  ym_write_data(address+1, (freq >> 8));  
}

//YMZ294 related : set the volume between 0x00 (mute) and 0xff(full)
//For the simplicity it sets the volume of the 3 channels at the same time.
//Just make another signature if youwant to control channel per channel.
void setVolumeYM2149(byte pVolume){
  ym_write_data(YM_ADDR_LEVEL_A, pVolume & 0x0f);
  ym_write_data(YM_ADDR_LEVEL_B, pVolume & 0x0f);
  ym_write_data(YM_ADDR_LEVEL_C, pVolume & 0x0f);  
}

//makes the setup for the Yamaha YMZ294 component
void setupYM2149(){

  
  //no noise 
  ym_write_data(YM_ADDR_NOISE, 0x00);
  //mixer
  ym_write_data(YM_ADDR_MIXER, 0x38);
  //set volume to MUTE
  setVolumeYM2149(0xFF);  
}
