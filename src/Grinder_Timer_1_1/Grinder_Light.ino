void Grinder_Light(int _SSR_Relay ){

  int _Update_time = 20; // ms between update scan
  int _fadeAmount = 2 ; // Fade amount per update scan 
  static double _Last_Update_Time;
  static double _light_level;
  static int _Saved_light_Level;
  int _min_light_level=0;
  int _max_light_level=0;
  
  _min_light_level = map(minLightLevel, 0, 100, 0, 255);
  _max_light_level = map(maxLightLevel, 0, 100, 0, 255);

  if (millis() >= (_Last_Update_Time+_Update_time)){
    _Last_Update_Time=millis();
    
    // SSR is ON full light
    if (_SSR_Relay==2){
      _light_level = _max_light_level;
    }
    else if(_SSR_Relay==1){
      // SSR is OFF start to DIM light
      if (_light_level > _min_light_level){
        _light_level=_light_level-_fadeAmount;
      }  
        if (_light_level < _min_light_level){
          _light_level = _min_light_level;
        }
    }
    else if(_SSR_Relay==0){
      _light_level=sleepLightLevel;
    }
    
    // Check if outputs need update  
    if (_Saved_light_Level != _light_level){
      _Saved_light_Level = _light_level; 
      analogWrite(light_1, _light_level); 
      analogWrite(light_2, _light_level);
    }
    
  }





}
