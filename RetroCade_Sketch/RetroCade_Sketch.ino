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

  HardwareSerial Serial1(11);   //This is to define Serial1 for the ZPUino.

#include "RetroCade.h"
#include "SID.h"
#include "YM2149.h"
#include "MIDI.h" //Be sure to change MIDI.h to user Serial1 instead of Serial
#include "modplayer.h"
#include "ymplayer.h"
#include "SmallFS.h"
#include <LiquidCrystal.h>
#include <SD.h>

File root;

//#include "cbuffer.h"

#undef DO_CHECKS
#define DEBUG

//Instantiate the objects we will be using.
RETROCADE retrocade;
YM2149 ym2149;
SID sid;
YMPLAYER ymplayer;
MODPLAYER modplayer;

void setup(){
  #ifdef DEBUG
    Serial.begin(115200);
  #endif
  Serial1.begin(31250);

  //Setup pins for RetroCade MegaWing
  retrocade.setupMegaWing();
  
  modplayer.setup();
  ymplayer.setup(&ym2149);  
  
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
		SD.ls();  
        }
    root = SD.open("/");
  
  retrocade.printDirectory(root, 0);
}


void _zpu_interrupt()
{
  modplayer._zpu_interrupt();
  ymplayer._zpu_interrupt(); 
  retrocade.setTimeout();
}

void HandleControlChange(byte channel, byte number, byte value) {
  
 #ifdef DEBUG
  Serial.print("Change Control Channel: ");
  Serial.println(channel);
  Serial.print("Change Control Number: ");
  Serial.println(number);
  Serial.print("Change Control Value: ");
  Serial.println(value);
 #endif    
  
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
      //return;
      break;       
  }  
 
  switch (number) {  //TODO figure more efficient way to do this. Want to avoid case statements.
    case 117:
      modplayer.play(!value);
      ymplayer.play(!value);
      break;
    case 9:
      modplayer.loadFile("track1.mod");
      modplayer.play(value);
      break;
    case 10:
      ymplayer.loadFile("track1.ymd");
      ymplayer.play(value);
      break;      
    case 11:
      modplayer.loadFile("track2.mod");
      modplayer.play(value);
      break;
    case 12:
      ymplayer.loadFile("track2.ymd");
      ymplayer.play(value);
      break;        
    case 13:
      modplayer.loadFile("track3.mod");
      modplayer.play(value);
      break;  
    case 14:
      ymplayer.loadFile("track3.ymd");
      ymplayer.play(value);
      break;        
    case 84:
      modplayer.volume(value <<1);
      break;        
    default:
      return;
      break;       
  }    

}

void HandleNoteOn(byte channel, byte pitch, byte velocity) {
  byte activeChannel = retrocade.getActiveChannel();
  if ( activeChannel != 0 )
    channel = activeChannel;
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
  byte activeChannel = retrocade.getActiveChannel();
  if ( activeChannel != 0 )
    channel = activeChannel;  
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

void loop(){
  // Call MIDI.read the fastest you can for real-time performance.
  MIDI.read(&Serial);
  MIDI.read(&Serial1);  
  if (modplayer.getPlaying() == 1)
    modplayer.audiofill();
  if (ymplayer.getPlaying() == 1)
    ymplayer.audiofill(); 
  retrocade.handleJoystick();     
}


