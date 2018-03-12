#include "InternetButton/InternetButton.h"

InternetButton b = InternetButton();

unsigned long startTime = 0;
int ledNum = 0;
bool checkFlag = false;
bool subFlag = false;

void setup() {
    //initiating Internet button
    b.allLedsOff();
    b.begin();
    initTimer();
}

void loop() {
    checkButton();
}//main Application Loop

void timerMain(){
  while(b.buttonOn(3) == 0){
    if(millis()-startTime > 750UL && checkFlag){//750UL = .75seconds, ex: 327000UL = 5.45 minutes (at 5.45 minutes this will make a work session around 45 minutes with a 15 minute break)
        if(ledNum > 10){
            checkFlag = false;
            subFlag = true;
            alarmOn();
            break;
        }else if(ledNum > 7){
            b.ledOn(++ledNum, 100,0,0); //ex: b.ledOn(ledNumber, red, green, blue) feel free to Modify these colors
        }else{
            b.ledOn(++ledNum, 105, 0,255); // feel free to modify these colors :)
        }
        startTime = millis();
    }
  }
  b.allLedsOff();
  initTimer();
}//timerMain() is end of "work" cycle

void alarmOn(){
    while(b.buttonOn(3) == 0 && subFlag){
        b.rainbow(5);
        b.playSong("E5,2,G5,8,E6,8,C6,4,D6,8,G6,8,E5,2,G5,8,E6,8,C6,4,D6,8,G6,8");
        Particle.process();
    }
}//alarmOn function = end of full cycle

void initTimer(){
    ledNum = 0;
    b.ledOn(ledNum, 105,0,255);
    startTime = millis();
    checkFlag = false;
    subFlag = false;
}//initializing function for variables

void checkButton(){
  if(b.buttonOn(1)){
    b.allLedsOff();
    b.ledOn(1, 15, 0, 0);
    b.ledOn(11, 15, 0, 0);
    b.playNote("G3",8);
    b.allLedsOff();
  };

  if(b.buttonOn(2)){
    b.allLedsOff();
    b.ledOn(3, 15, 0, 0);
    b.playNote("G4",8);
    alarmTrigger();
    b.allLedsOff();
  };

  if(b.buttonOn(3)){
    b.allLedsOff();
    b.ledOn(6, 15, 0, 0);
    b.playNote("G5",8);
    b.allLedsOff();
  };

  if(b.buttonOn(4)){
    b.allLedsOff();
    b.ledOn(9, 15, 0, 0);
    b.playNote("G6",8);
    b.allLedsOff();
  };

}//checkButton() is for Flag Checking

void alarmTrigger(){
    b.allLedsOff();
    checkFlag = !checkFlag;
    timerMain();
}
