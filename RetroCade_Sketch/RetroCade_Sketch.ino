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
//#include "ptplay.h"
//#include "SmallFS.h"
//#include "cbuffer.h"

#undef DO_CHECKS
#undef DEBUG

//Instantiate the objects we will be using.
RETROCADE retrocade;
YM2149 ym2149;
SID sid;

//typedef void (F) ();

void setup(){
  #ifdef DEBUG
    Serial.begin(9600);
  #endif

  //Setup pins for RetroCade MegaWing
  retrocade.setupMegaWing();
  
  ///Setup volume to max levels
  ym2149.V1.setVolume(15);
  ym2149.V2.setVolume(15);
  ym2149.V3.setVolume(15);   
  sid.setVolume(15);

  sid.V1.setInstrument(0,0,15,0,0,0,0,1,0); //Calliope
// sid.V1.setInstrument(0,5,5,0,1,0,0,0,0); //Drum
  sid.V2.setInstrument(12,0,12,0,0,0,1,0,0); //Accordian
// sid.V1.setInstrument(0,9,2,1,0,0,1,0,0); //Guitar
  sid.V3.setInstrument(0,9,0,0,0,1,0,0,512); //Harpsicord
// sid.V1.setInstrument(0,9,9,0,0,1,0,0,2048); //Organ
// sid.V1.setInstrument(8,9,4,1,0,1,0,0,512); //Trumpet
// sid.V1.setInstrument(0,9,0,0,0,0,0,1,0); //Xylophone
// sid.V1.setInstrument(9,4,4,0,0,0,0,1,0); //Flute
   
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
 //SIDVoice *sidVoicePTR;
  
  //Setup the voice for each channel
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
  
//  //Setup the voice for each channel
//  switch (channel) {  //TODO figure more efficient way to do this. Want to avoid case statements.
//    case 1:
//      sidVoicePTR = &sid.V1;
//      break;
//    case 2:
//      sidVoicePTR = &sid.V2;
//      break;
//    case 3:
//      sidVoicePTR = &sid.V3;
//      break;
//    default:
//      return;
//      break;       
//  } 
  
//  //Handle the Control Changes for SID
//  switch (number) {  //TODO figure more efficient way to do this. Want to avoid case statements.
//    case 2:
//        sidVoicePTR->setTriangle(value);
//      break;
//    case 3:
//      sidVoicePTR->setSawtooth(value);
//      break;
//    case 4:
//      sidVoicePTR->setSquare(value);
//      break;
//    case 5:
//      sidVoicePTR->setNoise(value);
//      break;    
//    case 6:
//      sidVoicePTR->setRingMod(value);
//      break; 
//    case 7:
//      sidVoicePTR->setSync(value);
//      break;    
//    case 71:
//      sidVoicePTR->setEnvelopeDecay(value/8);
//      break;    
//    case 74:
//      sidVoicePTR->setEnvelopeAttack(value/8);
//      break;        
//    case 75:
//      sidVoicePTR->setPWLo(value << 1);
//      break;    
//    case 76:
//      sidVoicePTR->setPWHi(value);
//      break;    
//    case 91:
//      sidVoicePTR->setEnvelopeSustain(value/8);
//      break;    
//    case 93:
//      sidVoicePTR->setEnvelopeRelease(value/8);
//      break;               
//    default:
//      return;
//      break;       
//  }     
}

void HandleNoteOn(byte channel, byte pitch, byte velocity) {
 #ifdef DEBUG
  Serial.print("Note Received: ");
  Serial.println(pitch);
  Serial.print("Channel Received: ");
  Serial.println(channel);
 #endif
// sid.V1.setNote(pitch, 1);
// ym2149.setNote(1,pitch,true);
 
 
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
}

void HandleNoteOff(byte channel, byte pitch, byte velocity) {
   #ifdef DEBUG
    Serial.println("In NoteOff");
   #endif
   //sid.V1.setNote(pitch, 0);
   //ym2149.setNote(1,pitch,false);
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
}
