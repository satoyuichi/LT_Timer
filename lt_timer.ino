#include <LedControl.h>

#define PUSH_BUTTON 2
#define SELECT_SWITCH 4

LedControl lc = LedControl(12,11,10,1);

bool isLT;
bool isCounting = false;
bool isEnd = false;
unsigned long beginTime, endTime;

void setup() {
  Serial.begin(9600);

  lc.shutdown(0,false);
  lc.setIntensity(0,8);
  lc.clearDisplay(0);

  pinMode(PUSH_BUTTON, INPUT);
  pinMode(SELECT_SWITCH, INPUT);

  attachInterrupt(digitalPinToInterrupt(PUSH_BUTTON), onReleaseButton, RISING);
}

void loop() {
  unsigned long t = endTime - millis();

  if (endTime <= millis()) {
    isCounting = false;
  }

  if (isCounting) {
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
}

void onReleaseButton() {
  isLT = digitalRead(SELECT_SWITCH);
  beginTime = millis();
  endTime = isLT ? (5UL * 60UL * 1000UL + beginTime) : (15UL * 60UL * 1000UL + beginTime);
  isCounting = !isCounting;
}
