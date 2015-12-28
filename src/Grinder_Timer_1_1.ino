/******************************************************/
// Setup how many ticks the encoder gives per click
int encoderScaler=1;

// uncomment "#define _Load_std_Values_" to Load standard Values
//#define _Load_std_Values_

// Specify Communication between I2C or UART
// I think I2C is more stable than UART - the code is only tested with I2C
#define _Digole_Serial_I2C_  //To tell compiler compile the special communication only, OLED DATA => ARDUINO ANALOG IN A4  ---- OLED CLK => ARDUINO ANALOG IN A5 
//#define _Digole_Serial_UART_  //To tell compiler compile the special communication only, OLED DATA => ARDUINO DIGITAL IN 1 
/*******************************************************/

#include <Encoder.h>
#include <EEPROM.h>

// Setup interface with OLED Display
 
//all available are:_Digole_Serial_UART_, _Digole_Serial_I2C_ 
#include <DigoleSerial.h>
//--------UART setup, if you don't use UART, use // to comment following line
#if defined(_Digole_Serial_UART_)
DigoleSerialDisp mydisp(&Serial, 9600); //UART:Arduino UNO: Pin 1(TX)on arduino to RX on module
#endif
//--------I2C setup, if you don't use I2C, use // to comment following 2 lines
#if defined(_Digole_Serial_I2C_)
#include <Wire.h>
DigoleSerialDisp mydisp(&Wire,'\x27');  //I2C:Arduino UNO: SDA (data line) is on analog input pin 4, and SCL (clock line) is on analog input pin 5 on UNO and Duemilanove
#endif



//-------------------------


  const int buttonPin = 10;    // Number of the push button pin
  const int ButtonLedPin = 11; // Number of the Solid State pin
  const int SsrPin = 12;       // Number of the Solid State Relay pin
  const int light_1 = 3;       // Number of the Grinder Light 1 pin
  const int light_2 = 5;       // Number of the Grinder Light 2 pin
  Encoder myEnc(9, 8);         // Pins for encoder 



int Button_Switch = 0;
double button_Seen = 0;
double Grinding_Started = 0;
double Save_Time;
double timeLeft;


double Total_Runtime;
double Total_kg_Grinded;
double Gram_of_coffee_per_sec = 2.5;
double Grinder_Run_Time_ms;
double Screen_Saver;
double Sleep_Time_min = 10;
int Grinder_State = 0;
boolean Grinder_State_Just_Changed = true;

int timerMode=0;
int ResumeResetTime=5;
int sleepLightLevel=1;
int minLightLevel=5;
int maxLightLevel=100;
int screenSaverTime=10;
int resetTotalTimer=0;


// Resume Time Variables
double ReStartResetTimer;
double WaitToStartAgain;
boolean DisplayState;


// Declare Fonts
const unsigned char fonts[] = {6, 10, 18, 51, 120, 123};
const char *fontdir[] = {"0\xb0", "90\xb0", "180\xb0", "270\xb0"};





void setup() {


  
  // Setup PIns to In/Outputs
  pinMode(ButtonLedPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(SsrPin, OUTPUT);
  pinMode(light_1, OUTPUT);
  pinMode(light_2, OUTPUT);
  

  digitalWrite(buttonPin, HIGH); // Turn on Pull Up resistor
  Serial.begin(9600); //Start Serial Communication

  
  Init(); 

  


        
    

}

// Update values for Encoder function
//long oldPosition  = EEPROM.read(100);
//int Saved_Value_to_EEPROM = oldPosition;

void loop() {

   switch (Grinder_State){
    
    
    case 0:{ // Idle waiting for things to be done 
      if (Grinder_State_Just_Changed){
        Grinder_State_Just_Changed=false;
      
      }
      // Light in push button is pulsing
      Button_Led(false);
      // Grinder light is turned off
      Grinder_Light(1); 
      
      // Check if grinder time is changed by encoder
      Grinder_Run_Time_ms = Encoder_Counter_Grinder_Time();
    
      // Check Button State
      int TempButton = Button();
    
      // Short Activation of Button
      if ( 1 == TempButton ){
        
        Grinder_State = 1;
        Grinder_State_Just_Changed = true;
        Screen_Saver=millis();
        Save_Grinder_Run_Time_To_EEPROM(myEnc.read()); 
      }
      
      if ( 2 == TempButton ){
        
        Grinder_State = 100;
        Grinder_State_Just_Changed = true;
        Screen_Saver=millis();
        ClearOLED();  
      }
     
     // Nothing happens - fall a sleep
     if (millis() >= Screen_Saver + screenSaverTime*60000){
      Grinder_State = 10;
        Grinder_State_Just_Changed = true; 
      } 
    
    break;}

  /*********************************************
  Grinding
  *********************************************/    
  case 1: 
      if (Grinder_State_Just_Changed){
        Grinder_State_Just_Changed=false;
        digitalWrite(SsrPin, HIGH);  //Turn on motor
        Grinding_Started = millis();  // Log grinder start time
       }
      // Steady light in Push Button 
      Button_Led(true);
      // Turn on Grinder Light
      Grinder_Light(2);
      // Update grinder timer bar
      CountDownBar(Grinder_Run_Time_ms, true);
      
      //Grinder_Run_Time_ms = Encoder_Counter_Grinder_Time();
      
      // Update lower display
      //if (millis() + 50 <= Grinder_Run_Time_ms + Grinding_Started){ 
        UpdateLowerDisplay((Grinding_Started + (Grinder_Run_Time_ms)-millis())/1000);
      //}
      
      // Check for grinder finished
      if (millis() >= Grinder_Run_Time_ms + Grinding_Started){
        // Update total run time
        Total_Runtime = Total_Runtime + ((millis()-Grinding_Started)/1000);
        // goto grinder switch state 0
        Grinder_State = 0;
        // Grinder switch state just changed
        Grinder_State_Just_Changed = true;
        // Turn OFF Grinder Solid State Relay
        digitalWrite(SsrPin, LOW);
        //Update lower part of display
        UpdateLowerDisplayNOW(Grinder_Run_Time_ms/1000);
        // Update top part of display - clear counter bar
        CountDownBar(Grinder_Run_Time_ms, false);
        ClearTopOLED();
        // Save total runtime to EEprom
        Save_Total_Run_Time_To_EEPROM(Total_Runtime);
        // Update top part of display
        UpdateTotalRunTimeDisplay (Total_Runtime);
      }
      
      // if button is pushed stop grinding
      if ( 1 == Button()){
        if (timerMode==0){
          Serial.println("Grinder Stopped goto State 0");
          // Update total run time
          Total_Runtime = Total_Runtime + ((millis()-Grinding_Started)/1000);
          // goto grinder switch state 0
          Grinder_State = 0;
          // Grinder switch state just changed
          Grinder_State_Just_Changed = true;
          digitalWrite(SsrPin, LOW);
          //UpdateLowerDisplayNOW(Grinder_Run_Time_ms/1000);
          CountDownBar(Grinder_Run_Time_ms, false);
          ClearTopOLED();
          // Save total runtime to EEprom
          Save_Total_Run_Time_To_EEPROM(Total_Runtime);
          // Update top part of display
          UpdateTotalRunTimeDisplay (Total_Runtime);
        }
        else{
          // Save Time Left
          timeLeft=(Grinding_Started + (Grinder_Run_Time_ms)-millis());
          UpdateLowerDisplayNOW(timeLeft/1000);
          // Update total run time
          Total_Runtime = Total_Runtime + ((millis()-Grinding_Started)/1000);
          // goto grinder switch state 0
          Grinder_State = 3;
          Serial.println("Grinder Stopped goto State 3");
          Serial.print("Time Left: ");
          Serial.println(timeLeft);
          // Grinder switch state just changed
          Grinder_State_Just_Changed = true;
          digitalWrite(SsrPin, LOW);
          //UpdateLowerDisplayNOW(Grinder_Run_Time_ms/1000);
          CountDownBar(Grinder_Run_Time_ms, true);
          //ClearTopOLED();
          // Save total runtime to EEprom
          Save_Total_Run_Time_To_EEPROM(Total_Runtime);
          // Update top part of display
          //UpdateTotalRunTimeDisplay (Total_Runtime);
          
        
        
        
        
        }
      }
    
    
    
    break;
  
  
  /*********************************************
  Resume mode paused grinder
  *********************************************/  
  case 3:{
      if (Grinder_State_Just_Changed){
        Grinder_State_Just_Changed=false;
        WaitToStartAgain=millis();
        ReStartResetTimer=millis();
        DisplayState=1;
      
      }
      
      // Flash resuming time on display
      if (millis()>=WaitToStartAgain+400){
        WaitToStartAgain=millis();
      
        if (DisplayState==1){
          DisplayState=0;
          mydisp.setColor(0);
          mydisp.drawBox(0,28,110,36);
          mydisp.setColor(1);
          }
        else{
          DisplayState=1;
          UpdateLowerDisplayNOW(timeLeft/1000);
          }
      
      }
      // Reset resume timer fired go back to Idle state
      if (millis()>=ReStartResetTimer+ResumeResetTime*1000){
        // goto grinder switch state 0
        Grinder_State = 0;
        // Grinder switch state just changed
        Grinder_State_Just_Changed = true;
        //Update lower part of display
        UpdateLowerDisplayNOW(Grinder_Run_Time_ms/1000);
        // Update top part of display - clear counter bar
        CountDownBar(Grinder_Run_Time_ms, false);
        // Clear entire OLED
        ClearTopOLED();
        // Save total runtime to EEprom
        Save_Total_Run_Time_To_EEPROM(Total_Runtime);
        // Update top part of display
        UpdateTotalRunTimeDisplay (Total_Runtime);
      
      
      
      }
      
      
      
    // Check Button State
      int TempButton = Button();
    
      // Short Activation of Button
      if ( 1 == TempButton ){
        
        Grinder_State = 4;
        Grinder_State_Just_Changed = true;
        Screen_Saver=millis();
        //Save_Grinder_Run_Time_To_EEPROM(myEnc.read()); 
      }
      
      if ( 2 == TempButton ){
        
        Grinder_State = 100;
        Grinder_State_Just_Changed = true;
        Screen_Saver=millis();
        ClearOLED();  
      }
    
  
  break;}


  /*********************************************
  Resume mode grinding after pause
  *********************************************/  
  case 4: 
    if (Grinder_State_Just_Changed){
        Grinder_State_Just_Changed=false;
        digitalWrite(SsrPin, HIGH);  //Turn on motor
        Grinding_Started = millis();  // Log grinder start time
       }
      // Steady light in Push Button 
      Button_Led(true);
      // Turn on Grinder Light
      Grinder_Light(2);
      // Update grinder timer bar
      CountDownBar(Grinder_Run_Time_ms, true);
      
      //Grinder_Run_Time_ms = Encoder_Counter_Grinder_Time();
      
      // Update lower display
      if (millis() + 100 <= timeLeft + Grinding_Started){ 
        UpdateLowerDisplay((Grinding_Started + (timeLeft)-millis())/1000);
      }
      
      // Check for grinder finished
      if (millis() >= timeLeft + Grinding_Started){
        // Update total run time
        Total_Runtime = Total_Runtime + ((millis()-Grinding_Started)/1000);
        // goto grinder switch state 0
        Grinder_State = 0;
        // Grinder switch state just changed
        Grinder_State_Just_Changed = true;
        // Turn OFF Grinder Solid State Relay
        digitalWrite(SsrPin, LOW);
        //Update lower part of display
        UpdateLowerDisplayNOW(Grinder_Run_Time_ms/1000);
        // Update top part of display - clear counter bar
        CountDownBar(Grinder_Run_Time_ms, false);
        // Clear entire OLED
        ClearTopOLED();
        // Save total runtime to EEprom
        Save_Total_Run_Time_To_EEPROM(Total_Runtime);
        // Update top part of display
        UpdateTotalRunTimeDisplay (Total_Runtime);
      }
      
      // if button is pushed stop grinding
      if ( 1 == Button()){
        
          // Save Time Left
          timeLeft=(Grinding_Started + (timeLeft)-millis());
          // Update total run time
          Total_Runtime = Total_Runtime + ((millis()-Grinding_Started)/1000);
          // goto grinder switch state 0
          Grinder_State = 3;
          // Grinder switch state just changed
          Grinder_State_Just_Changed = true;
          digitalWrite(SsrPin, LOW);
          //UpdateLowerDisplayNOW(Grinder_Run_Time_ms/1000);
          CountDownBar(Grinder_Run_Time_ms, true);
          // Save total runtime to EEprom
          Save_Total_Run_Time_To_EEPROM(Total_Runtime);
          
          
        
        
        
        
        
      }
    
    
    
  break;


  /*********************************************
  Sleep Mode
  *********************************************/  
  case 10: 
      if (Grinder_State_Just_Changed){
        Grinder_State_Just_Changed=false;
        ClearOLED();
      
      }
       // Check for button push
       Button();
       // Puls Push button light 
       //Button_Led(false);
       Button_HeartBeat();
       // Check for update of encoder values
       Grinder_Run_Time_ms = Encoder_Counter_Grinder_Time();
       // Grinder Light
       Grinder_Light(0);
      
      // shoul screen saver be interruptet
      if (millis() <= Screen_Saver + 100){
            Grinder_State = 0;
            Grinder_State_Just_Changed = true;
            Screen_Saver=millis();
            Start_Screen();
          }  
      
      
 
    
    break;   
  /*********************************************
  Selection of Grinder Mode (Re-Start or Resume)
  *********************************************/
  case 100:{
  if (Grinder_State_Just_Changed){
        Grinder_State_Just_Changed=false;
        mydisp.setFont(fonts[2]); 
        mydisp.setTextPosAbs(18,10);
        mydisp.print("Timer mode");
        myEnc.write(timerMode*encoderScaler);
        //UpdateLowerDisplayInt(maxLightLevel);
        mydisp.setFont(120); //font 123 only have chars of space and 1 to 9
        //mydisp.setTextPosAbs(45,37);
        if (timerMode==0){mydisp.setTextPosAbs(0,50);mydisp.print("0-ReStart");}
        if (timerMode==1){mydisp.setTextPosAbs(0,50);mydisp.print("1-Resume");}
     }
     
     timerMode=Encoder_Mode(0,1);
     Grinder_Light(1);
    
     // Check Button State
      int TempButton = Button();
     
    if ( 1 == TempButton ){
        
        Grinder_State_Just_Changed = true;
        Grinder_State = 101;
        ClearOLED();
        Screen_Saver=millis(); 
      }
      
     
    if ( 2 == TempButton ){
        
        Grinder_State = 0;
        ClearOLED();
        Save_Menu_To_EEPROM();
        myEnc.write(EEPROM.read(100)*255+EEPROM.read(101));
        Start_Screen();
        //Grinder_State_Just_Changed = true;
        Screen_Saver=millis(); 
      }
    
    
  
  
  break;}


  /*********************************************
  Set Reset time for Re-Start Mode
  *********************************************/
  case 101:{
  if (Grinder_State_Just_Changed){
        Grinder_State_Just_Changed=false;
        mydisp.setFont(fonts[2]); 
        mydisp.setTextPosAbs(0,10);
        mydisp.println("Set Reset Time");
        mydisp.print("Resume Mode[s]");
        myEnc.write(ResumeResetTime*encoderScaler);
        UpdateLowerDisplayInt(ResumeResetTime);
     }
     
     ResumeResetTime=Encoder_Menu(0,100);
     Grinder_Light(0);
    
     // Check Button State
      int TempButton = Button();
     
    if ( 1 == TempButton ){
        
        Grinder_State_Just_Changed = true;
        Grinder_State = 109;
        ClearOLED();
        Screen_Saver=millis(); 
      }
      
     
    if ( 2 == TempButton ){
        
      
        Grinder_State = 0;
        ClearOLED();
        Save_Menu_To_EEPROM();
        myEnc.write(EEPROM.read(100)*255+EEPROM.read(101));
        Start_Screen();
        //Grinder_State_Just_Changed = true;
        Screen_Saver=millis(); 
      }
    
    
  
  
  break;}


  /*********************************************
  Selection of Sleep Light Level
  *********************************************/    
    case 109:{
    if (Grinder_State_Just_Changed){
        Grinder_State_Just_Changed=false;
        mydisp.setFont(fonts[2]); 
        mydisp.setTextPosAbs(0,10);
        mydisp.println("Sleep Light");
        mydisp.print("Level [pct]");
        myEnc.write(sleepLightLevel*encoderScaler);
        UpdateLowerDisplayInt(sleepLightLevel);
     }
     
     sleepLightLevel=Encoder_Menu(0,100);
     Grinder_Light(0);
    
     // Check Button State
      int TempButton = Button();
     
    if ( 1 == TempButton ){
        
        Grinder_State_Just_Changed = true;
        Grinder_State = 110;
        ClearOLED();
        Screen_Saver=millis(); 
      }
      
     
    if ( 2 == TempButton ){
        
      
        Grinder_State = 0;
        ClearOLED();
        Save_Menu_To_EEPROM();
        myEnc.write(EEPROM.read(100)*255+EEPROM.read(101));
        Start_Screen();
        //Grinder_State_Just_Changed = true;
        Screen_Saver=millis(); 
      }
    
    
    break;}


  /*********************************************
  Selection of Min Light Level
  *********************************************/    
    case 110:{
    if (Grinder_State_Just_Changed){
        Grinder_State_Just_Changed=false;
        mydisp.setFont(fonts[2]); 
        mydisp.setTextPosAbs(0,10);
        mydisp.println("Idle Light");
        mydisp.print("Level [pct]");
        myEnc.write(minLightLevel*encoderScaler);
        UpdateLowerDisplayInt(minLightLevel);
     }
     
     minLightLevel=Encoder_Menu(0,100);
     Grinder_Light(1 );
    
     // Check Button State
      int TempButton = Button();
     
    if ( 1 == TempButton ){
        
        Grinder_State_Just_Changed = true;
        Grinder_State = 120;
        ClearOLED();
        Screen_Saver=millis(); 
      }
      
     
    if ( 2 == TempButton ){
        
      
        Grinder_State = 0;
        ClearOLED();
        Save_Menu_To_EEPROM();
        myEnc.write(EEPROM.read(100)*255+EEPROM.read(101));
        Start_Screen();
        //Grinder_State_Just_Changed = true;
        Screen_Saver=millis(); 
      }
    
    
    break;}
    
  /*********************************************
  Selection of Max Light Level
  *********************************************/  
  case 120:{
  if (Grinder_State_Just_Changed){
        Grinder_State_Just_Changed=false;
        mydisp.setFont(fonts[2]); 
        mydisp.setTextPosAbs(0,10);
        mydisp.println("Grind Light");
        mydisp.print("Level [pct]");
        myEnc.write(maxLightLevel*encoderScaler);
        UpdateLowerDisplayInt(maxLightLevel);
     }
     
     maxLightLevel=Encoder_Menu(0,100);
     Grinder_Light(2);
    
     // Check Button State
      int TempButton = Button();
     
    if ( 1 == TempButton ){
        
        Grinder_State_Just_Changed = true;
        Grinder_State = 130;
        ClearOLED();
        Screen_Saver=millis(); 
      }
      
     
    if ( 2 == TempButton ){
        
        Grinder_State = 0;
        ClearOLED();
        Save_Menu_To_EEPROM();
        myEnc.write(EEPROM.read(100)*255+EEPROM.read(101));
        Start_Screen();
        //Grinder_State_Just_Changed = true;
        Screen_Saver=millis(); 
      }
    
    
  
  
  break;}
  
  /*********************************************
  Set Sleep Time
  *********************************************/  
  case 130:{
  if (Grinder_State_Just_Changed){
        Grinder_State_Just_Changed=false;
        mydisp.setFont(fonts[2]); 
        mydisp.setTextPosAbs(0,10);
        mydisp.println("Sleep time");
        mydisp.print("[min]");
                          
        myEnc.write(screenSaverTime*encoderScaler);
        UpdateLowerDisplayInt(screenSaverTime);
     }
     
     screenSaverTime=Encoder_Menu(0,240);
     Grinder_Light(1);
    
     // Check Button State
      int TempButton = Button();
     
    if ( 1 == TempButton ){
        
        Grinder_State_Just_Changed = true;
        Grinder_State = 140;
        ClearOLED();
        Screen_Saver=millis(); 
      }
      
     
    if ( 2 == TempButton ){
        
        Grinder_State = 0;
        ClearOLED();
        Save_Menu_To_EEPROM();
        myEnc.write(EEPROM.read(100)*255+EEPROM.read(101));
        Start_Screen();
        //Grinder_State_Just_Changed = true;
        Screen_Saver=millis(); 
      }
    
    
  
  
  break;}
  
  
  /*********************************************
  Reset Total Counter
  *********************************************/  
  case 140:{
  if (Grinder_State_Just_Changed){
        Grinder_State_Just_Changed=false;
        mydisp.setFont(fonts[2]); 
        mydisp.setTextPosAbs(0,10);
        mydisp.println("Reset Total");
        mydisp.print("Counter");
                          
         myEnc.write(resetTotalTimer*encoderScaler);
        //UpdateLowerDisplayInt(maxLightLevel);
        mydisp.setFont(120); //font 123 only have chars of space and 1 to 9
        //mydisp.setTextPosAbs(45,37);
        if (resetTotalTimer==0){mydisp.setTextPosAbs(20,55);mydisp.print("0-NO");}
        if (resetTotalTimer==1){mydisp.setTextPosAbs(15,55);mydisp.print("1-YES");}
     }
     
     resetTotalTimer=Encoder_Reset(0,1);
     Grinder_Light(1);
    
     // Check Button State
      int TempButton = Button();
     
    if ( 1 == TempButton ){
        if (resetTotalTimer==1){
          Grinder_State_Just_Changed = true;
          Grinder_State = 141;
          ClearOLED();
          Screen_Saver=millis();
          resetTotalTimer=0;
        }
        else if (resetTotalTimer==0){
          Grinder_State = 100;
          Grinder_State_Just_Changed = true;
          Screen_Saver=millis();
          ClearOLED(); 
        }
        
         
      }
      
     
    if ( 2 == TempButton ){
        
        Grinder_State = 0;
        ClearOLED();
        Save_Menu_To_EEPROM();
        myEnc.write(EEPROM.read(100)*255+EEPROM.read(101));
        Start_Screen();
        //Grinder_State_Just_Changed = true;
        Screen_Saver=millis(); 
      }
    
    
  
  
  break;}
  
  /*********************************************
  Are Yor sure you want to Reset
  *********************************************/  
  case 141:
  if (Grinder_State_Just_Changed){
        Grinder_State_Just_Changed=false;
        mydisp.setFont(fonts[2]); 
        mydisp.setTextPosAbs(0,10);
        mydisp.println("ARE YOU SURE?");
        //mydisp.print("Counter");
                          
         myEnc.write(resetTotalTimer*encoderScaler);
        //UpdateLowerDisplayInt(maxLightLevel);
        mydisp.setFont(120); //font 123 only have chars of space and 1 to 9
        //mydisp.setTextPosAbs(45,37);
        if (resetTotalTimer==0){mydisp.setTextPosAbs(20,55);mydisp.print("0-NO");}
        if (resetTotalTimer==1){mydisp.setTextPosAbs(15,55);mydisp.print("1-YES");}
     }
     
     resetTotalTimer=Encoder_Reset(0,1);
     Grinder_Light(1);
    
     // Check Button State
      int TempButton = Button();
     
    if ( 1 == TempButton ){
        if (resetTotalTimer==1){
        resetTotalTimer=0;  
        Grinder_State = 0;
        ClearOLED();
        Save_Menu_To_EEPROM();
        Total_Runtime=0;
        Save_Total_Run_Time_To_EEPROM(Total_Runtime);
        myEnc.write(EEPROM.read(100)*255+EEPROM.read(101));
        Start_Screen();
        //Grinder_State_Just_Changed = true;
        Screen_Saver=millis();
        }
        else if (resetTotalTimer==0){
          Grinder_State = 100;
          Grinder_State_Just_Changed = true;
          Screen_Saver=millis();
          ClearOLED(); 
        } 
         
      }
      
     
    if ( 2 == TempButton ){
        
        Grinder_State = 0;
        ClearOLED();
        Save_Menu_To_EEPROM();
        myEnc.write(EEPROM.read(100)*255+EEPROM.read(101));
        Start_Screen();
        //Grinder_State_Just_Changed = true;
        Screen_Saver=millis(); 
      }
    
    
  
  
  break;
  }  
        

  

 
}




