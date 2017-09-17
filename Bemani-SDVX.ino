//A drop in replacement program for the Benmai SDVX controller tutorial
#include <Keyboard.h>
#include <Mouse.h>
#include "Encoder.h"

//Debounce interval (milliseconds) - increase value if debug mode indicates more than one state change per press
//Typical debounce interveral 10 - 100ms (Cherry MX - 10ms)
#define debounce 50

//LED mode define - uncomment/define if using LED backlighting on buttons
#define LED

//Keybindings - Adjust hex ASCII value to change bindings
#define FxRCHAR 0x6D //m
#define FxLCHAR 0x63 //c
#define BTACHAR 0x64 //d
#define BTBCHAR 0x66 //f
#define BTCCHAR 0x6A //j
#define BTDCHAR 0x6B //k
#define STARTCHAR 0x31 //1

//Hardware pin declarations
#define FxRPIN 4
#define FxLPIN A0
#define BTAPIN A1
#define BTBPIN A2
#define BTCPIN A3
#define BTDPIN A4
#define STARTPIN A5
#define E1A 0
#define E1B 1
#define E2A 2
#define E2B 3

//LED pin declarations
//Change values from NULL to pin assignments if using LED backlighting
#define FxRLED 10
#define FxLLED 11
#define BTALED 9
#define BTBLED 8
#define BTCLED 7
#define BTDLED 6
#define STARTLED 5

//Variable declarations
int FxRFLAG, FxLFLAG, BTAFLAG, BTBFLAG, BTCFLAG, BTDFLAG, STARTFLAG = 0;
int MODEFLAG = 0;
unsigned long FxRTIMER, FxLTIMER, BTATIMER, BTBTIMER, BTCTIMER, BTDTIMER, STARTTIMER = 0;
unsigned long MODETIMER = 0;

Encoder encoderLeft(E2A, E2B);
Encoder encoderRight(E1B, E1A);

//Default state variable - Adjust to change default state (0 = debug, 1 = gamepad)
int active = 1;

void setup() {
  pinMode(FxRPIN, INPUT_PULLUP);
  pinMode(FxLPIN, INPUT_PULLUP);
  pinMode(BTAPIN, INPUT_PULLUP);
  pinMode(BTBPIN, INPUT_PULLUP);
  pinMode(BTCPIN, INPUT_PULLUP);
  pinMode(BTDPIN, INPUT_PULLUP);
  pinMode(STARTPIN, INPUT_PULLUP);

  pinMode(FxRLED, OUTPUT);
  pinMode(FxLLED, OUTPUT);
  pinMode(BTALED, OUTPUT);
  pinMode(BTBLED, OUTPUT);
  pinMode(BTCLED, OUTPUT);
  pinMode(BTDLED, OUTPUT);
  pinMode(STARTLED, OUTPUT);
  
  Serial.begin(115200);
}

void loop() {
  libInit();
  kbPress(FxRPIN, &FxRFLAG, FxRCHAR, &FxRTIMER, FxRLED);
  kbPress(FxLPIN, &FxLFLAG, FxLCHAR, &FxLTIMER, FxLLED);
  kbPress(BTAPIN, &BTAFLAG, BTACHAR, &BTATIMER, BTALED);
  kbPress(BTBPIN, &BTBFLAG, BTBCHAR, &BTBTIMER, BTBLED);
  kbPress(BTCPIN, &BTCFLAG, BTCCHAR, &BTCTIMER, BTCLED);
  kbPress(BTDPIN, &BTDFLAG, BTDCHAR, &BTDTIMER, BTDLED);
  kbPress(STARTPIN, &STARTFLAG, STARTCHAR, &STARTTIMER, STARTLED);
  leftEnc();
  rightEnc();
}

void libInit(){
  if(digitalRead(FxRPIN) == LOW && digitalRead(FxLPIN) == LOW && digitalRead(STARTPIN) && active == 0 && millis()-MODETIMER > debounce && MODEFLAG == 0){
    active = 1;
    Keyboard.begin();
    Mouse.begin();
    Serial.println("Gamepad Mode");
    Serial.println("Mouse & keyboard active.");
    FxRFLAG=FxLFLAG=BTAFLAG=BTBFLAG=BTCFLAG=BTDFLAG=STARTFLAG=0;
    Serial.println("Flags reset.");
    MODEFLAG = 1;
  }
  else if(digitalRead(FxRPIN) == LOW && digitalRead(FxLPIN) == LOW && digitalRead(STARTPIN) && active == 1 && millis()-MODETIMER > debounce && MODEFLAG == 1){
    active = 0;
    Keyboard.releaseAll();
    Keyboard.end();
    Mouse.end();
    Serial.println("Debug Mode");
    Serial.println("Mouse & keyboard inactive.");
    FxRFLAG=FxLFLAG=BTAFLAG=BTBFLAG=BTCFLAG=BTDFLAG=STARTFLAG=0;
    Serial.println("Flags reset.");
    MODEFLAG = 0;
  }
}

void kbPress(int pin, int *flag, int key, unsigned long *timer, int led){
  if(digitalRead(pin) == LOW && *flag == 0){
    if(active == true){
      Keyboard.press(key);
      digitalWrite(led, HIGH);
    }
    else{
      Serial.print("Pin: ");
      Serial.print(pin);
      Serial.println(" is LOW");
    }
    *flag = 1;
    *timer = millis();
  }
  if(digitalRead(pin) == HIGH && *flag == 1 && millis()-*timer > debounce){
    if(active == true){
      Keyboard.release(key);
      digitalWrite(led, LOW);
    }
    else{
      Serial.print("Pin: ");
      Serial.print(pin);
      Serial.println(" is HIGH");
    }
    *flag = 0;
  }
}


void leftEnc(){
  Mouse.move(encoderLeft.read(), 0, 0);
  encoderLeft.write(0);
}

void rightEnc() {
  Mouse.move(0, encoderRight.read(), 0);
  encoderRight.write(0);
}

