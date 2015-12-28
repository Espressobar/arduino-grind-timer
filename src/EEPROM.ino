//*************************************************************************
//        Save Total Run Time to EEPROM @ Pos. MSB->200, 201 & LSB->202
//*************************************************************************
void Save_Total_Run_Time_To_EEPROM(double _Total_Run_Time){
    static double _Saved_Value = -1;
    
    if (_Saved_Value != _Total_Run_Time){
      _Saved_Value = _Total_Run_Time;
      double _temp = _Total_Run_Time*100;    
      byte _Mega_Byte = _temp/65535;
      byte _High_Byte = (_temp - (_Mega_Byte*65535)) / 255;
      byte _Low_Byte =  _temp - (_Mega_Byte*65535) - (_High_Byte*255);
      EEPROM.write(200, _Mega_Byte);
      EEPROM.write(201, _High_Byte);
      EEPROM.write(202, _Low_Byte);
    }

}

//*************************************************************************
//        Save Grinder Run Time to EEPROM @ Pos. MSB->100 & LSB->101
//*************************************************************************
void Save_Grinder_Run_Time_To_EEPROM(int _Grinder_Run_Time){
    static double _Saved_Value = 0;
    
    if (_Saved_Value != _Grinder_Run_Time){
      _Saved_Value != _Grinder_Run_Time;
      byte _High_Byte = _Grinder_Run_Time/255;
      byte _Low_Byte =  _Grinder_Run_Time - (_High_Byte*255); 
      EEPROM.write(100, _High_Byte);
      EEPROM.write(101, _Low_Byte);
    }

}


//*************************************************************************
//        Update Menu Value to EEPROM
//*************************************************************************
void Save_Menu_To_EEPROM(){
    
    if (EEPROM.read(300) != timerMode){
      EEPROM.write(300, timerMode);
    }
    
    if (EEPROM.read(301) != ResumeResetTime){
      EEPROM.write(301, ResumeResetTime);
    }
    
    if (EEPROM.read(302) != sleepLightLevel){
      EEPROM.write(302, sleepLightLevel);
    }    
    
    if (EEPROM.read(303) != minLightLevel){
      EEPROM.write(303, minLightLevel);
    }
    
    if (EEPROM.read(304) != maxLightLevel){
      EEPROM.write(304, maxLightLevel);
    }
    
    if (EEPROM.read(305) != screenSaverTime){
      EEPROM.write(305, screenSaverTime);
    }
    

    
    

}
