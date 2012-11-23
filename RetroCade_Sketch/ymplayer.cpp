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

YMPLAYER::YMPLAYER(){
  
}
 
void YMPLAYER::setup(YM2149* ym){    
  //underruns = 0;
  timerTicks = 0;
  ymTimeStamp = 0;
  resetYMFlag = 0;
  counter = 0;
  playing = false;
  ym2149 = ym;

}

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
  //mod->mastervolume = volume;
}

void YMPLAYER::audiofill()
{
	int r;
	ymframe f;
	while (!YMaudioBuffer.isFull()) {
          switch (fileType) {  //TODO figure more efficient way to do this. Want to avoid case statements.
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
                    switch (fileType) {  //TODO figure more efficient way to do this. Want to avoid case statements.
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
            ym2149->V1.setVolume(15);
            ym2149->V2.setVolume(15);
            ym2149->V3.setVolume(15);
            resetYMFlag = 0;
            ymTimeStamp = 1;
          }
        }
  }
}

