/*!
 *  @file		SID.h
 *  Project		SID Library
 *	@brief		SID Library for the ZPUino
 *	Version		1.0
 *  @author		Jack Gassett 
 *	@date		4/10/12
 *  License		Creative Commons Atribution
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

//#define SID_ADDR_FREQ_LOW_V1         0x00
//#define SID_ADDR_FREQ_HI_V1          0x01
//#define SID_ADDR_PW_LOW_V1           0x02
//#define SID_ADDR_PW_HI_V1            0x03
//#define SID_ADDR_CONTROLREG_V1       0x04
//#define SID_ADDR_ATTACK_DECAY_V1     0x05
//#define SID_ADDR_SUSTAIN_RELEASE_V1  0x06
//
//#define SID_ADDR_FREQ_LOW_V2         0x07
//#define SID_ADDR_FREQ_HI_V2          0x08
//#define SID_ADDR_PW_LOW_V2           0x09
//#define SID_ADDR_PW_HI_V2            0x0A
//#define SID_ADDR_CONTROLREG_V2       0x0B
//#define SID_ADDR_ATTACK_DECAY_V2     0x0C
//#define SID_ADDR_SUSTAIN_RELEASE_V2  0x0D
//
//#define SID_ADDR_FREQ_LOW_V3         0x0E
//#define SID_ADDR_FREQ_HI_V3          0x0F
//#define SID_ADDR_PW_LOW_V3           0x10
//#define SID_ADDR_PW_HI_V3            0x11
//#define SID_ADDR_CONTROLREG_V3       0x12
//#define SID_ADDR_ATTACK_DECAY_V3     0x13
//#define SID_ADDR_SUSTAIN_RELEASE_V3  0x14
//
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
    void setNote(byte voice, int note);
    void setPWLo(byte voice, byte dutyCycle); 
    void setPWHi(byte voice, byte dutyCycle);    
    void setGate(byte voice, boolean active);
    void setSync(byte voice, boolean active);
    void setRingMod(byte voice, boolean active);
    void setTest(byte voice, boolean active);
    void setTriangle(byte voice, boolean active);
    void setSawtooth(byte voice, boolean active);
    void setPulse(byte voice, boolean active);
    void setNoise(byte voice, boolean active);
    void setEnvelopeAttack(byte voice, byte rate);
    void setEnvelopeDecay(byte voice, byte rate);
    void setEnvelopeSustain(byte voice, byte level);
    void setEnvelopeRelease(byte voice, byte rate); 
    void setInstrument(byte attack, byte decay, byte sustain, byte release, bool noise, bool square, bool sawtooth, bool triangle, int pwm);
    void reset(); 
  private:
    void writeData(unsigned char address, unsigned char data);
    int baseAddress;
    int SID_ADDR_FREQ_LOW;
    int SID_ADDR_FREQ_HI;
    int SID_ADDR_PW_LOW;
    int SID_ADDR_PW_HI;
    int SID_ADDR_CONTROLREG;
    int SID_ADDR_ATTACK_DECAY;
    int SID_ADDR_SUSTAIN_RELEASE;
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
//    SID_REG_CONTROLREG_STRUCT SID_REG_CONTROLREG_V1;
//    SID_REG_CONTROLREG_STRUCT SID_REG_CONTROLREG_V2;
//    SID_REG_CONTROLREG_STRUCT SID_REG_CONTROLREG_V3;

    struct SID_REG_ATTACK_DECAY_STRUCT{
        unsigned int ATTACK : 4; 
        unsigned int DECAY : 4;    
    };
    SID_REG_ATTACK_DECAY_STRUCT SID_REG_ATTACK_DECAY;
//    SID_REG_ATTACK_DECAY_STRUCT SID_REG_ATTACK_DECAY_V1;
//    SID_REG_ATTACK_DECAY_STRUCT SID_REG_ATTACK_DECAY_V2;
//    SID_REG_ATTACK_DECAY_STRUCT SID_REG_ATTACK_DECAY_V3;
    
    struct SID_REG_SUSTAIN_RELEASE_STRUCT{
        unsigned int SUSTAIN : 4; 
        unsigned int RELEASE : 4;    
    };
     SID_REG_SUSTAIN_RELEASE_STRUCT SID_REG_SUSTAIN_RELEASE;    
//    SID_REG_SUSTAIN_RELEASE_STRUCT SID_REG_SUSTAIN_RELEASE_V1;
//    SID_REG_SUSTAIN_RELEASE_STRUCT SID_REG_SUSTAIN_RELEASE_V2;
//    SID_REG_SUSTAIN_RELEASE_STRUCT SID_REG_SUSTAIN_RELEASE_V3; 
    
    
//    static const byte SID_ADDR_FREQ_Array[4]; 
//    static const byte SID_ADDR_PW_Array[4];
//    SID_REG_CONTROLREG_STRUCT *SID_REG_CONTROLREG_Array[4];      
};

class SID
{
  public:
    SIDVoice V1;  
    SID();
    static void writeData(unsigned char address, unsigned char data);   
    void setVolume(byte voice, byte volume);      
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
