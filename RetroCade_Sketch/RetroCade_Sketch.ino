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

#include "RetroCade.h"
#include "SID.h"
#include "YM2149.h"
#include "MIDI.h" //Be sure to change MIDI.h to user Serial1 instead of Serial
#include "modplayer.h"
#include "SmallFS.h"
//#include "cbuffer.h"

#undef DO_CHECKS
#define DEBUG

//Instantiate the objects we will be using.
RETROCADE retrocade;
YM2149 ym2149;
SID sid;
MODPLAYER modplayer;

void setup(){
  #ifdef DEBUG
    Serial.begin(9600);
  #endif

  modplayer.setup();

  //Setup pins for RetroCade MegaWing
  retrocade.setupMegaWing();
  
  ///Set volume to max levels
  ym2149.V1.setVolume(15);
  ym2149.V2.setVolume(15);
  ym2149.V3.setVolume(15);   
  sid.setVolume(15);

  //Select an instrument for each SID Voice.
  sid.V1.setInstrument(0,0,15,0,0,0,0,1,0); //Calliope
  sid.V2.setInstrument(12,0,12,0,0,0,1,0,0); //Accordian
  sid.V3.setInstrument(0,9,0,0,0,1,0,0,512); //Harpsicord
   
  // Initiate MIDI communications, listen to all channels
  MIDI.begin(MIDI_CHANNEL_OMNI);
 
  // Connect the HandleNoteOn function to the library, so it is called upon reception of a NoteOn.
  MIDI.setHandleNoteOn(HandleNoteOn); // Put only the name of the function
  MIDI.setHandleControlChange(HandleControlChange); // Put only the name of the function
  MIDI.setHandleNoteOff(HandleNoteOff); // Put only the name of the function
// MIDI.setHandleProgramChange(HandleProgramChange); // Put only the name of the function
// MIDI.setHandlePitchBend(HandlePitchBend); // Put only the name of the function
  
}

void loop(){
  // Call MIDI.read the fastest you can for real-time performance.
  MIDI.read();
  if (modplayer.getPlaying() == 1)
    modplayer.audiofill();
}

void _zpu_interrupt()
{
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
//	if (modplayer.audioBuffer.hasData()) {
//		unsigned v = modplayer.audioBuffer.pop();
//		SIGMADELTADATA = v;
//	} else {
//          //SIGMADELTADATA=0x80008000;
//          modplayer.underruns++;
//	}
//	TMR0CTL &= ~(BIT(TCTLIF));
modplayer._zpu_interrupt();
}

void HandleControlChange(byte channel, byte number, byte value) {
  //Define which voice responds to each channel
  switch (channel) {  //TODO figure more efficient way to do this. Want to avoid case statements.
    case 1:
      sid.V1.handleCC(number, value);
      break;
    case 2:
      sid.V2.handleCC(number, value);
      break;
    case 3:
      sid.V3.handleCC(number, value);
      break;
    case 4:
      ym2149.V1.handleCC(number, value);
      break;
    case 5:
      ym2149.V2.handleCC(number, value);
      break;
    case 6:
      ym2149.V3.handleCC(number, value);
      break;      
    default:
      return;
      break;       
  }  
 #ifdef DEBUG
  Serial.print("Change Control Channel: ");
  Serial.println(channel);
  Serial.print("Change Control Number: ");
  Serial.println(number);
  Serial.print("Change Control Value: ");
  Serial.println(value);
 #endif  
}

void HandleNoteOn(byte channel, byte pitch, byte velocity) {
  switch (channel){
    case 1:
      sid.V1.setNote(pitch, 1);
      break;
    case 2:
      sid.V2.setNote(pitch, 1);
      break;
    case 3:
      sid.V3.setNote(pitch, 1);
      break;
     case 4:
      ym2149.V1.setNote(pitch, 1);
      break;
     case 5:
      ym2149.V2.setNote(pitch, 1);
      break;
     case 6:
      ym2149.V3.setNote(pitch, 1);
      break;
    default:
      break;
  }
 #ifdef DEBUG
  Serial.print("Note Received: ");
  Serial.println(pitch);
  Serial.print("Channel Received: ");
  Serial.println(channel);
 #endif   
}

void HandleNoteOff(byte channel, byte pitch, byte velocity) {
  switch(channel){
      case 1:
        sid.V1.setNote(pitch, 0);
        break;
      case 2:
        sid.V2.setNote(pitch, 0);
        break;
      case 3:
        sid.V3.setNote(pitch, 0);
        break;
      case 4:
        ym2149.V1.setNote(128, 1);
        break;
      case 5:
        ym2149.V2.setNote(128, 1);
        break;
      case 6:
        ym2149.V3.setNote(128, 1);
        break;
      default:
        return;
  }
   #ifdef DEBUG
    Serial.println("In NoteOff");
   #endif  
}
