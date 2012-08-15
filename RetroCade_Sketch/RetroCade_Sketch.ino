/*
  Gadget Factory
  RetroCade Synthesizer
 
 This example will let you play the YM2149 and SID audio chip integrated into the ZPUino Soft Processor using a MIDI instrument such as a keyboard or guitar. 
 
 To learn more about this example visit its home page at the Papilio Audio Wiki:
 http://www.kickstarter.com/projects/13588168/retrocade-synth-one-chiptune-board-to-rule-them-al
 
 To learn more about the Papilio and Gadget Factory products visit:
 http://www.GadgetFactory.net
 
 Hardware:
 * Connect a MIDI-Audio Wing to CH
 * Connect an Audio Wing to AL
 * User an Arcade MegaWing
 
 *******IMPORTANT********************
 Be sure to load the ZPUino "RetroCade" variant to the Papilio's SPI Flash before loading this sketch.

 created 2012
 by Jack Gassett
 http://www.gadgetfactory.net
 
 This example code is Creative Commons Attribution.
 */

#include "MIDI.h"    //Be sure to change MIDI.h to user Serial1 instead of Serial
#include "SID.h"
#include "YM2149.h"
#include "ptplay.h"
#include "SmallFS.h"
#include "cbuffer.h"
#include <LiquidCrystal.h>

#undef DO_CHECKS
#undef DEBUG


YM_REG_MIXER_STRUCT YM_REG_MIXER;
YM_REG_ENVSHAPE_STRUCT YM_REG_ENVSHAPE;
YM_REG_LEVEL_STRUCT YM_REG_VA_LEVEL;
YM_REG_LEVEL_STRUCT YM_REG_VB_LEVEL;
YM_REG_LEVEL_STRUCT YM_REG_VC_LEVEL;


//#define AUDIOPIN WING_A_15  //Connected to the Arcade MegaWing
//#define AUDIOPIN WING_A_7    //Audio Wing connected to AL. This is for Jack 1
//#define AUDIOPIN WING_C_8    //Midi Audio Wing connected to CH. This is for Jack 1
//#define AUDIOPINEXTRA WING_C_9

//#define AUDIO_RIGHT WING_C_8
//#define AUDIO_LEFT WING_C_9

////Connected to Audio Wing on AH
//#define AUDIOPIN WING_A_15
////#define AUDIO_RIGHT WING_A_15
//#define AUDIO_LEFT WING_A_13

//Connected to Audio Wing on AH
//#define AUDIOPIN WING_C_7
////#define AUDIO_RIGHT WING_A_15
//#define AUDIO_LEFT WING_C_5
//
//#define SERIAL1RXPIN WING_A_7  //MIDI Wing connected to CH


//Definition for the RetroCade MegaWing
#define AUDIOPIN WING_B_0
//#define AUDIO_RIGHT WING_A_15
#define AUDIO_LEFT WING_B_1

#define SERIAL1RXPIN WING_C_1  //MIDI Wing connected to CH


int sid_midi[] = {//MIDI note number
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

CircularBuffer<ymframe,2> YMaudioBuffer;

SmallFSFile ymaudiofile;

LiquidCrystal lcd(WING_B_10, WING_B_9, WING_B_8, WING_B_7, WING_B_6, WING_B_5, WING_B_4);

SmallFSFile modfile;
char buf[128];
pt_mod_s * mod;
CircularBuffer<unsigned,7> audioBuffer;
unsigned underruns=0;

byte nrpn;
byte transpose_v1;
byte transpose_v2;
byte transpose_v3;
int pitch, playYM, playMOD, playTrack, counter, ymTimeStamp, resetYMFlag;
int freqV1, freqV2, freqV3, freqV4, freqV5, freqV6;

//static unsigned int timerTicks = 0;

void _zpu_interrupt()
{
  counter++;
  if ( counter == 340 ) {
        counter = 1;
        ymTimeStamp++;
	// Play YM file
	if (YMaudioBuffer.hasData()) {
		int i;
		ymframe f = YMaudioBuffer.pop();
		for (i=0;i<14; i++) {
			YM2149REG(i) = f.regval[i];
		}
	}
        else{ 
          if (resetYMFlag == 1){
            //reset_ym2149();
            resetYMFlag = 0;
            ymTimeStamp = 1;
          }
        }
  }
	// Play mod file
	if (audioBuffer.hasData()) {
		unsigned v = audioBuffer.pop();
		SIGMADELTADATA = v;
	} else {
          //SIGMADELTADATA=0x80008000;
          underruns++;
	}
	TMR0CTL &= ~(BIT(TCTLIF));
}

void audiofill()
{
	// Check audio
	int r;

	ymframe f;
	while (!YMaudioBuffer.isFull()) {
		r = ymaudiofile.read(&f.regval[0], 16);
		if (r==0) {
			ymaudiofile.seek(0,SEEK_SET);
			ymaudiofile.read(&f.regval[0], 16);
		}
		YMaudioBuffer.push(f);
	}
}

extern unsigned char __end__;

pt_mod_s *pt_init_smallfs(SmallFSFile &file)
{
	unsigned char buf[256]; // Some buffer. Let's hope this fits on stack.
	unsigned char *bp=&buf[0];

	int i, j, k,l;
	//double tempf;
	pt_mod_s *mod;
	pt_sample_s *s;
	pt_pattern_s *pat;
	int i2, i3;
	pt_patterndata_s *p;

	/* M.K. signature */

	// smallfs bp = buf + 0x438;
    file.seek(0x438, SEEK_SET);


//#ifdef DO_CHECKS
	file.read(&bp[0], 4);
	if (!(bp[0] == 'M' && bp[1] == '.' && bp[2] == 'K' && bp[3] == '.')) {
		Serial.println("Invalid file");
		return NULL;
	}
//#endif

	/* determine number of patterns */

	k = 0;
	// smallfs bp = buf + 952;
	file.seek(952, SEEK_SET);
	file.read(&buf,128);

	for (i = 0; i < 128; ++i)
	{
		j = *bp++;
		if (j > k) k = j;
	}
	k++;

#ifdef DO_CHECKS
	if (bufsize < 1084 + (k << 8))
	{
		return NULL;		/* incomplete */
	}
#endif

	Serial.print("Number of patterns: ");
	Serial.println(k);



	//if (mod == NULL)
	//{
	/* return the required size in bytes */
	//	return sizeof(*mod) + k * sizeof( pt_pattern_s);
	//}

	// TODO: alternative to this, please

	//mod=(pt_mod_s *)calloc(1,sizeof(*mod) + k * sizeof( pt_pattern_s));
	Serial.print("Memory needed: ");
    Serial.println(  sizeof(*mod) + k * sizeof( pt_pattern_s));
/*	while (1) {
	}*/
	//while(1) {}

	mod = (pt_mod_s*)&__end__;
	memset(mod, 0, sizeof(*mod));


	mod->numpat = k;
	//mod->pattern = ( pt_pattern_s *) (mod + 1);

	for (i = 0; i < 4; ++i)
	{
		mod->chan[i].sp = -1;
	}

	mod->chan[0].pan = 0;
	mod->chan[1].pan = 255;
	mod->chan[2].pan = 255;
	mod->chan[3].pan = 0;

	mod->mastervolume = 256;
	//	mod->flags = 0;										//MODF_ALLOWPANNING;
	mod->flags = MODF_ALLOWFILTER;		//MODF_ALLOWPANNING;

	mod->speed = 6;
	mod->ciaspeed = 125;
	// mod->freq = freq;
	mod->vbllen = FREQ / 50;

#if 0
	for(i = 0; i < 20; ++i)
		mod->name[i] = buf[i];
#endif


	/* samples */

	// smallfs bp = buf + 20;
	file.seek(20,SEEK_SET);

	for (i = 1; i < 32; ++i)
	{
		s = &mod->sample[i];
		file.read(buf,30);
#ifdef DEBUG
//		Serial.print("Name: ");
//		for(l = 0; l < 22, buf[l]; ++l) {
//			Serial.write(buf[l]);
//		}
//		Serial.println("");
#endif

#if 0
		for(l = 0; l < 22; ++l)
			s->name[l] = bp[l];
#endif
		// SmallFS - read everything
		j = buf[22];
		k = buf[23];
		s->length = ((j << 8) + k) << 1;
#ifdef DEBUG
//		Serial.print("Sample ");
//		Serial.print(i);
//		Serial.print(" size is ");
//		Serial.print(s->length);
//		Serial.println(" bytes");
#endif
		j = buf[24];
		s->ft = j & 15;
		j = buf[25];
		s->volume = j;
		j = buf[26];
		k = buf[27];
		s->repeat = ((j << 8) + k) << 1;
		j = buf[28];
		k = buf[29];
		s->replen = ((j << 8) + k) << 1;
		bp += 30;
	}
	/* mod length */


	file.seek(950,SEEK_SET);
    file.read(buf,1);
	//j = buf[950];
	mod->length = buf[0];//j;

	/* positions */

	file.seek(952,SEEK_SET);
	file.read(buf,128);

	// bp = buf + 952;
    bp=&buf[0];
	for (i = 0; i < 128; ++i)
	{
		j = *bp++;
		mod->pos[i] = j;
	}

	/* patterns */

    /*
	file.seek(1084,SEEK_SET);

	file.read(buf,256); // 64 * 4
	Serial.print("Loading patterns:");
	Serial.println(mod->numpat);
	*/
#if 0

	//bp = buf + 1084;
	bp = &buf[0];
	unsigned offset = 1084;

	for (i = 0; i < mod->numpat; ++i)
	{
		for (i2 = 0; i2 < 64; ++i2)
		{
			for (i3 = 0; i3 < 4; ++i3)
			{
				Serial.print("PAT ");
				Serial.print(i); Serial.print(" ");
				Serial.print(i2); Serial.print(" ");
				Serial.print(i3); Serial.print(" ");
				Serial.println(offset);
				offset+=4;
			}
		}
	}
#endif

	Serial.println("Loadind sample data ptr");
	/* ptrs to sampledata */

	bp = (unsigned char*)(1084 + ((64*4*4)*mod->numpat));

	for (i = 1; i < 32; ++i)
	{
		s = &mod->sample[i];

		s->offset = (unsigned)bp;
		bp += s->length;
#ifdef DO_CHECKS
		if (bp - buf > bufsize)
		{
			return FALSE;			/* sample ptr past end of file */
		}
#endif

		s->length <<= 14;			/* include oversampling precision */
		s->repeat <<= 14;
		s->replen <<= 14;
		s->repend = s->repeat + s->replen;
	}

    Serial.println("All loaded");
	/* init lowpass filter */
	mod->filter =FALSE;

#ifdef DO_LOWPASS
	mod->flags |= MODF_ALLOWFILTER;


	tempf = 3.14159265358979 * 4000 / freq;
	tempf = atan(tempf);
	tempf = -(tempf-1)/(1+tempf);
	mod->fltb = (float) tempf;
	mod->flta = (float) ((1 - tempf) / 2);
#endif

	mod->state=PT_STATE_PLAYSONG;

	return mod;
}


void write_data(unsigned char address, unsigned char data)
{
  //YM2149REG(address) = data;
  SIDREG(address) = data;
}

//void set_voice(unsigned char address, int i)
//{
//  write_data(address, sid_midi[i] + pitch);
//  write_data(address+1, (sid_midi[i] >> 8) + pitch);  
//}

void set_freq(unsigned char address, int freq)
{
  write_data(address, freq);
  write_data(address+1, (freq >> 8));  
}

void set_pwm(unsigned char address, int i)
{
  write_data(address, i);
  write_data(address+1, i >> 8);  
}

void reset_ym2149(){
  YM_REG_ENVSHAPE.EMPTY = 0;
  YM_REG_ENVSHAPE.CONT = 1;
  YM_REG_ENVSHAPE.ATT = 1;
  YM_REG_ENVSHAPE.ALT = 1;
  YM_REG_ENVSHAPE.HOLD = 1;  
  
  YM_REG_MIXER.EMPTY = 0;
  YM_REG_MIXER.NOISEC = 1;
  YM_REG_MIXER.NOISEB = 1;
  YM_REG_MIXER.NOISEA = 1;
  YM_REG_MIXER.TONEC = 0;
  YM_REG_MIXER.TONEB = 0;
  YM_REG_MIXER.TONEA = 0;  
  
  YM_REG_VA_LEVEL.EMPTY = 0;  
  YM_REG_VA_LEVEL.MODE = 0;  
  YM_REG_VA_LEVEL.LEVEL = 7;   
  
  YM_REG_VB_LEVEL.EMPTY = 0;  
  YM_REG_VC_LEVEL.EMPTY = 0; 

  YM2149REG(YM_ADDR_LEVEL_A) = *(char*)&YM_REG_VA_LEVEL;
  YM2149REG(YM_ADDR_SHAPE_E) = *(char*)&YM_REG_ENVSHAPE;
  YM2149REG(YM_ADDR_MIXER) = *(char*)&YM_REG_MIXER; 

  freqV4 = ym2149_note2MIDI[128];
  ym_set_freq(YM_ADDR_FREQ_A,freqV4);   
  freqV5 = ym2149_note2MIDI[128];
  ym_set_freq(YM_ADDR_FREQ_B,freqV5);     
  freqV6 = ym2149_note2MIDI[128];
  ym_set_freq(YM_ADDR_FREQ_C,freqV6);   
}  

void reset_sid(){
  //Filter
  SIDREG(SID_ADDR_FILTER_FC_LOW) = 0;
  SIDREG(SID_ADDR_FILTER_FC_HI) = 0;
  SIDREG(SID_ADDR_FILTER_RES_FILT) = 0;
  SIDREG(SID_ADDR_FILTER_MODE_VOL) = 0xF;
  
  //Voice1
  SIDREG(SID_ADDR_V1_FREQ_LOW) = 0;
  SIDREG(SID_ADDR_V1_FREQ_HI) = 0; 
  SIDREG(SID_ADDR_V1_PW_LOW) = 0;
  SIDREG(SID_ADDR_V1_PW_HI) = 0; 
  SID_REG_V1_ATTACK_DECAY.ATTACK = 0;
  SID_REG_V1_ATTACK_DECAY.DECAY = 0; 
  SID_REG_V1_SUSTAIN_RELEASE.SUSTAIN = 0;
  SID_REG_V1_SUSTAIN_RELEASE.RELEASE = 0;
  SID_REG_V1_CONTROLREG.NOISE_WAVE = 0;
  SID_REG_V1_CONTROLREG.SQUARE_WAVE = 0;
  SID_REG_V1_CONTROLREG.SAWTOOTH_WAVE = 0;
  SID_REG_V1_CONTROLREG.TRIANGLE_WAVE = 0; 
  SID_REG_V1_CONTROLREG.TEST = 0;  
  SID_REG_V1_CONTROLREG.RING_MOD = 0;
  SID_REG_V1_CONTROLREG.SYNC = 0;
  SID_REG_V1_CONTROLREG.GATE = 0;
  SIDREG(SID_ADDR_V1_ATTACK_DECAY) = *(char*)&SID_REG_V1_ATTACK_DECAY;
  SIDREG(SID_ADDR_V1_SUSTAIN_RELEASE) = *(char*)&SID_REG_V1_SUSTAIN_RELEASE; 
  SIDREG(SID_ADDR_V1_CONTROLREG) = *(char*)&SID_REG_V1_CONTROLREG;
  
  //Voice2
  SIDREG(SID_ADDR_V2_FREQ_LOW) = 0;
  SIDREG(SID_ADDR_V2_FREQ_HI) = 0; 
  SIDREG(SID_ADDR_V2_PW_LOW) = 0;
  SIDREG(SID_ADDR_V2_PW_HI) = 0; 
  SID_REG_V2_ATTACK_DECAY.ATTACK = 0;
  SID_REG_V2_ATTACK_DECAY.DECAY = 0; 
  SID_REG_V2_SUSTAIN_RELEASE.SUSTAIN = 0;
  SID_REG_V2_SUSTAIN_RELEASE.RELEASE = 0;
  SID_REG_V2_CONTROLREG.NOISE_WAVE = 0;
  SID_REG_V2_CONTROLREG.SQUARE_WAVE = 0;
  SID_REG_V2_CONTROLREG.SAWTOOTH_WAVE = 0;
  SID_REG_V2_CONTROLREG.TRIANGLE_WAVE = 0; 
  SID_REG_V2_CONTROLREG.TEST = 0;  
  SID_REG_V2_CONTROLREG.RING_MOD = 0;
  SID_REG_V2_CONTROLREG.SYNC = 0;
  SID_REG_V2_CONTROLREG.GATE = 0;
  SIDREG(SID_ADDR_V2_ATTACK_DECAY) = *(char*)&SID_REG_V2_ATTACK_DECAY;
  SIDREG(SID_ADDR_V2_SUSTAIN_RELEASE) = *(char*)&SID_REG_V2_SUSTAIN_RELEASE; 
  SIDREG(SID_ADDR_V2_CONTROLREG) = *(char*)&SID_REG_V2_CONTROLREG;

  //Voice3
  SIDREG(SID_ADDR_V3_FREQ_LOW) = 0;
  SIDREG(SID_ADDR_V3_FREQ_HI) = 0; 
  SIDREG(SID_ADDR_V3_PW_LOW) = 0;
  SIDREG(SID_ADDR_V3_PW_HI) = 0; 
  SID_REG_V3_ATTACK_DECAY.ATTACK = 0;
  SID_REG_V3_ATTACK_DECAY.DECAY = 0; 
  SID_REG_V3_SUSTAIN_RELEASE.SUSTAIN = 0;
  SID_REG_V3_SUSTAIN_RELEASE.RELEASE = 0;
  SID_REG_V3_CONTROLREG.NOISE_WAVE = 0;
  SID_REG_V3_CONTROLREG.SQUARE_WAVE = 0;
  SID_REG_V3_CONTROLREG.SAWTOOTH_WAVE = 0;
  SID_REG_V3_CONTROLREG.TRIANGLE_WAVE = 0; 
  SID_REG_V3_CONTROLREG.TEST = 0;  
  SID_REG_V3_CONTROLREG.RING_MOD = 0;
  SID_REG_V3_CONTROLREG.SYNC = 0;
  SID_REG_V3_CONTROLREG.GATE = 0;
  SIDREG(SID_ADDR_V3_ATTACK_DECAY) = *(char*)&SID_REG_V3_ATTACK_DECAY;
  SIDREG(SID_ADDR_V3_SUSTAIN_RELEASE) = *(char*)&SID_REG_V3_SUSTAIN_RELEASE; 
  SIDREG(SID_ADDR_V3_CONTROLREG) = *(char*)&SID_REG_V3_CONTROLREG;  
 
}

//makes the setup for the SID component
void setupSID(){
//waveform
//
//    0 = triangle
//    1 = sawtooth
//    2 = pulse (square)
//    3 = noise
//    4 = ring modulation   
  
//Envelope                                               Wave-
//Number    Instrument  Attack  Decay  Sustain  Release  form   Width
//
//0         Piano       0       9      0        0        2      1536
//1         Accordion   12      0      12       0        1
//2         Calliope    0       0      15       0        0
//3         Drum        0       5      5        0        3
//4         Flute       9       4      4        0        0
//5         Guitar      0       9      2        1        1
//6         Harpsicord  0       9      0        0        2      512
//7         Organ       0       9      9        0        2      2048
//8         Trumpet     8       9      4        1        2      512
//9         Xylophone   0       9      0        0        0  
  
  
  //Set ADSR Attack=0 Decay=9 Sustain=0 Release=0 For Piano 
  //Address 5 = 0xA8 for attack and decay
  //Address 6 = 0xA9 for sustain and release
  SID_REG_V1_ATTACK_DECAY.ATTACK = 0x0;
  SID_REG_V1_ATTACK_DECAY.DECAY = 0x09; 
  SID_REG_V1_SUSTAIN_RELEASE.SUSTAIN = 0x00;
  SID_REG_V1_SUSTAIN_RELEASE.RELEASE = 0x00;
  SID_REG_V1_CONTROLREG.SQUARE_WAVE = 1;
  set_pwm(SID_ADDR_V1_PW_LOW, 1536);  
  
  //Set ADSR Attack=0 Decay=9 Sustain=0 Release=0 For Piano 
  //Address 5 = 0xA8 for attack and decay
  //Address 6 = 0xA9 for sustain and release
  SID_REG_V2_ATTACK_DECAY.ATTACK = 0x0;
  SID_REG_V2_ATTACK_DECAY.DECAY = 0x09; 
  SID_REG_V2_SUSTAIN_RELEASE.SUSTAIN = 0x00;
  SID_REG_V2_SUSTAIN_RELEASE.RELEASE = 0x00;
  SID_REG_V2_CONTROLREG.SQUARE_WAVE = 1;
  set_pwm(SID_ADDR_V2_PW_LOW, 1536);  
  
    //Set ADSR Attack=0 Decay=9 Sustain=0 Release=0 For Piano 
  //Address 5 = 0xA8 for attack and decay
  //Address 6 = 0xA9 for sustain and release
  SID_REG_V3_ATTACK_DECAY.ATTACK = 0x0;
  SID_REG_V3_ATTACK_DECAY.DECAY = 0x09; 
  SID_REG_V3_SUSTAIN_RELEASE.SUSTAIN = 0x00;
  SID_REG_V3_SUSTAIN_RELEASE.RELEASE = 0x00;
  SID_REG_V3_CONTROLREG.SQUARE_WAVE = 1;
  set_pwm(SID_ADDR_V3_PW_LOW, 1536);  

  SIDREG(SID_ADDR_V1_ATTACK_DECAY) = *(char*)&SID_REG_V1_ATTACK_DECAY;
  SIDREG(SID_ADDR_V1_SUSTAIN_RELEASE) = *(char*)&SID_REG_V1_SUSTAIN_RELEASE; 
  SIDREG(SID_ADDR_V1_CONTROLREG) = *(char*)&SID_REG_V1_CONTROLREG;

  SIDREG(SID_ADDR_V2_ATTACK_DECAY) = *(char*)&SID_REG_V2_ATTACK_DECAY;
  SIDREG(SID_ADDR_V2_SUSTAIN_RELEASE) = *(char*)&SID_REG_V2_SUSTAIN_RELEASE; 
  SIDREG(SID_ADDR_V2_CONTROLREG) = *(char*)&SID_REG_V2_CONTROLREG;

  SIDREG(SID_ADDR_V3_ATTACK_DECAY) = *(char*)&SID_REG_V3_ATTACK_DECAY;
  SIDREG(SID_ADDR_V3_SUSTAIN_RELEASE) = *(char*)&SID_REG_V3_SUSTAIN_RELEASE; 
  SIDREG(SID_ADDR_V3_CONTROLREG) = *(char*)&SID_REG_V3_CONTROLREG;  
  
  //Set Volume
  write_data(0x18, 0xF);

}

void setup(){

  playYM = 0;
  playMOD = 0;
  playTrack = 0;
  counter = 1;
  ymTimeStamp = 0;
  
#ifdef DEBUG 
  Serial.begin(9600);
#endif

	SIGMADELTACTL=0x3;
	//pinMode(AUDIO_RIGHT,OUTPUT);
	pinMode(AUDIO_LEFT,OUTPUT);
	pinModePPS(AUDIO_LEFT,HIGH);
	//pinModePPS(AUDIO_RIGHT,HIGH);

	//outputPinForFunction(AUDIO_RIGHT, IOPIN_SIGMADELTA0);
        outputPinForFunction(AUDIO_LEFT, IOPIN_SIGMADELTA1);

	pinMode(AUDIOPIN,OUTPUT);
	digitalWrite(AUDIOPIN,HIGH);
	outputPinForFunction(AUDIOPIN, 8);
	pinModePPS(AUDIOPIN, HIGH);

#ifdef AUDIOPINEXTRA
	pinMode(AUDIOPINEXTRA,OUTPUT);
	outputPinForFunction(AUDIOPINEXTRA, 8);
	pinModePPS(AUDIOPINEXTRA, HIGH);
#endif  
  
	pinMode(SERIAL1RXPIN,INPUT);
	inputPinForFunction(SERIAL1RXPIN, 1);
	//pinModePPS(SERIAL1RXPIN, HIGH);  
  
	if (SmallFS.begin()<0) {
		Serial.println("No SmalLFS found.");
		//while(1);
	}

	ymaudiofile = SmallFS.open("track1.ymdat");
	modfile = SmallFS.open("track1.mod");
        mod = pt_init_smallfs(modfile);
        
	TMR0CTL = 0;
	TMR0CNT = 0;
	TMR0CMP = ((CLK_FREQ/2) / FREQ )- 1;
	TMR0CTL = _BV(TCTLENA)|_BV(TCTLCCM)|_BV(TCTLDIR)|
		_BV(TCTLCP0) | _BV(TCTLIEN);

	INTRMASK = BIT(INTRLINE_TIMER0); // Enable Timer0 interrupt

	INTRCTL=1;        

//	INTRMASK = BIT(INTRLINE_TIMER1); // Enable Timer1 interrupt
//
//	//INTRCTL=1;
//
//	// Start timer, 50Hz (prescaler 64)
//	TMR0CTL = 0;
//	TMR0CNT = 0;
//	TMR0CMP = (CLK_FREQ/(50U*64))-1;
//	TMR0CNT = 0x0;
//	TMR0CTL = BIT(TCTLENA)|BIT(TCTLCCM)|BIT(TCTLDIR)|BIT(TCTLCP2)|BIT(TCTLCP0)|BIT(TCTLIEN);  
  
   ///Setup the pin modes for the YM2149 and SID
   reset_sid();
   
   setupSID();
   setupYM2149();
   reset_ym2149();   
   
   
  // Initiate MIDI communications, listen to all channels
  MIDI.begin(MIDI_CHANNEL_OMNI);
 
  // Connect the HandleNoteOn function to the library, so it is called upon reception of a NoteOn.
  MIDI.setHandleNoteOn(HandleNoteOn);  // Put only the name of the function
  MIDI.setHandleNoteOff(HandleNoteOff);  // Put only the name of the function
  MIDI.setHandleControlChange(HandleControlChange);  // Put only the name of the function  
  MIDI.setHandleProgramChange(HandleProgramChange);  // Put only the name of the function    
  MIDI.setHandlePitchBend(HandlePitchBend);  // Put only the name of the function  

  //Setup LCD
  pinMode(WING_C_14, OUTPUT);     //Set contrast to GND
  digitalWrite(WING_C_14, LOW);   //Set contrast to GND    
 // set up the LCD's number of columns and rows:
 lcd.begin(16,2);
 // clear the LCD screen:
 lcd.clear();
 lcd.setCursor(3,0);
 lcd.print("RetroCade");   
 lcd.setCursor(3,1);
 lcd.print("Prototype"); 
  
}

void loop(){
  // Call MIDI.read the fastest you can for real-time performance.
  MIDI.read();
  //Serial.println("Serial Test");
  if (playTrack == 1){
    if (playYM == 1){
      if (ymTimeStamp == 32)
        ymTimeStamp = 0;
      if (ymTimeStamp == 0 && playMOD == 1){
            modfile = SmallFS.open("kick.mod");
            mod = pt_init_smallfs(modfile);
      }
      if (ymTimeStamp == 16 && playMOD == 1){
            modfile = SmallFS.open("hihat.mod");
            mod = pt_init_smallfs(modfile);
      }      
      audiofill();       
    }
    
    if (playMOD == 1){
      int i;
      pt_render(modfile, mod, buf, NULL, 2, 16 /* Samples */, 1, 16, 1);
  	for (i=0;i<32;i+=2) {
  	  unsigned v = buf[i];
  	  v += buf[i+1]<<8;
  	  v += buf[i]<<16;
          v += buf[i+1]<<24;
  	  v+=0x80008000;
          while (audioBuffer.isFull());
  	    audioBuffer.push(v);
  	}
    }      
  }
}

void HandlePitchBend(byte channel, int bend) { 
  #ifdef DEBUG
  Serial.print("Pitch Bend Received: ");
  Serial.println(channel);  
  Serial.println(bend); 
  #endif  
  if (SID_REG_V1_CONTROLREG.RING_MOD == 0){
    if (freqV1 + bend > 388) 
     set_freq(SID_ADDR_V1_FREQ_LOW,freqV1+bend); 
  else 
    if (freqV3 + bend > 388) 
     set_freq(SID_ADDR_V3_FREQ_LOW,freqV3+bend); 
  }
  if (freqV2 + bend > 388) 
   set_freq(SID_ADDR_V2_FREQ_LOW,freqV1+bend); 
  if (freqV3 + bend > 388) 
   set_freq(SID_ADDR_V3_FREQ_LOW,freqV1+bend);
//  if (freqV4 + bend > 388) 
//   ym_set_freq(YM_ADDR_FREQ_A,freqV1+bend);
//  if (freqV5 + bend > 388) 
//   ym_set_freq(YM_ADDR_FREQ_B,freqV1+bend);
//  if (freqV6 + bend > 388) 
//   ym_set_freq(YM_ADDR_FREQ_C,freqV1+bend);  
}

void HandleProgramChange(byte channel, byte number) { 
//  #ifdef DEBUG
//    Serial.print("Program Change Received: ");
//    Serial.print(channel);  
//    Serial.println(number);    
//  #endif 
//  switch(channel){
//    case 1:
//      switch(number){
//          //Voice 1
//          case 0:
//            SID_REG_V1_CONTROLREG.TRIANGLE_WAVE = !SID_REG_V1_CONTROLREG.TRIANGLE_WAVE;
//            break;
//          case 1:
//            SID_REG_V1_CONTROLREG.SAWTOOTH_WAVE = !SID_REG_V1_CONTROLREG.SAWTOOTH_WAVE;
//            break;
//          case 2:
//            SID_REG_V1_CONTROLREG.SQUARE_WAVE = !SID_REG_V1_CONTROLREG.SQUARE_WAVE;
//            break;
//          case 3:
//            SID_REG_V1_CONTROLREG.NOISE_WAVE = !SID_REG_V1_CONTROLREG.NOISE_WAVE;
//            break;   
//          case 4:
//            SID_REG_V1_CONTROLREG.RING_MOD = !SID_REG_V1_CONTROLREG.RING_MOD;
//            break;   
//           case 5:
//            SID_REG_V1_CONTROLREG.SYNC = !SID_REG_V1_CONTROLREG.SYNC;
//            break; 
//         default:
//            return;              
//      }
//      SIDREG(SID_ADDR_V1_CONTROLREG) = *(char*)&SID_REG_V1_CONTROLREG;  
//      break;
//    case 2:
//      switch(number){
//          //Voice 2
//          case 0:
//            SID_REG_V2_CONTROLREG.TRIANGLE_WAVE = !SID_REG_V2_CONTROLREG.TRIANGLE_WAVE;
//            break;
//          case 1:
//            SID_REG_V2_CONTROLREG.SAWTOOTH_WAVE = !SID_REG_V2_CONTROLREG.SAWTOOTH_WAVE;
//            break;
//          case 2:
//            SID_REG_V2_CONTROLREG.SQUARE_WAVE = !SID_REG_V2_CONTROLREG.SQUARE_WAVE;
//            break;
//          case 3:
//            SID_REG_V2_CONTROLREG.NOISE_WAVE = !SID_REG_V2_CONTROLREG.NOISE_WAVE;
//            break;  
//         default:
//            return;               
//      }
//      SIDREG(SID_ADDR_V2_CONTROLREG) = *(char*)&SID_REG_V2_CONTROLREG;  
//      break;    
//    case 3:
//      switch(number){
//          //Voice 3
//          case 0:
//            SID_REG_V3_CONTROLREG.TRIANGLE_WAVE = !SID_REG_V3_CONTROLREG.TRIANGLE_WAVE;
//            break;
//          case 1:
//            SID_REG_V3_CONTROLREG.SAWTOOTH_WAVE = !SID_REG_V3_CONTROLREG.SAWTOOTH_WAVE;
//            break;
//          case 2:
//            SID_REG_V3_CONTROLREG.SQUARE_WAVE = !SID_REG_V3_CONTROLREG.SQUARE_WAVE;
//            break;
//          case 3:
//            SID_REG_V3_CONTROLREG.NOISE_WAVE = !SID_REG_V3_CONTROLREG.NOISE_WAVE;
//            break;  
//          default:
//            return;               
//      }
//      SIDREG(SID_ADDR_V3_CONTROLREG) = *(char*)&SID_REG_V3_CONTROLREG;  
//      break; 
//   default:
//      return;         
//  }
}

void HandleControlChange(byte channel, byte number, byte value) { 
  byte tmp;
 #ifdef DEBUG 
  Serial.print("Change Control Channel: ");
  Serial.println(channel); 
  Serial.print("Change Control Number: ");  
  Serial.println(number);  
  Serial.print("Change Control Value: ");    
  Serial.println(value); 
 #endif  
  
  switch(channel) {
    case 1:
      switch(number) {
        //Voice1
        case 74:
          SID_REG_V1_ATTACK_DECAY.ATTACK = value/8;
          break;
        case 71:
          SID_REG_V1_ATTACK_DECAY.DECAY = value/8;
          break;
        case 91:
          SID_REG_V1_SUSTAIN_RELEASE.SUSTAIN = value/8;
          break;      
        case 93:
          SID_REG_V1_SUSTAIN_RELEASE.RELEASE = value/8;
          break; 
        case 75:  //Voice1 PW Fine
          SIDREG(SID_ADDR_V1_PW_LOW) = (value << 1);
          break;      
        case 76:  //Voice1 PW Coarse
          SIDREG(SID_ADDR_V1_PW_HI) = (value << 1);
          break;   
        case 92:  //Voice3 Ring Mod Fine
          SIDREG(SID_ADDR_V3_FREQ_LOW) = (value << 1);
          break;      
        case 95:  //Voice3 Ring Mod Coarse
          SIDREG(SID_ADDR_V3_FREQ_HI) = (value << 1);
          break; 
        case 2:
          if (value == 0)
            SID_REG_V1_CONTROLREG.TRIANGLE_WAVE = 0;
          else
            SID_REG_V1_CONTROLREG.TRIANGLE_WAVE = 1;
          break;
        case 3:
          if (value == 0)
            SID_REG_V1_CONTROLREG.SAWTOOTH_WAVE = 0;
          else
            SID_REG_V1_CONTROLREG.SAWTOOTH_WAVE = 1;            
          break;
        case 4:
          if (value == 0)
            SID_REG_V1_CONTROLREG.SQUARE_WAVE = 0;
          else
            SID_REG_V1_CONTROLREG.SQUARE_WAVE = 1;             
          break;
        case 5:
          if (value == 0)
            SID_REG_V1_CONTROLREG.NOISE_WAVE = 0;
          else
            SID_REG_V1_CONTROLREG.NOISE_WAVE = 1;             
          break;   
        case 6:
          if (value == 0)
            SID_REG_V1_CONTROLREG.RING_MOD = 0;
          else
            SID_REG_V1_CONTROLREG.RING_MOD = 1;             
          break;   
         case 7:
           if (value == 0)
            SID_REG_V1_CONTROLREG.SYNC = 0;
          else
            SID_REG_V1_CONTROLREG.SYNC = 1;             
          break;  
        default:
          return;             
      }      
      SIDREG(SID_ADDR_V1_ATTACK_DECAY) = *(char*)&SID_REG_V1_ATTACK_DECAY;
      SIDREG(SID_ADDR_V1_SUSTAIN_RELEASE) = *(char*)&SID_REG_V1_SUSTAIN_RELEASE;
      SIDREG(SID_ADDR_V1_CONTROLREG) = *(char*)&SID_REG_V1_CONTROLREG;  
//    case 2:
//      switch(number) {
//        //Voice1
//        case 74:
//          SID_REG_V2_ATTACK_DECAY.ATTACK = value/8;
//          break;
//        case 71:
//          SID_REG_V2_ATTACK_DECAY.DECAY = value/8;
//          break;
//        case 91:
//          SID_REG_V2_SUSTAIN_RELEASE.SUSTAIN = value/8;
//          break;      
//        case 93:
//          SID_REG_V2_SUSTAIN_RELEASE.RELEASE = value/8;
//          break; 
//        case 75:  //Voice1 PW Fine
//          SIDREG(SID_ADDR_V2_PW_LOW) = (value << 1);
//          break;      
//        case 76:  //Voice1 PW Coarse
//          SIDREG(SID_ADDR_V2_PW_HI) = (value << 1);
//        break;  
//          if (value == 0)
//            SID_REG_V2_CONTROLREG.TRIANGLE_WAVE = 0;
//          else
//            SID_REG_V2_CONTROLREG.TRIANGLE_WAVE = 1;             
//          break;
//        case 3:
//          if (value == 0)
//            SID_REG_V2_CONTROLREG.SAWTOOTH_WAVE = 0;
//          else
//            SID_REG_V2_CONTROLREG.SAWTOOTH_WAVE = 1;             
//          break;
//        case 4:
//          if (value == 0)
//            SID_REG_V2_CONTROLREG.SQUARE_WAVE = 0;
//          else
//            SID_REG_V2_CONTROLREG.SQUARE_WAVE = 1;             
//          break;
//        case 5:
//          if (value == 0)
//            SID_REG_V2_CONTROLREG.NOISE_WAVE = 0;
//          else
//            SID_REG_V2_CONTROLREG.NOISE_WAVE = 1;             
//          break;   
//        default:
//          return;             
//      }      
//      SIDREG(SID_ADDR_V2_ATTACK_DECAY) = *(char*)&SID_REG_V2_ATTACK_DECAY;
//      SIDREG(SID_ADDR_V2_SUSTAIN_RELEASE) = *(char*)&SID_REG_V2_SUSTAIN_RELEASE;
//      SIDREG(SID_ADDR_V2_CONTROLREG) = *(char*)&SID_REG_V2_CONTROLREG;       
//    case 3:
//      switch(number) {
//        //Voice1
//        case 74:
//          SID_REG_V3_ATTACK_DECAY.ATTACK = value/8;
//          break;
//        case 71:
//          SID_REG_V3_ATTACK_DECAY.DECAY = value/8;
//          break;
//        case 91:
//          SID_REG_V3_SUSTAIN_RELEASE.SUSTAIN = value/8;
//          break;      
//        case 93:
//          SID_REG_V3_SUSTAIN_RELEASE.RELEASE = value/8;
//          break; 
//        case 75:  //Voice1 PW Fine
//          SIDREG(SID_ADDR_V3_PW_LOW) = (value << 1);
//          break;      
//        case 76:  //Voice1 PW Coarse
//          SIDREG(SID_ADDR_V3_PW_HI) = (value << 1);
//          break; 
//        case 2:
//          if (value == 0)
//            SID_REG_V3_CONTROLREG.TRIANGLE_WAVE = 0;
//          else
//            SID_REG_V3_CONTROLREG.TRIANGLE_WAVE = 1;             
//          break;
//        case 3:
//          if (value == 0)
//            SID_REG_V3_CONTROLREG.SAWTOOTH_WAVE = 0;
//          else
//            SID_REG_V3_CONTROLREG.SAWTOOTH_WAVE = 1;             
//          break;
//        case 4:
//          if (value == 0)
//            SID_REG_V3_CONTROLREG.SQUARE_WAVE = 0;
//          else
//            SID_REG_V3_CONTROLREG.SQUARE_WAVE = 1;             
//          break;
//        case 5:
//          if (value == 0)
//            SID_REG_V3_CONTROLREG.NOISE_WAVE = 0;
//          else
//            SID_REG_V3_CONTROLREG.NOISE_WAVE = 1;             
//          break; 
//        default:
//          return;             
//      }      
//      SIDREG(SID_ADDR_V3_ATTACK_DECAY) = *(char*)&SID_REG_V3_ATTACK_DECAY;
//      SIDREG(SID_ADDR_V3_SUSTAIN_RELEASE) = *(char*)&SID_REG_V3_SUSTAIN_RELEASE; 
//      SIDREG(SID_ADDR_V3_CONTROLREG) = *(char*)&SID_REG_V3_CONTROLREG;        
    case 4:  //YM2149 Voice A
      switch(number) {
        //VoiceA
        case 2:
          if (value == 0)
            YM_REG_ENVSHAPE.CONT = 0;
          else
            YM_REG_ENVSHAPE.CONT = 1;             
          break;
        case 3:
          if (value == 0)
            YM_REG_ENVSHAPE.ATT = 0;
          else
            YM_REG_ENVSHAPE.ATT = 1;             
          break;
        case 4:
          if (value == 0)
            YM_REG_ENVSHAPE.ALT = 0;
          else
            YM_REG_ENVSHAPE.ALT = 1;             
          break;
        case 5:
          if (value == 0)
            YM_REG_ENVSHAPE.HOLD = 0;
          else
            YM_REG_ENVSHAPE.HOLD = 1;             
          break;   
        case 6:
          if (value == 0)
            YM_REG_MIXER.NOISEA = 1;
          else
            YM_REG_MIXER.NOISEA = 0;             
          break;  
        case 7:
          if (value == 0)
            YM_REG_MIXER.TONEA = 1;
          else
            YM_REG_MIXER.TONEA = 0;             
          break;  
        case 8:
          if (value == 0)
            YM_REG_VA_LEVEL.MODE = 0;
          else
            YM_REG_VA_LEVEL.MODE = 1;             
          break;           
        case 74:
          YM2149REG(YM_ADDR_NOISE) = value/4;
          break;
        case 71:
          YM_REG_VA_LEVEL.LEVEL = value/8;
          break;
        case 75:  //YM Envelope Freq Fine
          YM2149REG(YM_ADDR_FREQ_E) = (value << 1);
          break;      
        case 76:  //YM Envelope Freq Coarse
          YM2149REG(YM_ADDR_FREQ_E+1) = (value << 1);
          break; 
        default:
          return;        
      }      
      YM2149REG(YM_ADDR_LEVEL_A) = *(char*)&YM_REG_VA_LEVEL;
      YM2149REG(YM_ADDR_SHAPE_E) = *(char*)&YM_REG_ENVSHAPE;
      YM2149REG(YM_ADDR_MIXER) = *(char*)&YM_REG_MIXER;      
//    case 5:  //YM2149 Voice B
//      switch(number) {
//        //VoiceB
//        case 2:
//          if (value == 0)
//            YM_REG_ENVSHAPE.CONT = 0;
//          else
//            YM_REG_ENVSHAPE.CONT = 1;             
//          break;
//        case 3:
//          if (value == 0)
//            YM_REG_ENVSHAPE.ATT = 0;
//          else
//            YM_REG_ENVSHAPE.ATT = 1;             
//          break;
//        case 4:
//          if (value == 0)
//            YM_REG_ENVSHAPE.ALT = 0;
//          else
//            YM_REG_ENVSHAPE.ALT = 1;             
//          break;
//        case 5:
//          if (value == 0)
//            YM_REG_ENVSHAPE.HOLD = 0;
//          else
//            YM_REG_ENVSHAPE.HOLD = 1;             
//          break;   
//        case 6:
//          if (value == 0)
//            YM_REG_MIXER.NOISEB = 1;
//          else
//            YM_REG_MIXER.NOISEB = 0;             
//          break;  
//        case 7:
//          if (value == 0)
//            YM_REG_MIXER.TONEB = 1;
//          else
//            YM_REG_MIXER.TONEB = 0;             
//          break;  
//        case 8:
//          if (value == 0)
//            YM_REG_VB_LEVEL.MODE = 0;
//          else
//            YM_REG_VB_LEVEL.MODE = 1;             
//          break;           
//        case 74:
//          YM2149REG(YM_ADDR_NOISE) = value/4;
//          break;
//        case 71:
//          YM_REG_VB_LEVEL.LEVEL = value/8;
//          break;
//        case 75:  //YM Envelope Freq Fine
//          YM2149REG(YM_ADDR_FREQ_E) = (value << 1);
//          break;      
//        case 76:  //YM Envelope Freq Coarse
//          YM2149REG(YM_ADDR_FREQ_E+1) = (value << 1);
//          break; 
//        default:
//          return;        
//      }      
//      YM2149REG(YM_ADDR_LEVEL_B) = *(char*)&YM_REG_VB_LEVEL;
//      YM2149REG(YM_ADDR_SHAPE_E) = *(char*)&YM_REG_ENVSHAPE;
//      YM2149REG(YM_ADDR_MIXER) = *(char*)&YM_REG_MIXER;
    case 6:  //YM2149 Voice C
      switch(number) {
        //VoiceC
        case 2:
          if (value == 0)
            YM_REG_ENVSHAPE.CONT = 0;
          else
            YM_REG_ENVSHAPE.CONT = 1;             
          break;
        case 3:
          if (value == 0)
            YM_REG_ENVSHAPE.ATT = 0;
          else
            YM_REG_ENVSHAPE.ATT = 1;             
          break;
        case 4:
          if (value == 0)
            YM_REG_ENVSHAPE.ALT = 0;
          else
            YM_REG_ENVSHAPE.ALT = 1;             
          break;
        case 5:
          if (value == 0)
            YM_REG_ENVSHAPE.HOLD = 0;
          else
            YM_REG_ENVSHAPE.HOLD = 1;             
          break;   
        case 6:
          if (value == 0)
            YM_REG_MIXER.NOISEC = 1;
          else
            YM_REG_MIXER.NOISEC = 0;             
          break;  
        case 7:
          if (value == 0)
            YM_REG_MIXER.TONEC = 1;
          else
            YM_REG_MIXER.TONEC = 0;             
          break;  
        case 8:
          if (value == 0)
            YM_REG_VC_LEVEL.MODE = 0;
          else
            YM_REG_VC_LEVEL.MODE = 1;             
          break;   
        case 9:
          if (value == 0)
            playMOD = 0;
          else{
            modfile = SmallFS.open("track1.mod");
            mod = pt_init_smallfs(modfile);
            playMOD = 1;             
          }
          break;   
        case 11:
          if (value == 0)
            playMOD = 0;
          else{
            modfile = SmallFS.open("track2.mod");
            mod = pt_init_smallfs(modfile);
            playMOD = 1;             
          }
          break;  
        case 13:
          if (value == 0)
            playMOD = 0;
          else{
            modfile = SmallFS.open("track3.mod");
            mod = pt_init_smallfs(modfile);
            playMOD = 1;             
          }
          break;            
        case 10:
          if (value == 0){
            playYM = 0;
            resetYMFlag = 1;
          }
          else{
            ymaudiofile = SmallFS.open("track1.ymdat"); 
            ymTimeStamp = 0;
            playYM = 1;             
          }
          break;    
         case 12:
          if (value == 0){
            playYM = 0;
            resetYMFlag = 1;
          }
          else{
            ymaudiofile = SmallFS.open("track2.ymdat"); 
            playYM = 1;             
            ymTimeStamp = 0;            
          }
          break;   
         case 14:
          if (value == 0){
            playYM = 0;
            resetYMFlag = 1;
          }
          else{
            ymaudiofile = SmallFS.open("track3.ymdat"); 
            playYM = 1;     
            ymTimeStamp = 0;            
          }
          break;            
        case 74:
          YM2149REG(YM_ADDR_NOISE) = value/4;
          break;
        case 71:
          YM_REG_VC_LEVEL.LEVEL = value/8;
          break;
        case 75:  //YM Envelope Freq Fine
          YM2149REG(YM_ADDR_FREQ_E) = (value << 1);
          break;      
        case 76:  //YM Envelope Freq Coarse
          YM2149REG(YM_ADDR_FREQ_E+1) = (value << 1);
          break; 
        case 72:  //YM Envelope Freq Coarse
          setVolumeYM2149(value << 1);
          break; 
        case 84:  //YM Envelope Freq Coarse
          mod->mastervolume = (value << 1);
          break;           
        default:
          return;   
      }      
//      YM2149REG(YM_ADDR_LEVEL_C) = *(char*)&YM_REG_VC_LEVEL;
//      YM2149REG(YM_ADDR_SHAPE_E) = *(char*)&YM_REG_ENVSHAPE;
//      YM2149REG(YM_ADDR_MIXER) = *(char*)&YM_REG_MIXER; 
    case 16:  //Transport
      switch(number) {
        //Transport
        case 117:
          if (value == 0)
            playTrack = 1;
          break;          
        case 116:
          if (value == 127){
            playTrack = 0;
            resetYMFlag = 1;
          }
          break; 
        default:
          return;
      } 
   default:
    return;        
  }      
}

void HandleNoteOn(byte channel, byte pitch, byte velocity) { 
 #ifdef DEBUG
  Serial.print("Note Received: ");
  Serial.println(pitch);
  Serial.print("Channel Received: ");
  Serial.println(channel);  
 #endif 
  switch (channel){
    case 1:
      freqV1 = sid_midi[pitch];
      set_freq(SID_ADDR_V1_FREQ_LOW,freqV1);
      SID_REG_V1_CONTROLREG.GATE = 1;
      SIDREG(SID_ADDR_V1_CONTROLREG) = *(char*)&SID_REG_V1_CONTROLREG;    
      break;
    case 2:
      freqV2 = sid_midi[pitch];
      set_freq(SID_ADDR_V2_FREQ_LOW,freqV2);
      SID_REG_V2_CONTROLREG.GATE = 1;
      SIDREG(SID_ADDR_V2_CONTROLREG) = *(char*)&SID_REG_V2_CONTROLREG;    
      break;
    case 3:
      freqV3 = sid_midi[pitch];
      set_freq(SID_ADDR_V3_FREQ_LOW,freqV3);
      SID_REG_V3_CONTROLREG.GATE = 1;
      SIDREG(SID_ADDR_V3_CONTROLREG) = *(char*)&SID_REG_V3_CONTROLREG;    
      break;     
    case 4:
      freqV4 = ym2149_note2MIDI[pitch];
      ym_set_freq(YM_ADDR_FREQ_A,freqV4);  
      break;        
    case 5:
      freqV5 = ym2149_note2MIDI[pitch];
      ym_set_freq(YM_ADDR_FREQ_B,freqV5);  
      break;        
    case 6:
      freqV6 = ym2149_note2MIDI[pitch];
      ym_set_freq(YM_ADDR_FREQ_C,freqV6);  
      break;  
    case 7:
      switch (pitch){
        case 35:
            modfile = SmallFS.open("kick.mod");
            mod = pt_init_smallfs(modfile);
        break;
        case 38:
            modfile = SmallFS.open("hihat.mod");
            mod = pt_init_smallfs(modfile);
        break;
        default:
          return;
      }
      break;       
    default:
      break;             
  }
  

}

void HandleNoteOff(byte channel, byte pitch, byte velocity) { 

  switch(channel){
      case 1:
        SID_REG_V1_CONTROLREG.GATE = 0;
        SIDREG(SID_ADDR_V1_CONTROLREG) = *(char*)&SID_REG_V1_CONTROLREG;
        break;
      case 2:
        SID_REG_V2_CONTROLREG.GATE = 0;
        SIDREG(SID_ADDR_V2_CONTROLREG) = *(char*)&SID_REG_V2_CONTROLREG;
        break;
      case 3:
        SID_REG_V3_CONTROLREG.GATE = 0;
        SIDREG(SID_ADDR_V3_CONTROLREG) = *(char*)&SID_REG_V3_CONTROLREG; 
        break;
      case 4:
        freqV4 = ym2149_note2MIDI[128];
        ym_set_freq(YM_ADDR_FREQ_A,freqV4); 
        break;        
      case 5:
        freqV5 = ym2149_note2MIDI[128];
        ym_set_freq(YM_ADDR_FREQ_B,freqV5); 
        break;  
      case 6:
        freqV6 = ym2149_note2MIDI[128];
        ym_set_freq(YM_ADDR_FREQ_C,freqV6); 
        break; 
      default:
        return;      
  }  
   #ifdef DEBUG 
    Serial.println("In NoteOff");
   #endif
}

