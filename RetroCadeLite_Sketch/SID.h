/*!
 *  @file		SID.h
 *  Project		SID Library
 *	@brief		SID Library for the ZPUino
 *	Version		1.0
 *  @author		Jack Gassett 
 *	@date		4/10/12
 *  License		GPL
 */

#ifndef LIB_SID_H_
#define LIB_SID_H_

#include <inttypes.h> 
#include <zpuino-types.h>
#include <zpuino.h>
#include "Arduino.h"

#define SID_ADDR_BASE_V1               0x00
#define SID_ADDR_BASE_V2               0x07
#define SID_ADDR_BASE_V3               0x0E

#define SID_ADDR_FILTER_FC_LOW       0x15
#define SID_ADDR_FILTER_FC_HI        0x16
#define SID_ADDR_FILTER_RES_FILT     0x17
#define SID_ADDR_FILTER_MODE_VOL     0x18

#define SID_ADDR_MISC_ENV3           0x1C

#define SIDBASE IO_SLOT(14)
#define SIDREG(x) REGISTER(SIDBASE,x)

class SIDVoice
{ 
  public:
    SIDVoice();
    SIDVoice(int address);
    void setBase(int address);   
    void setNote(int note, boolean active);
    void setFreq(int freq);
    void setPWLo(byte dutyCycle); 
    void setPWHi(byte dutyCycle);    
    void setGate(boolean active);
    void setSync(boolean active);
    void setRingMod(boolean active);
    void setTest(boolean active);
    void setTriangle(boolean active);
    void setSawtooth(boolean active);
    void setSquare(boolean active);
    void setSquare(boolean active, int pwm);
    void setNoise(boolean active);
    void setEnvelopeAttack(byte rate);
    void setEnvelopeDecay(byte rate);
    void setEnvelopeSustain(byte level);
    void setEnvelopeRelease(byte rate); 
    void setInstrument(const char* name, byte attack, byte decay, byte sustain, byte release, bool noise, bool square, bool sawtooth, bool triangle, int pwm);
    void loadInstrument(byte instrument);
    void handleCC(byte number, byte value);
    void reset(); 
    int getCurrentFreq();
  private:
    void writeData(unsigned char address, unsigned char data);
    void ringMod(byte baseOffset, byte valueOffset, byte value);
    int baseAddress;
    int currentFreq;
    int SID_ADDR_FREQ_LOW;
    int SID_ADDR_FREQ_HI;
    int SID_ADDR_PW_LOW;
    int SID_ADDR_PW_HI;
    int SID_ADDR_CONTROLREG;
    int SID_ADDR_ATTACK_DECAY;
    int SID_ADDR_SUSTAIN_RELEASE;
    char instrumentName[25];
    struct SID_REG_CONTROLREG_STRUCT{
        unsigned int NOISE_WAVE : 1; 
        unsigned int SQUARE_WAVE : 1;
        unsigned int SAWTOOTH_WAVE : 1;
        unsigned int TRIANGLE_WAVE : 1;
        unsigned int TEST : 1;
        unsigned int RING_MOD : 1;
        unsigned int SYNC : 1;
        unsigned int GATE : 1;  
    } ;
    SID_REG_CONTROLREG_STRUCT SID_REG_CONTROLREG;

    struct SID_REG_ATTACK_DECAY_STRUCT{
        unsigned int ATTACK : 4; 
        unsigned int DECAY : 4;    
    };
    SID_REG_ATTACK_DECAY_STRUCT SID_REG_ATTACK_DECAY;
    
    struct SID_REG_SUSTAIN_RELEASE_STRUCT{
        unsigned int SUSTAIN : 4; 
        unsigned int RELEASE : 4;    
    };
     SID_REG_SUSTAIN_RELEASE_STRUCT SID_REG_SUSTAIN_RELEASE;    
};

class SID
{
  public:
    SIDVoice V1;  
    SIDVoice V2;  
    SIDVoice V3;  
    SID();
    static void writeData(unsigned char address, unsigned char data);   
    void setVolume(byte volume);      
    void reset();
    static const int MIDI2freq[129];
  private:  
    struct SID_REG_MODE_VOLUME_STRUCT{
        unsigned int OFF : 1; 
        unsigned int HP : 1;
        unsigned int BP : 1;
        unsigned int LP : 1;
        unsigned int VOLUME : 4; 
    } ;  
    SID_REG_MODE_VOLUME_STRUCT SID_REG_MODE_VOLUME;
};
#endif // LIB_SID_H_
