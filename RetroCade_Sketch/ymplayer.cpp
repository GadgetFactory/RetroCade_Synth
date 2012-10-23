#include "ymplayer.h"
#include "YM2149.h"

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
//  Serial.println("Press Key to start");
//    while (!Serial.available());
//  Serial.flush();
//  Serial.println("Starting");
  
 	if (SmallFS.begin()<0) {
		Serial.println("No SmalLFS found.");
		//while(1);
	}
	ymaudiofile = SmallFS.open("track1.ymdat");

USPICTL=BIT(SPICP1)|BIT(SPICPOL)|BIT(SPISRE)|BIT(SPIEN)|BIT(SPIBLOCK);  
	int i;
	Serial.println("Starting SD Card");

	digitalWrite(CSPIN,LOW);

	for (i=0;i<51200;i++)
		USPIDATA=0xff;

	digitalWrite(CSPIN,HIGH);

	for (i=0;i<51200;i++)
		USPIDATA=0xff;

	if (!SD.begin(CSPIN)) {
		Serial.println("init failed!");
		Serial.println(SD.errorCode());
	} else {
		Serial.println("done.");
		//SD.ls();  
        }
    ymSDfile = SD.open("TRACK1~1.YMD");

        //mod = pt_init_smallfs(modfile);
        
//	TMR0CTL = 0;
//	TMR0CNT = 0;
//	TMR0CMP = ((CLK_FREQ/2) / FREQ )- 1;
//	TMR0CTL = _BV(TCTLENA)|_BV(TCTLCCM)|_BV(TCTLDIR)|
//		_BV(TCTLCP0) | _BV(TCTLIEN);
//
//	INTRMASK = BIT(INTRLINE_TIMER0); // Enable Timer0 interrupt
//
//	INTRCTL=1;   
}

void YMPLAYER::loadFile(const char* name)
{
  ymaudiofile = SmallFS.open(name);
  ymSDfile = SD.open(name);
//  playYM = 1;     
  ymTimeStamp = 0;  
}

void YMPLAYER::play(boolean play)
{
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
	// Check audio
	int r;

	ymframe f;
	while (!YMaudioBuffer.isFull()) {
//		r = ymaudiofile.read(&f.regval[0], 16);
		r = ymSDfile.read(&f.regval[0], 16);
		if (r==0) {
//			ymaudiofile.seek(0,SEEK_SET);
//			ymaudiofile.read(&f.regval[0], 16);
			ymSDfile.seek(SEEK_SET);
			ymSDfile.read(&f.regval[0], 16);
		}
		YMaudioBuffer.push(f);
	}
}

boolean YMPLAYER::getPlaying()
{
  return playing;
}

void YMPLAYER::_zpu_interrupt()
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
            resetYMFlag = 0;
            ymTimeStamp = 1;
          }
        }
  }
}

