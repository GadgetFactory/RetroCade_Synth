#ifndef _PTREPLAY_H
#define _PTREPLAY_H

/*
** protracker 2.3a player
** (C) 2001, 2003 Ronald Hof and Timm S. Mueller
*/

class SmallFSFile;
class RamFSFile;

#define CONSTANT_FREQ
#define FREQ 17000
#define EMBEDDED
#define PATTERN_OFFSET 1084

#define SKIP_NAMES

//#define DO_LOWPASS /* emulate $E0 */
#define DO_SONGEND /* handle song end */
#define DO_MASTERVOLUME /* use mastervolume */
#define DO_CLAMPING /* clamp output */
#undef DO_PANNING /* use $8 for panning */

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef NULL
#define NULL 0
#endif

#ifndef LIBAPI
#define LIBAPI
#endif

#define MODF_DOSONGEND 0x0001 /* perform song-end detection */
#define MODF_ALLOWPANNING 0x0002 /* allow mod to use command $8 for panning */
#define MODF_ALLOWFILTER 0x0004 /* allow mod to set lowpass filter */
#define MODF_SONGEND 0x0008 /* songend occured */

#define PT_STATE_PLAYSONG 1
#define PT_STATE_STOP 2
#define PT_STATE_PLAYPATTERN 3

typedef struct
{
//char name[32];
//signed char *data;
    unsigned offset;
int length, repeat, replen, repend;	/* <<14, includes oversampling precision */
int ft;
int volume;
int pad;
}pt_sample_s;	/* 32 bytes */

typedef struct
{
int sample, prevsample, sp, per, freq, oldsp;
int dp, basevol,prevvol, vol, dvol, pan, ft;
unsigned int flags;
int dtp, tptarget, tpsrc;
int retrig, cutoff, delay;
int arp, arpbase;
int vibspeed, vibdepth, avibspeed, avibindex;
int trespeed, tredepth, atrespeed, atreindex;
int vibwave, tremwave, glissando, funkoffset,funkcnt;
int delaysample;
int loopstart, loopcount, loopflg;
} pt_channel_s;

typedef struct
{
int period, sample;
unsigned char effect, efd1, efd2, efboth;
int pad;
} pt_patterndata_s;	/* 16 bytes */

typedef struct
{
pt_patterndata_s data[64][4];
}pt_pattern_s;

typedef struct
{
pt_sample_s sample[32];
pt_channel_s chan[4];
int pos[128];
#ifndef SKIP_NAMES
char name[32];
#endif
#ifndef EMBEDDED
pt_pattern_s *pattern;
#endif
unsigned int flags;
int numpat;
int length;
#ifndef CONSTANT_FREQ
int freq;
#endif
int vbllen;
int mastervolume;
int spos, ppos, speed, ciaspeed, bpos, tick;

int cspos,cppos;
int patdelay, skiptopos, filter;
int songloopcount;
#ifdef DO_LOWPASS
float flta, fltb;
float fltpi[4];
float fltpo[4];
#endif
//char *buf;
int state;
} pt_mod_s;

#ifdef __cplusplus
extern "C" {
#endif

//LIBAPI unsigned int pt_init( pt_mod_s *pmod, unsigned char *buf, int bufsize, int freq);
LIBAPI void pt_render(RamFSFile&,pt_mod_s *pmod, char *buf, char *buf2, int bufmodulo, int numsmp, int scale, int depth, int channels);
pt_mod_s * pt_load(char *filename, int freq);
void pt_free(pt_mod_s * mod);
pt_mod_s *pt_init(unsigned char *buf, int bufsize, int freq);

#ifdef __cplusplus
}
#endif

#endif
