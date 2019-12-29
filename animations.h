/*
 *     --== Davelopware Arduino XMas Light Animations ==--
 * 
 * Animated light effects such as for led christmas card effects
 * 
 * Copyright: Dave Amphlett 2019
 * 
 */

#ifndef davelopwarexmasanimations_h
#define davelopwarexmasanimations_h

#include "Arduino.h"
#include "dwlight.h"

//DWAnimation animFlickerOn;
//DWAnimation animFlickerOff;


const DWAnimationFrame flickerOnFrames[] = {
  {1,30},
  {3,2},
  {6,30},
  {7,5},
  {8,30},
  {9,5},
  {10,30},
  {12,5},
  
  {50,50},
  {51,10},
  {52,50},
  {53,10},
  {54,50},
  
  {100,100},
  {102,30},
  {104,100},

  {200,255}
};
DWAnimation animFlickerOn("flicker_on", flickerOnFrames, sizeof(flickerOnFrames));

const DWAnimationFrame flickerOffFrames[] = {
  {0,100},
  {2,30},
  {3,100},

  {100,50},
  {101,10},
  {102,50},
  {103,10},
  {104,50},

  {150,5},
  {151,30},
  {152,5},
  {153,30},
  {154,5},
  {156,30},
  {200,0}
};
DWAnimation animFlickerOff("flicker_off", flickerOffFrames, sizeof(flickerOffFrames));

const DWAnimationFrame fireplaceOnFrames[] = {
  {0,0},
  {300,30},
  {700,50},
  {900,100},
  {1000,255}
};
DWAnimation animFireplaceOn("fireplace_on", fireplaceOnFrames, sizeof(fireplaceOnFrames));

const DWAnimationFrame fireplaceThrobFrames[] = {
  {0,255},
  {50,80},
  {120,100},
  {180,30},
  {200,255},
  {1000,255}
};
DWAnimation animFireplaceThrob("fireplace_throb", fireplaceThrobFrames, sizeof(fireplaceThrobFrames));

const DWAnimationFrame fireplaceOffFrames[] = {
  {0,255},
  {500,100},
  {1000,80},
  {2000,50},
  {5000,10}
};
DWAnimation animFireplaceOff("fireplace_off", fireplaceOffFrames, sizeof(fireplaceOffFrames));


#endif // davelopwarexmasanimations_h
