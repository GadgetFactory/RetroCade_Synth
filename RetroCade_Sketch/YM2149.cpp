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

struct YM_REG_ENVSHAPE_STRUCT{
    unsigned int EMPTY : 4;
    unsigned int CONT : 1; 
    unsigned int ATT : 1;
    unsigned int ALT : 1;
    unsigned int HOLD : 1;      
} ;
YM_REG_ENVSHAPE_STRUCT YM_REG_ENVSHAPE;   

void YMVoice::setBase(int freqAddress, int volumeAddress)
{
  YM_ADDR_FREQ = freqAddress;
  YM_ADDR_LEVEL = volumeAddress; 
}

/*!
@par Description
Set a voice to a MIDI Note
@par Syntax
ym2149.V1.setNote(note, active)\n
ym2149.V2.setNote(note, active)\n
ym2149.V3.setNote(note, active)

@param note The MIDI note to use
@param active Whether to activate the gate and play the note. 1 plays the note 0 does not

@par Example
~~~~~~~~{.c}
#include "YM2149.h"

YM2149 ym2149;

void setup()
{
	ym2149.V1.setNote(51, 1)
	ym2149.V2.setNote(52, 1)
	ym2149.V3.setNote(53, 1)
}

void loop() {}
~~~~~~~~
\n
*/
void YMVoice::setNote(int note, boolean active)
{
  //setTone(active);
  YM2149::writeData(YM_ADDR_FREQ, YM2149::MIDI2freq[note]);
  YM2149::writeData(YM_ADDR_FREQ+1, (YM2149::MIDI2freq[note] >> 8));
  currentFreq = YM2149::MIDI2freq[note];
}

/*!
@par Description
Set the frequency of the YM2149 voice
@par Datasheet
[From Datasheet] (https://github.com/GadgetFactory/RetroCade_Synth/blob/master/docs/ym2149.pdf?raw=true)\n
![Freq] (freq1.png)
![Freq] (freq2.png)
@par Syntax
ym2149.V1.setFreq(freq)\n
ym2149.V2.setFreq(freq)\n
ym2149.V3.setFreq(freq)

@param freq The frequency to use for the voice.

@par Example
~~~~~~~~{.c}
#include "YM2149.h"

YM2149 ym2149;

void setup()
{
	ym2149.V1.setFreq(6207)
	ym2149.V2.setFreq(6577)
	ym2149.V3.setFreq(6968)
}

void loop() {}
~~~~~~~~
\n
*/
void YMVoice::setFreq(int freq)
{
  YM2149::writeData(YM_ADDR_FREQ, freq);
  YM2149::writeData(YM_ADDR_FREQ+1, (freq >> 8));  
}

/*!
@par Description
Return what the current frequency is set to.
@par Syntax
int frequency = ym2149.V1.getCurrentFreq()
@par Returns
the frequency of the voice (int)

\n
*/
int YMVoice::getCurrentFreq()
{
  return currentFreq;
}

/*!
@par Description
Sets the Noise bit of the control register
@par Datasheet
[From Datasheet] (https://github.com/GadgetFactory/RetroCade_Synth/blob/master/docs/ym2149.pdf?raw=true)\n
![Noise] (noise.png)

@par Syntax
ym2149.V1.setNoise(active)

@param active 1 on 0 turns off

\n
*/
void YMVoice::setNoise(boolean active)
{
  switch (YM_ADDR_FREQ) { 
    case 0:
      YM_REG_MIXER.NOISEA = !active;
      break;
    case 2:
      YM_REG_MIXER.NOISEB = !active;
      break;
    case 4:
      YM_REG_MIXER.NOISEC = !active;
      break;
    default:
      return;
      break;        
  }  
  YM2149REG(YM_ADDR_MIXER) = *(char*)&YM_REG_MIXER;
}

/*!
@par Description
Sets the Envelope bit of the level register
@par Datasheet
[From Datasheet] (https://github.com/GadgetFactory/RetroCade_Synth/blob/master/docs/ym2149.pdf?raw=true)\n
![Envelope] (level.png)

@par Syntax
ym2149.V1.setEnvelope(active)

@param active 1 on 0 turns off

\n
*/
void YMVoice::setEnvelope(boolean active)
{
      YM_REG_LEVEL.MODE = active;
      YM2149REG(YM_ADDR_LEVEL) = *(char*)&YM_REG_LEVEL;  
}

/*!
@par Description
Sets the Tone bit of the Mixer register
@par Datasheet
[From Datasheet] (https://github.com/GadgetFactory/RetroCade_Synth/blob/master/docs/ym2149.pdf?raw=true)\n
![Tone] (mixer.png)

@par Syntax
ym2149.V1.setTone(active)

@param active 1 on 0 turns off

\n
*/
void YMVoice::setTone(boolean active)
{
  int tmp;
  if (active == true) 
    tmp = 0;
  else
    tmp = 1;
  switch (YM_ADDR_LEVEL) {  
    case 0x08:
      YM_REG_MIXER.TONEA = tmp;
      break;
    case 0x09:
      YM_REG_MIXER.TONEB = tmp;
      break;
    case 0x0A:
      YM_REG_MIXER.TONEC = tmp;
      break;
    default:
      return;
      break;       
  }
  YM2149REG(YM_ADDR_MIXER) = *(char*)&YM_REG_MIXER; 
  //Serial.println(*(char*)&YM_REG_MIXER, BIN);
}

/*!
@par Description
Sets the Volume bit of the level register
@par Datasheet
[From Datasheet] (https://github.com/GadgetFactory/RetroCade_Synth/blob/master/docs/ym2149.pdf?raw=true)\n
![Volume] (level.png)

@par Syntax
ym2149.V1.setVolume(volume)

@param volume byte from 0 to 15 sets the volume, 15 is loudest.

@par Example
~~~~~~~~{.c}
#include "YM2149.h"

YM2149 ym2149;

void setup()
{
	//Turn volume to the max value
	ym2149.setVolume(15);
}

void loop() {}
~~~~~~~~
\n
*/
void YMVoice::setVolume(byte volume)
{
  YM_REG_LEVEL.LEVEL = volume;   
  YM2149REG(YM_ADDR_LEVEL) = *(char*)&YM_REG_LEVEL;  
  //YM2149::writeData(YM_ADDR_LEVEL, volume & 0x0f);
}

/*!
@par Description
Return what the current volume is set to.
@par Syntax
int frequency = ym2149.V1.getVolume()
@par Returns
the volume of the voice (int)

\n
*/
byte YMVoice::getVolume()
{
  return YM_REG_LEVEL.LEVEL;
}

/*!
@par Description
Reset a specific voice to zeroes.
@par Syntax
ym2149.V1.reset()
ym2149.V2.reset()
ym2149.V3.reset()
@par Parameters
none
@par Example
~~~~~~~~{.c}
#include "YM2149.h"

YM2149 ym2149;

void setup()
{
	ym2149.V1.reset();
	ym2149.V2.reset();
	ym2149.V3.reset();
}

void loop() {}
~~~~~~~~
\n
*/
void YMVoice::reset()
{
  //set frequency to no freq
  setNote(128,true);   

  //set volume to zero and envelope mode off
  YM_REG_LEVEL.EMPTY = 0;  
  YM_REG_LEVEL.MODE = 0;  
  YM_REG_LEVEL.LEVEL = 0;   
  YM2149REG(YM_ADDR_LEVEL) = *(char*)&YM_REG_LEVEL;
}

void YMVoice::handleCC(byte number, byte value)
{
  //Handle the Control Changes for YM2149
  switch (number) {  
    case 2:
      YM2149::setEnvelopeCONT(value);
      break;
    case 3:
      YM2149::setEnvelopeATT(value);
      break;
    case 4:
      YM2149::setEnvelopeALT(value);
      break;
    case 5:
      YM2149::setEnvelopeHOLD(value);
      break;    
    case 6:
      setNoise(value);
      break; 
    case 7:
      setTone(value);
      break;    
    case 8:
      setEnvelope(value);
      break; 
    case 71:
      setVolume(value/8);
      break;       
    case 74:
      YM2149::setNoiseFrequency(value);
      break;        
    case 75:
      YM2149::setEnvelopeFrequencyLo(value << 1);
      break;    
    case 76:
      YM2149::setEnvelopeFrequencyHi(value << 1);
      break;                 
    default:
      return;
      break;       
  }     
}

/*!
@par Description
	Contructor that creates an instance of the YM2149 object that contains three YM2149Voice objects. V1, V2, V3 
These objects are used to interact with and control the registers for the YM2149 hardware implemented inside the Papilio FPGA board.
Calling this contructor will initialize the YM2149 and YM2149Voice objects to their default values.
@par Datasheet
[From Datasheet] (https://github.com/GadgetFactory/RetroCade_Synth/blob/master/docs/ym2149.pdf?raw=true)\n
A list of the YM2149 Registers:
![Register] (registers.png)
@par Syntax
	YM2149()
@par Parameters
	none
@par Example
~~~~~~~~{.c}
#include "YM2149.h"

YM2149 ym2149;

void setup()
{
	ym2149.setVolume(15);
	ym2149.V1.setNote(51, 1);
	ym2149.V2.setNote(51, 1);
	ym2149.V3.setNote(51, 1);	
}

void loop() {}
~~~~~~~~
\n
*/
YM2149::YM2149(){  
  V1.setBase(YM_ADDR_FREQ_A, YM_ADDR_LEVEL_A);
  V2.setBase(YM_ADDR_FREQ_B, YM_ADDR_LEVEL_B);
  V3.setBase(YM_ADDR_FREQ_C, YM_ADDR_LEVEL_C);  
  reset();
}

/*!
@par Description
Use this public function to bypass the YM2149 and YM2149Voice functions and write directly to the registers of the YM2149 hardware.
@par Syntax
YM2149::writeData(address, data)
@param address the address from the YM2149 datasheet for the register you want to write to.
@param data what you want to write to the YM2149 register

@par Example
~~~~~~~~{.c}
#include "YM2149.h"

YM2149 ym2149;

void setup()
{
	//This will set Voice 1 Freq Lo to 0
	YM2149::writeData(0, 0);
}

void loop() {}
~~~~~~~~
\n
*/
void YM2149::writeData(unsigned char address, unsigned char data)
{
  YM2149REG(address) = data;
}

/*!
@par Description
Sets the Noise frequency bits of the Noise Frequency register
@par Datasheet
[From Datasheet] (https://github.com/GadgetFactory/RetroCade_Synth/blob/master/docs/ym2149.pdf?raw=true)\n
![noise] (noise.png)

@par Syntax
ym2149.V1.setNoiseFrequency(freq)

@param freq 5 bit noise frequency

\n
*/
void YM2149::setNoiseFrequency(byte freq)
{    
  writeData(YM_ADDR_NOISE, freq);
}

/*!
@par Description
Sets the 16 bit Envelope Frequency registers
@par Datasheet
[From Datasheet] (https://github.com/GadgetFactory/RetroCade_Synth/blob/master/docs/ym2149.pdf?raw=true)\n
![envelope] (envelope.png)

@par Syntax
ym2149.V1.setEnvelopeFrequency(freq)

@param freq 16 bit Envelope Frequency

\n
*/
void YM2149::setEnvelopeFrequency(int freq)
{    
  writeData(YM_ADDR_FREQ_E, freq);
  writeData(YM_ADDR_FREQ_E+1, (freq >> 8));
}

void YM2149::setEnvelopeFrequencyLo(byte freq)
{    
  writeData(YM_ADDR_FREQ_E, freq);
}

void YM2149::setEnvelopeFrequencyHi(byte freq)
{    
  writeData(YM_ADDR_FREQ_E+1, (freq >> 8));
}

/*!
@par Description
Sets the CONT bit of the Envelope Shape Register
@par Datasheet
[From Datasheet] (https://github.com/GadgetFactory/RetroCade_Synth/blob/master/docs/ym2149.pdf?raw=true)\n
![envelope] (envshape1.png)
![envelope] (envshape2.png)

@par Syntax
ym2149.V1.setEnvelopeCONT(active)

@param active 1 for on 0 for off

\n
*/
void YM2149::setEnvelopeCONT(boolean active)
{
  YM_REG_ENVSHAPE.CONT = active; 
  YM2149REG(YM_ADDR_SHAPE_E) = *(char*)&YM_REG_ENVSHAPE;
}

/*!
@par Description
Sets the ATT bit of the Envelope Shape Register
@par Datasheet
[From Datasheet] (https://github.com/GadgetFactory/RetroCade_Synth/blob/master/docs/ym2149.pdf?raw=true)\n
![envelope] (envshape1.png)
![envelope] (envshape2.png)

@par Syntax
ym2149.V1.setEnvelopeATT(active)

@param active 1 for on 0 for off

\n
*/
void YM2149::setEnvelopeATT(boolean active)
{
  YM_REG_ENVSHAPE.ATT = active; 
  YM2149REG(YM_ADDR_SHAPE_E) = *(char*)&YM_REG_ENVSHAPE;
}

/*!
@par Description
Sets the ALT bit of the Envelope Shape Register
@par Datasheet
[From Datasheet] (https://github.com/GadgetFactory/RetroCade_Synth/blob/master/docs/ym2149.pdf?raw=true)\n
![envelope] (envshape1.png)
![envelope] (envshape2.png)

@par Syntax
ym2149.V1.setEnvelopeALT(active)

@param active 1 for on 0 for off

\n
*/
void YM2149::setEnvelopeALT(boolean active)
{
  YM_REG_ENVSHAPE.ALT = active; 
  YM2149REG(YM_ADDR_SHAPE_E) = *(char*)&YM_REG_ENVSHAPE;
}

/*!
@par Description
Sets the HOLD bit of the Envelope Shape Register
@par Datasheet
[From Datasheet] (https://github.com/GadgetFactory/RetroCade_Synth/blob/master/docs/ym2149.pdf?raw=true)\n
![envelope] (envshape1.png)
![envelope] (envshape2.png)

@par Syntax
ym2149.V1.setEnvelopeHOLD(active)

@param active 1 for on 0 for off

\n
*/
void YM2149::setEnvelopeHOLD(boolean active)
{
  YM_REG_ENVSHAPE.HOLD = active; 
  YM2149REG(YM_ADDR_SHAPE_E) = *(char*)&YM_REG_ENVSHAPE;
}

/*!
@par Description
Set all YM2149 registers to their default values.
@par Syntax
ym2149.reset()
@par Parameters
none
@par Example
~~~~~~~~{.c}
#include "YM2149.h"

YM2149 ym2149;

void setup()
{
	ym2149.reset();
}

void loop() {}
~~~~~~~~
\n
*/
void YM2149::reset(){
  //no noise 
  writeData(YM_ADDR_NOISE, 0x00);
  
  //turn off noise and audio channels
  YM_REG_MIXER.EMPTY = 0;
  YM_REG_MIXER.NOISEC = 1;
  YM_REG_MIXER.NOISEB = 1;
  YM_REG_MIXER.NOISEA = 1;
  YM_REG_MIXER.TONEC = 0;
  YM_REG_MIXER.TONEB = 0;
  YM_REG_MIXER.TONEA = 0;    
  YM2149REG(YM_ADDR_MIXER) = *(char*)&YM_REG_MIXER;   
  
  //set freq of envelope to zero
  setEnvelopeFrequency(0);
  
  //set shape of envelope to defaults
  YM_REG_ENVSHAPE.EMPTY = 0;
  YM_REG_ENVSHAPE.CONT = 1;
  YM_REG_ENVSHAPE.ATT = 1;
  YM_REG_ENVSHAPE.ALT = 1;
  YM_REG_ENVSHAPE.HOLD = 1;  
  YM2149REG(YM_ADDR_SHAPE_E) = *(char*)&YM_REG_ENVSHAPE;
  
  //reset voices
  V1.reset();
  V2.reset();
  V3.reset();
}  
