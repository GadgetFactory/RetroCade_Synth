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


//#include "SID.h"
#include "YM2149.h"
#include "MIDI.h"    //Be sure to change MIDI.h to user Serial1 instead of Serial
//#include "ptplay.h"
//#include "SmallFS.h"
//#include "cbuffer.h"


#undef DO_CHECKS
#define DEBUG

//Connected to Audio Wing on AH
#define AUDIOPIN WING_C_7
//#define AUDIO_RIGHT WING_A_15
#define AUDIO_LEFT WING_C_5

#define SERIAL1RXPIN WING_A_7  //MIDI Wing connected to CH


//YM2149 ym2149;

byte nrpn;
byte transpose_v1;
byte transpose_v2;
byte transpose_v3;
int pitch, playYM, playMOD, playTrack, counter, ymTimeStamp, resetYMFlag;
int freqV1, freqV2, freqV3, freqV4, freqV5, freqV6;

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
  
   ///Setup the pin modes for the YM2149 and SID
//   reset_sid();
//   
//   setupSID();
   //setupYM2149();
   //ym2149.reset();
   
  // Initiate MIDI communications, listen to all channels
  MIDI.begin(MIDI_CHANNEL_OMNI);
 
  // Connect the HandleNoteOn function to the library, so it is called upon reception of a NoteOn.
  MIDI.setHandleNoteOn(HandleNoteOn);  // Put only the name of the function
  MIDI.setHandleControlChange(HandleControlChange);  // Put only the name of the function  
  //MIDI.setHandleNoteOff(HandleNoteOff);  // Put only the name of the function  
//  MIDI.setHandleProgramChange(HandleProgramChange);  // Put only the name of the function    
//  MIDI.setHandlePitchBend(HandlePitchBend);  // Put only the name of the function    
  
}

void loop(){
  // Call MIDI.read the fastest you can for real-time performance.
  MIDI.read();
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
}

void HandleNoteOn(byte channel, byte pitch, byte velocity) { 
 #ifdef DEBUG
  Serial.print("Note Received: ");
  Serial.println(pitch);
  Serial.print("Channel Received: ");
  Serial.println(channel);  
 #endif 
}

