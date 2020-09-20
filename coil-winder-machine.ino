/*
   The Coil-winder machine 
   https://github.com/kapion/coil-winder-machine.git

   
   AlexNuno Technologies (c)


   NORMAL MODE {
    //direction depends on reverseKey
    single click - start/stop motor
    when stop and hold - main counter to 0
    when stop and double click - switch to SETUP MODE (display out Edit)
   } 

   SETUP MODE {
    //direction depends on reverseKey
    single click - increase/decrease stopCount value per 100
    hold - increase/decrease stopCount value per 500
    double click - store stopCount value
   }

   ALL MODES {
    switch reverse is working
   }
*/

#define CLK 2
#define DIO 3
#define HERCON 4
#define hallSensorPin 5
#define BUTTON 7
#define PIN_RELAY 10

#include "GyverTM1637.h" //digital display
#include "GyverButton.h" //buttons

GyverTM1637 disp(CLK, DIO);
GButton btn(BUTTON);

boolean reverse; //reverseKey
boolean switchOn; //relay ON
boolean herconState;
boolean buttonState;
boolean savedHerconState;
boolean savedButtonState;

int hallState = LOW;;
int prevHallState = HIGH;

boolean setupMode;

int stopCount = 7000;
uint32_t btnTimer = 0;

int oneStepSetup = 100;
int longStepSetup = 500;

int levelLight = 7;
boolean toDark = true; //need to dark

int mainCounter = 0;

void setup() {
  Serial.begin(9600);
  disp.clear();
  disp.brightness(levelLight);  // bright, 0 - 7 

  btn.setDebounce(50);        // debounce
  btn.setTimeout(500);        // time out for hold 
  
  pinMode(HERCON, INPUT_PULLUP);
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(PIN_RELAY, OUTPUT);
  digitalWrite(PIN_RELAY, HIGH); // relay off
  pinMode(hallSensorPin, INPUT_PULLUP);     

  attachInterrupt(digitalPinToInterrupt(BUTTON), isr, CHANGE);
  
  welcomeText();

  disp.displayInt(mainCounter);
}

void isr() {
  btn.tick();  // check the interrupt to catch the button press anyway
}

void loop() {
  body();
}


void showEdit() {
  // printout Edit
  disp.displayByte(_E, _d, _i, _t);

  uint32_t current = millis();
  while (millis () - current < 1000) { 
    for (int i = 7; i > 0; i--) { // change bright to dark
      disp.brightness(i);   
      delay(40);
    }
    for (int i = 0; i < 8; i++) { // change bright to light
      disp.brightness(i); 
      delay(40);
    }
  }
}

void showSave() {
  // printout Stor
  disp.displayByte(_S, _t, _o, _r);
  delay(1000);
}

int brightNormal() {
  levelLight = 7;
  toDark = true;
  return levelLight;
}

int bright() {
  if (levelLight>=7) 
      toDark = true;
  if (levelLight<=0) 
      toDark = false;
  
  if (toDark) 
   levelLight--;
  else 
   levelLight++;

  return levelLight;
}


void body() {

    //check button press
    btn.tick();
    bool isHold = btn.isHold();
    bool isSingle = btn.isSingle();
    bool isDouble = btn.isDouble();

    //direction mode
    reverse = !digitalRead(HERCON);
  
    if (setupMode && !switchOn) {
      disp.brightness(bright());   
      
      if (isHold && !reverse) {
        stopCount+=longStepSetup;
        Serial.println("stopCount up "+String(stopCount));
        delay(500);
      }
      if (isHold && reverse) {
        stopCount-=longStepSetup;
        Serial.println("stopCount down "+String(stopCount));
         delay(500);
      }

      if (isSingle && !reverse) {
        stopCount+=oneStepSetup;
        Serial.println("stopCount up "+String(stopCount));
      }
      
      if (isSingle && reverse) {
        stopCount-=oneStepSetup;
        Serial.println("stopCount down "+String(stopCount));
      }

      //apply limits
      if (stopCount > 9900)
        stopCount = 9900;
      else if (stopCount < 0) 
        stopCount = 0;
    
      disp.displayInt(stopCount);
    }

    //switch MODE
    if (isDouble && !switchOn)  {
       setupMode = !setupMode;
       if (setupMode) {
         Serial.println("setup MODE");
         showEdit();
       } else {
         Serial.println("normal MODE"); 
         disp.brightness(brightNormal());  
         showSave();
       }
       disp.displayInt(mainCounter);
    }
    
    //clean counter
    if (isHold && !switchOn && !setupMode) { 
      Serial.println("clean counter");
      mainCounter = 0;    
      disp.displayInt(mainCounter);
      delay(500);
    }

    //relay action
    if (isSingle && !setupMode) {
       Serial.println("switchOn press");
       switchOn = !switchOn;
       relayAction();
    }


    //main count rotates
    if (switchOn && !setupMode) {
      hallState = digitalRead(hallSensorPin);
        if ((prevHallState == HIGH) && (hallState == LOW)) {         
           if (reverse) {
            --mainCounter;
           } else {
            ++mainCounter;
           }
           disp.displayInt(mainCounter);
           
           //stop on count
           if (mainCounter == stopCount) { switchOn = false; relayAction(); }
          }
        prevHallState = hallState;
    }
  
}


void relayAction() {
       if (switchOn) {
         Serial.println("switchOn is true");
         digitalWrite(PIN_RELAY, LOW);
       }
       else {
         Serial.println("switchOn is false");
         digitalWrite(PIN_RELAY, HIGH);
       }
}

void welcomeText() {
    byte welcome_banner[] = {_H, _E, _L, _L, _O};
    disp.runningString(welcome_banner, sizeof(welcome_banner), 200);  // 200 это время в миллисекундах!
}
