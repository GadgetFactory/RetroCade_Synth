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

void RETROCADE::setupMegaWing()
{
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
}
