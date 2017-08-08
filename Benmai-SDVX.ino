//A drop in replacement program for the Benmai SDVX controller tutorial
#include <Keyboard.h>
#include <Mouse.h>

//Debounce interval (milliseconds) - increase value if debug mode indicates more than one state change per press
//Typical debounce interveral 10 - 100ms (Cherry MX - 10ms)
#define debounce 10

//LED mode define - uncomment/define if using LED backlighting on buttons
//#define LED

//Keybindings - Adjust hex ASCII value to change bindings
#define FxRCHAR 0x61
#define FxLCHAR 0x62
#define BTACHAR 0x63
#define BTBCHAR 0x64
#define BTCCHAR 0x65
#define BTDCHAR 0x66  
#define STARTCHAR 0x67

//Hardware pin declarations
#define FxRPIN 4
#define FxLPIN A0
#define BTAPIN A1
#define BTBPIN A2
#define BTCPIN A3
#define BTDPIN A4
#define STARTPIN A5
#define encoderPinA 0
#define encoderPinB 1
#define encoderPinC 3
#define encoderPinD 2

//LED pin declarations
//Change values from NULL to pin assignments if using LED backlighting
#define FxRLED NULL
#define FxLLED NULL
#define BTALED NULL
#define BTBLED NULL
#define BTCLED NULL
#define BTDLED NULL
#define STARTLED NULL

//Variable declarations
int encoderPos[] = {0,0};
static boolean rotating[] = {false,false};

boolean A_set = false;
boolean B_set = false;
boolean C_set = false;
boolean D_set = false;

int FxRFLAG, FxLFLAG, BTAFLAG, BTBFLAG, BTCFLAG, BTDFLAG, STARTFLAG = 0;
int MODEFLAG = 0;
unsigned long FxRTIMER, FxLTIMER, BTATIMER, BTBTIMER, BTCTIMER, BTDTIMER, STARTTIMER = 0;
unsigned long MODETIMER = 0;

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

  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);

  attachInterrupt(0, doEncoderC, CHANGE);
  attachInterrupt(1, doEncoderD, CHANGE);
  attachInterrupt(2, doEncoderA, CHANGE);
  attachInterrupt(3, doEncoderB, CHANGE);

  Serial.begin(9600);
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

void doEncoderA()
{
  if( digitalRead(encoderPinA) != A_set )
  {  
    A_set = !A_set;
    if ( A_set && !B_set ){
      encoderPos[0] += 1;
    }
    rotating[0] = false;  
  }
}
 
void doEncoderB()
{
  if( digitalRead(encoderPinB) != B_set ) {
    B_set = !B_set;
    if( B_set && !A_set ){
      encoderPos[0] -= 1;
    }
    rotating[0] = false;
  }
}
 
void doEncoderC()
{
  if( digitalRead(encoderPinC) != C_set )
  {  
    C_set = !C_set;
    if ( C_set && !D_set ){
      encoderPos[1] += 1;
    }
    rotating[1] = false;
  }
}
 
void doEncoderD()
{
  if( digitalRead(encoderPinD) != D_set ) {
    D_set = !D_set;
    if( D_set && !C_set ){
      encoderPos[1] -= 1;
    }
    rotating[1] = false;
  }
}

