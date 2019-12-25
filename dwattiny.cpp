
#include "dwattiny.h"

bool dwattiny_debugLightOn = false;

void dwattiny_toggleDebugLight(int wait = 0) {
  dwattiny_debugLightOn = ! dwattiny_debugLightOn;
  digitalWrite(T85Pin3Dout, ( dwattiny_debugLightOn ? HIGH : LOW ));
  if (wait > 0) {
    delay(wait);
  }
}

