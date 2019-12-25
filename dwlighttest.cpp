#include "Arduino.h"
#include "dwlighttest.h"

//#define DWLIGHTTEST 1

#ifdef DWLIGHTTEST

void dwlighttest_setup() {
  delay(1000);
  Serial.begin(115200);
  delay(1000);
}

DWAnimationFrame simpleFrames[] = {
  {1,10},
  {5,20},
  {10,20}
};
DWAnimation animSimple("simple", simpleFrames, sizeof(simpleFrames));

DWAnimationFrame flickerOnFrames[] = {
  {1,30},
  {5,2},
  {10,30},
  {12,5},
  {14,30},
  {16,5},
  {18,30},
  {20,5},
  
  {100,50},
  {102,10},
  {104,50},
  {106,10},
  {108,50},
  
  {200,100},
  {202,30},
  {204,100},

  {500,250}
};
DWAnimation animFlickerOn("flicker_on", flickerOnFrames, sizeof(flickerOnFrames));

void dwlighttest_basic() {
  Serial.println("DW LIGHT TEST=====");
  DWLight light(-1, true);
  light.setup();
  light.debugDump();
//  light.doStep();
//  light.debugDump();
//  light.setModeSimple(DWLightModeOn);
//  light.debugDump();
//  light.doStep();
//  light.debugDump();
//  light.doStep();
//  light.debugDump();
//  light.setModeAnimation(&animFlickerOn);
//  light.doStep();
//  light.debugDump();
//  for(int i = 0; i < 20; i++) {
//    light.doStep();
//  }
  light.setAnimationEndCallback(dwlighttest_callbackHandler);
  light.setModeAnimation(&animSimple);
  for(int i = 0; i < 50; i++) {
    Serial.println("v");
    light.doStep();
  }
  
}

void dwlighttest_callbackHandler(void* pdwlight) {
  DWLight* _pdwlight = reinterpret_cast<DWLight*>(pdwlight);
  Serial.println("dwlighttest_callbackHandler called...");
//  _pdwlight->debugDump();
  _pdwlight->resetStep();

  if (_pdwlight->getMode() == DWLightModeAnimation && _pdwlight->getAnimation() == &animSimple) {
    _pdwlight->setModeSimple(DWLightModeOn, 3);
  } else if (_pdwlight->getMode() == DWLightModeOn) {
    _pdwlight->setModeSimple(DWLightModeOff, 5);
  } else {
    _pdwlight->setModeSimple(DWLightModeOn, 10);
  }
}


#else

void dwlighttest_setup() {}

void dwlighttest_basic() {}

#endif // DWLIGHTTEST
