/*!
 *  @file		modplayer.cpp
 *  Project		modplayer Library
 *	@brief		modplayer wrapper for the ptplay library for the ZPUino, ported by Alvaro Lopes
 *	Version		1.0
 *  @author		Jack Gassett 
 *	@date		4/10/12
 *  License		GPL
 */

#include "modplayer.h"
#include <SD.h>
#include "ptplay.h"
#include "RetroCade.h"

#define DEBUG

MODPLAYER::MODPLAYER(){
  
}
 
void MODPLAYER::setup(){    
  underruns = 0;
  playing = false;
  fileLoaded = false;
}

void MODPLAYER::loadFile(const char* name)
{
  fileLoaded = false;
  modSmallFSfile = SmallFS.open(name);
  modSDfile =SD.open(name);
  boolean smallFsCheck = modSmallFSfile.valid();
  boolean sdFsCheck = modSDfile;
  if (sdFsCheck){
    Serial.println("Opening Mod File from SD Card.");
    modRAMfile = RamFS.open(&modSDfile);
    modSDfile.close();
    fileLoaded = true;
  }
  else if (smallFsCheck){
    Serial.println("Opening Mod File from SmallFS.");
    modRAMfile = RamFS.open(&modSmallFSfile);
    fileLoaded = true;
  }
  else {
    Serial.println("No mod files to play in SmallFS or on SD card.");
    fileLoaded = false; 
  }
  mod = pt_init_smallfs();  
}

void MODPLAYER::play(boolean play)
{
  if (fileLoaded)
    playing = play;
  else
    Serial.println("No mod file to play.");
}

void MODPLAYER::volume(int volume)
{
  mod->mastervolume = volume;
}

void MODPLAYER::audiofill()
{
    int i;
    pt_render(modRAMfile, mod, buf, NULL, 2, 16 /* Samples */, 1, 16, 1);
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

boolean MODPLAYER::getPlaying()
{
  return playing;
}

void MODPLAYER::zpu_interrupt()
{
	// Play mod file
	if (audioBuffer.hasData()) {
		unsigned v = audioBuffer.pop();
		SIGMADELTADATA = v;
	} else {
          underruns++;
	}
	TMR0CTL &= ~(BIT(TCTLIF));    //TODO: This should probably go into main interupt
}


//extern unsigned char __end__;  
pt_mod_s *MODPLAYER::pt_init_smallfs()
{
//        Serial.println("Starting pt_init");
	unsigned char buf[256]; // Some buffer. Let's hope this fits on stack.
	unsigned char *bp=&buf[0];

	int i, j, k;
	//double tempf;
	pt_mod_s *mod;
	pt_sample_s *s;
	//pt_pattern_s *pat;
	//int i2, i3;
	//pt_patterndata_s *p;

	/* M.K. signature */

	// smallfs bp = buf + 0x438;

               modRAMfile.seek(0x438, SEEK_SET);



//#ifdef DEBUG

              modRAMfile.read(&bp[0], 4);

	
        Serial.print(bp[0], HEX);
        Serial.print(" ");
        Serial.print(bp[1], HEX);
        Serial.print(" ");
        Serial.print(bp[2], HEX);
        Serial.print(" ");
        Serial.print(bp[3], HEX);
        Serial.print(" ");        
        Serial.println(" ");        
      
	if (!(bp[0] == 'M' && bp[1] == '.' && bp[2] == 'K' && bp[3] == '.')) {
		Serial.println("Invalid MOD file");
		return NULL;
	}
//#endif

	/* determine number of patterns */

	k = 0;
	// smallfs bp = buf + 952;
      	      modRAMfile.seek(952, SEEK_SET);
      	      modRAMfile.read(&buf,128);


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

	//mod = (pt_mod_s*)&__end__;
        mod = (pt_mod_s*)malloc(sizeof(pt_mod_s));
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

              modRAMfile.seek(20, SEEK_SET);

	

	for (i = 1; i < 32; ++i)
	{
		s = &mod->sample[i];
                    modRAMfile.read(buf,30);
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


	    modRAMfile.seek(950, SEEK_SET);
            modRAMfile.read(buf,1);


	//j = buf[950];
	mod->length = buf[0];//j;

	/* positions */

	    modRAMfile.seek(952, SEEK_SET);
	    modRAMfile.read(buf,128);

	// bp = buf + 952;
    bp=&buf[0];
	for (i = 0; i < 128; ++i)
	{
		j = *bp++;
		mod->pos[i] = j;
	}

	/* patterns */

    /*
	file.seek(1084);

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

	Serial.println("Loading sample data ptr");
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
