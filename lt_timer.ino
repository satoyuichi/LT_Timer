#include <LedControl.h>

#define START_BUTTON 2
#define END_BUTTON 3
#define RING_BELL 4

#define SELECT_SWITCH 5

#define ADDITIONAL_TIME 300UL

LedControl lc = LedControl(12, 10, 11, 1);

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
  pinMode(RING_BELL, OUTPUT);
  pinMode(SELECT_SWITCH, INPUT);

  attachInterrupt(digitalPinToInterrupt(START_BUTTON), onStartButton, FALLING);
  attachInterrupt(digitalPinToInterrupt(END_BUTTON), onEndButton, FALLING);

  isLT = true;
  beginTime = millis();
  endTime = 5UL * 60UL * 1000UL + beginTime + ADDITIONAL_TIME;
  isCounting = true;
}

void loop() {
  bool isRing = false;

  if (endTime <= millis()) {
    isCounting = false;
  }

  if (isCounting) {
    t = endTime - millis();

    if (isLT) {
      isRing = ring(t, 2500UL * 60UL + 100UL, 100UL) |
      ring(t, 1000UL * 60UL + 300UL, 100UL) |
      ring(t, 1000UL * 60UL + 100UL, 100UL) |
      ring(t, 500UL, 100UL) |
      ring(t, 300UL, 100UL) |
      ring(t, 100UL, 100UL);
    }
    else {
    }
  }

  if (isRing) {
    digitalWrite(RING_BELL, HIGH);
  }
  else {
    digitalWrite(RING_BELL, LOW);
  }

  display7Seg( t );
}

void display7Seg(unsigned long t) {
  unsigned long min = t / 1000 / 60;
  unsigned long sec = (t - (min * 60 * 1000)) / 1000;

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
  endTime = isLT ? (5UL * 60UL * 1000UL + beginTime + ADDITIONAL_TIME) : (15UL * 60UL * 1000UL + beginTime + ADDITIONAL_TIME);
  isCounting = true;
}

void onEndButton() {
  Serial.println ("onEndButton");

  attachInterrupt(digitalPinToInterrupt(START_BUTTON), onStartButton, FALLING);
  isCounting = false;
}

bool ring(unsigned long t, unsigned long start_t, unsigned long until) {
  if ((start_t >= t) && (t > (start_t - until))) {
    return true;
  }
  else {
    return false;
  }
}
