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
#include <zpuino.h>
#include <zpuino-types.h>

#define SID_ADDR_V1_FREQ_LOW         0x00
#define SID_ADDR_V1_FREQ_HI          0x01
#define SID_ADDR_V1_PW_LOW           0x02
#define SID_ADDR_V1_PW_HI            0x03
#define SID_ADDR_V1_CONTROLREG       0x04
#define SID_ADDR_V1_ATTACK_DECAY     0x05
#define SID_ADDR_V1_SUSTAIN_RELEASE  0x06

#define SID_ADDR_V2_FREQ_LOW         0x07
#define SID_ADDR_V2_FREQ_HI          0x08
#define SID_ADDR_V2_PW_LOW           0x09
#define SID_ADDR_V2_PW_HI            0x0A
#define SID_ADDR_V2_CONTROLREG       0x0B
#define SID_ADDR_V2_ATTACK_DECAY     0x0C
#define SID_ADDR_V2_SUSTAIN_RELEASE  0x0D

#define SID_ADDR_V3_FREQ_LOW         0x0E
#define SID_ADDR_V3_FREQ_HI          0x0F
#define SID_ADDR_V3_PW_LOW           0x10
#define SID_ADDR_V3_PW_HI            0x11
#define SID_ADDR_V3_CONTROLREG       0x12
#define SID_ADDR_V3_ATTACK_DECAY     0x13
#define SID_ADDR_V3_SUSTAIN_RELEASE  0x14

#define SID_ADDR_FILTER_FC_LOW       0x15
#define SID_ADDR_FILTER_FC_HI        0x16
#define SID_ADDR_FILTER_RES_FILT     0x17
#define SID_ADDR_FILTER_MODE_VOL     0x18

#define SID_ADDR_MISC_ENV3           0x1C

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
SID_REG_CONTROLREG_STRUCT SID_REG_V1_CONTROLREG;
SID_REG_CONTROLREG_STRUCT SID_REG_V2_CONTROLREG;
SID_REG_CONTROLREG_STRUCT SID_REG_V3_CONTROLREG;

struct SID_REG_ATTACK_DECAY_STRUCT{
    unsigned int ATTACK : 4; 
    unsigned int DECAY : 4;    
};
SID_REG_ATTACK_DECAY_STRUCT SID_REG_V1_ATTACK_DECAY;
SID_REG_ATTACK_DECAY_STRUCT SID_REG_V2_ATTACK_DECAY;
SID_REG_ATTACK_DECAY_STRUCT SID_REG_V3_ATTACK_DECAY;

struct SID_REG_SUSTAIN_RELEASE_STRUCT{
    unsigned int SUSTAIN : 4; 
    unsigned int RELEASE : 4;    
};
SID_REG_SUSTAIN_RELEASE_STRUCT SID_REG_V1_SUSTAIN_RELEASE;
SID_REG_SUSTAIN_RELEASE_STRUCT SID_REG_V2_SUSTAIN_RELEASE;
SID_REG_SUSTAIN_RELEASE_STRUCT SID_REG_V3_SUSTAIN_RELEASE;

//struct SID_REG_FREQ_STRUCT{
//    unsigned int FHI : 8; 
//    unsigned int FLOW : 8;    
//};
//SID_REG_FREQ_STRUCT SID_REG_V1_FREQ;
//SID_REG_FREQ_STRUCT SID_REG_V2_FREQ;
//SID_REG_FREQ_STRUCT SID_REG_V3_FREQ;

#define SIDBASE IO_SLOT(14)
#define SIDREG(x) REGISTER(SIDBASE,x)

#endif // LIB_SID_H_
