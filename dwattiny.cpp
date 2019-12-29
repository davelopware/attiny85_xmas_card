
#include "dwattiny.h"

bool dwattiny_debugLightOn = false;

void dwattiny_toggleDebugLight(int wait = 0) {
  dwattiny_debugLightOn = ! dwattiny_debugLightOn;
  digitalWrite(T85Pin3Dout, ( dwattiny_debugLightOn ? HIGH : LOW ));
  if (wait > 0) {
    delay(wait);
  }
}

// ==========================================================================================================================
//
// watchdog timer stuff for ATTiny85
//
// copied from https://forum.arduino.cc/index.php?topic=558075.0
//
// ==========================================================================================================================

#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

byte dwattiny_saveADCSRA;                 // variable to save the content of the ADC for later. if needed.
//volatile byte dwattiny_counterWD = 0;     // Count how many times WDog has fired. Used in the timing of the 
                                          // loop to increase the delay before the LED is illuminated. For example,
                                          // if WDog is set to 1 second TimeOut, and the counterWD loop to 10, the delay
                                          // between LED illuminations is increased to 1 x 10 = 10 seconds
void dwattiny_watchdog_setup() {
  noInterrupts ();                        // turn off interrupts as a precaution
  dwattiny_resetWatchDog ();              // do this first in case WDog fires
  interrupts ();                          // turn on interrupts

//  pinMode ( ledPin, OUTPUT );           // I could put to INPUT between sleep_enable() and interrupts()
//                                        // to save more power, then to OUTPUT in the ISR after wdt_disable()
}

void dwattiny_watchdog_loop() {
  dwattiny_watchdog_sleepNow();                   // then set up and enter sleep mode
//  if ( dwattiny_counterWD == 9 )          // if the WDog has fired 10 times......
//  {
//    dwattiny_counterWD = 0;               // reset the counterWD for another 10 WDog firings
//    return true;
//  } else {
//    return false;
//  }
}

void dwattiny_watchdog_sleepNow() {
  set_sleep_mode ( SLEEP_MODE_PWR_DOWN ); // set sleep mode Power Down
  dwattiny_saveADCSRA = ADCSRA;           // save the state of the ADC. We can either restore it or leave it turned off.
  ADCSRA = 0;                             // turn off the ADC
  power_all_disable ();                   // turn power off to ADC, TIMER 1 and 2, Serial Interface
  
  noInterrupts ();                        // turn off interrupts as a precaution
  dwattiny_resetWatchDog ();              // reset the WatchDog before beddy bies
  sleep_enable ();                        // allows the system to be commanded to sleep
  interrupts ();                          // turn on interrupts
  
  sleep_cpu ();                           // send the system to sleep, night night!

  sleep_disable ();                       // after ISR fires, return to here and disable sleep
  power_all_enable ();                    // turn on power to ADC, TIMER1 and 2, Serial Interface
  
  // ADCSRA = dwattiny_saveADCSRA;                 // turn on and restore the ADC if needed. Commented out, not needed.
  
}

void dwattiny_resetWatchDog ()
{
  MCUSR = 0;
  WDTCR = bit ( WDCE ) | bit ( WDE ) | bit ( WDIF ); // allow changes, disable reset, clear existing interrupt
  WDTCR = bit ( WDIE );                              // set WDIE ( Interrupt only, no Reset ) and 16ms TimeOut
//  WDTCR = bit ( WDIE ) | bit ( WDP2 )| bit ( WDP1 ); // set WDIE ( Interrupt only, no Reset ) and 1 second TimeOut
                                                     
  wdt_reset ();                            // reset WDog to parameters
  
}

ISR ( WDT_vect )
{
  wdt_disable ();                           // until next time....
//  dwattiny_counterWD ++;                    // increase the WDog firing counter. Used in the loop to time the flash
//                                            // interval of the LED. If you only want the WDog to fire within the normal 
//                                            // presets, say 2 seconds, then comment out this command and also the associated
//                                            // commands in the if ( counterWD..... ) loop, except the 2 digitalWrites and the
//                                            // delay () commands.
}

