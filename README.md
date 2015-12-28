# Arduino Grind Timer
Arduino project designed to control Mazzer espresso grinders by allowing users to control grind time and view grind status on an OLED display. The initial import for this repository is based on the original project by Henrik Thomsen.

The current version is 1.1.

# Example components

Arduino Uno
http://www.ebay.co.uk/itm/151166200963

0,96" OLED Display White
http://www.ebay.co.uk/itm/251233903648

LED pushbutton (start/stop grind)
http://www.ebay.co.uk/itm/280871195459

Encoder and turn knob (adjust grind time)
http://www.ebay.co.uk/itm/121195339728
http://www.ebay.co.uk/itm/380642097967

Diodes
http://www.ebay.co.uk/itm/300808099987

Solid State Relay
http://www.ebay.co.uk/itm/141087415791

# Instructions
Copy the three libraries from the lib folder: "EEPROM, Encoder & DigoleSerial" to "Arduino\libraries" folder and restart arduino application

Open "Grinder_Timer.ino".

# User Settings

There are some settings in the Grinder_Timer_1_1.ino that can be changed by the user:

* Encoder Scaler - used if your enceoder gives more electical pulses per physical click
* "//#define _Load_std_Values_" can be changed to "#define _Load_std_Values_" to load standard value to the EEPROM
* Selection of communication protocol to OLED - my experience is that I2C is the most stable	
