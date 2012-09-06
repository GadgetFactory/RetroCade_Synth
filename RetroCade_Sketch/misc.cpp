//#define AUDIOPIN WING_A_15  //Connected to the Arcade MegaWing
//#define AUDIOPIN WING_A_7    //Audio Wing connected to AL. This is for Jack 1
//#define AUDIOPIN WING_C_8    //Midi Audio Wing connected to CH. This is for Jack 1
//#define AUDIOPINEXTRA WING_C_9

//#define AUDIO_RIGHT WING_C_8
//#define AUDIO_LEFT WING_C_9

////Connected to Audio Wing on AH
//#define AUDIOPIN WING_A_15
////#define AUDIO_RIGHT WING_A_15
//#define AUDIO_LEFT WING_A_13

//void write_data(unsigned char address, unsigned char data)
//{
//  //YM2149REG(address) = data;
//  SIDREG(address) = data;
//}

//void set_voice(unsigned char address, int i)
//{
//  write_data(address, sid_midi[i] + pitch);
//  write_data(address+1, (sid_midi[i] >> 8) + pitch);  
//}

//void set_freq(unsigned char address, int freq)
//{
//  write_data(address, freq);
//  write_data(address+1, (freq >> 8));  
//}

//void set_pwm(unsigned char address, int i)
//{
//  write_data(address, i);
//  write_data(address+1, i >> 8);  
//}

//void HandlePitchBend(byte channel, int bend) { 
//  #ifdef DEBUG
//  Serial.print("Pitch Bend Received: ");
//  Serial.println(channel);  
//  Serial.println(bend); 
//  #endif  
//  if (SID_REG_V1_CONTROLREG.RING_MOD == 0){
//    if (freqV1 + bend > 388) 
//     set_freq(SID_ADDR_V1_FREQ_LOW,freqV1+bend); 
//  else 
//    if (freqV3 + bend > 388) 
//     set_freq(SID_ADDR_V3_FREQ_LOW,freqV3+bend); 
//  }
//  if (freqV2 + bend > 388) 
//   set_freq(SID_ADDR_V2_FREQ_LOW,freqV1+bend); 
//  if (freqV3 + bend > 388) 
//   set_freq(SID_ADDR_V3_FREQ_LOW,freqV1+bend);
////  if (freqV4 + bend > 388) 
////   ym_set_freq(YM_ADDR_FREQ_A,freqV1+bend);
////  if (freqV5 + bend > 388) 
////   ym_set_freq(YM_ADDR_FREQ_B,freqV1+bend);
////  if (freqV6 + bend > 388) 
////   ym_set_freq(YM_ADDR_FREQ_C,freqV1+bend);  
//}

//void HandleProgramChange(byte channel, byte number) { 
//  #ifdef DEBUG
//    Serial.print("Program Change Received: ");
//    Serial.print(channel);  
//    Serial.println(number);    
//  #endif 
//  switch(channel){
//    case 1:
//      switch(number){
//          //Voice 1
//          case 0:
//            SID_REG_V1_CONTROLREG.TRIANGLE_WAVE = !SID_REG_V1_CONTROLREG.TRIANGLE_WAVE;
//            break;
//          case 1:
//            SID_REG_V1_CONTROLREG.SAWTOOTH_WAVE = !SID_REG_V1_CONTROLREG.SAWTOOTH_WAVE;
//            break;
//          case 2:
//            SID_REG_V1_CONTROLREG.SQUARE_WAVE = !SID_REG_V1_CONTROLREG.SQUARE_WAVE;
//            break;
//          case 3:
//            SID_REG_V1_CONTROLREG.NOISE_WAVE = !SID_REG_V1_CONTROLREG.NOISE_WAVE;
//            break;   
//          case 4:
//            SID_REG_V1_CONTROLREG.RING_MOD = !SID_REG_V1_CONTROLREG.RING_MOD;
//            break;   
//           case 5:
//            SID_REG_V1_CONTROLREG.SYNC = !SID_REG_V1_CONTROLREG.SYNC;
//            break; 
//         default:
//            return;              
//      }
//      SIDREG(SID_ADDR_V1_CONTROLREG) = *(char*)&SID_REG_V1_CONTROLREG;  
//      break;
//    case 2:
//      switch(number){
//          //Voice 2
//          case 0:
//            SID_REG_V2_CONTROLREG.TRIANGLE_WAVE = !SID_REG_V2_CONTROLREG.TRIANGLE_WAVE;
//            break;
//          case 1:
//            SID_REG_V2_CONTROLREG.SAWTOOTH_WAVE = !SID_REG_V2_CONTROLREG.SAWTOOTH_WAVE;
//            break;
//          case 2:
//            SID_REG_V2_CONTROLREG.SQUARE_WAVE = !SID_REG_V2_CONTROLREG.SQUARE_WAVE;
//            break;
//          case 3:
//            SID_REG_V2_CONTROLREG.NOISE_WAVE = !SID_REG_V2_CONTROLREG.NOISE_WAVE;
//            break;  
//         default:
//            return;               
//      }
//      SIDREG(SID_ADDR_V2_CONTROLREG) = *(char*)&SID_REG_V2_CONTROLREG;  
//      break;    
//    case 3:
//      switch(number){
//          //Voice 3
//          case 0:
//            SID_REG_V3_CONTROLREG.TRIANGLE_WAVE = !SID_REG_V3_CONTROLREG.TRIANGLE_WAVE;
//            break;
//          case 1:
//            SID_REG_V3_CONTROLREG.SAWTOOTH_WAVE = !SID_REG_V3_CONTROLREG.SAWTOOTH_WAVE;
//            break;
//          case 2:
//            SID_REG_V3_CONTROLREG.SQUARE_WAVE = !SID_REG_V3_CONTROLREG.SQUARE_WAVE;
//            break;
//          case 3:
//            SID_REG_V3_CONTROLREG.NOISE_WAVE = !SID_REG_V3_CONTROLREG.NOISE_WAVE;
//            break;  
//          default:
//            return;               
//      }
//      SIDREG(SID_ADDR_V3_CONTROLREG) = *(char*)&SID_REG_V3_CONTROLREG;  
//      break; 
//   default:
//      return;         
//  }
//}


//void HandleControlChange(byte channel, byte number, byte value) { 
//  //byte tmp;
// #ifdef DEBUG 
//  Serial.print("Change Control Channel: ");
//  Serial.println(channel); 
//  Serial.print("Change Control Number: ");  
//  Serial.println(number);  
//  Serial.print("Change Control Value: ");    
//  Serial.println(value); 
// #endif  
  
//  switch(channel) {
//    case 1:
//      switch(number) {
//        //Voice1
//        case 74:
//          SID_REG_V1_ATTACK_DECAY.ATTACK = value/8;
//          break;
//        case 71:
//          SID_REG_V1_ATTACK_DECAY.DECAY = value/8;
//          break;
//        case 91:
//          SID_REG_V1_SUSTAIN_RELEASE.SUSTAIN = value/8;
//          break;      
//        case 93:
//          SID_REG_V1_SUSTAIN_RELEASE.RELEASE = value/8;
//          break; 
//        case 75:  //Voice1 PW Fine
//          SIDREG(SID_ADDR_V1_PW_LOW) = (value << 1);
//          break;      
//        case 76:  //Voice1 PW Coarse
//          SIDREG(SID_ADDR_V1_PW_HI) = (value << 1);
//          break;   
//        case 92:  //Voice3 Ring Mod Fine
//          SIDREG(SID_ADDR_V3_FREQ_LOW) = (value << 1);
//          break;      
//        case 95:  //Voice3 Ring Mod Coarse
//          SIDREG(SID_ADDR_V3_FREQ_HI) = (value << 1);
//          break; 
//        case 2:
//          if (value == 0)
//            SID_REG_V1_CONTROLREG.TRIANGLE_WAVE = 0;
//          else
//            SID_REG_V1_CONTROLREG.TRIANGLE_WAVE = 1;
//          break;
//        case 3:
//          if (value == 0)
//            SID_REG_V1_CONTROLREG.SAWTOOTH_WAVE = 0;
//          else
//            SID_REG_V1_CONTROLREG.SAWTOOTH_WAVE = 1;            
//          break;
//        case 4:
//          if (value == 0)
//            SID_REG_V1_CONTROLREG.SQUARE_WAVE = 0;
//          else
//            SID_REG_V1_CONTROLREG.SQUARE_WAVE = 1;             
//          break;
//        case 5:
//          if (value == 0)
//            SID_REG_V1_CONTROLREG.NOISE_WAVE = 0;
//          else
//            SID_REG_V1_CONTROLREG.NOISE_WAVE = 1;             
//          break;   
//        case 6:
//          if (value == 0)
//            SID_REG_V1_CONTROLREG.RING_MOD = 0;
//          else
//            SID_REG_V1_CONTROLREG.RING_MOD = 1;             
//          break;   
//         case 7:
//           if (value == 0)
//            SID_REG_V1_CONTROLREG.SYNC = 0;
//          else
//            SID_REG_V1_CONTROLREG.SYNC = 1;             
//          break;  
//        default:
//          return;             
//      }      
//      SIDREG(SID_ADDR_V1_ATTACK_DECAY) = *(char*)&SID_REG_V1_ATTACK_DECAY;
//      SIDREG(SID_ADDR_V1_SUSTAIN_RELEASE) = *(char*)&SID_REG_V1_SUSTAIN_RELEASE;
//      SIDREG(SID_ADDR_V1_CONTROLREG) = *(char*)&SID_REG_V1_CONTROLREG;  
//    case 2:
//      switch(number) {
//        //Voice1
//        case 74:
//          SID_REG_V2_ATTACK_DECAY.ATTACK = value/8;
//          break;
//        case 71:
//          SID_REG_V2_ATTACK_DECAY.DECAY = value/8;
//          break;
//        case 91:
//          SID_REG_V2_SUSTAIN_RELEASE.SUSTAIN = value/8;
//          break;      
//        case 93:
//          SID_REG_V2_SUSTAIN_RELEASE.RELEASE = value/8;
//          break; 
//        case 75:  //Voice1 PW Fine
//          SIDREG(SID_ADDR_V2_PW_LOW) = (value << 1);
//          break;      
//        case 76:  //Voice1 PW Coarse
//          SIDREG(SID_ADDR_V2_PW_HI) = (value << 1);
//        break;  
//          if (value == 0)
//            SID_REG_V2_CONTROLREG.TRIANGLE_WAVE = 0;
//          else
//            SID_REG_V2_CONTROLREG.TRIANGLE_WAVE = 1;             
//          break;
//        case 3:
//          if (value == 0)
//            SID_REG_V2_CONTROLREG.SAWTOOTH_WAVE = 0;
//          else
//            SID_REG_V2_CONTROLREG.SAWTOOTH_WAVE = 1;             
//          break;
//        case 4:
//          if (value == 0)
//            SID_REG_V2_CONTROLREG.SQUARE_WAVE = 0;
//          else
//            SID_REG_V2_CONTROLREG.SQUARE_WAVE = 1;             
//          break;
//        case 5:
//          if (value == 0)
//            SID_REG_V2_CONTROLREG.NOISE_WAVE = 0;
//          else
//            SID_REG_V2_CONTROLREG.NOISE_WAVE = 1;             
//          break;   
//        default:
//          return;             
//      }      
//      SIDREG(SID_ADDR_V2_ATTACK_DECAY) = *(char*)&SID_REG_V2_ATTACK_DECAY;
//      SIDREG(SID_ADDR_V2_SUSTAIN_RELEASE) = *(char*)&SID_REG_V2_SUSTAIN_RELEASE;
//      SIDREG(SID_ADDR_V2_CONTROLREG) = *(char*)&SID_REG_V2_CONTROLREG;       
//    case 3:
//      switch(number) {
//        //Voice1
//        case 74:
//          SID_REG_V3_ATTACK_DECAY.ATTACK = value/8;
//          break;
//        case 71:
//          SID_REG_V3_ATTACK_DECAY.DECAY = value/8;
//          break;
//        case 91:
//          SID_REG_V3_SUSTAIN_RELEASE.SUSTAIN = value/8;
//          break;      
//        case 93:
//          SID_REG_V3_SUSTAIN_RELEASE.RELEASE = value/8;
//          break; 
//        case 75:  //Voice1 PW Fine
//          SIDREG(SID_ADDR_V3_PW_LOW) = (value << 1);
//          break;      
//        case 76:  //Voice1 PW Coarse
//          SIDREG(SID_ADDR_V3_PW_HI) = (value << 1);
//          break; 
//        case 2:
//          if (value == 0)
//            SID_REG_V3_CONTROLREG.TRIANGLE_WAVE = 0;
//          else
//            SID_REG_V3_CONTROLREG.TRIANGLE_WAVE = 1;             
//          break;
//        case 3:
//          if (value == 0)
//            SID_REG_V3_CONTROLREG.SAWTOOTH_WAVE = 0;
//          else
//            SID_REG_V3_CONTROLREG.SAWTOOTH_WAVE = 1;             
//          break;
//        case 4:
//          if (value == 0)
//            SID_REG_V3_CONTROLREG.SQUARE_WAVE = 0;
//          else
//            SID_REG_V3_CONTROLREG.SQUARE_WAVE = 1;             
//          break;
//        case 5:
//          if (value == 0)
//            SID_REG_V3_CONTROLREG.NOISE_WAVE = 0;
//          else
//            SID_REG_V3_CONTROLREG.NOISE_WAVE = 1;             
//          break; 
//        default:
//          return;             
//      }      
//      SIDREG(SID_ADDR_V3_ATTACK_DECAY) = *(char*)&SID_REG_V3_ATTACK_DECAY;
//      SIDREG(SID_ADDR_V3_SUSTAIN_RELEASE) = *(char*)&SID_REG_V3_SUSTAIN_RELEASE; 
//      SIDREG(SID_ADDR_V3_CONTROLREG) = *(char*)&SID_REG_V3_CONTROLREG;        
//    case 4:  //YM2149 Voice A
//      switch(number) {
//        //VoiceA
//        case 2:
//          if (value == 0)
//            YM_REG_ENVSHAPE.CONT = 0;
//          else
//            YM_REG_ENVSHAPE.CONT = 1;             
//          break;
//        case 3:
//          if (value == 0)
//            YM_REG_ENVSHAPE.ATT = 0;
//          else
//            YM_REG_ENVSHAPE.ATT = 1;             
//          break;
//        case 4:
//          if (value == 0)
//            YM_REG_ENVSHAPE.ALT = 0;
//          else
//            YM_REG_ENVSHAPE.ALT = 1;             
//          break;
//        case 5:
//          if (value == 0)
//            YM_REG_ENVSHAPE.HOLD = 0;
//          else
//            YM_REG_ENVSHAPE.HOLD = 1;             
//          break;   
//        case 6:
//          if (value == 0)
//            YM_REG_MIXER.NOISEA = 1;
//          else
//            YM_REG_MIXER.NOISEA = 0;             
//          break;  
//        case 7:
//          if (value == 0)
//            YM_REG_MIXER.TONEA = 1;
//          else
//            YM_REG_MIXER.TONEA = 0;             
//          break;  
//        case 8:
//          if (value == 0)
//            YM_REG_VA_LEVEL.MODE = 0;
//          else
//            YM_REG_VA_LEVEL.MODE = 1;             
//          break;           
//        case 74:
//          YM2149REG(YM_ADDR_NOISE) = value/4;
//          break;
//        case 71:
//          YM_REG_VA_LEVEL.LEVEL = value/8;
//          break;
//        case 75:  //YM Envelope Freq Fine
//          YM2149REG(YM_ADDR_FREQ_E) = (value << 1);
//          break;      
//        case 76:  //YM Envelope Freq Coarse
//          YM2149REG(YM_ADDR_FREQ_E+1) = (value << 1);
//          break; 
//        default:
//          return;        
//      }      
//      YM2149REG(YM_ADDR_LEVEL_A) = *(char*)&YM_REG_VA_LEVEL;
//      YM2149REG(YM_ADDR_SHAPE_E) = *(char*)&YM_REG_ENVSHAPE;
//      YM2149REG(YM_ADDR_MIXER) = *(char*)&YM_REG_MIXER;      
//    case 5:  //YM2149 Voice B
//      switch(number) {
//        //VoiceB
//        case 2:
//          if (value == 0)
//            YM_REG_ENVSHAPE.CONT = 0;
//          else
//            YM_REG_ENVSHAPE.CONT = 1;             
//          break;
//        case 3:
//          if (value == 0)
//            YM_REG_ENVSHAPE.ATT = 0;
//          else
//            YM_REG_ENVSHAPE.ATT = 1;             
//          break;
//        case 4:
//          if (value == 0)
//            YM_REG_ENVSHAPE.ALT = 0;
//          else
//            YM_REG_ENVSHAPE.ALT = 1;             
//          break;
//        case 5:
//          if (value == 0)
//            YM_REG_ENVSHAPE.HOLD = 0;
//          else
//            YM_REG_ENVSHAPE.HOLD = 1;             
//          break;   
//        case 6:
//          if (value == 0)
//            YM_REG_MIXER.NOISEB = 1;
//          else
//            YM_REG_MIXER.NOISEB = 0;             
//          break;  
//        case 7:
//          if (value == 0)
//            YM_REG_MIXER.TONEB = 1;
//          else
//            YM_REG_MIXER.TONEB = 0;             
//          break;  
//        case 8:
//          if (value == 0)
//            YM_REG_VB_LEVEL.MODE = 0;
//          else
//            YM_REG_VB_LEVEL.MODE = 1;             
//          break;           
//        case 74:
//          YM2149REG(YM_ADDR_NOISE) = value/4;
//          break;
//        case 71:
//          YM_REG_VB_LEVEL.LEVEL = value/8;
//          break;
//        case 75:  //YM Envelope Freq Fine
//          YM2149REG(YM_ADDR_FREQ_E) = (value << 1);
//          break;      
//        case 76:  //YM Envelope Freq Coarse
//          YM2149REG(YM_ADDR_FREQ_E+1) = (value << 1);
//          break; 
//        default:
//          return;        
//      }      
//      YM2149REG(YM_ADDR_LEVEL_B) = *(char*)&YM_REG_VB_LEVEL;
//      YM2149REG(YM_ADDR_SHAPE_E) = *(char*)&YM_REG_ENVSHAPE;
//      YM2149REG(YM_ADDR_MIXER) = *(char*)&YM_REG_MIXER;
//    case 6:  //YM2149 Voice C
//      switch(number) {
//        //VoiceC
//        case 2:
//          if (value == 0)
//            YM_REG_ENVSHAPE.CONT = 0;
//          else
//            YM_REG_ENVSHAPE.CONT = 1;             
//          break;
//        case 3:
//          if (value == 0)
//            YM_REG_ENVSHAPE.ATT = 0;
//          else
//            YM_REG_ENVSHAPE.ATT = 1;             
//          break;
//        case 4:
//          if (value == 0)
//            YM_REG_ENVSHAPE.ALT = 0;
//          else
//            YM_REG_ENVSHAPE.ALT = 1;             
//          break;
//        case 5:
//          if (value == 0)
//            YM_REG_ENVSHAPE.HOLD = 0;
//          else
//            YM_REG_ENVSHAPE.HOLD = 1;             
//          break;   
//        case 6:
//          if (value == 0)
//            YM_REG_MIXER.NOISEC = 1;
//          else
//            YM_REG_MIXER.NOISEC = 0;             
//          break;  
//        case 7:
//          if (value == 0)
//            YM_REG_MIXER.TONEC = 1;
//          else
//            YM_REG_MIXER.TONEC = 0;             
//          break;  
//        case 8:
//          if (value == 0)
//            YM_REG_VC_LEVEL.MODE = 0;
//          else
//            YM_REG_VC_LEVEL.MODE = 1;             
//          break;   
//        case 9:
//          if (value == 0)
//            playMOD = 0;
//          else{
//            modfile = SmallFS.open("track1.mod");
//            mod = pt_init_smallfs(modfile);
//            playMOD = 1;             
//          }
//          break;   
//        case 11:
//          if (value == 0)
//            playMOD = 0;
//          else{
//            modfile = SmallFS.open("track2.mod");
//            mod = pt_init_smallfs(modfile);
//            playMOD = 1;             
//          }
//          break;  
//        case 13:
//          if (value == 0)
//            playMOD = 0;
//          else{
//            modfile = SmallFS.open("track3.mod");
//            mod = pt_init_smallfs(modfile);
//            playMOD = 1;             
//          }
//          break;            
//        case 10:
//          if (value == 0){
//            playYM = 0;
//            resetYMFlag = 1;
//          }
//          else{
//            ymaudiofile = SmallFS.open("track1.ymdat"); 
//            ymTimeStamp = 0;
//            playYM = 1;             
//          }
//          break;    
//         case 12:
//          if (value == 0){
//            playYM = 0;
//            resetYMFlag = 1;
//          }
//          else{
//            ymaudiofile = SmallFS.open("track2.ymdat"); 
//            playYM = 1;             
//            ymTimeStamp = 0;            
//          }
//          break;   
//         case 14:
//          if (value == 0){
//            playYM = 0;
//            resetYMFlag = 1;
//          }
//          else{
//            ymaudiofile = SmallFS.open("track3.ymdat"); 
//            playYM = 1;     
//            ymTimeStamp = 0;            
//          }
//          break;            
//        case 74:
//          YM2149REG(YM_ADDR_NOISE) = value/4;
//          break;
//        case 71:
//          YM_REG_VC_LEVEL.LEVEL = value/8;
//          break;
//        case 75:  //YM Envelope Freq Fine
//          YM2149REG(YM_ADDR_FREQ_E) = (value << 1);
//          break;      
//        case 76:  //YM Envelope Freq Coarse
//          YM2149REG(YM_ADDR_FREQ_E+1) = (value << 1);
//          break; 
//        case 72:  //YM Envelope Freq Coarse
//          setVolumeYM2149(value << 1);
//          break; 
//        case 84:  //YM Envelope Freq Coarse
//          mod->mastervolume = (value << 1);
//          break;           
//        default:
//          return;   
//      }      
//      YM2149REG(YM_ADDR_LEVEL_C) = *(char*)&YM_REG_VC_LEVEL;
//      YM2149REG(YM_ADDR_SHAPE_E) = *(char*)&YM_REG_ENVSHAPE;
//      YM2149REG(YM_ADDR_MIXER) = *(char*)&YM_REG_MIXER; 
//    case 16:  //Transport
//      switch(number) {
//        //Transport
//        case 117:
//          if (value == 0)
//            playTrack = 1;
//          break;          
//        case 116:
//          if (value == 127){
//            playTrack = 0;
//            resetYMFlag = 1;
//          }
//          break; 
//        default:
//          return;
//      } 
//   default:
//    return;        
//  }      
//}


//void HandleNoteOn(byte channel, byte pitch, byte velocity) { 
// #ifdef DEBUG
//  Serial.print("Note Received: ");
//  Serial.println(pitch);
//  Serial.print("Channel Received: ");
//  Serial.println(channel);  
// #endif 
//  switch (channel){
//    case 1:
//      freqV1 = sid_midi[pitch];
//      set_freq(SID_ADDR_V1_FREQ_LOW,freqV1);
//      SID_REG_V1_CONTROLREG.GATE = 1;
//      SIDREG(SID_ADDR_V1_CONTROLREG) = *(char*)&SID_REG_V1_CONTROLREG;    
//      break;
//    case 2:
//      freqV2 = sid_midi[pitch];
//      set_freq(SID_ADDR_V2_FREQ_LOW,freqV2);
//      SID_REG_V2_CONTROLREG.GATE = 1;
//      SIDREG(SID_ADDR_V2_CONTROLREG) = *(char*)&SID_REG_V2_CONTROLREG;    
//      break;
//    case 3:
//      freqV3 = sid_midi[pitch];
//      set_freq(SID_ADDR_V3_FREQ_LOW,freqV3);
//      SID_REG_V3_CONTROLREG.GATE = 1;
//      SIDREG(SID_ADDR_V3_CONTROLREG) = *(char*)&SID_REG_V3_CONTROLREG;    
//      break;     
//    case 4:
//      freqV4 = pitch;
//      ym_set_freq(YM_ADDR_FREQ_A,freqV4);  
//      break;        
//    case 5:
//      freqV5 = pitch;
//      ym_set_freq(YM_ADDR_FREQ_B,freqV5);  
//      break;        
//    case 6:
//      freqV6 = pitch;
//      ym_set_freq(YM_ADDR_FREQ_C,freqV6);  
//      break;  
//    case 7:
//      switch (pitch){
//        case 35:
//            modfile = SmallFS.open("kick.mod");
//            mod = pt_init_smallfs(modfile);
//        break;
//        case 38:
//            modfile = SmallFS.open("hihat.mod");
//            mod = pt_init_smallfs(modfile);
//        break;
//        default:
//          return;
//      }
//      break;       
//    default:
//      break;             
//  }
//}

//void HandleNoteOff(byte channel, byte pitch, byte velocity) { 
//  switch(channel){
//      case 1:
//        SID_REG_V1_CONTROLREG.GATE = 0;
//        SIDREG(SID_ADDR_V1_CONTROLREG) = *(char*)&SID_REG_V1_CONTROLREG;
//        break;
//      case 2:
//        SID_REG_V2_CONTROLREG.GATE = 0;
//        SIDREG(SID_ADDR_V2_CONTROLREG) = *(char*)&SID_REG_V2_CONTROLREG;
//        break;
//      case 3:
//        SID_REG_V3_CONTROLREG.GATE = 0;
//        SIDREG(SID_ADDR_V3_CONTROLREG) = *(char*)&SID_REG_V3_CONTROLREG; 
//        break;
//      case 4:
//        freqV4 = 128;
//        ym_set_freq(YM_ADDR_FREQ_A,freqV4); 
//        break;        
//      case 5:
//        freqV5 = 128;
//        ym_set_freq(YM_ADDR_FREQ_B,freqV5); 
//        break;  
//      case 6:
//        freqV6 = 128;
//        ym_set_freq(YM_ADDR_FREQ_C,freqV6); 
//        break; 
//      default:
//        return;      
//  }  
//   #ifdef DEBUG 
//    Serial.println("In NoteOff");
//   #endif
//}

//void loop(){
  // Call MIDI.read the fastest you can for real-time performance.
//  MIDI.read();
//  if (playTrack == 1){
//    if (playYM == 1){
//      if (ymTimeStamp == 32)
//        ymTimeStamp = 0;
//      if (ymTimeStamp == 0 && playMOD == 1){
//            modfile = SmallFS.open("kick.mod");
//            mod = pt_init_smallfs(modfile);
//      }
//      if (ymTimeStamp == 16 && playMOD == 1){
//            modfile = SmallFS.open("hihat.mod");
//            mod = pt_init_smallfs(modfile);
//      }      
//      audiofill();       
//    }
    
//    if (playMOD == 1){
//      int i;
//      pt_render(modfile, mod, buf, NULL, 2, 16 /* Samples */, 1, 16, 1);
//  	for (i=0;i<32;i+=2) {
//  	  unsigned v = buf[i];
//  	  v += buf[i+1]<<8;
//  	  v += buf[i]<<16;
//          v += buf[i+1]<<24;
//  	  v+=0x80008000;
//          while (audioBuffer.isFull());
//  	    audioBuffer.push(v);
//  	}
//    }      
  //}
//}
