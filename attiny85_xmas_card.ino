
#include "dwlight.h"
#include "dwlighttest.h"
#include "animations.h"
#include "dwattiny.h"
#include "animations.h"


DWLight lightStreet(T85Pin6Aout, true);
DWLight lightHouseDown(T85Pin5Aout, true);
DWLight lightHouseUp(T85Pin2Dout, false);

#define TEST_DELAY 2

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

  lightStreet.test(100);
  lightHouseDown.test(100);
  lightHouseUp.test(100);

  startXmas();
}


const int MILLI_BLOCK_COUNT = 16;
long milliBlockStart = 0;

void loop() {
  long millisNow = millis();
  if (milliBlockStart + MILLI_BLOCK_COUNT < millisNow) {
    milliBlockStart = millisNow;
    nMilliLoop();
  }
}


void nMilliLoop() {
  lightStreet.doStep();
  lightHouseDown.doStep();
  lightHouseUp.doStep();
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



