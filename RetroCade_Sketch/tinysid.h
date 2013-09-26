/*!
 *  @file		tinysid.cpp
 *  Project		tinysid Library
 *	@brief		tinysid library for the ZPUino
 *	Version		1.0
 *  @author		Jack Gassett 
 *	@date		9/24/13
 *  License		GPL-Unknown?
 
TinySID library is the work of: 
Tammo Hinrichs - 6510 and SID routines
Rainer Sinsch  - PSP TinySID
Alvaro Lopes - Made work with ZPUino Soft Processor
 
See
http://www.informatik.uni-frankfurt.de/~sinsch/TinySid.htm
http://www.syntheticsw.com/~sinsch/?id=7298b&s=k2
http://www.syntheticsw.com/~sinsch
 
 */

#ifndef _TINYSID_H
#define _TINYSID_H

#include "sidtune_Layla_Mix.h"
#include "SID.h"
#include <SmallFS.h>
#include "ramFS.h"

#define AUDIO_J1_L WING_B_1
#define AUDIO_J1_R WING_B_0

#define AUDIO_J2_L WING_B_3
#define AUDIO_J2_R WING_B_2

#define SIDBASE IO_SLOT(14)
#define SIDREG(x) REGISTER(SIDBASE,x)

#define SIDTUNE sidtune_Layla_Mix
#define RETROCADE // Define this if you have a retrocade hw


// -- do not touch this below, it's a C64 emulator.

#define FLAG_N 128
#define FLAG_V 64
#define FLAG_B 16
#define FLAG_D 8
#define FLAG_I 4
#define FLAG_Z 2
#define FLAG_C 1


static unsigned char memory[65536];

typedef enum {OPadc, OPand, OPasl, OPbcc, OPbcs, OPbeq, OPbit, OPbmi, OPbne, OPbpl, OPbrk, OPbvc, OPbvs, OPclc,
  OPcld, OPcli, OPclv, OPcmp, OPcpx, OPcpy, OPdec, OPdex, OPdey, OPeor, OPinc, OPinx, OPiny, OPjmp,
  OPjsr, OPlda, OPldx, OPldy, OPlsr, OPnop, OPora, OPpha, OPphp, OPpla, OPplp, OProl, OPror, OPrti,
  OPrts, OPsbc, OPsec, OPsed, OPsei, OPsta, OPstx, OPsty, OPtax, OPtay, OPtsx, OPtxa, OPtxs, OPtya,
  OPxxx} insts;

#define imp 0
#define imm 1
#define abs 2
#define absx 3
#define absy 4
#define zp 6
#define zpx 7
#define zpy 8
#define ind 9
#define indx 10
#define indy 11
#define acc 12
#define rel 13
#define xxx 14

static uint8_t opcodes[256]= {
  OPbrk,OPora,OPxxx,OPxxx,OPxxx,OPora,OPasl,OPxxx,OPphp,OPora,OPasl,OPxxx,OPxxx,OPora,OPasl,OPxxx,
  OPbpl,OPora,OPxxx,OPxxx,OPxxx,OPora,OPasl,OPxxx,OPclc,OPora,OPxxx,OPxxx,OPxxx,OPora,OPasl,OPxxx,
  OPjsr,OPand,OPxxx,OPxxx,OPbit,OPand,OProl,OPxxx,OPplp,OPand,OProl,OPxxx,OPbit,OPand,OProl,OPxxx,
  OPbmi,OPand,OPxxx,OPxxx,OPxxx,OPand,OProl,OPxxx,OPsec,OPand,OPxxx,OPxxx,OPxxx,OPand,OProl,OPxxx,
  OPrti,OPeor,OPxxx,OPxxx,OPxxx,OPeor,OPlsr,OPxxx,OPpha,OPeor,OPlsr,OPxxx,OPjmp,OPeor,OPlsr,OPxxx,
  OPbvc,OPeor,OPxxx,OPxxx,OPxxx,OPeor,OPlsr,OPxxx,OPcli,OPeor,OPxxx,OPxxx,OPxxx,OPeor,OPlsr,OPxxx,
  OPrts,OPadc,OPxxx,OPxxx,OPxxx,OPadc,OPror,OPxxx,OPpla,OPadc,OPror,OPxxx,OPjmp,OPadc,OPror,OPxxx,
  OPbvs,OPadc,OPxxx,OPxxx,OPxxx,OPadc,OPror,OPxxx,OPsei,OPadc,OPxxx,OPxxx,OPxxx,OPadc,OPror,OPxxx,
  OPxxx,OPsta,OPxxx,OPxxx,OPsty,OPsta,OPstx,OPxxx,OPdey,OPxxx,OPtxa,OPxxx,OPsty,OPsta,OPstx,OPxxx,
  OPbcc,OPsta,OPxxx,OPxxx,OPsty,OPsta,OPstx,OPxxx,OPtya,OPsta,OPtxs,OPxxx,OPxxx,OPsta,OPxxx,OPxxx,
  OPldy,OPlda,OPldx,OPxxx,OPldy,OPlda,OPldx,OPxxx,OPtay,OPlda,OPtax,OPxxx,OPldy,OPlda,OPldx,OPxxx,
  OPbcs,OPlda,OPxxx,OPxxx,OPldy,OPlda,OPldx,OPxxx,OPclv,OPlda,OPtsx,OPxxx,OPldy,OPlda,OPldx,OPxxx,
  OPcpy,OPcmp,OPxxx,OPxxx,OPcpy,OPcmp,OPdec,OPxxx,OPiny,OPcmp,OPdex,OPxxx,OPcpy,OPcmp,OPdec,OPxxx,
  OPbne,OPcmp,OPxxx,OPxxx,OPxxx,OPcmp,OPdec,OPxxx,OPcld,OPcmp,OPxxx,OPxxx,OPxxx,OPcmp,OPdec,OPxxx,
  OPcpx,OPsbc,OPxxx,OPxxx,OPcpx,OPsbc,OPinc,OPxxx,OPinx,OPsbc,OPnop,OPxxx,OPcpx,OPsbc,OPinc,OPxxx,
  OPbeq,OPsbc,OPxxx,OPxxx,OPxxx,OPsbc,OPinc,OPxxx,OPsed,OPsbc,OPxxx,OPxxx,OPxxx,OPsbc,OPinc,OPxxx
};

static uint8_t modes[256]= {
 imp,indx,xxx,xxx,zp,zp,zp,xxx,imp,imm,acc,xxx,abs,abs,abs,xxx,
 rel,indy,xxx,xxx,xxx,zpx,zpx,xxx,imp,absy,xxx,xxx,xxx,absx,absx,xxx,
 abs,indx,xxx,xxx,zp,zp,zp,xxx,imp,imm,acc,xxx,abs,abs,abs,xxx,
 rel,indy,xxx,xxx,xxx,zpx,zpx,xxx,imp,absy,xxx,xxx,xxx,absx,absx,xxx,
 imp,indx,xxx,xxx,zp,zp,zp,xxx,imp,imm,acc,xxx,abs,abs,abs,xxx,
 rel,indy,xxx,xxx,xxx,zpx,zpx,xxx,imp,absy,xxx,xxx,xxx,absx,absx,xxx,
 imp,indx,xxx,xxx,zp,zp,zp,xxx,imp,imm,acc,xxx,ind,abs,abs,xxx,
 rel,indy,xxx,xxx,xxx,zpx,zpx,xxx,imp,absy,xxx,xxx,xxx,absx,absx,xxx,
 imm,indx,xxx,xxx,zp,zp,zp,xxx,imp,imm,acc,xxx,abs,abs,abs,xxx,
 rel,indy,xxx,xxx,zpx,zpx,zpy,xxx,imp,absy,acc,xxx,xxx,absx,absx,xxx,
 imm,indx,imm,xxx,zp,zp,zp,xxx,imp,imm,acc,xxx,abs,abs,abs,xxx,
 rel,indy,xxx,xxx,zpx,zpx,zpy,xxx,imp,absy,acc,xxx,absx,absx,absy,xxx,
 imm,indx,xxx,xxx,zp,zp,zp,xxx,imp,imm,acc,xxx,abs,abs,abs,xxx,
 rel,indy,xxx,xxx,zpx,zpx,zpx,xxx,imp,absy,acc,xxx,xxx,absx,absx,xxx,
 imm,indx,xxx,xxx,zp,zp,zp,xxx,imp,imm,acc,xxx,abs,abs,abs,xxx,
 rel,indy,xxx,xxx,zpx,zpx,zpx,xxx,imp,absy,acc,xxx,xxx,absx,absx,xxx
};

// ----------------------------------------------- globale Faulheitsvariablen

static int cycles;
static uint8_t bval;
static uint16_t wval;

// ----------------------------------------------------------------- Register

static uint8_t a,x,y,s,p;
static uint16_t pc;

// ----------------------------------------------------------- DER HARTE KERN

static uint8_t sidregs[32];

//void tinysetup();
void tinyLoadFile(const char* name);
void tinyloop();
void tinysid_zpu_interrupt();

#endif
