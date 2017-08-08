## Description:  
A drop in replacement for the Benmai SDVX controller tutorial featuring increased modularity and higher performance. Intended to be used with the Arduino Leonardo hardware configuration which can be found here https://sdvxii.pancakeapps.com/. This code can also be easily modified for other ATmega32u4 based microcontroller boards.  

Program emulates DJDAO SDVX/K-Shoot Mania style input modes. Arcade buttons send keystrokes. Rotary encoders emulate X/Y axis mouse movement.

## Features:  
Drop in replacement for Benmai SDVX controller program.
Higher button polling rate than original program - not limited to 200Hz.  
Software based timer debounce.  
User adjustable debounce interval.  
User selectable keybindings.  
User selectable default mode.  
LED backlighting support.  
Serial debug mode.  

## Arduino IDE Setup & Programming:  
1. Download and install the Arduino IDE: https://www.arduino.cc/en/Main/Software  
2. Open the .ino file.  
3. Set board type to Arduino Leonardo.  
4. Set the port to the numbered Arduino com port.  
5. Go to the file tab and open preferences.  
6. Check the Show verbose output during compilation and upload boxes.  
7. Go to the Sketches tab and click upload.  
8. If the log prints out programmer not in sync, you will need to reset the board a second after you click upload.  

## User Changeable Settings:

#### Modifying Debounce:  
Debounce interval is implemented using a global define. The interval is in milliseconds and can be can be fine tuned via debugging mode. If the debounce interval is set too short, multiple state changes will be logged in the serial console per button press. A typical debounce interval ranges between 10ms and 100ms. A Cherry MX keyswitch has a bounce time of roughly 5ms and mandates a 10ms debounce interval for reference.

#define debounce interval_in_ms

#### Kebindings:  
The keybindings are set using a global define. All keystrokes are input using their ASCII values however alternate declarations are also valid.

#define FxRCHAR ASCII_value

#### Default State:  
The default state of the gamepad is determined by a global variable. If the active variable is set to 1, the gamepad will be in gameplay mode upon powering on. If the active variable is set to 0, the gamepad will be automatically set to debugging mode upon powering on.

int active = 1_or_0;

#### Switching Gamepad States:
The current state of the gamepad can be changed without having to be reprogrammed by entering a key combination. Pressing and holding the FxR, FxL, and the Start button will change the gamepad mode.

#### Enabling and Using LED's:
LED functionality is already implemented in the program however all defined pin assignments are set to NULL by default. If you are using LED's, replace the NULL pin assignments to the LED drive pin you are using.

#define FxRLED LED_pin_assignment
