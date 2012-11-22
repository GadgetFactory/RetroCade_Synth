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

ChangeLog:
11/1/2012	Version .3
	-FlowStone Dashboard
		-Changed from SynthMaker to FlowStone
		-Added the ability to control the RetroCade without a MIDI output, can control over a Com Port instead.
		-Generated an EXE version of the Dashboard. VST will be generated when FlowStone 3.0 is released in November.
		-Zoom Controls added to Dashboard
			-Pressing "w" toggles full screen.
			-While in full screen pressing "z" fills the screen.
			-Pressing "x" returns to normal size.
	-RetroCade Sketch
		-Control the RetroCade from a COM port or MIDI In
		-Play YMdat and Mod files from SD card or SmallFS filesystem.
			-Place a file name track[1-3].mod or track[1-3].ymd onto the SD card to override the files found in smallFS. Must power cycle the board for the files to be found.
			-YM files must be converted to ymd files using the process and tools detailed here: (http://audio.gadgetfactory.net/index.php?n=Main.YM2149Flash)
			-Mod files play out of RamFS filesystem which is located entirely in the 8MB SDRAM.
	-Merged RetroCade Bit File: Shortcut to load RetroCade-.01d-zpuino-1.0-PapilioPro-S6LX9-RetroCade-1.04.bit with the Papilio Loader
		-Changes from RetroCade Sketch are merged into this bit file.
		-Smallfs version of MOD and YM files are merged into this bit file.
		
10/8/2012	Version .2
	-SynthMaker Dashboard
		-Added Midi In and Midi Out Selectors to the front panel.
		-Added SID Volume
		-Fixed CC#'s for all Presets
		-Renamed Presets from Hoppa to User

9/26/2012	Version .1
	-ZPUino IDE: Shortcut to open RetroCade Sketch with the ZPUino IDE.
	-Merged RetroCade Bit File: Shortcut to load RetroCade-.01b-zpuino-1.0-PapilioPro-S6LX9-RetroCade-1.04.bit with the Papilio Loader
		-MOD files, YM2149, SID chips play on both left and right channel now.
		-Code runs from SDRAM providing 8MB of code space!
		-RetroCade Sketch is merged into this bit file.
		-Smallfs version of MOD and YM files are merged into this bit file.
	-Papilio Loader
	-SynthMaker Dashboard: The dashboard requires that Synthmaker be installed. MIDI selections are not moved to the main page yet.

*/

  HardwareSerial Serial1(11);   //This is to define Serial1 for the ZPUino.

#include "RetroCade.h"
#include "SID.h"
#include "YM2149.h"
#include "MIDI.h" //Be sure to change MIDI.h to user Serial1 instead of Serial
//#include "modplayer.h"
//#include "ymplayer.h"
#include "SmallFS.h"
#include <LiquidCrystal.h>
#include <SD.h>

File root;

//#include "cbuffer.h"

#undef DO_CHECKS
//#define DEBUG

//Instantiate the objects we will be using.
RETROCADE retrocade;
YM2149 ym2149;
SID sid;
//YMPLAYER ymplayer;
//MODPLAYER modplayer;

void setup(){
//  #ifdef DEBUG
//    Serial.begin(115200);
//  #endif
  Serial.begin(115200);
  Serial1.begin(31250);

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
  
  retrocade.modplayer.setup();
  retrocade.ymplayer.setup(&ym2149); 

}


void _zpu_interrupt()
{
  retrocade.modplayer.zpu_interrupt();
  retrocade.ymplayer.zpu_interrupt(); 
  retrocade.setTimeout();
//  TMR0CTL &= ~(BIT(TCTLIF));
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
      retrocade.modplayer.play(!value);
      retrocade.ymplayer.play(!value);
      break;
    case 9:
      if (value == 127)
        retrocade.modplayer.loadFile("track1.mod");
      retrocade.modplayer.play(value);
      break;
    case 10:
      if (value == 127)
        retrocade.ymplayer.loadFile("track1.ymd");
      retrocade.ymplayer.play(value);
      break;      
    case 11:
      if (value == 127)
        retrocade.modplayer.loadFile("track2.mod");
      retrocade.modplayer.play(value);
      break;
    case 12:
      if (value == 127)
        retrocade.ymplayer.loadFile("track2.ymd");
      retrocade.ymplayer.play(value);
      break;        
    case 13:
      if (value == 127)
        retrocade.modplayer.loadFile("track3.mod");
      retrocade.modplayer.play(value);
      break;  
    case 14:
      if (value == 127)
        retrocade.ymplayer.loadFile("track3.ymd");
      retrocade.ymplayer.play(value);
      break;        
    case 84:
      retrocade.modplayer.volume(value <<1);
      break;    
    case 86:
      sid.setVolume(value/8);
      break;        
    default:
      return;
      break;       
  }    

}

void HandleNoteOn(byte channel, byte pitch, byte velocity) {
  #ifdef DEBUG
    Serial.print("Note Received: ");
    Serial.println(pitch);
    Serial.print("Channel Received: ");
    Serial.println(channel);
  #endif     
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
}

void HandleNoteOff(byte channel, byte pitch, byte velocity) {
  #ifdef DEBUG
    Serial.println("In NoteOff");
  #endif   
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
}

void loop(){
  // Call MIDI.read the fastest you can for real-time performance.
  MIDI.read(&Serial);
  MIDI.read(&Serial1);  
  if (retrocade.modplayer.getPlaying() == 1)
    retrocade.modplayer.audiofill();
  else
    retrocade.spaceInvadersLCD();          //Don't move the space invader when a mod file is playing
  if (retrocade.ymplayer.getPlaying() == 1)
    retrocade.ymplayer.audiofill(); 
  retrocade.handleJoystick();     
}


