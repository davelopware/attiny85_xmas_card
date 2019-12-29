
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

void callbackRestart(void* pdwlight) {
  DWLight* plight = reinterpret_cast<DWLight*>(pdwlight);

  plight->resetStep();
    if (plight == &lightStreet) {
      plight->setModeAnimateInterpolate(&animFireplaceOn);
    } else {
      plight->setModeAnimate(&animFireplaceOn);
    }
    return;

  if (plight->getAnimation() == &animFireplaceOn) {
    if (plight == &lightStreet) {
      plight->setModeAnimateInterpolate(&animFireplaceOff);
    } else {
      plight->setModeAnimate(&animFireplaceOff);
    }
  } else {
    if (plight == &lightStreet) {
      plight->setModeAnimateInterpolate(&animFlickerOn);
    } else {
      plight->setModeAnimate(&animFlickerOn);
    }
  }
  
}
void setup() {
  dwlighttest_setup();
//  dwlighttest_basic();

  lightStreet.setup();
//  lightStreet.setAnimationEndCallback(callbackLightStreetSequence);
  lightStreet.setAnimationEndCallback(callbackRestart);
  lightHouseDown.setup();
//  lightHouseDown.setAnimationEndCallback(callbackLightHouseDownSequence);
  lightHouseDown.setAnimationEndCallback(callbackRestart);

  delay(10);
  pinMode(T85Pin2Dout, OUTPUT);


  lightStreet.test(200);
  lightHouseDown.test(200);

//  lightStreet.setModeAnimate(&animFlickerOn);
  lightStreet.setModeAnimateInterpolate(&animFireplaceOn);
  lightHouseDown.setModeAnimate(&animFireplaceOn);
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

  if (plight->getMode() == DWLightModeAnimate && plight->getAnimation() == &animFlickerOn) {
    plight->setModeSimple(DWLightModeOn, 625);
    
  } else if (plight->getMode() == DWLightModeOn) {
    plight->setModeAnimate(&animFlickerOff);
    
  } else if (plight->getMode() == DWLightModeAnimate && plight->getAnimation() == &animFlickerOff) {
    plight->setModeSimple(DWLightModeOff, 625);
    
  } else if (plight->getMode() == DWLightModeOff) {
    plight->setModeAnimate(&animFlickerOn);

  } else {
    plight->setModeSimple(DWLightModeOff, 124);
  }
}

#define FIREPLACE_THROB_REPEAT 11
short fireplayThrobCount = 0;
void callbackLightHouseDownSequence(void* pdwlight) {
  DWLight* plight = reinterpret_cast<DWLight*>(pdwlight);

  plight->resetStep();

  if (plight->getAnimation() == &animFireplaceOn) {
if (pdwlight == &lightStreet) plight->setModeAnimateInterpolate(&animFireplaceThrob); else 
    plight->setModeAnimate(&animFireplaceThrob);
    fireplayThrobCount = 0;
    
  } else if (plight->getAnimation() == &animFireplaceThrob) {
    fireplayThrobCount++;
    if (fireplayThrobCount > FIREPLACE_THROB_REPEAT) {
if (pdwlight == &lightStreet) plight->setModeAnimateInterpolate(&animFireplaceOff); else 
      plight->setModeAnimate(&animFireplaceOff);
      fireplayThrobCount = 0;
    }
    
  } else if (plight->getAnimation() == &animFireplaceOff) {
    plight->setModeSimple(DWLightModeOff, 625);
    
  } else if (plight->getMode() == DWLightModeOff) {
if (pdwlight == &lightStreet) plight->setModeAnimateInterpolate(&animFireplaceOn); else 
    plight->setModeAnimate(&animFireplaceOn);
    
  } else {
    plight->setModeSimple(DWLightModeOff, 62);
  }
}



