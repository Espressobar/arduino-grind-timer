//*************************************************************************
//        Check if button is pressed - 50ms debounce filter
//*************************************************************************

int Button() {
    static double State_Change;
    static boolean State = 0;
    byte High_Byte = 0;
    byte Low_Byte = 0;
    
    switch (Button_Switch)  {
    // Wait for button to be pushed  
    case 0:
       if (!digitalRead(buttonPin)){
          Button_Switch = 1;
          Serial.println("Jump to State 1"); 
          button_Seen = millis();
          
          
          State_Change = millis();
            
          }
        return 0; // No button Pushed     
       
     
   
    break; 
    
    // 50 ms delay and check again
    case 1:
        
       if (millis() >= State_Change + 50){
          if (digitalRead(buttonPin)){
            Button_Switch = 0;
            Serial.println("Jump to State 0");  
          }
       else if (!digitalRead(buttonPin)){
            Button_Switch = 2;
            Serial.println("Jump to State 2");
          }
          
       }
   
    break;
  
    // Check for long or short button push
    case 2:
       // If    
       if (digitalRead(buttonPin)){
          Button_Switch = 0;
          Serial.println("Jump to State 0");
          button_Seen = millis() - button_Seen;
          Screen_Saver=millis();  //Delay Screen Saver
          
          
          return  1; // Short button Push
          break;
          
       }
       
       
       else if (!digitalRead(buttonPin) && millis() >= State_Change + 1000){
          Button_Switch = 3;
          Serial.println("Jump to State 3");
          return  2; // Long button Push 
          
          
          button_Seen = millis() - button_Seen;
          break;
          
             
       }
       else{
       return 0;
       }
    break;
    
    
    case 3:
    if (digitalRead(buttonPin)){
          Button_Switch = 0;
          Serial.println("Jump to State 0");
          Screen_Saver=millis();  //Delay Screen Saver
          return 0;
          break;
       }
        return 0;
    break;
    }
    
   
}

//*************************************************************************
//        Control of LED in Button
//*************************************************************************
void Button_Led(boolean _SSR_Relay){
  static int _Update_time = 20; // Update time in ms
  static double _Last_Update_Time;
  static int _fadeAmount = 2 ;
  static byte _brightness;
  static int _Saved_light_Level;
  
  
  if (millis() >= (_Last_Update_Time+_Update_time)){
    _Last_Update_Time=millis();
    if (_SSR_Relay){_brightness=255;}
    else if(_brightness == 255){_brightness = 50;}
    else{_brightness = _brightness + _fadeAmount;}

  // reverse the direction of the fading at the ends of the fade: 
  if (_brightness <= 0 || _brightness >= 100) {
    _fadeAmount = -_fadeAmount ; 
  }
  // Check if outputs need update
  if (_Saved_light_Level != _brightness){
   _Saved_light_Level = _brightness; 
   analogWrite(ButtonLedPin, _brightness);
  }

  }

}

void Button_HeartBeat(){
  static int _OffTime = 2000; // Update time in ms
  static int _OnTime = 25; // Update time in ms
  static double _Last_Update_Time;
  //static int _fadeAmount = 2 ;
  static byte _brightness=25;
  //static int _State=0;
  //static int _Saved_light_Level;
  if (millis() >= (_Last_Update_Time+_OffTime+_OnTime)){
      analogWrite(ButtonLedPin, 0);
      _Last_Update_Time=millis();
  }
  
  else if (millis() >= (_Last_Update_Time+_OffTime)){
    analogWrite(ButtonLedPin, _brightness);
    

 

  }

}
