/*!
 *  @file		RetroCade.h
 *  Project		RetroCade Library
 *	@brief		RetroCade Library for the ZPUino
 *	Version		1.0
 *  @author		Jack Gassett 
 *	@date		9/11/12
 *  License		Creative Commons Atribution
 */
 #include "RetroCade.h"
 #include "LiquidCrystal.h"
 #include "binary.h"
 #include "spaceinvaders.h"
 
#define FREQ 17000          //Freq for modplayer 
#define TIMEOUTMAX 7000    //Timeout for joystick 
#define INVADERSTIMERMAX 2000    //Timeout for LCD 
 
LiquidCrystal lcd(WING_B_10, WING_B_9, WING_B_8, WING_B_7, WING_B_6, WING_B_5, WING_B_4);

void RETROCADE::setupMegaWing()
{
  activeChannel = 0;
  activeInstrument = 0;
  timeout = TIMEOUTMAX;
  smallFs = false;
  sdFs = false;
  invadersCurLoc = 0;
  invadersCurSeg = 1;
  invadersTimer = INVADERSTIMERMAX;

  SIGMADELTACTL=0x3;
  //Move the audio output to the appropriate pins on the Papilio Hardware
  pinMode(AUDIO_J1_L,OUTPUT);
  digitalWrite(AUDIO_J1_L,HIGH);
  //outputPinForFunction(AUDIO_J1_L, IOPIN_SIGMADELTA0);
  outputPinForFunction(AUDIO_J1_L, 8);
  pinModePPS(AUDIO_J1_L, HIGH);

  pinMode(AUDIO_J1_R,OUTPUT);
  digitalWrite(AUDIO_J1_R,HIGH);
  outputPinForFunction(AUDIO_J1_R, 8);
  //outputPinForFunction(AUDIO_J1_R, IOPIN_SIGMADELTA1);
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
 
   //Start SmallFS
  if (SmallFS.begin()<0) {
	Serial.println("No SmalLFS found.");
  }
  else{
     Serial.println("SmallFS Started.");
     smallFs = true; 
  }
 
  //Setup SD Card
  USPICTL=BIT(SPICP1)|BIT(SPICPOL)|BIT(SPISRE)|BIT(SPIEN)|BIT(SPIBLOCK);
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
  
  Serial.println("Starting SD Card");

  if (!SD.begin(CSPIN)) {
	Serial.println("init failed!");
	Serial.println(SD.errorCode());
        sdFs = false;
  } else {
  	Serial.println("done.");
	sdFs = true;
  }
  
  //Setup Joystick
  pinMode(JSELECT, INPUT); 
  pinMode(JUP, INPUT); 
  pinMode(JDOWN, INPUT); 
  pinMode(JLEFT, INPUT); 
  pinMode(JRIGHT, INPUT);  
 
  //Setup LCD
  pinMode(WING_C_14, OUTPUT);     //Set contrast to GND
  digitalWrite(WING_C_14, LOW);   //Set contrast to GND    
 // set up the LCD's number of columns and rows:
 lcd.begin(16,2);
 // clear the LCD screen:
 lcd.clear();
// lcd.setCursor(0,1);
// lcd.print("CH:   RetroCade");
// lcd.setCursor(0,0);
// lcd.print("Instrument:");

 //Setup timer for YM and mod players
  TMR0CTL = 0;
  TMR0CNT = 0;
  TMR0CMP = ((CLK_FREQ/2) / FREQ )- 1;
  TMR0CTL = _BV(TCTLENA)|_BV(TCTLCCM)|_BV(TCTLDIR)|
  	_BV(TCTLCP0) | _BV(TCTLIEN);
  INTRMASK = BIT(INTRLINE_TIMER0); // Enable Timer0 interrupt
  INTRCTL=1;    

}

void RETROCADE::setTimeout()
{
  if (timeout!=0)
    timeout--;  
  if (invadersTimer!=0)
    invadersTimer--;      
}

byte RETROCADE::getActiveChannel()
{
  return activeChannel;
}

void RETROCADE::handleJoystick()
{
  if (timeout==0) {
    if (!digitalRead(JUP)) {
      if (activeChannel<6)
        activeChannel++;
        timeout = TIMEOUTMAX;
        lcd.setCursor(3,1);
        lcd.print(activeChannel);
    } else if (!digitalRead(JDOWN)) {
      if (activeChannel!=0)
        activeChannel--;
        timeout = TIMEOUTMAX;
        lcd.setCursor(3,1);
        if (activeChannel == 0)
          lcd.print(" ");
        else        
          lcd.print(activeChannel);
    } else if (!digitalRead(JRIGHT)) {
      if (activeInstrument<9)
        activeInstrument++;
        timeout = TIMEOUTMAX;
        lcd.setCursor(11,0);
        lcd.print(activeInstrument);
    } else if (!digitalRead(JLEFT)) {
      if (activeInstrument!=0)
        activeInstrument--;
        timeout = TIMEOUTMAX;
        lcd.setCursor(11,0);
        if (activeInstrument == 0)
          lcd.print(" ");
        else        
          lcd.print(activeInstrument);   
    }
  }
}

void RETROCADE::printDirectory(File dir, int numTabs) {
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       //Serial.println("**nomorefiles**");
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
   }
}

boolean RETROCADE::smallFsActive() {
  return smallFs;
}

boolean RETROCADE::sdFsActive() {
  return sdFs;
}

void RETROCADE::spaceInvadersLCD(){
/* 
  This Space Invaders alien crawling along the RetroCade LCD was created by JO3RI and adapted to the RetroCade by Jack Gassett.
  Please take a look at JO3RI's website for more cool hacks!
  
  Demo by JO3RI
  
  http://www.JO3RI.be/arduino/arduino-projects/lcd-16x2-demo
 
*/  
  
  if (invadersTimer == 0)
  {
       if (invadersCurSeg == 10){
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("RetroCade Synth");
        invadersCurSeg = 1;
        if (invadersCurLoc < 13)
          invadersCurLoc+=2;
        else
          invadersCurLoc = 0;         
       }
       else
         invadersCurSeg++;
       invadersTimer = INVADERSTIMERMAX;
       int a=invadersCurLoc;
       int b=a+1;
       int c=a+2;
       int d=a+3;
       int e=a+4;   
      switch (invadersCurSeg) {  
        case 1:
           lcd.createChar(0, charSP12a);
           lcd.createChar(1, charSP7a);
           lcd.createChar(2, charSP2a);
           lcd.createChar(3, charSP0);
           lcd.setCursor(a,0);
           lcd.write((byte)0);
           lcd.setCursor(b,0);
           lcd.write((byte)1);
           lcd.setCursor(c,0);
           lcd.write((byte)2);
           lcd.setCursor(d,0);
           lcd.write((byte)3);
           lcd.setCursor(e,0);
           lcd.write((byte)3);
          break;
        case 2:
           lcd.createChar(0, charSP0);
           lcd.createChar(1, charSP8b);
           lcd.createChar(2, charSP3b);
           lcd.createChar(3, charSP0);
           lcd.setCursor(a,0);
           lcd.write((byte)0);
           lcd.setCursor(b,0);
           lcd.write((byte)1);
           lcd.setCursor(c,0);
           lcd.write((byte)2);
           lcd.setCursor(d,0);
           lcd.write((byte)3);
           lcd.setCursor(e,0);
           lcd.write((byte)3);
          break;
        case 3:
           lcd.createChar(0, charSP0);
           lcd.createChar(1, charSP9a);
           lcd.createChar(2, charSP4a);
           lcd.setCursor(a,0);
           lcd.write((byte)0);
           lcd.setCursor(b,0);
           lcd.write((byte)1);
           lcd.setCursor(c,0);
           lcd.write((byte)2);
           lcd.setCursor(d,0);
           lcd.write((byte)0);
           lcd.setCursor(e,0);
           lcd.write((byte)0);
          break;
        case 4:
           lcd.createChar(0, charSP0);
           lcd.createChar(1, charSP10b);
           lcd.createChar(2, charSP5b);
           lcd.setCursor(a,0);
           lcd.write((byte)0);
           lcd.setCursor(b,0);
           lcd.write((byte)1);
           lcd.setCursor(c,0);
           lcd.write((byte)2);
           lcd.setCursor(d,0);
           lcd.write((byte)0);
           lcd.setCursor(e,0);
           lcd.write((byte)0);
          break;  
        case 5:
           lcd.createChar(0, charSP0);
           lcd.createChar(1, charSP11a);
           lcd.createChar(2, charSP6a);
           lcd.createChar(3, charSP1a);
           lcd.setCursor(a,0);
           lcd.write((byte)0);
           lcd.setCursor(b,0);
           lcd.write((byte)1);
           lcd.setCursor(c,0);
           lcd.write((byte)2);
           lcd.setCursor(d,0);
           lcd.write((byte)3);
           lcd.setCursor(e,0);
           lcd.write((byte)0);
          break;   
        case 6:
           lcd.createChar(0, charSP0);
           lcd.createChar(1, charSP12b);
           lcd.createChar(2, charSP7b);
           lcd.createChar(3, charSP2b);
           lcd.setCursor(a,0);
           lcd.write((byte)0);
           lcd.setCursor(b,0);
           lcd.write((byte)1);
           lcd.setCursor(c,0);
           lcd.write((byte)2);
           lcd.setCursor(d,0);
           lcd.write((byte)3);
           lcd.setCursor(e,0);
           lcd.write((byte)0);
          break;   
        case 7:
           lcd.createChar(0, charSP0);
           lcd.createChar(1, charSP0);
           lcd.createChar(2, charSP8a);
           lcd.createChar(3, charSP3a);
           lcd.setCursor(a,0);
           lcd.write((byte)0);
           lcd.setCursor(b,0);
           lcd.write((byte)1);
           lcd.setCursor(c,0);
           lcd.write((byte)2);
           lcd.setCursor(d,0);
           lcd.write((byte)3);
           lcd.setCursor(e,0);
           lcd.write((byte)0);
          break;   
        case 8:
           lcd.createChar(0, charSP0);
           lcd.createChar(2, charSP9b);
           lcd.createChar(3, charSP4b);
           lcd.setCursor(a,0);
           lcd.write((byte)0);
           lcd.setCursor(b,0);
           lcd.write((byte)0);
           lcd.setCursor(c,0);
           lcd.write((byte)2);
           lcd.setCursor(d,0);
           lcd.write((byte)3);
           lcd.setCursor(e,0);
           lcd.write((byte)0);
          break;   
        case 9:
           lcd.createChar(0, charSP0);
           lcd.createChar(2, charSP10a);
           lcd.createChar(3, charSP5a);
           lcd.setCursor(a,0);
           lcd.write((byte)0);
           lcd.setCursor(b,0);
           lcd.write((byte)0);
           lcd.setCursor(c,0);
           lcd.write((byte)2);
           lcd.setCursor(d,0);
           lcd.write((byte)3);
           lcd.setCursor(e,0);
           lcd.write((byte)0);
          break;  
        case 10:
           lcd.createChar(0, charSP0);
           lcd.createChar(2, charSP11b);
           lcd.createChar(3, charSP6b);
           lcd.createChar(4, charSP1b);
           lcd.setCursor(a,0);
           lcd.write((byte)0);
           lcd.setCursor(b,0);
           lcd.write((byte)0);
           lcd.setCursor(c,0);
           lcd.write((byte)2);
           lcd.setCursor(d,0);
           lcd.write((byte)3);
           lcd.setCursor(e,0);
           lcd.write((byte)4);
          break;        
        default:
          lcd.print("oops");
          break;  
      }  
  }
}
