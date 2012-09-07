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

#define AUDIO_J1_L WING_B_1
#define AUDIO_J1_R WING_B_0

#define AUDIO_J2_L WING_B_3
#define AUDIO_J2_R WING_B_2

#define SERIAL1RXPIN WING_C_1 
#define SERIAL1TXPIN WING_C_0

//Joystick
#define JSELECT WING_B_15
#define JDOWN WING_B_14
#define JUP WING_B_13
#define JRIGHT WING_B_12
#define JLEFT WING_B_11

//For SPI ADC1
#define SELPIN WING_C_9    //Selection Pin
#define DATAOUT WING_C_8   //MOSI
#define DATAIN  WING_C_7   //MISO
#define SPICLOCK WING_C_6  //Clock

//For SPI ADC2
#define SELPIN2 WING_C_5    //Selection Pin
#define DATAOUT2 WING_C_4   //MOSI
#define DATAIN2  WING_C_3   //MISO
#define SPICLOCK2 WING_C_2  //Clock

//SD Card
#define CSPIN  WING_C_13
#define SDIPIN WING_C_12
#define SCKPIN WING_C_11
#define SDOPIN WING_C_10

YM2149 ym2149;

byte nrpn;
byte transpose_v1;
byte transpose_v2;
byte transpose_v3;
int pitch, playYM, playMOD, playTrack, counter, ymTimeStamp, resetYMFlag;
int freqV1, freqV2, freqV3, freqV4, freqV5, freqV6;

void setup(){

  
#ifdef DEBUG 
  Serial.begin(9600);
#endif 

  //Move the audio output to the appropriate pins on the Papilio Hardware
  pinMode(AUDIO_J1_L,OUTPUT);
  digitalWrite(AUDIO_J1_L,HIGH);
  outputPinForFunction(AUDIO_J1_L, 8);
  pinModePPS(AUDIO_J1_L, HIGH);

  pinMode(AUDIO_J1_R,OUTPUT);
  digitalWrite(AUDIO_J1_R,HIGH);
  outputPinForFunction(AUDIO_J1_R, 8);
  pinModePPS(AUDIO_J1_R, HIGH);
  
  pinMode(AUDIO_J2_L,OUTPUT);
  digitalWrite(AUDIO_J2_L,HIGH);
  outputPinForFunction(AUDIO_J2_L, 8);
  pinModePPS(AUDIO_J2_L, HIGH);

  pinMode(AUDIO_J2_R,OUTPUT);
  digitalWrite(AUDIO_J2_R,HIGH);
  outputPinForFunction(AUDIO_J2_R, 8);
  pinModePPS(AUDIO_J2_R, HIGH);  
  
  //Move the second serial port pin to where we need it, this is for MIDI input.
  pinMode(SERIAL1RXPIN,INPUT);
  inputPinForFunction(SERIAL1RXPIN, 1);
  pinMode(SERIAL1TXPIN,OUTPUT);
  //digitalWrite(SERIAL1TXPIN,HIGH);
  outputPinForFunction(SERIAL1TXPIN, 6);
  pinModePPS(SERIAL1TXPIN, HIGH);  
 
  //Setup SD Card
  outputPinForFunction( SDIPIN, IOPIN_USPI_MOSI );
  pinModePPS(SDIPIN,HIGH);
  pinMode(SDIPIN,OUTPUT);

  outputPinForFunction( SCKPIN, IOPIN_USPI_SCK);
  pinModePPS(SCKPIN,HIGH);
  pinMode(SCKPIN,OUTPUT);

  pinModePPS(CSPIN,LOW);
  pinMode(CSPIN,OUTPUT);

  inputPinForFunction( SDOPIN, IOPIN_USPI_MISO );
  pinMode(SDOPIN,INPUT);   
  
   ///Setup the pin modes for the YM2149 and SID
   ym2149.setVolume(1, 0x7f);
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
  MIDI.setHandleNoteOff(HandleNoteOff);  // Put only the name of the function  
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
  switch (channel){
    case 1:
      ym2149.setNote(1,pitch,true); 
      break;         
    default:
      break;             
  }
}

void HandleNoteOff(byte channel, byte pitch, byte velocity) { 
   #ifdef DEBUG 
    Serial.println("In NoteOff");
   #endif  
  switch(channel){
      case 1:
        ym2149.setNote(1,pitch,false);
        break;
      default:
        return;      
  }  
}

