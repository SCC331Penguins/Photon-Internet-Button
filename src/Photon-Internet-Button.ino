// This #include statement was automatically added by the Particle IDE.
#include "InternetButton/InternetButton.h"
InternetButton b = InternetButton();
// ---- Air Quality Index ----
int AirQual []= {0,0,0};
int AirValCount = 0;
int manualDelayTimer = 0;
int manualDelayMax = 3000;

// ---- Alarm ----
unsigned long startTime = 0;
int ledNum = 0;
bool checkFlag = false;
bool subFlag = false;

void setup() {
  Serial.begin(9600);
  b.begin();
  b.allLedsOff();
  initTimer();
  // Subscribe to the webhook response event
  Particle.subscribe("hook-response/AQI", myHandler, MY_DEVICES);
  RGB.control(true);
}

// Handle the webhook response
void myHandler(const char *event, const char *data) {

  RGB.color(10,00,00); //show data is being parsed.
  static int i = 0;
    char dataString[4 * 4 + 3 + 1];
    strncpy(dataString, data, sizeof(dataString));
    i = 0;
    //Split string into tokens with ~
    char * AQI = strtok(dataString, "~");
    while (AQI != NULL) {

        // string to integer conversion operator. atoi
        AirQual[i] = atoi(AQI);
          AQI = strtok(NULL, "~");
        i++; //add one to i then do it again?
        AirValCount=i;
    }

}


void loop() {
    checkButton();
    if(manualDelayTimer == manualDelayMax){
      airQuality();
      manualDelayTimer = 0;
    }
    manualDelayTimer++;
}

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

void airQuality(){
  static unsigned long nextTrigger = 10 * 1000; //wait 10 seconds to run 1st run.
  b.allLedsOff();
    if (nextTrigger < millis()) {
        // polling Webhook every 10 minutes.
        nextTrigger = millis() + 10*60*1000;
        String data = String(10);
        // Trigger the webhook
        Particle.publish("AQI", data, PRIVATE); // Get that data !
    }
    int i =0;

    for ( i = 0; i < AirValCount; i++ ){
        if (i == 0){
            RGB.color(00,00,10); //o3
        }
        if (i == 1){
            RGB.color(10,10,00); //PM 2.5
        }
        if (i == 2){
            RGB.color(10,00,10); //PM 10
        }
            //Set LED Ring accordingly
            //... with a shit load of if statements.
            //What the values are:
            //0-50  Good  Green 0,228,0
            //51-100  Moderate Yellow 255,255,0
            //101-150  Unhealthy Sensitive Groups Orange 255,126,0
            //151-200  Unhealthy Red 255,0,0
            //201-300  Very Unhealthy Purple 153,0,76
            //301-500  Hazardous Maroon 126,0,35
        if (AirQual[i] > 0) {
            b.ledOn(1, 0, 228,00);
        }

        if (AirQual[i] > 25) {
            b.ledOn(2, 0, 228,00);
        }

        if (AirQual[i] > 51) {
            b.ledOn(3, 255,255,0);
        }

        if (AirQual[i] > 75) {
            b.ledOn(4, 255,255,0);
        }

        if (AirQual[i] > 101) {
            b.ledOn(5, 255,126,0);
        }

        if (AirQual[i] > 125) {
            b.ledOn(6, 255,126,0);
        }

        if (AirQual[i] > 151) {
            b.ledOn(7, 255,0,0);
        }

        if (AirQual[i] > 175) {
            b.ledOn(8, 255,0,0);
        }

        if (AirQual[i] > 201) {
            b.ledOn(9, 153,0,76);
        }

        if (AirQual[i] > 225) {
            b.ledOn(10, 153,0,76);
        }

        if (AirQual[i] > 301) {
            b.ledOn(11, 126,0,35);
        }
        //pause for 5 seconds be fore moving to the next value.
        //delay(5000);

    }
}
