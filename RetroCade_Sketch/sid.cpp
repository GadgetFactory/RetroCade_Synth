/*!
 *  @file		SID.cpp
 *  Project		C64 SID Library
 *	@brief		SID Library for the ZPUino
 *	Version		1.0
 *  @author		Jack Gassett 
 *	@date		4/10/12
 *  License		GPL
 */
#include "SID.h"

const int SID::MIDI2freq[] = {//MIDI note number
  291, 291, 291, 291, 291, 291, 291, 291,//0-7
  291, 291, 291, 291, 291, 291, 308, 326,//8-15
  346, 366, 388, 411, 435, 461, 489, 518,//16-23
  549, 581, 616, 652, 691, 732, 776, 822,//24-31
  871, 923, 978, 1036, 1097, 1163, 1232, 1305,//32-39
  1383, 1465, 1552, 1644, 1742, 1845, 1955, 2071,//40-47
  2195, 2325, 2463, 2610, 2765, 2930, 3104, 3288,//48-55
  3484, 3691, 3910, 4143, 4389, 4650, 4927, 5220,//56-63
  5530, 5859, 6207, 6577, 6968, 7382, 7821, 8286,//64-71
  8779, 9301, 9854, 10440, 11060, 11718, 12415, 13153,//72-79
  13935, 14764, 15642, 16572, 17557, 18601, 19709, 20897,//80-87
  22121, 23436, 24830, 26306, 27871, 29528, 31234, 33144,//88-95
  35115, 37203, 39415, 41759, 44242, 46873, 49660, 52613,//96-103
  55741, 59056, 62567, 66288, 66288, 66288, 66288, 66288,//104-111
  66288, 66288, 66288, 66288, 66288, 66288, 66288, 66288,//112-119
  66288, 66288, 66288, 66288, 66288, 66288, 66288, 66288,//120-127
  0//off
};


SID::SID(){  
  V1.setBase(SID_ADDR_BASE_V1);
  V1.reset();
  
  reset();
}  

void SID::writeData(unsigned char address, unsigned char data)
{
  SIDREG(address) = data;
}

SIDVoice::SIDVoice()
{
  SIDVoice(SID_ADDR_BASE_V1);  //If no base is specified then set to base of voice 1.
}

SIDVoice::SIDVoice(int address)
{
  baseAddress = address;    //TODO is this necessary?
  SID_ADDR_FREQ_LOW = baseAddress;
  SID_ADDR_FREQ_HI = baseAddress + 1;
  SID_ADDR_PW_LOW = baseAddress + 2; 
  SID_ADDR_PW_HI = baseAddress + 3;
  SID_ADDR_CONTROLREG = baseAddress + 4;
  SID_ADDR_ATTACK_DECAY = baseAddress + 5;
  SID_ADDR_SUSTAIN_RELEASE = baseAddress + 6;
}

void SIDVoice::setBase(int address)
{
  baseAddress = address;
  SID_ADDR_FREQ_LOW = baseAddress;
  SID_ADDR_FREQ_HI = baseAddress + 1;
  SID_ADDR_PW_LOW = baseAddress + 2; 
  SID_ADDR_PW_HI = baseAddress + 3;
  SID_ADDR_CONTROLREG = baseAddress + 4;
  SID_ADDR_ATTACK_DECAY = baseAddress + 5;
  SID_ADDR_SUSTAIN_RELEASE = baseAddress + 6;  
}

void SIDVoice::reset()
{
  SIDREG(SID_ADDR_FREQ_LOW) = 0;
  SIDREG(SID_ADDR_FREQ_HI) = 0; 
  SIDREG(SID_ADDR_PW_LOW) = 0;
  SIDREG(SID_ADDR_PW_HI) = 0; 
  SID_REG_ATTACK_DECAY.ATTACK = 0;
  SID_REG_ATTACK_DECAY.DECAY = 0; 
  SID_REG_SUSTAIN_RELEASE.SUSTAIN = 0;
  SID_REG_SUSTAIN_RELEASE.RELEASE = 0;
  SID_REG_CONTROLREG.NOISE_WAVE = 0;
  SID_REG_CONTROLREG.SQUARE_WAVE = 0;
  SID_REG_CONTROLREG.SAWTOOTH_WAVE = 0;
  SID_REG_CONTROLREG.TRIANGLE_WAVE = 0; 
  SID_REG_CONTROLREG.TEST = 0;  
  SID_REG_CONTROLREG.RING_MOD = 0;
  SID_REG_CONTROLREG.SYNC = 0;
  SID_REG_CONTROLREG.GATE = 0;
  SIDREG(SID_ADDR_ATTACK_DECAY) = *(char*)&SID_REG_ATTACK_DECAY;        //TODO: Make this a static function to save space.
  SIDREG(SID_ADDR_SUSTAIN_RELEASE) = *(char*)&SID_REG_SUSTAIN_RELEASE; 
  SIDREG(SID_ADDR_CONTROLREG) = *(char*)&SID_REG_CONTROLREG;    
}

void SIDVoice::setNote(byte voice, int note)
{
  SID::writeData(SID_ADDR_FREQ_LOW, SID::MIDI2freq[note]);
  SID::writeData(SID_ADDR_FREQ_HI, (SID::MIDI2freq[note] >> 8)); 
  SID_REG_CONTROLREG.GATE = 1; 
  SIDREG(SID_ADDR_CONTROLREG) = *(char*)&SID_REG_CONTROLREG;  
}

void SIDVoice::setPWLo(byte voice, byte dutyCycle) 
{
  //SIDREG(SID_ADDR_PW_Array[voice]) = (dutyCycle << 1);
}

void SIDVoice::setPWHi(byte voice, byte dutyCycle)
{
  //SIDREG(SID_ADDR_PW_Array[voice] + 1) = (dutyCycle << 1);
}

void SIDVoice::setGate(byte voice, boolean active)
{

}

void SIDVoice::setSync(byte voice, boolean active)
{
  //*SID_REG_CONTROLREG_Array[voice].SYNC = active;
//  SID_REG_CONTROLREG_V1.SYNC = active;
  //SIDREG(SID_ADDR_CONTROLREG_V1+((voice-1)*7)) = *(char*)&SID_REG_CONTROLREG_Array[voice];
}

void SIDVoice::setRingMod(byte voice, boolean active)
{

}

void SIDVoice::setTest(byte voice, boolean active)
{

}

void SIDVoice::setTriangle(byte voice, boolean active)
{

}

void SIDVoice::setSawtooth(byte voice, boolean active)
{

}

void SIDVoice::setPulse(byte voice, boolean active)
{

}

void SIDVoice::setNoise(byte voice, boolean active)
{

}

void SIDVoice::setEnvelopeAttack(byte voice, byte rate)
{

}
void SIDVoice::setEnvelopeDecay(byte voice, byte rate)
{

}
void SIDVoice::setEnvelopeSustain(byte voice, byte level)
{

}
void SIDVoice::setEnvelopeRelease(byte voice, byte rate)
{

}

void SIDVoice::setInstrument(byte attack, byte decay, byte sustain, byte rel, bool noise, bool square, bool sawtooth, bool triangle, int pwm)
{
  SID_REG_ATTACK_DECAY.ATTACK = attack;
  SID_REG_ATTACK_DECAY.DECAY = decay;
  SID_REG_SUSTAIN_RELEASE.SUSTAIN = sustain;
  SID_REG_SUSTAIN_RELEASE.RELEASE = rel;
  SID_REG_CONTROLREG.NOISE_WAVE = noise;
  SID_REG_CONTROLREG.SQUARE_WAVE = square;
  SID_REG_CONTROLREG.SAWTOOTH_WAVE = sawtooth;
  SID_REG_CONTROLREG.TRIANGLE_WAVE = triangle;
  //TODO: add PWM
  SIDREG(SID_ADDR_ATTACK_DECAY) = *(char*)&SID_REG_ATTACK_DECAY;        //TODO: Make this a static function to save space.
  SIDREG(SID_ADDR_SUSTAIN_RELEASE) = *(char*)&SID_REG_SUSTAIN_RELEASE; 
  SIDREG(SID_ADDR_CONTROLREG) = *(char*)&SID_REG_CONTROLREG;  
}

void SID::setVolume(byte voice, byte volume)  //TODO: get rid of voice
{
  SID_REG_MODE_VOLUME.VOLUME = volume;
  SIDREG(SID_ADDR_FILTER_MODE_VOL) = *(char*)&SID_REG_MODE_VOLUME;
}

void SID::reset(){
  //Filter
  SIDREG(SID_ADDR_FILTER_FC_LOW) = 0;
  SIDREG(SID_ADDR_FILTER_FC_HI) = 0;
  SIDREG(SID_ADDR_FILTER_RES_FILT) = 0;
  SID_REG_MODE_VOLUME.OFF = 0;
  SID_REG_MODE_VOLUME.HP = 0;
  SID_REG_MODE_VOLUME.BP = 0;
  SID_REG_MODE_VOLUME.LP = 0;
  SID_REG_MODE_VOLUME.VOLUME = 0x0;
  SIDREG(SID_ADDR_FILTER_MODE_VOL) = *(char*)&SID_REG_MODE_VOLUME;
  //SIDREG(SID_ADDR_FILTER_MODE_VOL) = 0xF; //TODO: Set this to zero
}


