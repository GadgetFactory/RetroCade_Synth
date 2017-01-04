/*
 Gadget Factory
 RetroCade Synth
 To learn more about the Papilio Schematic Library please visit http://learn.gadgetfactory.net

 Hardware:
   Use a Papilio Pro with the RetroCade MegaWing attached to it.
   
 Special Application Notes:
    Click on the link below to open the RetroCade Dashboard (Windows Only)
      sketchdir://Dashboards/FlowStone/RetroCade_Synth_DashBoard.exe

    Click on the link below to load the latest production RetroCade bit file with all of the sample audio files
      sketchdir://circuit/RetroCade-1.3-zpuino-2.0-PapilioPro-S6LX9-RetroCade-1.3.bit
      sketchdir://circuit/RetroCade-1.3-lcd-contrast-fix-zpuino-2.0-PapilioPro-S6LX9-RetroCade-1.3.bit   //Use this is you have problems with contrast on your LCD
      
    To use the older 1.1 version of the firmware (You will not be able to upload this sketch with the 1.0 version of ZPUino)
      sketchdir://circuit/RetroCade-1.1-zpuino-1.0-PapilioPro-S6LX9-RetroCade-1.1.bit
      
    NOTE: If you want all of the production samples available when you load this sketch to the RetroCade first load the production bit file to your board and then hold down the left shift key before uploading. 
    This will cause the program to be loaded directly to SDRAM and will not wipe out the samples in SPI Flash. Or, hit CTRL-K to open the sketch directory and rename smallfs-production to smallfs.
    It will take a long time to upload all of the music files every time you make a code change. The current smallfs folder has very small files in it to speed up code changes.
    
 created 2014
 by Jack Gassett
 http://www.gadgetfactory.net
 
 This example code is in the public domain.
License: GPL

ChangeLog:
1/4/2017      Version 1.3.1
        -Fix for some LCDs that have contrast issues.

4/21/2015      Version 1.3
        -Updated MIDI library to better handle NoteOffs.
        -Moved to ZPUino 2.0 with a DesignLab schematic.

1/29/2014      Version 1.2
        -Moved to Papilio Schematic Library and drew up a schematic of the RetroCade system.
        -Added Analog mode to the LCD.
        -Made joystick interaction for smallFS more intuitive. Cannot do the same for SD Card access without a lot of rework...

9/26/2013       Version 1.1
        -Added SID Analog Filters!!!!!!!!  Thanks to Alvie for writing the VHDL code.
        -Added SidPlayer library to process SID files from smallFS and SD Cards. Thanks to Alvie for porting to the ZPUino.

1/9/2013        Version 1.02
        -NoteOff fix from Lee O'D

11/23/2012      Version 1.01
        -RetroCade Sketch
                -YM Player Volume Control
                
11/23/2012      Version 1.0
        -RetroCade Sketch
                -SID Volume CC added.
                -Crawling Space Invaders added to the Welcome LCD screen.
                -Added ability to browse and play YMD and MOD files from the LCD screen.
                -Ring Mod and Sync CC's added.
                -Added YM2149 Noise Frequency.
                -SID V1 Instrument select from LCD.
                -SID Pitch Control
                -HiHat and Kick split keyboard from mod files on MIDI channel 7

11/1/2012	Version .3
	-FlowStone Dashboard
		-Changed from SynthMaker to FlowStone
		-Added the ability to control the RetroCade without a MIDI output, can control over a Com Port instead.
		-Generated an EXE version of the Dashboard. VST will be generated when FlowStone 3.0 is released in November.
		-Zoom Controls added to Dashboard
			-Pressing Up Arrow toggles full screen.
			-While in full screen pressing Left Arrow fills the screen.
			-Pressing Down Arrow returns to normal size.
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

  HardwareSerial Serial1(WishboneSlot(11));   //This is to define Serial1 for the ZPUino.

#include "RetroCade.h"
#include "SID.h"
#include "YM2149.h"
#include "MIDI.h" //Be sure to change MIDI.h to user Serial1 instead of Serial
#include "SmallFS.h"
#include <LiquidCrystal.h>
#include <SD.h>
//#include "VolumeController.h"
#include "ymplayer.h"
#include "modplayer.h"
#include "ramFS.h"
#include "cbuffer.h"
#include "sidplayer.h"
#include "SPIADC.h"
#include "SPI.h" 
#include <Timer.h>


byte lastpitch[8];
File root;

int sidplayercounter = 0;

//#define DEBUG

//Instantiate the objects we will be using.
RETROCADE retrocade;
//YM2149 ym2149;
//SID sid;

struct MySettings : public midi::DefaultSettings
{
    static const long BaudRate = 115200;
};

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);
#ifndef DEBUG
MIDI_CREATE_CUSTOM_INSTANCE(HardwareSerial, Serial, midiB, MySettings);
#endif   

void setup(){
  int input;
  Serial.begin(115200);
  //Serial1.begin(31250);

//  for (input=0; input<8; input++) {
//      VolumeController.set(input, 255, 255);
//  }
  //Setup pins for RetroCade MegaWing
  retrocade.setupMegaWing(); 
  
  //Set Wishbone slots for audio chips
  retrocade.sid.setup(14);
  retrocade.ym2149.setup(13);
   
  // Initiate MIDI communications, listen to all channels
  MIDI.begin(MIDI_CHANNEL_OMNI);
  #ifndef DEBUG
    midiB.begin(MIDI_CHANNEL_OMNI);
  #endif    
 
  // Connect the HandleNoteOn function to the library, so it is called upon reception of a NoteOn.
  MIDI.setHandleNoteOn(HandleNoteOn); // Put only the name of the function
  MIDI.setHandleControlChange(HandleControlChange); // Put only the name of the function
  MIDI.setHandleNoteOff(HandleNoteOff); // Put only the name of the function
// MIDI.setHandleProgramChange(HandleProgramChange); // Put only the name of the function
  MIDI.setHandlePitchBend(HandlePitchBend); // Put only the name of the function
  
  #ifndef DEBUG
    midiB.setHandleNoteOn(HandleNoteOn); // Put only the name of the function
    midiB.setHandleControlChange(HandleControlChange); // Put only the name of the function
    midiB.setHandleNoteOff(HandleNoteOff); // Put only the name of the function
    midiB.setHandlePitchBend(HandlePitchBend); // Put only the name of the function  
  #endif  
  
  retrocade.modplayer.setup(5);
  retrocade.ymplayer.setup(&retrocade.ym2149,13); 
  retrocade.sidplayer.setup(14);
  
  //retrocade.sidplayer.loadFile("music.sid");
  //retrocade.sidplayer.play(true);
  
  //analog.begin(CS(WING_C_9),WISHBONESLOT(8),ADCBITS(SPIADC_8BIT));

 //Setup timer for YM and mod players, this generates an interrupt at 1700hz
  Timers.begin();
  int r = Timers.periodicHz(17000, timer, 0, 1);
  if (r<0) {
      Serial.println("Fatal error!");
  }    
  
  ///Give some volume
  retrocade.ym2149.V1.setVolume(11);
  retrocade.ym2149.V2.setVolume(11);
  retrocade.ym2149.V3.setVolume(11);   
  retrocade.sid.setVolume(15);

  //Select an instrument for each SID Voice.
  retrocade.sid.V1.setInstrument("Calliope",0,0,15,0,0,0,0,1,2048); //Calliope
  retrocade.sid.V2.setInstrument("Accordian",12,0,12,0,0,0,1,0,2048); //Accordian
  retrocade.sid.V3.setInstrument("Harpsicord",0,9,0,0,0,1,0,0,512); //Harpsicord

}


bool timer(void*)
{
  sidplayercounter++;
  retrocade.modplayer.zpu_interrupt();
  retrocade.ymplayer.zpu_interrupt(); 
  if (sidplayercounter == 340) {
    retrocade.sidplayer.zpu_interrupt(); 
    sidplayercounter = 1;
  }
  retrocade.setTimeout();
  return true;
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
  switch (channel) {  
    case 1:
      retrocade.sid.V1.handleCC(number, value);
      break;
    case 2:
      retrocade.sid.V2.handleCC(number, value);
      break;
    case 3:
      retrocade.sid.V3.handleCC(number, value);
      break;
    case 4:
      retrocade.ym2149.V1.handleCC(number, value);
      break;
    case 5:
      retrocade.ym2149.V2.handleCC(number, value);
      break;
    case 6:
      retrocade.ym2149.V3.handleCC(number, value);
      break;      
    default:
      //return;
      break;       
  }  
 
  // These are active no matter which channel is selected.
  switch (number) {  
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
      retrocade.sid.setVolume(value/8);
      break;     
    case 117:
      retrocade.modplayer.play(!value);
      retrocade.ymplayer.play(!value);
      break;      
    default:
      return;
      break;       
  }    

}

void HandlePitchBend(byte channel, int bend) { 
  #ifdef DEBUG
  Serial.print("Pitch Bend Received: ");
  Serial.println(channel);  
  Serial.println(bend); 
  #endif  
  byte activeChannel = retrocade.getActiveChannel();
  if ( activeChannel != 0 )
    channel = activeChannel;
  int ymBend = bend/10;    
  switch (channel){
    case 1:
      if (retrocade.sid.V1.getCurrentFreq() + bend > 388)
        retrocade.sid.V1.setFreq(retrocade.sid.V1.getCurrentFreq()+bend);
      break;
    case 2:
      if (retrocade.sid.V2.getCurrentFreq() + bend > 388)
        retrocade.sid.V2.setFreq(retrocade.sid.V2.getCurrentFreq()+bend);
      break;
    case 3:
      if (retrocade.sid.V3.getCurrentFreq() + bend > 388)
        retrocade.sid.V3.setFreq(retrocade.sid.V3.getCurrentFreq()+bend);
      break;
     case 4:
      if (retrocade.ym2149.V1.getCurrentFreq() - ymBend > 100)
        retrocade.ym2149.V1.setFreq(retrocade.ym2149.V1.getCurrentFreq()-ymBend); 
      break;
     case 5:
      if (retrocade.ym2149.V2.getCurrentFreq() - ymBend > 100)
        retrocade.ym2149.V2.setFreq(retrocade.ym2149.V2.getCurrentFreq()-ymBend);  
      break;
     case 6:
      if (retrocade.ym2149.V3.getCurrentFreq() - ymBend > 100)
        retrocade.ym2149.V3.setFreq(retrocade.ym2149.V3.getCurrentFreq()-ymBend);  
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
  lastpitch[channel-1]=pitch;
  switch (channel){
    case 1:
      retrocade.sid.V1.setNote(pitch, 1);
      break;
    case 2:
      retrocade.sid.V2.setNote(pitch, 1);
      break;
    case 3:
      retrocade.sid.V3.setNote(pitch, 1);
      break;
     case 4:
      retrocade.ym2149.V1.setNote(pitch, 1);
      break;
     case 5:
      retrocade.ym2149.V2.setNote(pitch, 1);
      break;
     case 6:
      retrocade.ym2149.V3.setNote(pitch, 1);
      break;
     case 7:
      if (pitch > 60) {
          retrocade.modplayer.loadFile("hihat.mod");
          retrocade.modplayer.play(true);
      }
      else {
          retrocade.modplayer.loadFile("kick.mod");
          retrocade.modplayer.play(true);      
      }
     break;         
     default:
       return;
     break;
  }
}

void HandleNoteOff(byte channel, byte pitch, byte velocity) {
  #ifdef DEBUG
    Serial.println("In NoteOff");
    Serial.println(pitch);
  #endif   
  byte activeChannel = retrocade.getActiveChannel();
  if ( activeChannel != 0 )
    channel = activeChannel;  
  if (lastpitch[channel-1]!=pitch) { return; }
  switch(channel){
      case 1:
        retrocade.sid.V1.setNote(pitch, 0);
        break;
      case 2:
        retrocade.sid.V2.setNote(pitch, 0);
        break;
      case 3:
        retrocade.sid.V3.setNote(pitch, 0);
        break;
      case 4:
        retrocade.ym2149.V1.setNote(128, 1);
        break;
      case 5:
        retrocade.ym2149.V2.setNote(128, 1);
        break;
      case 6:
        retrocade.ym2149.V3.setNote(128, 1);
        break;
      case 7:
        retrocade.modplayer.play(false);
        break;        
      default:
        return;
      break; 
  } 
}

void loop(){
  // Call MIDI.read the fastest you can for real-time performance.
  MIDI.read();
  #ifndef DEBUG
    midiB.read();
  #endif    
  if (retrocade.modplayer.getPlaying() == 1)
    retrocade.modplayer.audiofill();
  else
  {
    retrocade.spaceInvadersLCD();          //Don't move the space invader when a mod file is playing
     
  }
  if (retrocade.ymplayer.getPlaying() == 1)
    retrocade.ymplayer.audiofill();
  if (retrocade.sidplayer.getPlaying() == 1)
    retrocade.sidplayer.audiofill(); 
  retrocade.handleJoystick();
  retrocade.updateAnalog();  
}

