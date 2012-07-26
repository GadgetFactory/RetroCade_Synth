/*
  Gadget Factory
  Papilio One YM2149 YM MIDI Synthesizer Example
 
 This example will let you play the YM2149 audio chip integrated into the ZPUino Soft Processor using a MIDI instrument such as a keyboard or guitar. 
 
 To learn more about this example visit its home page at the Papilio Audio Wiki:
 http://audio.gadgetfactory.net/index.php?n=Main.YM2149MIDISynthesizer
 
 To learn more about the Papilio and Gadget Factory products visit:
 http://www.GadgetFactory.net
 
 Hardware:
 * Connect a MIDI-Audio Wing to CH
 * Connect an Audio Wing to AL
 * User an Arcade MegaWing
 
 *******IMPORTANT********************
 Be sure to load the ZPUino "Apollo" variant to the Papilio's SPI Flash before loading this sketch.

 created 2012
 by Jack Gassett from Zetris code examples and YM2149 code examples at:
 http://kalshagar.wikispaces.com/Arduino+and+a+YMZ294

 http://www.gadgetfactory.net
 
 This example code is Creative Commons Attribution.
 */


#include "YM2149.h"
#include "MIDI.h"                   //Had to change MIDI.h to use Serial1 instead of Serial
#include <LiquidCrystal.h>

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

//For SPI ADC
//#define SELPIN A12 //Selection Pin
//#define DATAOUT A14//MOSI
//#define DATAIN  A13//MISO
//#define SPICLOCK A15 //Clock

LiquidCrystal lcd(WING_B_10, WING_B_9, WING_B_8, WING_B_7, WING_B_6, WING_B_5, WING_B_4);		//Connect LCD Wing to AH. Change all instances of AH to your desired Wing Slot.

void setup(){
  Serial.begin(9600);

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
  
  //Setup the pin modes for the YMZ294
  setupYM2149();
   
  //don't forget to set the volume or you wont hear a thing...
  setVolumeYM2149(0x7f); 

  // Initiate MIDI communications, listen to all channels
  MIDI.begin(MIDI_CHANNEL_OMNI); 
  MIDI.turnThruOn(); 
 
  // Connect the HandleNoteOn function to the library, so it is called upon reception of a NoteOn.
  MIDI.setHandleNoteOn(HandleNoteOn);  // Put only the name of the function
  MIDI.setHandleNoteOff(HandleNoteOff);  // Put only the name of the function  
  
  //Setup LCD
  pinMode(WING_C_14, OUTPUT);     //Set contrast to GND
  digitalWrite(WING_C_14, LOW);   //Set contrast to GND    
 // set up the LCD's number of columns and rows:
 lcd.begin(16,2);
 // clear the LCD screen:
 lcd.clear();
 lcd.setCursor(0,1);
 lcd.print("PAPILIO LCD DEMO"); 
 
  //Setup Joystick
  pinMode(JSELECT, INPUT); 
  pinMode(JUP, INPUT); 
  pinMode(JDOWN, INPUT); 
  pinMode(JLEFT, INPUT); 
  pinMode(JRIGHT, INPUT); 
  

  
  //Setup SPI ADC
// //set pin modes
// pinMode(SELPIN, OUTPUT);
// pinMode(DATAOUT, OUTPUT);
// pinMode(DATAIN, INPUT);
// pinMode(SPICLOCK, OUTPUT);
// //disable device to start with
// digitalWrite(SELPIN,HIGH);
// digitalWrite(DATAOUT,LOW);
// digitalWrite(SPICLOCK,LOW); 
}

void loop(){
  // Call MIDI.read the fastest you can for real-time performance.
  MIDI.read();
  lcd.home();
  if (digitalRead(JSELECT) == 0) {
    lcd.print("Joystick Select");
    ffiv();
  }
  if (digitalRead(JUP) == 0) 
    lcd.print("Joystick UP    ");
  if (digitalRead(JDOWN) == 0) 
    lcd.print("Joystick DOWN  ");
  if (digitalRead(JLEFT) == 0) 
    lcd.print("Joystick LEFT  ");
  if (digitalRead(JRIGHT) == 0) 
    lcd.print("Joystick RIGHT ");   
  
}

void HandleNoteOn(byte channel, byte pitch, byte velocity) { 

  // Do whatever you want when you receive a Note On.
  Serial.print("Note Received: ");
  Serial.println(pitch);
  set_ch(ADDR_FREQ_A,pitch);
  set_ch(ADDR_FREQ_B,pitch+4);
  set_ch(ADDR_FREQ_C,pitch+7);
  //MIDI.sendNoteOn(53,55,1);
  // Try to keep your callbacks short (no delays ect) as the contrary would slow down the loop()
  // and have a bad impact on real-time performance.
}

void HandleNoteOff(byte channel, byte pitch, byte velocity) { 
    set_ch(ADDR_FREQ_A,128);
    set_ch(ADDR_FREQ_B,128);
    set_ch(ADDR_FREQ_C,128);
//    Serial.print("In NoteOff");
}

void ffiv(){
  set_chA(60);
//  delay(500);
//  set_chA(62);
//  delay(500);
//  set_chA(64);
//  delay(500);
//  set_chA(65);
//  delay(500);
//  set_chA(64);
//  delay(500);
//  set_chA(62);
//  delay(500);
//  set_chA(60);
//  delay(500);
//  set_chA(128);
//  delay(500);
//  
//  set_chA(64);
//  delay(500);
//  set_chA(65);
//  delay(500);
//  set_chA(67);
//  delay(500);
//  set_chA(69);
//  delay(500);
//  set_chA(67);
//  delay(500);
//  set_chA(65);
//  delay(500);
//  set_chA(64);
//  delay(500);
//  set_chA(128);
//  delay(500);
//  
//  set_chA(60);
//  delay(500);
//  set_chA(128);
//  delay(500);
//  set_chA(60);
//  delay(500);
//  set_chA(128);
//  delay(500);
//  set_chA(60);
//  delay(500);
//  set_chA(128);
//  delay(500);
//  set_chA(60);
//  delay(500);
//  set_chA(128);
//  delay(500);
//  
//  set_chA(60);
//  delay(128);
//  set_chA(128);
//  delay(128);
//  set_chA(60);
//  delay(128);
//  set_chA(128);
//  delay(128);
//  set_chA(62);
//  delay(128);
//  set_chA(128);
//  delay(128);
//  set_chA(62);
//  delay(128);
//  set_chA(128);
//  delay(128);
//  set_chA(64);
//  delay(128);
//  set_chA(128);
//  delay(128);
//  set_chA(64);
//  delay(128);
//  set_chA(128);
//  delay(128);
//  set_chA(65);
//  delay(128);
//  set_chA(128);
//  delay(128);
//  set_chA(65);
//  delay(128);
//  set_chA(128);
//  delay(128);
//  set_chA(64);
//  delay(250);
//  set_chA(128);
//  delay(250);
//  set_chA(62);
//  delay(250);
//  set_chA(128);
//  delay(250);
//  set_chA(60);
//  delay(250);
//  set_chA(128);
//  delay(1000);
}

void set_chA(byte note) {
   MIDI.sendNoteOn(note,127,1); 
}
