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

#define SIDINSTRUMENTS 9
int sidInstrument[SIDINSTRUMENTS][9]=
        { {0,0,15,0,0,0,0,1,0},                                              
          {0,5,5,0,1,0,0,0,0},                                                                                           
          {12,0,12,0,0,0,1,0,0},
          {0,9,2,1,0,0,1,0,0},
          {0,9,0,0,0,1,0,0,512},
          {0,9,9,0,0,1,0,0,2048},
          {8,9,4,1,0,1,0,0,512},
          {0,9,0,0,0,0,0,1,0},
          {9,4,4,0,0,0,0,1,0} };
          
//byte sidInstrumentName[SIDINSTRUMENTS][20]=        //TODO: Goofy way to do this, change to struct or function when strcpy works.
//        { "Calliope",                                              
//          "Accordian",                                                                                           
//          "Harpsicord" };  

/*!
@par Description
	Contructor that creates an instance of the SID object that contains three SIDVoice objects. V1, V2, V3 
These objects are used to interact with and control the registers for the SID hardware implemented inside the Papilio FPGA board.
Calling this contructor will initialize the SID and SIDVoice objects to their default values.
@par Syntax
	SID()
@par Parameters
	none
@par Example
~~~~~~~~{.c}
#include "SID.h"

SID sid;

void setup()
{
	sid.setVolume(15);
	sid.V1.setInstrument("Calliope",0,0,15,0,0,0,0,1,0); //Calliope
	sid.V2.setInstrument("Accordian",12,0,12,0,0,0,1,0,0); //Accordian
	sid.V3.setInstrument("Harpsicord",0,9,0,0,0,1,0,0,512); //Harpsicord
	sid.V1.setNote(51, 1);
	sid.V2.setNote(51, 1);
	sid.V3.setNote(51, 1);	
}

void loop() {}
~~~~~~~~
\n
*/
SID::SID(){  
  V1.setBase(SID_ADDR_BASE_V1);
  V2.setBase(SID_ADDR_BASE_V2);
  V3.setBase(SID_ADDR_BASE_V3);

  //reset whole sid and initialize values.
  reset();
}  

/*!
@par Description
Use this public function to bypass the SID and SIDVoice functions and write directly to the registers of the SID hardware. Look at the SID datasheet for the registers (http://www.waitingforfriday.com/index.php/Commodore_SID_6581_Datasheet#SID_Control_Registers)
@par Syntax
SID::writeData(address, data)
@param address the address from the SID datasheet for the register you want to write to.
@param data what you want to write to the SID register

@par Example
~~~~~~~~{.c}
#include "SID.h"

SID sid;

void setup()
{
	//This will set Voice 1 Freq Lo to 0
	SID::writeData(0, 0);
}

void loop() {}
~~~~~~~~
\n
*/
void SID::writeData(unsigned char address, unsigned char data)
{
  SIDREG(address) = data;
}

SIDVoice::SIDVoice()
{

}

SIDVoice::SIDVoice(int address)    //TODO: Remove this or make it work right.
{

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

/*!
@par Description
Reset a specific voice to zeroes.
@par Syntax
sid.V1.reset()
sid.V2.reset()
sid.V3.reset()
@par Parameters
none
@par Example
~~~~~~~~{.c}
#include "SID.h"

SID sid;

void setup()
{
	sid.V1.reset();
	sid.V2.reset();
	sid.V3.reset();
}

void loop() {}
~~~~~~~~
\n
*/
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

/*!
@par Description
Set a voice to a MIDI Note
@par Syntax
sid.V1.setNote(note, active)\n
sid.V2.setNote(note, active)\n
sid.V3.setNote(note, active)

@param note The MIDI note to use
@param active Whether to activate the gate and play the note. 1 plays the note 0 does not

@par Example
~~~~~~~~{.c}
#include "SID.h"

SID sid;

void setup()
{
	sid.V1.setNote(51, 1)
	sid.V2.setNote(52, 1)
	sid.V3.setNote(53, 1)
}

void loop() {}
~~~~~~~~
\n
*/
void SIDVoice::setNote(int note, boolean active)
{
  SID::writeData(SID_ADDR_FREQ_LOW, SID::MIDI2freq[note]);
  SID::writeData(SID_ADDR_FREQ_HI, (SID::MIDI2freq[note] >> 8)); 
  setGate(active); 
  currentFreq = SID::MIDI2freq[note];
}

/*!
@par Description
Set the frequency of the SID voice
@par Datasheet
[From Datasheet] (http://www.waitingforfriday.com/index.php/Commodore_SID_6581_Datasheet#Freq_Lo.2FFreq_Hi_.28Registers_00-01.29)\n
Together these registers form a 16-bit number which linearly controls the Frequency of Oscillator 1. The frequency is determined by the following equation:
Fout = (Fn * Fclk/16777216) Hz
Where Fn is the 16-bit number in the Frequency registers and Fclk is the system clock applied to the 2 input (pin 6). For a standard 1.0 Mhz clock, the frequency is given by:
Fout = (Fn * 0.0596) Hz
A complete table of values for generating 8 octaves of the equally-tempered musical scale with concert A (440 Hz) tuning is provided in Appendix A. It should be noted that the frequency resolution of SID is sufficient for any tuning scale and allows sweeping from note to note (portamento) with no discernible frequency steps. 
@par Syntax
sid.V1.setFreq(freq)\n
sid.V2.setFreq(freq)\n
sid.V3.setFreq(freq)

@param freq The frequency to use, refer to the [datasheet] (http://www.waitingforfriday.com/index.php/Commodore_SID_6581_Datasheet#Appendix_A_-_Equal-Tempered_Musical_Scale_Values) for the note equivalent 

@par Example
~~~~~~~~{.c}
#include "SID.h"

SID sid;

void setup()
{
	sid.V1.setFreq(6207)
	sid.V2.setFreq(6577)
	sid.V3.setFreq(6968)
}

void loop() {}
~~~~~~~~
\n
*/
void SIDVoice::setFreq(int freq)
{
  SID::writeData(SID_ADDR_FREQ_LOW, freq);
  SID::writeData(SID_ADDR_FREQ_HI, (freq >> 8)); 
}

/*!
@par Description
Return what the current frequency is set to.
@par Syntax
int frequency = sid.V1.getCurrentFreq()
@par Returns
the frequency of the voice (int)

\n
*/
int SIDVoice::getCurrentFreq()
{
  return currentFreq;
}

/*!
@par Description
Sets the low register PW Register
@par Datasheet
[From Datasheet] (http://www.waitingforfriday.com/index.php/Commodore_SID_6581_Datasheet#PW_Lo.2FPW_Hi_.28Registers_02-03.29)\n
Together these registers form a 12-bit number (bits 4-7 of PW Hi are not used) which linearly controls the Pulse Width (duty cycle) of the Pulse waveform on Oscillator 1. The pulse width is determined by the following equation:
PWout = (PWn/40.95) %
Where PWn is the 12-bit number in the Pulse Width registers.
The pulse width resolution allows the width to be smoothly swept with no discernible stepping. Note that the Pulse waveform on Oscillator 1 must be selected in order for the Pulse Width registers to have any audible effect. A value of 0 or 4095 ($FFF) in the Pulse Width registers will produce a constant DC output, while a value of 2048 ($800) will produce a square wave. 
@par Syntax
sid.V1.setPWLo(dutyCycle)

@param dutyCycle Pulse Width

\n
*/
void SIDVoice::setPWLo(byte dutyCycle) 
{
  SIDREG(SID_ADDR_PW_LOW) = (dutyCycle);
}

/*!
@par Description
Sets the high register PW Register
@par Datasheet
[From Datasheet] (http://www.waitingforfriday.com/index.php/Commodore_SID_6581_Datasheet#PW_Lo.2FPW_Hi_.28Registers_02-03.29)\n
Together these registers form a 12-bit number (bits 4-7 of PW Hi are not used) which linearly controls the Pulse Width (duty cycle) of the Pulse waveform on Oscillator 1. The pulse width is determined by the following equation:
PWout = (PWn/40.95) %
Where PWn is the 12-bit number in the Pulse Width registers.
The pulse width resolution allows the width to be smoothly swept with no discernible stepping. Note that the Pulse waveform on Oscillator 1 must be selected in order for the Pulse Width registers to have any audible effect. A value of 0 or 4095 ($FFF) in the Pulse Width registers will produce a constant DC output, while a value of 2048 ($800) will produce a square wave. 
@par Syntax
sid.V1.setPWHi(dutyCycle)

@param dutyCycle Pulse Width

\n
*/
void SIDVoice::setPWHi(byte dutyCycle)
{
  SIDREG(SID_ADDR_PW_HI) = (dutyCycle);
}

/*!
@par Description
Control the voice gate
@par Datasheet
[From Datasheet] (http://www.waitingforfriday.com/index.php/Commodore_SID_6581_Datasheet#Control_Register_.28Register_04.29)\n
The GATE bit controls the Envelope Generator for the Voice. When this bit is set to a one, the Envelope Generator is Gated (triggered) and the ATTACK/DECAY/SUSTAIN cycle is initiated. When the bit is reset to a zero, the RELEASE cycle begins. The Envelope Generator controls the amplitude of Oscillator 1 appearing at the audio output, therefore, the GATE bit must be set (along with suitable envelope parameters) for the selected output of Oscillator 1 to be audible. A detailed discussion of the Envelope Generator can be found in Appendix B. 
@par Syntax
sid.V1.setGate(active)

@param active 1 to trigger the gate 0 to reset

\n
*/
void SIDVoice::setGate(boolean active)
{
  SID_REG_CONTROLREG.GATE = active; 
  SIDREG(SID_ADDR_CONTROLREG) = *(char*)&SID_REG_CONTROLREG;
}

/*!
@par Description
Sets the Sync bit of the control register
@par Datasheet
[From Datasheet] (http://www.waitingforfriday.com/index.php/Commodore_SID_6581_Datasheet#Control_Register_.28Register_04.29)\n
The SYNC bit, when set to a one, Synchronizes the fundamental frequency of Oscillator 1 with the fundamental frequency of Oscillator 3, producing “Hard Sync” effects. Varying the frequency of Oscillator 1 with respect to Oscillator 3 produces a wide range of complex harmonic structures from Voice 1 at the frequency of Oscillator 3. In order for sync to occur Oscillator 3 must be set to some frequency other than zero but preferably lower than the frequency of Oscillator 1. No other parameters of Voice 3 have any effect on sync. 
@par Syntax
sid.V1.setSync(active)

@param active 1 syncs 0 turns sync off

\n
*/
void SIDVoice::setSync(boolean active)
{
  SID_REG_CONTROLREG.SYNC = active;
  SIDREG(SID_ADDR_CONTROLREG) = *(char*)&SID_REG_CONTROLREG;
}

/*!
@par Description
Sets the Ring Mod bit of the control register
@par Datasheet
[From Datasheet] (http://www.waitingforfriday.com/index.php/Commodore_SID_6581_Datasheet#Control_Register_.28Register_04.29)\n
The RING MOD bit, when set to a one, replaces the Triangle waveform output of Oscillator 1 with a “Ring Modulated” combination of Oscillators 1 and 3. Varying the frequency of Oscillator 1 with respect to Oscillator 3 produces a wide range of non-harmonic overtone structures for creating bell or gong sounds and for special effects. In order for ring modulation to be audible, the Triangle waveform of Oscillator 1 must be selected and Oscillator 3 must be set to some frequency other than zero. No other parameters of Voice 3 have any effect on ring modulation. 
@par Syntax
sid.V1.setRingMod(active)

@param active 1 Ring Mod on 0 turns Ring Mod off

\n
*/
void SIDVoice::setRingMod(boolean active)
{
  SID_REG_CONTROLREG.RING_MOD = active; 
  SIDREG(SID_ADDR_CONTROLREG) = *(char*)&SID_REG_CONTROLREG;
}

/*!
@par Description
Sets the Test bit of the control register
@par Datasheet
[From Datasheet] (http://www.waitingforfriday.com/index.php/Commodore_SID_6581_Datasheet#Control_Register_.28Register_04.29)\n
The TEST bit, when set to a one, resets and locks Oscillator 1 at zero until the TEST bit is cleared. The Noise waveform output of Oscillator 1 is also reset and the Pulse waveform output is held at a DC level. Normally this bit is used for testing purposes, however, it can be used to synchronize Oscillator 1 to external events, allowing the generation of highly complex waveforms under real-time software control. 
@par Syntax
sid.V1.setTest(active)

@param active 1 Test on 0 turns Test off

\n
*/
void SIDVoice::setTest(boolean active)
{
  SID_REG_CONTROLREG.TEST = active; 
  SIDREG(SID_ADDR_CONTROLREG) = *(char*)&SID_REG_CONTROLREG;
}

/*!
@par Description
Sets the Triangle bit of the control register
@par Datasheet
[From Datasheet] (http://www.waitingforfriday.com/index.php/Commodore_SID_6581_Datasheet#Control_Register_.28Register_04.29)\n
When set to a one, the Triangle waveform output of Oscillator 1 is selected. The Triangle waveform is low in harmonics and has a mellow, flute-like quality. 
@par Syntax
sid.V1.setTriangle(active)

@param active 1 on 0 turns off

\n
*/
void SIDVoice::setTriangle(boolean active)
{
  SID_REG_CONTROLREG.TRIANGLE_WAVE = active; 
  SIDREG(SID_ADDR_CONTROLREG) = *(char*)&SID_REG_CONTROLREG;
}

/*!
@par Description
Sets the Sawtooth bit of the control register
@par Datasheet
[From Datasheet] (http://www.waitingforfriday.com/index.php/Commodore_SID_6581_Datasheet#Control_Register_.28Register_04.29)\n
When set to a one, the Sawtooth waveform of Oscillator 1 is selected. The sawtooth waveform is rich in even and odd harmonics and has a bright, brassy quality. 
@par Syntax
sid.V1.setSawtooth(active)

@param active 1 on 0 turns off

\n
*/
void SIDVoice::setSawtooth(boolean active)
{
  SID_REG_CONTROLREG.SAWTOOTH_WAVE = active; 
  SIDREG(SID_ADDR_CONTROLREG) = *(char*)&SID_REG_CONTROLREG;
}

/*!
@par Description
Sets the Square bit of the control register
@par Datasheet
[From Datasheet] (http://www.waitingforfriday.com/index.php/Commodore_SID_6581_Datasheet#Control_Register_.28Register_04.29)\n
When set to a one, the Pulse waveform output of Oscillator 1 is selected. The harmonic content of this waveform can be adjusted by the Pulse Width registers, producing tone Qualities ranging from a bright, hollow square wave to a nasal, reedy pulse. Sweeping the pulse width in real-time produces a dynamic “phasing” effect which adds a sense of motion to the sound. Rapidly jumping between different pulse widths can produce interesting harmonic sequences.  
@par Syntax
sid.V1.setSquare(active)

@param active 1 on 0 turns off

\n
*/
void SIDVoice::setSquare(boolean active)
{
  SID_REG_CONTROLREG.SQUARE_WAVE = active; 
  SIDREG(SID_ADDR_CONTROLREG) = *(char*)&SID_REG_CONTROLREG;
}

/*!
@par Description
Sets the Square bit of the control register and the PWM at the same time
@par Datasheet
[From Datasheet] (http://www.waitingforfriday.com/index.php/Commodore_SID_6581_Datasheet#Control_Register_.28Register_04.29)\n
When set to a one, the Pulse waveform output of Oscillator 1 is selected. The harmonic content of this waveform can be adjusted by the Pulse Width registers, producing tone Qualities ranging from a bright, hollow square wave to a nasal, reedy pulse. Sweeping the pulse width in real-time produces a dynamic “phasing” effect which adds a sense of motion to the sound. Rapidly jumping between different pulse widths can produce interesting harmonic sequences.  
@par Syntax
sid.V1.setSquare(active, pwm)

@param active 1 on 0 turns off
@param pwm set the pwm register for the voice

\n
*/
void SIDVoice::setSquare(boolean active, int pwm)
{
  SID_REG_CONTROLREG.SQUARE_WAVE = active;
  SIDREG(SID_ADDR_CONTROLREG) = *(char*)&SID_REG_CONTROLREG;
  setPWLo(pwm);
  setPWHi(pwm >> 8);  
}

/*!
@par Description
Sets the Noise bit of the control register
@par Datasheet
[From Datasheet] (http://www.waitingforfriday.com/index.php/Commodore_SID_6581_Datasheet#Control_Register_.28Register_04.29)\n
When set to a one, the Noise output waveform of Oscillator 1 is selected. This output is a random signal which changes at the frequency of Oscillator 1. The sound quality can be varied from a low rumbling to hissing white noise via the Oscillator 1 Frequency registers. Noise is useful in creating explosions, gunshots, jet engines, wind, surf and other un-pitched sounds, as well as snare drums and cymbals. Sweeping the Oscillator frequency with Noise selected produces a dramatic rushing effect. One of the output waveforms must be selected for Oscillator 1 to be audible, however it is NOT necessary to deselect waveforms to silence the output of Voice 1. The amplitude of Voice 1 at the final output is a function of the Envelope Generator only.
NOTE: The oscillator output waveforms are NOT additive. If more than one output waveform is selected simultaneously, the result will be a logical ANDing of the waveforms. Although this technique can be used to generate additional waveforms beyond the four listed above, it must be used with care. If any other waveform is selected while Noise is on, the Noise output can “lock up”. If this occurs, the Noise output will remain silent until reset by the TEST bit or by bringing /RES (pin 5) low. 
@par Syntax
sid.V1.setNoise(active)

@param active 1 on 0 turns off

\n
*/
void SIDVoice::setNoise(boolean active)
{
  SID_REG_CONTROLREG.NOISE_WAVE = active; 
  SIDREG(SID_ADDR_CONTROLREG) = *(char*)&SID_REG_CONTROLREG;
}

/*!
@par Description
Sets the Attack bits
@par Datasheet
[From Datasheet] (http://www.waitingforfriday.com/index.php/Commodore_SID_6581_Datasheet#Attack.2FDecay_.28Register_05.29)\n
Select 1 of 16 ATTACK rates for the Voice 1 Envelope Generator. The ATTACK rate determines how rapidly the output of Voice 1 rises from zero to peak amplitude when the Envelope Generator is Gated. The 16 ATTACK rates are listed in Table 2 in the datasheet.
@par Syntax
sid.V1.setEnvelopeAttack(rate)

@param rate Attack Rate as defined in datasheet

\n
*/
void SIDVoice::setEnvelopeAttack(byte rate)
{
  SID_REG_ATTACK_DECAY.ATTACK = rate;
  SIDREG(SID_ADDR_ATTACK_DECAY) = *(char*)&SID_REG_ATTACK_DECAY;
}

/*!
@par Description
Sets the Decay bits
@par Datasheet
[From Datasheet] (http://www.waitingforfriday.com/index.php/Commodore_SID_6581_Datasheet#Attack.2FDecay_.28Register_05.29)\n
Select 1 of 16 DECAY rates for the Envelope Generator. The DECAY cycle follows the ATTACK cycle and the DECAY rate determines how rapidly the output falls from the peak amplitude to the selected SUSTAIN level. The 16 DECAY rates are listed in Table 2 of the datasheet. 
@par Syntax
sid.V1.setEnvelopeDecay(rate)

@param rate Decay Rate as defined in datasheet

\n
*/
void SIDVoice::setEnvelopeDecay(byte rate)
{
  SID_REG_ATTACK_DECAY.DECAY = rate;
  SIDREG(SID_ADDR_ATTACK_DECAY) = *(char*)&SID_REG_ATTACK_DECAY;
}

/*!
@par Description
Sets the Sustain bits
@par Datasheet
[From Datasheet] (http://www.waitingforfriday.com/index.php/Commodore_SID_6581_Datasheet#Sustain.2FRelease_.28Register_06.29)\n
Select 1 of 16 SUSTAIN levels for the Envelope Generator. The SUSTAIN cycle follows the DECAY cycle and the output of Voice 1 will remain at the selected SUSTAIN amplitude as long as the Gate bit remains set. The SUSTAIN levels range from zero to peak amplitude in 16 linear steps, with a SUSTAIN value of 0 selecting zero amplitude and a SUSTAIN value of 15 (#F) selecting the peak amplitude.
A SUSTAIN value of 8 would cause Voice 1 to SUSTAIN at an amplitude one-half the peak amplitude reached by the ATTACK cycle. 
@par Syntax
sid.V1.setEnvelopeSustain(level)

@param level Sustain Level as defined in datasheet

\n
*/
void SIDVoice::setEnvelopeSustain(byte level)
{
  SID_REG_SUSTAIN_RELEASE.SUSTAIN = level;
  SIDREG(SID_ADDR_SUSTAIN_RELEASE) = *(char*)&SID_REG_SUSTAIN_RELEASE;
}

/*!
@par Description
Sets the Release bits
@par Datasheet
[From Datasheet] (http://www.waitingforfriday.com/index.php/Commodore_SID_6581_Datasheet#Sustain.2FRelease_.28Register_06.29)\n
Select 1 of 16 RELEASE rates for the Envelope Generator. The RELEASE cycle follows the SUSTAIN cycle when the Gate bit is reset to zero. At this time, the output of Voice 1 will fall from the SUSTAIN amplitude to zero amplitude at the selected RELEASE rate. The 16 RELEASE rates are identical to the DECAY rates.
NOTE: The cycling of the Envelope Generator can be altered at any point via the Gate bit. The Envelope Generator can be Gated and Released without restriction. For example, if the Gate bit is reset before the envelope has finished the ATTACK cycle, the RELEASE cycle will immediately begin, starting from whatever amplitude had been reached. If the envelope is then Gated again (before the RELEASE cycle has reached zero amplitude), another ATTACK cycle will begin, starting from whatever amplitude had been reached. This technique can be used to generate complex amplitude envelopes via real-time software control. 
@par Syntax
sid.V1.setEnvelopeRelease(rate)

@param rate Release Rate as defined in datasheet

\n
*/
void SIDVoice::setEnvelopeRelease(byte rate)
{
  SID_REG_SUSTAIN_RELEASE.RELEASE = rate;
  SIDREG(SID_ADDR_SUSTAIN_RELEASE) = *(char*)&SID_REG_SUSTAIN_RELEASE;
}

void SIDVoice::handleCC(byte number, byte value)
{
  //Handle the Control Changes for SID
  switch (number) {  //TODO figure more efficient way to do this. Want to avoid case statements.
    case 2:
      setTriangle(value);
      break;
    case 3:
      setSawtooth(value);
      break;
    case 4:
      setSquare(value);
      break;
    case 5:
      setNoise(value);
      break;    
    case 6:
      setRingMod(value);
      break; 
    case 7:
      setSync(value);
      break;    
    case 71:
      setEnvelopeDecay(value/8);
      break;    
    case 74:
      setEnvelopeAttack(value/8);
      break;        
    case 75:
      setPWLo(value << 1);
      break;    
    case 76:
      setPWHi(value);
      break;    
    case 91:
      setEnvelopeSustain(value/8);
      break;    
    case 92:  //This is for Ring Modulation Fine. 
      ringMod(0,1, value); 
      break;         
    case 93:
      setEnvelopeRelease(value/8);
      break;    
    case 95:  //This is for Ring Modulation Coarse.  
      ringMod(1,0, value);            
    default:
      return;
      break;       
  }     
}

void SIDVoice::ringMod(byte baseOffset, byte valueOffset, byte value)
{
      switch (baseAddress) {  
        case SID_ADDR_BASE_V1:                          //When we are on Voice 1 we need to combine with Frequency of Voice 3
          SIDREG(SID_ADDR_BASE_V3 + baseOffset) = (value << valueOffset);
          break;
        case SID_ADDR_BASE_V2:                          //When we are on Voice 2 we need to combine with Frequency of Voice 1
          SIDREG(SID_ADDR_BASE_V1 + baseOffset) = (value << valueOffset);
          break;
        case SID_ADDR_BASE_V3:                          //When we are on Voice 3 we need to combine with Frequency of Voice 2
          SIDREG(SID_ADDR_BASE_V2 + baseOffset) = (value << valueOffset);
          break;
        default:
          return;
          break;       
      }  
}

/*!
@par Description
Define an instrument for the Voice
@par Syntax
setInstrument(name, attack, decay, sustain, rel, noise, square, sawtooth, triangle, pwm)

@param name Name of Instrument to be shown on LCD
@param attack Attack for instrument
@param decay Decay for instrument
@param sustain Sustain for instrument
@param rel Release for instrument
@param noise Noise bit for instrument
@param square Square bit for instrument
@param sawtooth Sawtooth bit for instrument
@param triangle Triangle bit for instrument
@param pwm PWM value for instrument


@par Example
~~~~~~~~{.c}
#include "SID.h"

SID sid;

void setup()
{
  sid.V1.setInstrument("Calliope",0,0,15,0,0,0,0,1,0); //Calliope
  sid.V2.setInstrument("Accordian",12,0,12,0,0,0,1,0,0); //Accordian
  sid.V3.setInstrument("Harpsicord",0,9,0,0,0,1,0,0,512); //Harpsicord
}

void loop() {}
~~~~~~~~
\n
*/
void SIDVoice::setInstrument(const char* name,byte attack, byte decay, byte sustain, byte rel, bool noise, bool square, bool sawtooth, bool triangle, int pwm)
{
  Serial.println("In setinstrument");
  Serial.println(name);
  strcpy(instrumentName, name);
  Serial.println(instrumentName);
  SID_REG_ATTACK_DECAY.ATTACK = attack;
  SID_REG_ATTACK_DECAY.DECAY = decay;
  SID_REG_SUSTAIN_RELEASE.SUSTAIN = sustain;
  SID_REG_SUSTAIN_RELEASE.RELEASE = rel;
  SID_REG_CONTROLREG.NOISE_WAVE = noise;
  SID_REG_CONTROLREG.SQUARE_WAVE = square;
  SID_REG_CONTROLREG.SAWTOOTH_WAVE = sawtooth;
  SID_REG_CONTROLREG.TRIANGLE_WAVE = triangle;
  setPWLo(pwm);
  setPWHi(pwm >> 8);  

  SIDREG(SID_ADDR_ATTACK_DECAY) = *(char*)&SID_REG_ATTACK_DECAY;        //TODO: Make this a static function to save space.
  SIDREG(SID_ADDR_SUSTAIN_RELEASE) = *(char*)&SID_REG_SUSTAIN_RELEASE; 
  SIDREG(SID_ADDR_CONTROLREG) = *(char*)&SID_REG_CONTROLREG;  
}

void SIDVoice::loadInstrument(byte instrument)
{
//  Serial.println("In setinstrument");
//  Serial.println(name);
//  strcpy(instrumentName, sidInstrumentName);
//  Serial.println(instrumentName);
  SID_REG_ATTACK_DECAY.ATTACK = sidInstrument[instrument][0];
  SID_REG_ATTACK_DECAY.DECAY = sidInstrument[instrument][1];
  SID_REG_SUSTAIN_RELEASE.SUSTAIN = sidInstrument[instrument][2];
  SID_REG_SUSTAIN_RELEASE.RELEASE = sidInstrument[instrument][3];
  SID_REG_CONTROLREG.NOISE_WAVE = sidInstrument[instrument][4];
  SID_REG_CONTROLREG.SQUARE_WAVE = sidInstrument[instrument][5];
  SID_REG_CONTROLREG.SAWTOOTH_WAVE = sidInstrument[instrument][6];
  SID_REG_CONTROLREG.TRIANGLE_WAVE = sidInstrument[instrument][7];
  setPWLo(sidInstrument[instrument][8]);
  setPWHi((sidInstrument[instrument][8]) >> 8);  

  SIDREG(SID_ADDR_ATTACK_DECAY) = *(char*)&SID_REG_ATTACK_DECAY;        //TODO: Make this a static function to save space.
  SIDREG(SID_ADDR_SUSTAIN_RELEASE) = *(char*)&SID_REG_SUSTAIN_RELEASE; 
  SIDREG(SID_ADDR_CONTROLREG) = *(char*)&SID_REG_CONTROLREG;  
}

/*!
@par Description
Set the SID master volume
@par Syntax
sid.setVolume(volume)
@param volume number between 0 and 15

@par Example
~~~~~~~~{.c}
#include "SID.h"

SID sid;

void setup()
{
	//Turn volume to the max value
	sid.setVolume(15);
}

void loop() {}
~~~~~~~~
\n
*/
void SID::setVolume(byte volume)  //TODO: get rid of voice
{
  SID_REG_MODE_VOLUME.VOLUME = volume;
  SIDREG(SID_ADDR_FILTER_MODE_VOL) = *(char*)&SID_REG_MODE_VOLUME;
}

/*!
@par Description
Set all SID registers to their default values.
@par Syntax
sid.reset()
@par Parameters
none
@par Example
~~~~~~~~{.c}
#include "SID.h"

SID sid;

void setup()
{
	sid.reset();
}

void loop() {}
~~~~~~~~
\n
*/
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
  V1.reset();
  V2.reset();
  V3.reset();  
}


