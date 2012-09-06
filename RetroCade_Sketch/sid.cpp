
//void reset_sid(){
//  //Filter
//  SIDREG(SID_ADDR_FILTER_FC_LOW) = 0;
//  SIDREG(SID_ADDR_FILTER_FC_HI) = 0;
//  SIDREG(SID_ADDR_FILTER_RES_FILT) = 0;
//  SIDREG(SID_ADDR_FILTER_MODE_VOL) = 0xF;
//  
//  //Voice1
//  SIDREG(SID_ADDR_V1_FREQ_LOW) = 0;
//  SIDREG(SID_ADDR_V1_FREQ_HI) = 0; 
//  SIDREG(SID_ADDR_V1_PW_LOW) = 0;
//  SIDREG(SID_ADDR_V1_PW_HI) = 0; 
//  SID_REG_V1_ATTACK_DECAY.ATTACK = 0;
//  SID_REG_V1_ATTACK_DECAY.DECAY = 0; 
//  SID_REG_V1_SUSTAIN_RELEASE.SUSTAIN = 0;
//  SID_REG_V1_SUSTAIN_RELEASE.RELEASE = 0;
//  SID_REG_V1_CONTROLREG.NOISE_WAVE = 0;
//  SID_REG_V1_CONTROLREG.SQUARE_WAVE = 0;
//  SID_REG_V1_CONTROLREG.SAWTOOTH_WAVE = 0;
//  SID_REG_V1_CONTROLREG.TRIANGLE_WAVE = 0; 
//  SID_REG_V1_CONTROLREG.TEST = 0;  
//  SID_REG_V1_CONTROLREG.RING_MOD = 0;
//  SID_REG_V1_CONTROLREG.SYNC = 0;
//  SID_REG_V1_CONTROLREG.GATE = 0;
//  SIDREG(SID_ADDR_V1_ATTACK_DECAY) = *(char*)&SID_REG_V1_ATTACK_DECAY;
//  SIDREG(SID_ADDR_V1_SUSTAIN_RELEASE) = *(char*)&SID_REG_V1_SUSTAIN_RELEASE; 
//  SIDREG(SID_ADDR_V1_CONTROLREG) = *(char*)&SID_REG_V1_CONTROLREG;
//  
//  //Voice2
//  SIDREG(SID_ADDR_V2_FREQ_LOW) = 0;
//  SIDREG(SID_ADDR_V2_FREQ_HI) = 0; 
//  SIDREG(SID_ADDR_V2_PW_LOW) = 0;
//  SIDREG(SID_ADDR_V2_PW_HI) = 0; 
//  SID_REG_V2_ATTACK_DECAY.ATTACK = 0;
//  SID_REG_V2_ATTACK_DECAY.DECAY = 0; 
//  SID_REG_V2_SUSTAIN_RELEASE.SUSTAIN = 0;
//  SID_REG_V2_SUSTAIN_RELEASE.RELEASE = 0;
//  SID_REG_V2_CONTROLREG.NOISE_WAVE = 0;
//  SID_REG_V2_CONTROLREG.SQUARE_WAVE = 0;
//  SID_REG_V2_CONTROLREG.SAWTOOTH_WAVE = 0;
//  SID_REG_V2_CONTROLREG.TRIANGLE_WAVE = 0; 
//  SID_REG_V2_CONTROLREG.TEST = 0;  
//  SID_REG_V2_CONTROLREG.RING_MOD = 0;
//  SID_REG_V2_CONTROLREG.SYNC = 0;
//  SID_REG_V2_CONTROLREG.GATE = 0;
//  SIDREG(SID_ADDR_V2_ATTACK_DECAY) = *(char*)&SID_REG_V2_ATTACK_DECAY;
//  SIDREG(SID_ADDR_V2_SUSTAIN_RELEASE) = *(char*)&SID_REG_V2_SUSTAIN_RELEASE; 
//  SIDREG(SID_ADDR_V2_CONTROLREG) = *(char*)&SID_REG_V2_CONTROLREG;
//
//  //Voice3
//  SIDREG(SID_ADDR_V3_FREQ_LOW) = 0;
//  SIDREG(SID_ADDR_V3_FREQ_HI) = 0; 
//  SIDREG(SID_ADDR_V3_PW_LOW) = 0;
//  SIDREG(SID_ADDR_V3_PW_HI) = 0; 
//  SID_REG_V3_ATTACK_DECAY.ATTACK = 0;
//  SID_REG_V3_ATTACK_DECAY.DECAY = 0; 
//  SID_REG_V3_SUSTAIN_RELEASE.SUSTAIN = 0;
//  SID_REG_V3_SUSTAIN_RELEASE.RELEASE = 0;
//  SID_REG_V3_CONTROLREG.NOISE_WAVE = 0;
//  SID_REG_V3_CONTROLREG.SQUARE_WAVE = 0;
//  SID_REG_V3_CONTROLREG.SAWTOOTH_WAVE = 0;
//  SID_REG_V3_CONTROLREG.TRIANGLE_WAVE = 0; 
//  SID_REG_V3_CONTROLREG.TEST = 0;  
//  SID_REG_V3_CONTROLREG.RING_MOD = 0;
//  SID_REG_V3_CONTROLREG.SYNC = 0;
//  SID_REG_V3_CONTROLREG.GATE = 0;
//  SIDREG(SID_ADDR_V3_ATTACK_DECAY) = *(char*)&SID_REG_V3_ATTACK_DECAY;
//  SIDREG(SID_ADDR_V3_SUSTAIN_RELEASE) = *(char*)&SID_REG_V3_SUSTAIN_RELEASE; 
//  SIDREG(SID_ADDR_V3_CONTROLREG) = *(char*)&SID_REG_V3_CONTROLREG;  
// 
//}
//
////makes the setup for the SID component
//void setupSID(){
////waveform
////
////    0 = triangle
////    1 = sawtooth
////    2 = pulse (square)
////    3 = noise
////    4 = ring modulation   
//  
////Envelope                                               Wave-
////Number    Instrument  Attack  Decay  Sustain  Release  form   Width
////
////0         Piano       0       9      0        0        2      1536
////1         Accordion   12      0      12       0        1
////2         Calliope    0       0      15       0        0
////3         Drum        0       5      5        0        3
////4         Flute       9       4      4        0        0
////5         Guitar      0       9      2        1        1
////6         Harpsicord  0       9      0        0        2      512
////7         Organ       0       9      9        0        2      2048
////8         Trumpet     8       9      4        1        2      512
////9         Xylophone   0       9      0        0        0  
//  
//  
//  //Set ADSR Attack=0 Decay=9 Sustain=0 Release=0 For Piano 
//  //Address 5 = 0xA8 for attack and decay
//  //Address 6 = 0xA9 for sustain and release
//  SID_REG_V1_ATTACK_DECAY.ATTACK = 0x0;
//  SID_REG_V1_ATTACK_DECAY.DECAY = 0x09; 
//  SID_REG_V1_SUSTAIN_RELEASE.SUSTAIN = 0x00;
//  SID_REG_V1_SUSTAIN_RELEASE.RELEASE = 0x00;
//  SID_REG_V1_CONTROLREG.SQUARE_WAVE = 1;
//  set_pwm(SID_ADDR_V1_PW_LOW, 1536);  
//  
//  //Set ADSR Attack=0 Decay=9 Sustain=0 Release=0 For Piano 
//  //Address 5 = 0xA8 for attack and decay
//  //Address 6 = 0xA9 for sustain and release
//  SID_REG_V2_ATTACK_DECAY.ATTACK = 0x0;
//  SID_REG_V2_ATTACK_DECAY.DECAY = 0x09; 
//  SID_REG_V2_SUSTAIN_RELEASE.SUSTAIN = 0x00;
//  SID_REG_V2_SUSTAIN_RELEASE.RELEASE = 0x00;
//  SID_REG_V2_CONTROLREG.SQUARE_WAVE = 1;
//  set_pwm(SID_ADDR_V2_PW_LOW, 1536);  
//  
//    //Set ADSR Attack=0 Decay=9 Sustain=0 Release=0 For Piano 
//  //Address 5 = 0xA8 for attack and decay
//  //Address 6 = 0xA9 for sustain and release
//  SID_REG_V3_ATTACK_DECAY.ATTACK = 0x0;
//  SID_REG_V3_ATTACK_DECAY.DECAY = 0x09; 
//  SID_REG_V3_SUSTAIN_RELEASE.SUSTAIN = 0x00;
//  SID_REG_V3_SUSTAIN_RELEASE.RELEASE = 0x00;
//  SID_REG_V3_CONTROLREG.SQUARE_WAVE = 1;
//  set_pwm(SID_ADDR_V3_PW_LOW, 1536);  
//
//  SIDREG(SID_ADDR_V1_ATTACK_DECAY) = *(char*)&SID_REG_V1_ATTACK_DECAY;
//  SIDREG(SID_ADDR_V1_SUSTAIN_RELEASE) = *(char*)&SID_REG_V1_SUSTAIN_RELEASE; 
//  SIDREG(SID_ADDR_V1_CONTROLREG) = *(char*)&SID_REG_V1_CONTROLREG;
//
//  SIDREG(SID_ADDR_V2_ATTACK_DECAY) = *(char*)&SID_REG_V2_ATTACK_DECAY;
//  SIDREG(SID_ADDR_V2_SUSTAIN_RELEASE) = *(char*)&SID_REG_V2_SUSTAIN_RELEASE; 
//  SIDREG(SID_ADDR_V2_CONTROLREG) = *(char*)&SID_REG_V2_CONTROLREG;
//
//  SIDREG(SID_ADDR_V3_ATTACK_DECAY) = *(char*)&SID_REG_V3_ATTACK_DECAY;
//  SIDREG(SID_ADDR_V3_SUSTAIN_RELEASE) = *(char*)&SID_REG_V3_SUSTAIN_RELEASE; 
//  SIDREG(SID_ADDR_V3_CONTROLREG) = *(char*)&SID_REG_V3_CONTROLREG;  
//  
//  //Set Volume
//  write_data(0x18, 0xF);
//
//}

