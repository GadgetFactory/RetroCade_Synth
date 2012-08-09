/*
  Gadget Factory
  RetroCade MegaWing Test Plan
 
 This example will let you play the YM2149 audio chip integrated into the ZPUino Soft Processor using a MIDI instrument such as a keyboard or guitar. 
 
 To learn more about this example visit its home page at the Papilio Audio Wiki:
 http://audio.gadgetfactory.net/index.php?n=Main.YM2149MIDISynthesizer
 
 To learn more about the Papilio and Gadget Factory products visit:
 http://www.GadgetFactory.net
 
 Hardware:
 * Connect a RetroCade MegaWing
 
 *******IMPORTANT********************
 Be sure to load the ZPUino "RetroCade" variant to the Papilio's SPI Flash before loading this sketch.

 created 2012
 
 This example code is Creative Commons Attribution.
 */


#include "YM2149.h"
#include "SmallFS.h"
#include "MIDI.h"                   //Had to change MIDI.h to use Serial1 instead of Serial
#include <LiquidCrystal.h>
#include "cbuffer.h"
//#include <SD.h>

#define DEBUG(x...)

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
//#define SELPIN WING_C_5    //Selection Pin
//#define DATAOUT WING_C_4   //MOSI
//#define DATAIN  WING_C_3   //MISO
//#define SPICLOCK WING_C_2  //Clock

//SD Card
//#define CSPIN  WING_C_13
//#define SDIPIN WING_C_12
//#define SCKPIN WING_C_11
//#define SDOPIN WING_C_10

#define HAVE_YM

struct ymframe {
	unsigned char regval[16];
};

CircularBuffer<ymframe,2> YMaudioBuffer;

SmallFSFile ymaudiofile;

//Sd2Card card;



static unsigned int timerTicks = 0;

int adc;
//boolean firstPress = TRUE;

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
 
  //Setup SD Card
//  outputPinForFunction( SDIPIN, IOPIN_USPI_MOSI );
//  pinModePPS(SDIPIN,HIGH);
//  pinMode(SDIPIN,OUTPUT);
//
//  outputPinForFunction( SCKPIN, IOPIN_USPI_SCK);
//  pinModePPS(SCKPIN,HIGH);
//  pinMode(SCKPIN,OUTPUT);
//
//  pinModePPS(CSPIN,LOW);
//  pinMode(CSPIN,OUTPUT);
//
//  inputPinForFunction( SDOPIN, IOPIN_USPI_MISO );
//  pinMode(SDOPIN,INPUT);   

//  //For SD Card
//  USPICTL=BIT(SPICP1)|BIT(SPICP0)|BIT(SPICPOL)|BIT(SPISRE)|BIT(SPIEN)|BIT(SPIBLOCK); 
//
//  digitalWrite(CSPIN,LOW);
//  int i;
//  for (i=0;i<51200;i++)
//	USPIDATA=0xff;
//
//  digitalWrite(CSPIN,HIGH);
//
//  for (i=0;i<51200;i++)
//	USPIDATA=0xff;
//
//  if (!card.init(SPI_HALF_SPEED, CSPIN)) {
//    Serial.println("initialization failed. Things to check:");
//    Serial.println("* is a card is inserted?");
//    Serial.println("* Is your wiring correct?");
//    Serial.println("* did you change the chipSelect pin to match your shield or module?");
//          //return;
//    } else {
//      Serial.println("Wiring is correct and a card is present."); 
//    }  
  
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
 lcd.setCursor(0,0);
 lcd.print("RetroCade Test"); 
// lcd.setCursor(0,1); 
// lcd.print("Press to Start");  
// delay(2000);
// lcd.clear();
 
  //Setup Joystick
  pinMode(JSELECT, INPUT); 
  pinMode(JUP, INPUT); 
  pinMode(JDOWN, INPUT); 
  pinMode(JLEFT, INPUT); 
  pinMode(JRIGHT, INPUT); 
  

  
  //Setup SPI ADC
   //set pin modes
   pinMode(SELPIN, OUTPUT);
   pinMode(DATAOUT, OUTPUT);
   pinMode(DATAIN, INPUT);
   pinMode(SPICLOCK, OUTPUT);
   //disable device to start with
   digitalWrite(SELPIN,HIGH);
   digitalWrite(DATAOUT,LOW);
   digitalWrite(SPICLOCK,LOW); 
   
	if (SmallFS.begin()<0) {
		Serial.println("No SmalLFS found, aborting.");
		//while(1);
	}

	ymaudiofile = SmallFS.open("audio.dat");
   
	INTRMASK = BIT(INTRLINE_TIMER0); // Enable Timer0 interrupt

	INTRCTL=1;

	// Start timer, 50Hz (prescaler 64)
	TMR0CMP = (CLK_FREQ/(50U*64))-1;
	TMR0CNT = 0x0;
	TMR0CTL = BIT(TCTLENA)|BIT(TCTLCCM)|BIT(TCTLDIR)|BIT(TCTLCP2)|BIT(TCTLCP0)|BIT(TCTLIEN);

        //For SD Card
        //USPICTL=BIT(SPICP1)|BIT(SPICP0)|BIT(SPICPOL)|BIT(SPISRE)|BIT(SPIEN)|BIT(SPIBLOCK);  
}

void loop(){
  // Call MIDI.read the fastest you can for real-time performance.
  MIDI.read();
  audiofill();

//  lcd.home();
//  if (digitalRead(JUP) == 0) {
//    lcd.setCursor(0,0);
//    lcd.print("U");}
//  if (digitalRead(JDOWN) == 0) {
//    lcd.setCursor(1,0);
//    lcd.print("D");}
//  if (digitalRead(JLEFT) == 0) {
//    lcd.setCursor(2,0);
//    lcd.print("L");}
//  if (digitalRead(JRIGHT) == 0) {
//    lcd.setCursor(3,0);
//    lcd.print("R"); }
//  if (digitalRead(JSELECT) == 0) {
//    lcd.setCursor(4,0);
//    lcd.print("S");
//    setup();
//    //ffiv();
//  }    
//  
//  for (int i=1; i<9; i++){
//    adc = read_adc(i);
//    lcd.setCursor(i,1);
//    if (adc == 4064) {
//      lcd.print(i);
//    }
//    else {
//      lcd.print(" ");
//    }
////    Serial.print(i-1);
////    Serial.print(": ");
////    Serial.print(adc, DEC); 
////    Serial.print(": ");
//  }
//      delay(100);
//      Serial.println("");
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

int read_adc(int channel){
  int adcvalue = 0;
  int temp;
  byte commandbits = 00000000; //command bits - dont care (2), chn (3), dont care (3)

  //allow channel selection
  commandbits=((channel)<<3);
    //Serial.print("CommandBits: ");
  //Serial.print(commandbits, BIN);
  //Serial.print(" ");

  digitalWrite(SELPIN,LOW); //Select adc
  // setup bits to be written
  for (int i=7; i>=3; i--){
    temp = commandbits&1<<i;
    digitalWrite(DATAOUT,temp);
//    Serial.print(temp, DEC);
//    Serial.println(" ");
    //cycle clock
    digitalWrite(SPICLOCK,HIGH);
    digitalWrite(SPICLOCK,LOW);
  }


  //read bits from adc
  for (int i=11; i>=0; i--){
    adcvalue+=digitalRead(DATAIN)<<i;
    //cycle clock
    digitalWrite(SPICLOCK,HIGH);
    digitalWrite(SPICLOCK,LOW);
  }
  
  digitalWrite(SPICLOCK,HIGH);    //ignores 2 null bits
  digitalWrite(SPICLOCK,LOW);
  digitalWrite(SPICLOCK,HIGH);
  digitalWrite(SPICLOCK,LOW);
  
  digitalWrite(SELPIN, HIGH); //turn off device
  return adcvalue;
}

void _zpu_interrupt()
{
	// Play
	if (YMaudioBuffer.hasData()) {
		int i;
		ymframe f = YMaudioBuffer.pop();
		for (i=0;i<16; i++) {
			YM2149REG(i) = f.regval[i];
		}
	}
	timerTicks++;

	TMR0CTL &= ~(BIT(TCTLIF));
}

void audiofill()
{
	// Check audio
	int r;

#ifdef HAVE_YM
	ymframe f;
	while (!YMaudioBuffer.isFull()) {
		r = ymaudiofile.read(&f.regval[0], 16);
		if (r==0) {
			ymaudiofile.seek(0,SEEK_SET);
			ymaudiofile.read(&f.regval[0], 16);
		}
		YMaudioBuffer.push(f);
	}
#endif
}



void set_chA(byte note) {
   MIDI.sendNoteOn(note,127,1); 
}
