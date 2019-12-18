#include <LedControl.h>

#define START_BUTTON 2
#define END_BUTTON 2

#define SELECT_SWITCH 4

LedControl lc = LedControl(12,11,10,1);

bool isLT;
bool isCounting = false;
bool isEnd = false;
unsigned long beginTime, endTime;
unsigned long t;

void setup() {
  Serial.begin(9600);

  lc.shutdown(0,false);
  lc.setIntensity(0,8);
  lc.clearDisplay(0);

  pinMode(START_BUTTON, INPUT_PULLUP);
  pinMode(END_BUTTON, INPUT_PULLUP);
  pinMode(SELECT_SWITCH, INPUT);

  attachInterrupt(digitalPinToInterrupt(START_BUTTON), onStartButton, FALLING);
  attachInterrupt(digitalPinToInterrupt(END_BUTTON), onEndButton, FALLING);
}

void loop() {
  if (endTime <= millis()) {
    isCounting = false;
  }

  if (isCounting) {
    t = endTime - millis();

    if (isLT) {
      if ((2.5 * 60 * 1000) < t) {
      }
      else if (((1UL * 60UL * 1000UL) < t) && (t < (2.5 * 60 * 1000))) {
      }
      else if (((59UL * 1000UL) < t) && (t < (1UL * 60UL * 1000UL))) {
      }
    }
    else {
      if ((5UL * 60UL * 1000UL) < t) {
      }
      else if (((1UL * 60UL * 1000UL) < t) && (t < (5UL * 60UL * 1000UL))) {
      }
      else if (((59UL * 1000UL) < t) && (t < (1UL * 60UL * 1000UL))) {
      }
    }
  }

  display7Seg( t );
}

void display7Seg(unsigned long t) {
  unsigned long min = t / 1000 / 60;
  unsigned long sec = (t - (min * 60 * 1000)) / 1000;

//  Serial.println (min);
//  Serial.println (sec);

  String strMin = String(min);
  String strSec = String(sec);

  bool isDp = sec & 0x01;

  if(min < 10) {

    lc.setChar(0, 0, ' ', false);
    lc.setChar(0, 4, ' ', false);

    if (min == 0) {
      lc.setChar(0, 1, ' ', isDp);
      lc.setChar(0, 5, ' ', isDp);
    }
    else {
      lc.setDigit(0, 1, strMin.charAt(0) - '0', isDp);
      lc.setDigit(0, 5, strMin.charAt(0) - '0', isDp);
    }
  }
  else {
    lc.setDigit(0, 0, strMin.charAt(0) - '0', false);
    lc.setDigit(0, 1, strMin.charAt(1) - '0', isDp);
    lc.setDigit(0, 4, strMin.charAt(0) - '0', false);
    lc.setDigit(0, 5, strMin.charAt(1) - '0', isDp);
  }

  if(sec < 10) {
    lc.setDigit(0, 2, 0,false);
    lc.setDigit(0, 3, strSec.charAt(0) - '0', false);
    lc.setDigit(0, 6, 0,false);
    lc.setDigit(0, 7, strSec.charAt(0) - '0', false);
  }
  else {
    lc.setDigit(0, 2, strSec.charAt(0) - '0', false);
    lc.setDigit(0, 3, strSec.charAt(1) - '0', false);
    lc.setDigit(0, 6, strSec.charAt(0) - '0', false);
    lc.setDigit(0, 7, strSec.charAt(1) - '0', false);
  }
}

void onStartButton() {
  detachInterrupt(digitalPinToInterrupt(START_BUTTON));
  Serial.println ("onStartButton");

  isLT = digitalRead(SELECT_SWITCH);
  beginTime = millis();
  endTime = isLT ? (5UL * 60UL * 1000UL + beginTime) : (15UL * 60UL * 1000UL + beginTime);
  isCounting = true;
}

void onEndButton() {
  Serial.println ("onEndButton");

  attachInterrupt(digitalPinToInterrupt(START_BUTTON), onStartButton, FALLING);
  isCounting = false;
}
