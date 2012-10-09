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
 
LiquidCrystal lcd(WING_B_10, WING_B_9, WING_B_8, WING_B_7, WING_B_6, WING_B_5, WING_B_4);

void RETROCADE::setupMegaWing()
{
  activeChannel = 0;
  activeInstrument = 0;
  timeout = 17000;

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
 lcd.setCursor(0,1);
 lcd.print("CH:   RetroCade");
 lcd.setCursor(0,0);
 lcd.print("Instrument:");
}

void RETROCADE::setTimeout()
{
  if (timeout!=0)
    timeout--;  
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
        timeout = 7000;
        lcd.setCursor(3,1);
        lcd.print(activeChannel);
    } else if (!digitalRead(JDOWN)) {
      if (activeChannel!=0)
        activeChannel--;
        timeout = 7000;
        lcd.setCursor(3,1);
        if (activeChannel == 0)
          lcd.print(" ");
        else        
          lcd.print(activeChannel);
    } else if (!digitalRead(JRIGHT)) {
      if (activeInstrument<9)
        activeInstrument++;
        timeout = 7000;
        lcd.setCursor(11,0);
        lcd.print(activeInstrument);
    } else if (!digitalRead(JLEFT)) {
      if (activeInstrument!=0)
        activeInstrument--;
        timeout = 7000;
        lcd.setCursor(11,0);
        if (activeInstrument == 0)
          lcd.print(" ");
        else        
          lcd.print(activeInstrument);   
    }
  }
}
