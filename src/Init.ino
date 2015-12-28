void Init(){

  #if defined(_Load_std_Values_)
    timerMode=0;
    ResumeResetTime=5;
    sleepLightLevel=1;
    minLightLevel=5;
    maxLightLevel=100;
    screenSaverTime=10;
    Save_Grinder_Run_Time_To_EEPROM(500);  // Set to 5,00 sec
    Save_Menu_To_EEPROM();
  #endif
  
  
  
  
  // Read Grinder Run Time from EEPROM and transfer value to Encoder function  
  myEnc.write(EEPROM.read(100)*255+EEPROM.read(101));
  
  //Calculate Grinder Run Time
  Grinder_Run_Time_ms = myEnc.read();
  Grinder_Run_Time_ms = Grinder_Run_Time_ms*10; //Convert to ms
 
  // Read Total Run Time from EEPROM 3 bytes gives 
  double temp = EEPROM.read(200);
  Total_Runtime =temp*65535;
  temp = EEPROM.read(201);
  Total_Runtime = Total_Runtime + (temp*255);
  temp = EEPROM.read(202);
  Total_Runtime = Total_Runtime + temp;
  

  
  Total_Runtime=Total_Runtime/100;
      
  if (Total_Runtime < 0){Total_Runtime = -Total_Runtime;}
  Total_kg_Grinded = Total_Runtime*Gram_of_coffee_per_sec/1000;
  
  timerMode=EEPROM.read(300);
  ResumeResetTime=EEPROM.read(301);
  sleepLightLevel=EEPROM.read(302);
  minLightLevel=EEPROM.read(303);
  maxLightLevel=EEPROM.read(304);
  screenSaverTime=EEPROM.read(305);


  // Start Display  
  mydisp.begin();
  
  // Start-up delay show Welcome screen
  //delay(5000);

  /*----------for text LCD adapter and graphic LCD adapter ------------*/
  mydisp.clearScreen(); //CLear screen
  mydisp.disableCursor(); //disable cursor, enable cursore use: enableCursor();


  // Set Contrast of Display 0 is minimum
  mydisp.setContrast(0);
  
  
 Start_Screen(); // Setup screen function
  
 Save_Time=millis();
 Screen_Saver = millis();  // Save Point for screen saver (Turn off screen)

  



}
