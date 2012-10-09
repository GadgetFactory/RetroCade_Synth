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
		r = ymaudiofile.read(&f.regval[0], 16);
		if (r==0) {
			ymaudiofile.seek(0,SEEK_SET);
			ymaudiofile.read(&f.regval[0], 16);
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

//    struct ymframe {                //TODO: This should go in ymplayer.h
//    	unsigned char regval[14];
//    };  

//CircularBuffer<ymframe,2> YMaudioBuffer;
//
//SmallFSFile ymaudiofile;

//void setup()
//	if (SmallFS.begin()<0) {
//		Serial.println("No SmalLFS found.");
//		//while(1);
//	}

//	ymaudiofile = SmallFS.open("track1.ymdat");
//	modfile = SmallFS.open("track1.mod");
//        mod = pt_init_smallfs(modfile);
        
//	TMR0CTL = 0;
//	TMR0CNT = 0;
//	TMR0CMP = ((CLK_FREQ/2) / FREQ )- 1;
//	TMR0CTL = _BV(TCTLENA)|_BV(TCTLCCM)|_BV(TCTLDIR)|
//		_BV(TCTLCP0) | _BV(TCTLIEN);
//
//	INTRMASK = BIT(INTRLINE_TIMER0); // Enable Timer0 interrupt
//
//	INTRCTL=1;        

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
//}

//void audiofill()
//{
//	// Check audio
//	int r;
//
//	ymframe f;
//	while (!YMaudioBuffer.isFull()) {
//		r = ymaudiofile.read(&f.regval[0], 16);
//		if (r==0) {
//			ymaudiofile.seek(0,SEEK_SET);
//			ymaudiofile.read(&f.regval[0], 16);
//		}
//		YMaudioBuffer.push(f);
//	}
//}

//static unsigned int timerTicks = 0;

//void _zpu_interrupt()
//{
//  counter++;
//  if ( counter == 340 ) {
//        counter = 1;
//        ymTimeStamp++;
//	// Play YM file
//	if (YMaudioBuffer.hasData()) {
//		int i;
//		ymframe f = YMaudioBuffer.pop();
//		for (i=0;i<14; i++) {
//			YM2149REG(i) = f.regval[i];
//		}
//	}
//        else{ 
//          if (resetYMFlag == 1){
//            //reset_ym2149();
//            resetYMFlag = 0;
//            ymTimeStamp = 1;
//          }
//        }
//  }
//	// Play mod file
//	if (audioBuffer.hasData()) {
//		unsigned v = audioBuffer.pop();
//		SIGMADELTADATA = v;
//	} else {
//          //SIGMADELTADATA=0x80008000;
//          underruns++;
//	}
//	TMR0CTL &= ~(BIT(TCTLIF));
//}
//
