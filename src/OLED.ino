//*************************************************************************
//Update lower part of display fastest update time is 81 ms faster will cause strange display behavior
//*************************************************************************
void UpdateLowerDisplay(double _Display_Val){
  // Last value written to display
  static double _Saved_Value;
  //last display update time [ms]
  static double _Save_Time; 
  
   
  
   if (millis() >= _Save_Time+81){
    _Save_Time=millis();

    if (_Saved_Value != _Display_Val){
      _Saved_Value = _Display_Val;
      //_Display_Val=round(_Display_Val*10);
      //_Display_Val=_Display_Val/10;
      mydisp.setFont(123); //font 123 only have chars of space and 1 to 9
      mydisp.setTextPosAbs(10,63);
      mydisp.print(_Display_Val);
      
    }

    
  } 
  
}

//*************************************************************************
//Update lower part of display fastest update time is 81 ms faster will cause strange display behavior
//*************************************************************************
void UpdateLowerDisplayInt(int _Display_Val){
  
  // Last value written to display
  //static int _Saved_Value;
  //last display update time [ms]
  static double _Save_Time; 
  
   
  
   if (millis() >= _Save_Time+100){
    _Save_Time=millis();

    //if (_Saved_Value != _Display_Val){
      //_Saved_Value = _Display_Val;*/
      mydisp.setFont(123); //font 123 only have chars of space and 1 to 9
      mydisp.setTextPosAbs(15,63);
      if (_Display_Val<10){mydisp.print("0");}
      if (_Display_Val<100){mydisp.print("0");}
      mydisp.print(_Display_Val);
      
    //}

    
  } 
  
}


//*************************************************************************
// Force update of lower display
//*************************************************************************
void UpdateLowerDisplayNOW(double _Display_Val){

      mydisp.setFont(123); //font 123 only have chars of space and 1 to 9
      mydisp.setTextPosAbs(10,63);
      mydisp.print(_Display_Val);

  
}

//*************************************************************************
// Update total run time
//*************************************************************************
void UpdateTotalRunTimeDisplay (double _Total_RunTime){
    mydisp.setFont(fonts[3]); //font 123 only have chars of space and 1 to 9
    mydisp.setTextPosAbs(0,18);
    mydisp.print(_Total_RunTime,1);
    mydisp.print('s');

}
//*************************************************************************
// update counter bar
//*************************************************************************
void CountDownBar(double _Grinder_Run_Time_ms, boolean _SSR_Relay){
  
  static double _Last_Update_Time;
  static double _Update_time = 54;
  
  static double _Saved_Value;
  static double _Save_Time;
  static int Bar_Length = 0;
  static int switch_Bar = 0;
  
if (millis() >= (_Last_Update_Time+_Update_time) || !_SSR_Relay){
    
  _Update_time = (_Grinder_Run_Time_ms) / 126; 
  _Last_Update_Time=millis();

  switch (switch_Bar)  {
    
  case 0:
    if (_SSR_Relay){
        
      // Clear top part of display
      ClearTopOLED();
      // Draw double framed rectangle
      mydisp.drawFrame(0,0,128,20);
      mydisp.drawFrame(1,1,125,18);
      // Calcuate number of second separators
      int Number_of_second_bars = _Grinder_Run_Time_ms / 1000;
      // Calculate pixels between Second separators
      int Pixels_between_Bars = (128000 / (_Grinder_Run_Time_ms));
      // Draw second separators
      for (uint8_t j = 0; j < Number_of_second_bars+1; j++){ //move string to right
        mydisp.drawLine(j*Pixels_between_Bars,19,j*Pixels_between_Bars,1);
        delay(5);// 5 ms delay between line drawing
      }
     Bar_Length = 1;
     
     // Next cycle start counting up bar
     switch_Bar=1; 

    }

  break;  
  
  case 1:
    if (!_SSR_Relay){
      switch_Bar=0;
      break;}
    
    // add new line to counter bar
    mydisp.drawLine(Bar_Length,1,Bar_Length,19); 
    Bar_Length++;
    
      
  
  break;

  }
 
} 
}
//*************************************************************************
// Clear top part of Display
//*************************************************************************
void ClearTopOLED(){
  mydisp.setColor(0);
  mydisp.drawBox(0,0,127,24);
  mydisp.setColor(1);
}
//*************************************************************************
// Clear entire display
//*************************************************************************
void ClearOLED(){
  mydisp.setColor(0);
  delay(5);
  mydisp.drawBox(0,0,127,63);
  delay(5);
  mydisp.setColor(1);
}
//*************************************************************************
// Setup entire display
//*************************************************************************
void Start_Screen(){

  UpdateLowerDisplayNOW(Grinder_Run_Time_ms/1000);

  // Print "S" after grinder timer
  mydisp.setFont(fonts[3]); 
  mydisp.setTextPosAbs(111,64);
  mydisp.print("S");
  
  
  // Print Total Grind Time
  mydisp.setTextPosAbs(0,18);
  mydisp.print(Total_Runtime,1);
  mydisp.print("s");
  
  // Draw divider line in display
  mydisp.drawLine(0,25,128,25);

}
