
#include "dwlight.h"
#include "dwlighttest.h"
#include "animations.h"
#include "dwattiny.h"
#include "animations.h"


DWLight lightStreet(T85Pin6Aout, true);
DWLight lightHouseDown(T85Pin5Aout, true);
DWLight lightHouseUp(T85Pin2Dout, false);

#define TEST_DELAY 2

void callbackLightStreetSequence(void* pdwlight);
void callbackLightHouseDownSequence(void* pdwlight);

void setup() {
//  dwlighttest_setup();
//  dwlighttest_basic();

  lightStreet.setup();
  lightStreet.setAnimationEndCallback(callbackLightStreetSequence);
  lightHouseDown.setup();
  lightHouseDown.setAnimationEndCallback(callbackLightHouseDownSequence);

  delay(10);
  pinMode(T85Pin2Dout, OUTPUT);


  lightStreet.test(200);
  lightHouseDown.test(200);

  lightStreet.setModeAnimation(&animFlickerOn);
  lightHouseDown.setModeAnimation(&animFireplaceOn);
}


const int MILLI_BLOCK_COUNT = 16;
long milliBlockStart = 0;

void loop() {
  if (milliBlockStart == 0) {
    delay(1000);
  }
  
  long millisNow = millis();
  if (milliBlockStart + 16 < millisNow) {
    milliBlockStart = millisNow;
    nMilliLoop();
  }
}


void nMilliLoop() {
  lightStreet.doStep();
  lightHouseDown.doStep();
//  delay(50);
}

void callbackLightStreetSequence(void* pdwlight) {
  DWLight* plight = reinterpret_cast<DWLight*>(pdwlight);

  plight->resetStep();

  if (plight->getMode() == DWLightModeAnimation && plight->getAnimation() == &animFlickerOn) {
    plight->setModeSimple(DWLightModeOn, 625);
    
  } else if (plight->getMode() == DWLightModeOn) {
    plight->setModeAnimation(&animFlickerOff);
    
  } else if (plight->getMode() == DWLightModeAnimation && plight->getAnimation() == &animFlickerOff) {
    plight->setModeSimple(DWLightModeOff, 625);
    
  } else if (plight->getMode() == DWLightModeOff) {
    plight->setModeAnimation(&animFlickerOn);

  } else {
    plight->setModeSimple(DWLightModeOff, 124);
  }
}

#define FIREPLACE_THROB_REPEAT 10
short fireplayThrobCount = 0;
void callbackLightHouseDownSequence(void* pdwlight) {
  DWLight* plight = reinterpret_cast<DWLight*>(pdwlight);

  plight->resetStep();

  if (plight->getMode() == DWLightModeAnimation && plight->getAnimation() == &animFireplaceOn) {
    plight->setModeAnimation(&animFireplaceThrob);
    fireplayThrobCount = 0;
    
  } else if (plight->getMode() == DWLightModeAnimation && plight->getAnimation() == &animFireplaceThrob) {
    fireplayThrobCount++;
    if (fireplayThrobCount > FIREPLACE_THROB_REPEAT) {
      plight->setModeAnimation(&animFireplaceOff);
      fireplayThrobCount = 0;
    }
    
  } else if (plight->getMode() == DWLightModeAnimation && plight->getAnimation() == &animFireplaceOff) {
    plight->setModeSimple(DWLightModeOff, 625);
    
  } else if (plight->getMode() == DWLightModeOff) {
    plight->setModeAnimation(&animFireplaceOn);
    
  } else {
    plight->setModeSimple(DWLightModeOff, 62);
  }
}



