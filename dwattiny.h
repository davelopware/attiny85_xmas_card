/*
 *     --== Davelopware Arduino ATTiny Constants ==--
 * 
 * Ease arduino development for ATTiny chips
 * 
 * Implemented so far:
 * 
 *  + ATTiny85
 * 
 * Copyright: Dave Amphlett 2019
 * 
 */

#ifndef davelopwareattiny_h
#define davelopwareattiny_h

#include "Arduino.h"

// ATtiny85 explicit pin mappings
const int T85Pin2Dout = 3;
const int T85Pin2Ain = A3;
const int T85Pin3Dout = 4;
const int T85Pin3Ain = A2;
const int T85Pin5Dout = 0;
const int T85Pin5Aout = 0;
const int T85Pin6Dout = 1;
const int T85Pin6Aout = 1;
const int T85Pin7Dout = 1;
const int T85Pin7Ain = A1;


void dwattiny_toggleDebugLight(int wait = 0);

void dwattiny_watchdog_setup();
void dwattiny_watchdog_loop();
void dwattiny_watchdog_sleepNow();
void dwattiny_resetWatchDog();



#endif // davelopwareattiny_h
