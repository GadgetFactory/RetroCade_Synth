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

#include "tinysid.h"
#include "SID.h"
#include <SmallFS.h>
#include "ramFS.h"

SID sid;

unsigned short play_addr;
volatile int tick = 0;

static uint8_t getmem(uint16_t addr)
{
	if (addr == 0xdd0d) memory[addr]=0;
	//printf("READ %04x = %04x\n", addr, memory[addr]);
	return memory[addr];
}

static void writeSIDreg(int reg, int value)
{
	//sidregs[reg] = value;
        sid.writeData(reg, value);
}

static void setmem(uint16_t addr, uint8_t value)
{
//	printf("WRITE %04x <- %02x\n", addr, value);

	if ((addr&0xfc00)==0xd400) {
		//printf("SID WRITE %04x <- %02x cycle %d\n",addr,value,cycles);
		writeSIDreg(addr & 31, value);
	}

	if ((addr==0xdc04) || (addr==0xdc05)) {
		// printf("CIA WRITE %04x <- %02x cycle %d\n",addr,value,cycles);
	}
	memory[addr]=value;
}

static uint8_t getaddr(int mode)
{
  uint16_t ad,ad2;  
  switch(mode)
  {
    case imp:
      cycles+=2;
      return 0;
    case imm:
      cycles+=2;
      return getmem(pc++);
    case abs:
      cycles+=4;
      ad=getmem(pc++);
      ad|=getmem(pc++)<<8;
      return getmem(ad);
    case absx:
      cycles+=4;
      ad=getmem(pc++);
      ad|=256*getmem(pc++);
      ad2=ad+x;
      if ((ad2&0xff00)!=(ad&0xff00))
        cycles++;
      return getmem(ad2);
    case absy:
      cycles+=4;
      ad=getmem(pc++);
      ad|=256*getmem(pc++);
      ad2=ad+y;
      if ((ad2&0xff00)!=(ad&0xff00))
        cycles++;
      return getmem(ad2);
    case zp:
      cycles+=3;
      ad=getmem(pc++);
      return getmem(ad);
    case zpx:
      cycles+=4;
      ad=getmem(pc++);
      ad+=x;
      return getmem(ad&0xff);
    case zpy:
      cycles+=4;
      ad=getmem(pc++);
      ad+=y;
      return getmem(ad&0xff);
    case indx:
      cycles+=6;
      ad=getmem(pc++);
      ad+=x;
      ad2=getmem(ad&0xff);
      ad++;
      ad2|=getmem(ad&0xff)<<8;
      return getmem(ad2);
    case indy:
      cycles+=5;
      ad=getmem(pc++);
      ad2=getmem(ad);
      ad2|=getmem((ad+1)&0xff)<<8;
      ad=ad2+y;
      if ((ad2&0xff00)!=(ad&0xff00))
        cycles++;
      return getmem(ad);
    case acc:
      cycles+=2;
      return a;
  }  
  return 0;
}


static void setaddr(int mode, uint8_t val)
{
  uint16_t ad,ad2;
  switch(mode)
  {
    case abs:
      cycles+=2;
      ad=getmem(pc-2);
      ad|=256*getmem(pc-1);
      setmem(ad,val);
      return;
    case absx:
      cycles+=3;
      ad=getmem(pc-2);
      ad|=256*getmem(pc-1);
      ad2=ad+x;
      if ((ad2&0xff00)!=(ad&0xff00))
        cycles--;
      setmem(ad2,val);
      return;
    case zp:
      cycles+=2;
      ad=getmem(pc-1);
      setmem(ad,val);
      return;
    case zpx:
      cycles+=2;
      ad=getmem(pc-1);
      ad+=x;
      setmem(ad&0xff,val);
      return;
    case acc:
      a=val;
      return;
  }
}


static void putaddr(int mode, uint8_t val)
{
  uint16_t ad,ad2;
  switch(mode)
  {
    case abs:
      cycles+=4;
      ad=getmem(pc++);
      ad|=getmem(pc++)<<8;
      setmem(ad,val);
      return;
    case absx:
      cycles+=4;
      ad=getmem(pc++);
      ad|=getmem(pc++)<<8;
      ad2=ad+x;
      setmem(ad2,val);
      return;
    case absy:
      cycles+=4;
      ad=getmem(pc++);
      ad|=getmem(pc++)<<8;
      ad2=ad+y;
      if ((ad2&0xff00)!=(ad&0xff00))
        cycles++;
      setmem(ad2,val);
      return;
    case zp:
      cycles+=3;
      ad=getmem(pc++);
      setmem(ad,val);
      return;
    case zpx:
      cycles+=4;
      ad=getmem(pc++);
      ad+=x;
      setmem(ad&0xff,val);
      return;
    case zpy:
      cycles+=4;
      ad=getmem(pc++);
      ad+=y;
      setmem(ad&0xff,val);
      return;
    case indx:
      cycles+=6;
      ad=getmem(pc++);
      ad+=x;
      ad2=getmem(ad&0xff);
      ad++;
      ad2|=getmem(ad&0xff)<<8;
      setmem(ad2,val);
      return;
    case indy:
      cycles+=5;
      ad=getmem(pc++);
      ad2=getmem(ad);
      ad2|=getmem((ad+1)&0xff)<<8;
      ad=ad2+y;
      setmem(ad,val);
      return;
    case acc:
      cycles+=2;
      a=val;
      return;
  }
}


static inline void setflags(int flag, int cond)
{
  // cond?p|=flag:p&=~flag;
  if (cond) p|=flag;
  else p&=~flag;
}


static inline void push(uint8_t val)
{
  setmem(0x100+s,val);
  if (s) s--;
}

static inline uint8_t pop()
{
  if (s<0xff) s++;
  return getmem(0x100+s);
}

static void branch(int flag)
{
  signed char dist;
  dist=(signed char)getaddr(imm);
  wval=pc+dist;
  if (flag)
  {
    cycles+=((pc&0x100)!=(wval&0x100))?2:1;
    pc=wval;
  }
}

// ----------------------------------------------------- ffentliche Routinen

static void cpuReset()
{
  a=x=y=0;
  p=0;
  s=255;
  pc=getaddr(0xfffc);
}

static void cpuResetTo(uint16_t npc)
{
  a=0;
  x=0;
  y=0;
  p=0;
  s=255;
  pc=npc;
}

#define OP(x...)
//do { fprintf(stderr,x); fprintf(stderr,"\n"); } while(0)

static int cpuParse()
{
	uint8_t opc;
	int cmd, addr, c;
	cycles=0;

	//#ifdef TRACE
	//  cpuStatus();
	//  if (opcodes[getmem(pc)]==xxx) getch();
	//#endif

	opc=getmem(pc++);
	cmd=opcodes[opc];
	addr=modes[opc];
//	fprintf(stderr,"$%04x [%02x]: ", pc-1, opc);
	switch (cmd)
	{
	case OPadc:
		OP("adc");
		wval=(uint16_t)a+getaddr(addr)+((p&FLAG_C)?1:0);
		setflags(FLAG_C, wval&0x100);
		a=(uint8_t)wval;
		setflags(FLAG_Z, !a);
		setflags(FLAG_N, a&0x80);
		setflags(FLAG_V, (!!(p&FLAG_C)) ^ (!!(p&FLAG_N)));
		break;
	case OPand:
		OP("and");
		bval=getaddr(addr);
		a&=bval;
		setflags(FLAG_Z, !a);
		setflags(FLAG_N, a&0x80);
		break;
	case OPasl:
		OP("asl");
		wval=getaddr(addr);
		wval<<=1;
		setaddr(addr,(uint8_t)wval);
		setflags(FLAG_Z,!wval);
		setflags(FLAG_N,wval&0x80);
		setflags(FLAG_C,wval&0x100);
		break;
	case OPbcc:
		OP("bcc");
		branch(!(p&FLAG_C));
		break;
	case OPbcs:
		OP("bcs");
		branch(p&FLAG_C);
		break;
	case OPbne:
		OP("bne");
		branch(!(p&FLAG_Z));
		break;
	case OPbeq:
		OP("beq");
		branch(p&FLAG_Z);
		break;
	case OPbpl:
		OP("bpl");
		branch(!(p&FLAG_N));
		break;
	case OPbmi:
		OP("bmi");
		branch(p&FLAG_N);
		break;
	case OPbvc:
		OP("bvc");
		branch(!(p&FLAG_V));
		break;
	case OPbvs:
		OP("bvs");
		branch(p&FLAG_V);
		break;
	case OPbit:
		OP("bit");
		bval=getaddr(addr);
		setflags(FLAG_Z,!(a&bval));
		setflags(FLAG_N,bval&0x80);
		setflags(FLAG_V,bval&0x40);
		break;
	case OPbrk:
		OP("brk");
		push(pc&0xff);
		push(pc>>8);
		push(p);
		setflags(FLAG_B,1);
		pc=getmem(0xfffe);
		cycles+=7;
		break;
	case OPclc:
		OP("clc");
		cycles+=2;
		setflags(FLAG_C,0);
		break;
	case OPcld:
		OP("cld");
		cycles+=2;
		setflags(FLAG_D,0);
		break;
	case OPcli:
		OP("cli");
		cycles+=2;
		setflags(FLAG_I,0);
		break;
	case OPclv:
		OP("clv");
		cycles+=2;
		setflags(FLAG_V,0);
		break;
	case OPcmp:
		OP("cmp");
		bval=getaddr(addr);
		wval=(uint16_t)a-bval;
		setflags(FLAG_Z,!wval);
		setflags(FLAG_N,wval&0x80);
		setflags(FLAG_C,a>=bval);
		break;
	case OPcpx:
		OP("cpx");
		bval=getaddr(addr);
		wval=(uint16_t)x-bval;
		setflags(FLAG_Z,!wval);
		setflags(FLAG_N,wval&0x80);
		setflags(FLAG_C,a>=bval);
		break;
	case OPcpy:
		OP("cpy");
		bval=getaddr(addr);
		wval=(uint16_t)y-bval;
		setflags(FLAG_Z,!wval);
		setflags(FLAG_N,wval&0x80);
		setflags(FLAG_C,a>=bval);
		break;
	case OPdec:
		OP("dec");
		bval=getaddr(addr);
		bval--;
		setaddr(addr,bval);
		setflags(FLAG_Z,!bval);
		setflags(FLAG_N,bval&0x80);
		break;
	case OPdex:
		OP("dex");
		cycles+=2;
		x--;
		setflags(FLAG_Z,!x);
		setflags(FLAG_N,x&0x80);
		break;
	case OPdey:
		OP("dey");
		cycles+=2;
		y--;
		setflags(FLAG_Z,!y);
		setflags(FLAG_N,y&0x80);
		break;
	case OPeor:
		OP("eor");
		bval=getaddr(addr);
		a^=bval;
		setflags(FLAG_Z,!a);
		setflags(FLAG_N,a&0x80);
		break;
	case OPinc:
		OP("inc");
		bval=getaddr(addr);
		bval++;
		setaddr(addr,bval);
		setflags(FLAG_Z,!bval);
		setflags(FLAG_N,bval&0x80);
		break;
	case OPinx:
		OP("inx");
		cycles+=2;
		x++;
		setflags(FLAG_Z,!x);
		setflags(FLAG_N,x&0x80);
		break;
	case OPiny:
		OP("iny");
		cycles+=2;
		y++;
		setflags(FLAG_Z,!y);
		setflags(FLAG_N,y&0x80);
		break;
	case OPjmp:
		cycles+=3;
		wval=getmem(pc++);
		wval|=256*getmem(pc++);
		switch (addr)
		{
		case abs:
			OP("jmp $%04x",wval);
			pc=wval;
			break;
		case ind:
			pc=getmem(wval);
			pc|=256*getmem(wval+1);
			OP("jmpr $%04x",pc);
			cycles+=2;
			break;
		}
		break;
	case OPjsr:
		OP("jsr");
		cycles+=6;
		push((pc+2));
		push((pc+2)>>8);
		wval=getmem(pc++);
		wval|=256*getmem(pc++);
		pc=wval;
		break;
	case OPlda:
		OP("lda");
		a=getaddr(addr);
		setflags(FLAG_Z,!a);
		setflags(FLAG_N,a&0x80);
		break;
	case OPldx:
		OP("ldx");
		x=getaddr(addr);
		setflags(FLAG_Z,!x);
		setflags(FLAG_N,x&0x80);
		break;
	case OPldy:
		OP("ldy");
		y=getaddr(addr);
		setflags(FLAG_Z,!y);
		setflags(FLAG_N,y&0x80);
		break;
	case OPlsr:
		OP("lsr");
		//bval=wval=getaddr(addr);
		bval=getaddr(addr); wval=(uint8_t)bval;
		wval>>=1;
		setaddr(addr,(uint8_t)wval);
		setflags(FLAG_Z,!wval);
		setflags(FLAG_N,wval&0x80);
		setflags(FLAG_C,bval&1);
		break;
	case OPnop:
		OP("nop");
		cycles+=2;
		break;
	case OPora:
		OP("ora");
		bval=getaddr(addr);
		a|=bval;
		setflags(FLAG_Z,!a);
		setflags(FLAG_N,a&0x80);
		break;
	case OPpha:
		OP("pha");
		push(a);
		cycles+=3;
		break;
	case OPphp:
		OP("php");
		push(p);
		cycles+=3;
		break;
	case OPpla:
		OP("pla");
		a=pop();
		setflags(FLAG_Z,!a);
		setflags(FLAG_N,a&0x80);
		cycles+=4;
		break;
	case OPplp:
		OP("plp");
		p=pop();
		cycles+=4;
		break;
	case OProl:
		OP("rol");
		bval=getaddr(addr);
		c=!!(p&FLAG_C);
		setflags(FLAG_C,bval&0x80);
		bval<<=1;
		bval|=c;
		setaddr(addr,bval);
		setflags(FLAG_N,bval&0x80);
		setflags(FLAG_Z,!bval);
		break;
	case OPror:
		OP("ror");
		bval=getaddr(addr);
		c=!!(p&FLAG_C);
		setflags(FLAG_C,bval&1);
		bval>>=1;
		bval|=128*c;
		setaddr(addr,bval);
		setflags(FLAG_N,bval&0x80);
		setflags(FLAG_Z,!bval);
		break;
	case OPrti:
		OP("rti");
		// NEU, rti wie rts, au�r das alle register wieder vom stack kommen
		//p=pop();
		p=pop();
		y=pop();
		x=pop();
		a=pop();
		// in_nmi = false;
		//write_console("NMI EXIT!");
	case OPrts:
		OP("rts");
		wval=256*pop();
		wval|=pop();
		pc=wval;
		cycles+=6;
		break;
	case OPsbc:
		OP("sbc");
		bval=getaddr(addr)^0xff;
		wval=(uint16_t)a+bval+((p&FLAG_C)?1:0);
		setflags(FLAG_C, wval&0x100);
		a=(uint8_t)wval;
		setflags(FLAG_Z, !a);
		setflags(FLAG_N, a>127);
		setflags(FLAG_V, (!!(p&FLAG_C)) ^ (!!(p&FLAG_N)));
		break;
	case OPsec:
		OP("sec");
		cycles+=2;
		setflags(FLAG_C,1);
		break;
	case OPsed:
		OP("sec");
		cycles+=2;
		setflags(FLAG_D,1);
		break;
	case OPsei:
		OP("sei");
		cycles+=2;
		setflags(FLAG_I,1);
		break;
	case OPsta:
		OP("sta");
		putaddr(addr,a);
		break;
	case OPstx:
		OP("stx");
		putaddr(addr,x);
		break;
	case OPsty:
		OP("sty");
		putaddr(addr,y);
		break;
	case OPtax:
		OP("tax");
		cycles+=2;
		x=a;
		setflags(FLAG_Z, !x);
		setflags(FLAG_N, x&0x80);
		break;
	case OPtay:
		OP("tay");
		cycles+=2;
		y=a;
		setflags(FLAG_Z, !y);
		setflags(FLAG_N, y&0x80);
		break;
	case OPtsx:
		OP("tsx");
		cycles+=2;
		x=s;
		setflags(FLAG_Z, !x);
		setflags(FLAG_N, x&0x80);
		break;
	case OPtxa:
		OP("txa");
		cycles+=2;
		a=x;
		setflags(FLAG_Z, !a);
		setflags(FLAG_N, a&0x80);
		break;
	case OPtxs:
		OP("txs");
		cycles+=2;
		s=x;
		break;
	case OPtya:
		OP("tya");
		cycles+=2;
		a=y;
		setflags(FLAG_Z, !a);
		setflags(FLAG_N, a&0x80);
		break;
	}

	return cycles;
}

int cpuJSR(uint16_t npc, uint8_t na)
{
  int ccl;
  
  a=na;
  x=0;
  y=0;
  p=0;
  s=255;
  pc=npc;
  push(0);
  push(0);
  ccl=0;    
  while (pc)
  {
    ccl+=cpuParse();
  }  
  return ccl;
}

static unsigned short LoadSIDFromMemory(const void *pSidData, unsigned short *load_addr,
								 unsigned short *init_addr, unsigned short *play_addr,
								 unsigned char *subsongs, unsigned char *startsong,
								 unsigned char *speed, unsigned short size)
{
	unsigned char *pData;
    unsigned char data_file_offset;

    pData = (unsigned char*)pSidData;
    data_file_offset = pData[7];

    *load_addr = pData[8]<<8;
    *load_addr|= pData[9];

    *init_addr = pData[10]<<8;
    *init_addr|= pData[11];

    *play_addr = pData[12]<<8;
    *play_addr|= pData[13];

    *subsongs = pData[0xf]-1;
    *startsong = pData[0x11]-1;

    *load_addr = pData[data_file_offset];
    *load_addr|= pData[data_file_offset+1]<<8;
        
    *speed = pData[0x15];
    
    memset(memory, 0, sizeof(memory));
    memcpy(&memory[*load_addr], &pData[data_file_offset+2], size-(data_file_offset+2));
    
    if (*play_addr == 0)
    {
        cpuJSR(*init_addr, 0);
        *play_addr = (memory[0x0315]<<8)+memory[0x0314];
    }

    return *load_addr;
}

static void dumpregs()
{
	int i;
	Serial.print("R: ");
	for (i=0;i<25;i++) {
		Serial.print(sidregs[i], HEX);
		Serial.print(" ");
	}
    Serial.println("");
}

void tinysid_zpu_interrupt() {
	tick=1;
	TMR0CTL &= ~(BIT(TCTLIF));
}
static unsigned short init_addr;
//void tinysetup()
void tinyLoadFile(const char* name)
{
	unsigned short load_addr;
	unsigned char subsongs,startsong,speed;
	SmallFSFile sidSmallFSfile;
	File sidSDfile;

  //fileLoaded = false;
  if (SmallFS.begin()==0)
	sidSmallFSfile = SmallFS.open(name);
  sidSDfile = SD.open(name);	//TODO begin SD?
  boolean smallFsCheck = sidSmallFSfile.valid();
  boolean sdFsCheck = sidSDfile;
  if (sdFsCheck){
    Serial.println("Opening SID File from SD Card.");
    //modRAMfile = RamFS.open(&sidSDfile);
	unsigned size = sidSDfile.size();
	 unsigned char *buf = (unsigned char*)malloc(size);
	/* Allocate */
	sidSDfile.read(buf,size);	
	LoadSIDFromMemory(buf,
					  &load_addr,
					  &init_addr,
					  &play_addr,
					  &subsongs,
					  &startsong,
					  &speed,
					  size);	
    sidSDfile.close();
    //fileLoaded = true;
  }
  else if (smallFsCheck){
    Serial.println("Opening SID File from SmallFS.");
    //modRAMfile = RamFS.open(&sidSmallFSfile);
    //fileLoaded = true;
	 unsigned size = sidSmallFSfile.size();
	 unsigned char *buf = (unsigned char*)malloc(size);
	/* Allocate */
	sidSmallFSfile.read(buf,size);	
	LoadSIDFromMemory(buf,
					  &load_addr,
					  &init_addr,
					  &play_addr,
					  &subsongs,
					  &startsong,
					  &speed,
					  size);	
  }
  else {
    Serial.println("No sid files to play in SmallFS or on SD card.");
    //fileLoaded = false; 
  }

	//SmallFSFile file;

	//Serial.begin(115200);
	//Serial.println("Starting");
	// if (SmallFS.begin()==0) {
		// file = SmallFS.open(name);
		 //if (file.valid()) {
/* 			 unsigned size = file->size();
			 Serial.print("Size: ");
			 Serial.println(size);
             unsigned char *buf = (unsigned char*)malloc(size);
			/* Allocate */
			//file->read(buf,size); */

			// LoadSIDFromMemory(buf,
							  // &load_addr,
							  // &init_addr,
							  // &play_addr,
							  // &subsongs,
							  // &startsong,
							  // &speed,
							  // size);

		//}
	// }
	// else {
		// LoadSIDFromMemory(  SIDTUNE,
						  // &load_addr,
						  // &init_addr,
						  // &play_addr,
						  // &subsongs,
						  // &startsong,
						  // &speed,
						  // sizeof(SIDTUNE));
	// }

	cpuReset();
	memset(sidregs,0,sizeof(sidregs));

/* #ifdef RETROCADE
	//Move the audio output to the appropriate pins on the Papilio Hardware
	pinMode(AUDIO_J1_L,OUTPUT);
	digitalWrite(AUDIO_J1_L,HIGH);
	//outputPinForFunction(AUDIO_J1_L, IOPIN_SIGMADELTA0);
	outputPinForFunction(AUDIO_J1_L, 8);
	pinModePPS(AUDIO_J1_L, HIGH);

	pinMode(AUDIO_J1_R,OUTPUT);
	digitalWrite(AUDIO_J1_R,HIGH);
	outputPinForFunction(AUDIO_J1_R, 8);
	//outputPinForFunction(AUDIO_J1_R, IOPIN_SIGMADELTA1);
	pinModePPS(AUDIO_J1_R, HIGH);

	pinMode(AUDIO_J2_L,OUTPUT);
	digitalWrite(AUDIO_J2_L,HIGH);
	outputPinForFunction(AUDIO_J2_L, 8);
	pinModePPS(AUDIO_J2_L, HIGH);

	pinMode(AUDIO_J2_R,OUTPUT);
	digitalWrite(AUDIO_J2_R,HIGH);
	outputPinForFunction(AUDIO_J2_R, 8);
	pinModePPS(AUDIO_J2_R, HIGH);
#else

	pinMode(WING_C_0,OUTPUT);
	digitalWrite(WING_C_0,HIGH);
	outputPinForFunction(WING_C_0, 8);
	pinModePPS(WING_C_0, HIGH);

	pinMode(WING_C_1,OUTPUT);
	digitalWrite(WING_C_1,HIGH);
	outputPinForFunction(WING_C_1, 8);
	pinModePPS(WING_C_1, HIGH);

#endif */

	cpuJSR(init_addr, 0);
	// Prescale 64 (101b), generate 50Hz tick

/* 	TMR0CMP = (CLK_FREQ/(50*64))-1;
	TMR0CNT = 0x0;
	TMR0CTL = BIT(TCTLENA)|BIT(TCTLCCM)|BIT(TCTLDIR)|BIT(TCTLCP2)|BIT(TCTLCP0)|BIT(TCTLIEN);
	INTRMASK = BIT(INTRLINE_TIMER0); // Enable Timer0 interrupt
	INTRCTL=1; */
}

void tinyloop()
{
	//while (1) {
		if (tick == 1) {
			int clocks = cpuJSR(play_addr,0);
			tick=0;
		}
		 //while (!tick);
		/* Write SID regs here */
		//dumpregs();
                         
/* 		if (Serial.available()) {
			int r=Serial.read();
			if (r=='e') {
				Serial.println("Enable");
				sid.writeData(25,1);
			}
			if (r=='d') {
				Serial.println("Disable");
				sid.writeData(25,0);
			}
			if (r=='r') {
				// Restart
				Serial.println("Restart");
				cpuReset();
				memset(sidregs,0,sizeof(sidregs));
				cpuJSR(init_addr, 0);
			}
		} */
	//}
}
