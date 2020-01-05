/*
 * attiny85_xmas_card
 * 
 * Animates led's for inclusion within a christmas card.
 * Prototype project from which animation library will be extracted in future
 * 
 * The Christmas Card
 * ------------------
 *    Locally sourced christmas card with an outdoor snowy christmas village scene
 *    with 3 victorian style street lights and a house next to a church.
 * 
 *    I 'lit' the 3 street lights on one chain of led's direct to an analogue output,
 *    the house downstairs window to a smaller chain of led's direct to the other
 *    analogue output, and the house upstairs window to another small chain of led's
 *    via a 220ohm resistor to a digital output.
 * 
 *    I also used Pin 2 as a digital input with 1k pulldown resistor by putting it
 *    high for manually 'fast-forwarding' through the animations
 * 
 * The ATTiny85 PinOut
 * -------------------
 *                                  ATTiny85
 *                                  -------
 *                                --|1 * 8|-- Vin 
 *       lightHouseUp T85Pin2Dout --|2   7|--
 *   BTN_SPEED_UP_PIN T85Pin3Ain  --|3   6|-- T85Pin6Aout lightStreet
 *                            Gnd --|4   5|-- T85Pin5Aout lightHouseDown
 *                                  -------
 * 
 * Animation Stepping
 * ------------------
 * 
 *    The animations are performed step at a time, by calling loopStep() approx every 16ms.
 * 
 *    When no PWM outputs are active (canSleep() returns true), then the Watchdog timer
 *    is used to sleep the attiny85 completly in between steps (making a considerable power saving).
 * 
 *    However if PWM outputs are active the sleeping the chip would freeze the PWM, so the 'milli-block'
 *    variables and code in loop() are used to implement the 16ms gaps between steps.
 * 
 */
#include "dwlight.h"
#include "animations.h"
#include "dwattiny.h"
//#include "dwlighttest.h"  // only used during debugging and testing


DWLight lightStreet(T85Pin6Aout, true);
DWLight lightHouseDown(T85Pin5Aout, true);
DWLight lightHouseUp(T85Pin2Dout, false);

#define BTN_SPEED_UP_PIN T85Pin3Ain


void setup() {
  // Used during debugging and testing - see dwlighttest.h and dwlighttest.cpp
  //dwlighttest_setup();
  //dwlighttest_basic();

  // Setup the pins and light animations
  lightStreet.setup(callbackLightStreetSequence);
  lightHouseDown.setup(callbackLightHouseDownSequence);
  lightHouseUp.setup(callbackLightHouseUpSequence);
  pinMode(BTN_SPEED_UP_PIN, INPUT);

  // Run the startup test blinking
  // this helps ensure you know which led chain to connect to which pins,
  // and gives users of the card something to see when the first turn it on
  lightStreet.test(100);
  lightHouseDown.test(100);
  lightHouseUp.test(100);

  // Setup the watchdog timer
  dwattiny_watchdog_setup();

  // Kick into the start of the overal animations sequence
  startXmas();
}


// Used to implmenet non-watchdog timer 16ms step gaps while PWM is needed
const int MILLI_BLOCK_COUNT = 16;
long milliBlockStart = 0;


void loop() {

  if (canSleep()) {
    // No PWM currently running, so power-down sleep the chip for approx 16ms then carry on
    dwattiny_watchdog_loop();
    
    long millisNow = millis();
    milliBlockStart = millisNow;
    loopStep();
    
  } else {
    // PWM is currently running, so check if 16ms has elapsed
    long millisNow = millis();
    if (milliBlockStart + MILLI_BLOCK_COUNT < millisNow) {
      milliBlockStart = millisNow;
      loopStep();
    }
  }
}

// After running the overall animations, wait for BETWEEN_NIGHTS_LIMIT steps
//    before starting again 
#define BETWEEN_NIGHTS_LIMIT 18750  // about 5 minutes
bool betweenNights = false;
int betweenNightsCount = 0;

// when BTN_SPEED_UP_PIN is high, then skip forward steps SPEED_UP_RATE at a time
//    enables you to fast-forward through the animations which eases debugging and testing
#define SPEED_UP_RATE 10
bool btnSpeedUpWasPressed = false;

/*
 * sub-loop step - every 16ms ish, either by watchdog timer or MILLI_BLOCK_COUNT in loop
 */
void loopStep() {
  bool btnSpeedUpPressed = digitalRead(BTN_SPEED_UP_PIN);
  bool speedUp = btnSpeedUpPressed;

  // if NOT in the gap between running the animation sequences - run them!
  if ( ! betweenNights ) {
    byte stepsToDo = speedUp ? SPEED_UP_RATE : 1; // apply speedup if necessary
    for (byte step = 0; step < stepsToDo; step++) {
      lightStreet.doStep();
      lightHouseDown.doStep();
      lightHouseUp.doStep();
    }
    if (hasAllStopped()) {
      betweenNights = true;
      betweenNightsCount = 0;
    }

  // else we must be in the 5 minute gap between Nights between running the animation sequences
  } else {
    betweenNightsCount += speedUp ? SPEED_UP_RATE : 1; // apply speedup if necessary
    if (betweenNightsCount >= BETWEEN_NIGHTS_LIMIT) {
      betweenNights = false;
      betweenNightsCount = 0;
      startXmas();
    }
  }
}


/*
 * Returns true if all lights are sleepable (ie if they aren't running PWM's)
 */
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



