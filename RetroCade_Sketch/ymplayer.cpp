/*!
 *  @file		ymplayer.cpp
 *  Project		ymplayer Library
 *	@brief		ymplayer library for the ZPUino
 *	Version		1.0
 *  @author		Jack Gassett 
 *	@date		4/10/12
 *  License		GPL
 */
 
#include "ymplayer.h"
#include "YM2149.h"

#define DEBUG
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

YMPLAYER::YMPLAYER(){
  
}
 
/*!
@par Description
Initialize the YMPlayer
@par Syntax
ymplayer.setup(ym)
@param YM2149 Pointer to a YM2149 object

@par Example
~~~~~~~~{.c}
#include "YM2149.h"
#include "ymplayer.h"

YM2149 ym2149;

void setup()
{
	ymplayer.setup(&ym2149);
}

void loop() {}
~~~~~~~~
\n
*/
void YMPLAYER::setup(YM2149* ym){    
  //underruns = 0;
  timerTicks = 0;
  ymTimeStamp = 0;
  resetYMFlag = 0;
  counter = 0;
  playing = false;
  ym2149 = ym;
  volumeAdjust = 4;
}

/*!
@par Description
Load a ym file from smallfs or a SD card. The provided file name will be searched for in the smallfs filesystem first and the SD file system second. If the filename exists in both locations the SD version will override the smallfs version.
@par Syntax
ymplayer.loadFile(name)
@param name filename of the ym file to open.

@par Example
~~~~~~~~{.c}
#include "YM2149.h"
#include "ymplayer.h"

YM2149 ym2149;

void setup()
{
	ymplayer.setup(&ym2149);
	ymplayer.loadFile("track1.ymd");
	ymplayer.play(true);
}

void loop() {}
~~~~~~~~
\n
*/
void YMPLAYER::loadFile(const char* name)
{
  ymSDfile.close();
  ymTimeStamp = 0;  
  ymSmallFSfile = SmallFS.open(name);
  if (!ymSmallFSfile.valid()) {
    #ifdef DEBUG  
      Serial.println("There is no smallfs File.");
    #endif     
  }
    
  ymSDfile = SD.open(name);    //TODO: Should check for SD filesystem init first.
  if (!ymSDfile){
    #ifdef DEBUG  
      Serial.println("There is no SD File.");
    #endif     
  }
}

/*!
@par Description
Play a ym file after it has been loaded.
@par Syntax
ymplayer.play(play)
@param play Will play the file if set to 1 or true. 

@par Example
~~~~~~~~{.c}
#include "YM2149.h"
#include "ymplayer.h"

YM2149 ym2149;

void setup()
{
	ymplayer.setup(&ym2149);
	ymplayer.loadFile("track1.ymd");
	ymplayer.play(true);
}

void loop() {}
~~~~~~~~
\n
*/
void YMPLAYER::play(boolean play)
{
  boolean smallfscheck = ymSmallFSfile.valid();
  if (!ymSDfile && !smallfscheck) {
    play = false;
    #ifdef DEBUG
      Serial.println("Error: No SD or SmallFS File Available");
    #endif  
    return; 
  }
  
  if (smallfscheck)
   fileType = SmallFSType;
  if (ymSDfile)
   fileType = SDFSType; 
  
  playing = play;
  if (play == true)
    resetYMFlag = 1;
}

void YMPLAYER::volume(int volume)
{
  volumeAdjust = volume;
}

void YMPLAYER::audiofill()
{
	int r;
	ymframe f;
	while (!YMaudioBuffer.isFull()) {
          switch (fileType) {  
            case SmallFSType:
              r = ymSmallFSfile.read(&f.regval[0], 16);
              break;
            case SDFSType:
              r = ymSDfile.read(&f.regval[0], 16);
              break;     
            default:
              return;
              break;       
          }                    
            
  		if (r==0) {
                    switch (fileType) {  
                      case SmallFSType:
  			      ymSmallFSfile.seek(0,SEEK_SET);
  			      ymSmallFSfile.read(&f.regval[0], 16); 
                        break;
                      case SDFSType:
  			      ymSDfile.seek(0);
  			      ymSDfile.read(&f.regval[0], 16); 
                        break;     
                      default:
                        return;
                        break;       
                    }    
  		}
                //Adjust the volume level
//                f.regval[8] = constrain(f.regval[8] - volumeAdjust, 0, 15);
//                f.regval[9] = constrain(f.regval[9] - volumeAdjust, 0, 15);
//                f.regval[10] = constrain(f.regval[10] - volumeAdjust, 0, 15);  
                //Adjust the volume level individually
                f.regval[8] = constrain((f.regval[8] - (15-ym2149->V1.getVolume())), 0, 15);
                f.regval[9] = constrain((f.regval[9] - (15-ym2149->V2.getVolume())), 0, 15);
                f.regval[10] = constrain((f.regval[10] - (15-ym2149->V3.getVolume())), 0, 15);                                 
  		YMaudioBuffer.push(f);
	}
}

boolean YMPLAYER::getPlaying()
{
  return playing;
}

void YMPLAYER::zpu_interrupt()
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
                  YM2149::writeData(i, f.regval[i]);
			//YM2149REG(i) = f.regval[i];
		}
	}
        else{ 
          if (resetYMFlag == 1){
            ym2149->reset();
            ym2149->V1.setVolume(11);
            ym2149->V2.setVolume(11);
            ym2149->V3.setVolume(11);
            resetYMFlag = 0;
            ymTimeStamp = 1;
          }
        }
  }
}

