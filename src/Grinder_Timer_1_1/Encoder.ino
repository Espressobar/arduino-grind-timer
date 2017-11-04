double Encoder_Counter_Grinder_Time() {
  static int _oldPosition = 0;
  static double _Display_Val;
  long _newPosition = myEnc.read()/encoderScaler;
  
  if (_newPosition != _oldPosition) {
    _oldPosition = _newPosition;
    Screen_Saver=millis();  //Delay Screen Saver
    
    _Display_Val = _newPosition;
    // Convert to ms
    _Display_Val = _Display_Val * 10;
    UpdateLowerDisplay(_Display_Val/1000);

  }
  return _oldPosition*10; 
}

double Encoder_Menu(int _minVal, int _maxVal){
   static int _oldPosition = 0;
  //static double _Display_Val;
  int _newPosition = myEnc.read()/encoderScaler;
  
  if (_newPosition != _oldPosition) {
    if (_newPosition < _minVal){_newPosition=_minVal; myEnc.write(_minVal);}
    if (_newPosition > _maxVal){_newPosition=_maxVal; myEnc.write(_maxVal);} 
    _oldPosition = _newPosition;
    
    UpdateLowerDisplayInt(_newPosition);

  }
  return _newPosition; 
  
  
  
}

double Encoder_Mode(int _minVal, int _maxVal){
   static int _oldPosition = 0;
  //static double _Display_Val;
  int _newPosition = myEnc.read()/encoderScaler;
  
  if (_newPosition != _oldPosition) {
    Serial.print("Encoder Changed Value to: " );
    Serial.print(_newPosition);
    if (_newPosition < _minVal){_newPosition=_minVal; myEnc.write(_minVal); Serial.println(" Corrected to 0");}
    if (_newPosition > _maxVal){_newPosition=_maxVal; myEnc.write(_maxVal); Serial.println(" Corrected to 1");} 
    _oldPosition = _newPosition;
    
    mydisp.setFont(120); //font 123 only have chars of space and 1 to 9
      mydisp.setColor(0);
      mydisp.drawBox(0,20,127,44);
      mydisp.setColor(1);
      if (_oldPosition==0){mydisp.setTextPosAbs(0,50);mydisp.print("0-ReStart");}
      if (_oldPosition==1){mydisp.setTextPosAbs(0,50);mydisp.print("1-Resume");}

  }
  return _newPosition; 
  
  
  
}


double Encoder_Reset(int _minVal, int _maxVal){
   static int _oldPosition = 0;
  //static double _Display_Val;
  int _newPosition = myEnc.read()/encoderScaler;
  
  if (_newPosition != _oldPosition) {
    Serial.print("Encoder Changed Value to: " );
    Serial.print(_newPosition);
    if (_newPosition < _minVal){_newPosition=_minVal; myEnc.write(_minVal); Serial.println(" Corrected to 0");}
    if (_newPosition > _maxVal){_newPosition=_maxVal; myEnc.write(_maxVal); Serial.println(" Corrected to 1");} 
    _oldPosition = _newPosition;
    
    mydisp.setFont(120); //font 123 only have chars of space and 1 to 9
      
      if (_oldPosition==0){
        mydisp.setColor(0);
        mydisp.drawBox(0,25,127,39);
        mydisp.setColor(1);
        mydisp.setTextPosAbs(20,55);
        mydisp.print("0-NO");
        }
      if (_oldPosition==1){
        
        mydisp.drawBox(0,25,127,39);
        mydisp.setColor(0);
        mydisp.drawBox(0,25,127,39);
        mydisp.setColor(1);
        mydisp.setTextPosAbs(15,55);
        mydisp.print("1-YES"); 
        }

  }
  return _newPosition; 
  
  
  
}

