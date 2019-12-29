
#include "dwlight.h"
#include "dwlighttest.h"
#include "animations.h"
#include "dwattiny.h"
#include "animations.h"


DWLight lightStreet(T85Pin6Aout, true);
DWLight lightHouseDown(T85Pin5Aout, true);
DWLight lightHouseUp(T85Pin2Dout, false);

#define BTN_SPEED_UP_PIN T85Pin3Ain
#define TEST_DELAY 2

bool canSleep();
bool hasAllStopped();
void startXmas();
void callbackLightStreetSequence(void* pdwlight);
void callbackLightHouseDownSequence(void* pdwlight);
void callbackLightHouseUpSequence(void* pdwlight);



void setup() {
//  dwlighttest_setup();
//  dwlighttest_basic();

  lightStreet.setup(callbackLightStreetSequence);
  lightHouseDown.setup(callbackLightHouseDownSequence);
  lightHouseUp.setup(callbackLightHouseUpSequence);

  pinMode(BTN_SPEED_UP_PIN, INPUT);

  lightStreet.test(100);
  lightHouseDown.test(100);
  lightHouseUp.test(100);

  dwattiny_watchdog_setup();
  
  startXmas();
}


const int MILLI_BLOCK_COUNT = 16;
long milliBlockStart = 0;

void loop() {
  long millisNow = millis();
  if (canSleep()) {
    dwattiny_watchdog_loop();
    milliBlockStart = millisNow;
    loopStep();
  } else {
    millisNow = millis();
    if (milliBlockStart + MILLI_BLOCK_COUNT < millisNow) {
      milliBlockStart = millisNow;
      loopStep();
    }
  }
}

#define BETWEEN_NIGHTS_LIMIT 18750
bool betweenNights = false;
int betweenNightsCount = 0;
bool btnSpeedUpWasPressed = false;

/*
 * sub-loop step - every 16ms ish, either by watchdog timer or MILLI_BLOCK_COUNT in loop
 */
void loopStep() {
  bool btnSpeedUpPressed = digitalRead(BTN_SPEED_UP_PIN);
  bool speedUp = btnSpeedUpPressed;
  
//  if (btnSpeedUpPressed && ! btnSpeedUpWasPressed) {
//    btnSpeedUpWasPressed = true;
//    speedUp = true;
//  } else if ( ! btnSpeedUpPressed && btnSpeedUpWasPressed) {
//    btnSpeedUpWasPressed = false;
//  }
  
  if ( ! betweenNights ) {
    byte stepsToDo = speedUp ? 10 : 1;
    for (byte step = 0; step < stepsToDo; step++) {
      lightStreet.doStep();
      lightHouseDown.doStep();
      lightHouseUp.doStep();
    }
    if (hasAllStopped()) {
      betweenNights = true;
      betweenNightsCount = 0;
    }
  } else {
    betweenNightsCount += speedUp ? 10 : 1;
    if (betweenNightsCount >= BETWEEN_NIGHTS_LIMIT) {
      betweenNights = false;
      betweenNightsCount = 0;
      startXmas();
    }
  }
}

bool canSleep() {
  return
    lightStreet.isSleepable() &&
    lightHouseDown.isSleepable() &&
    lightHouseUp.isSleepable();
}

bool hasAllStopped() {
  return
    lightStreet.getMode() == DWLightModeStop &&
    lightHouseDown.getMode() == DWLightModeStop &&
    lightHouseUp.getMode() == DWLightModeStop;
}

void startXmas() {
  lightStreet.setModeSimple(DWLightModeOff, 5000);
  lightHouseDown.setModeAnimateInterpolate(&animFireplaceOn);
  lightHouseUp.setModeSimple(DWLightModeOff);
  lightHouseUp.doStep();
}

void callbackLightStreetSequence(void* pdwlight) {
  DWLight* plight = reinterpret_cast<DWLight*>(pdwlight);

  plight->resetStep();

  if (plight->getMode() == DWLightModeOff) {
    plight->setModeAnimate(&animFlickerOn);
    
  } else if (plight->getAnimation() == &animFlickerOn) {
    plight->setModeSimple(DWLightModeOn, 15000);
    
  } else if (plight->getMode() == DWLightModeOn) {
    plight->setModeAnimate(&animFlickerOff);
    
  } else if (plight->getAnimation() == &animFlickerOff) {
    plight->setModeSimple(DWLightModeStop);
    
  }
}

#define FIREPLACE_THROB_REPEAT 10
short fireplayThrobCount = 0;
void callbackLightHouseDownSequence(void* pdwlight) {
  DWLight* plight = reinterpret_cast<DWLight*>(pdwlight);

  plight->resetStep();

  if (plight->getAnimation() == &animFireplaceOn) {
    plight->setModeAnimateInterpolate(&animFireplaceThrob);
    fireplayThrobCount = 0;
    
  } else if (plight->getAnimation() == &animFireplaceThrob) {
    fireplayThrobCount++;
    if (fireplayThrobCount >= FIREPLACE_THROB_REPEAT) {
      plight->setModeAnimateInterpolate(&animFireplaceOff);
      fireplayThrobCount = 0;
      lightHouseUp.setModeSimple(DWLightModeOn, 3000);
    }
    
  } else if (plight->getAnimation() == &animFireplaceOff) {
    plight->setModeManual(0,0);
    plight->doStep();
    plight->setModeSimple(DWLightModeStop);
  }
}

void callbackLightHouseUpSequence(void* pdwlight) {
  DWLight* plight = reinterpret_cast<DWLight*>(pdwlight);

  plight->resetStep();

  if (plight->getMode() == DWLightModeOn) {
    plight->setModeSimple(DWLightModeOff, 2);
  } else if (plight->getMode() == DWLightModeOff) {
    plight->setModeSimple(DWLightModeStop);
  }
}



