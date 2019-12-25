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

  {200,250}
};
DWAnimation animFlickerOn("flicker_on", flickerOnFrames, sizeof(flickerOnFrames));

const DWAnimationFrame flickerOffFrames[] = {
  {1,100},
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
  {160,2},
  {170,30},
};
DWAnimation animFlickerOff("flicker_off", flickerOffFrames, sizeof(flickerOffFrames));

const DWAnimationFrame fireplaceOnFrames[] = {
  {1,10},
  {30,20},
  {60,30},
  {90,40},
  {120,50},
  {150,60},
  {180,70},
  {210,80},
  {240,100},
};
DWAnimation animFireplaceOn("fireplace_on", fireplaceOnFrames, sizeof(fireplaceOnFrames));

const DWAnimationFrame fireplaceThrobFrames[] = {
  {1,100},
  {120,100},
  {122,80},
  {124,120},
  {242,100},
  {244,150},
  {360,120},
  {362,180},
  {480,150},
  {600,120},
  {720,100},
  {722,120},
  {724,100},
};
DWAnimation animFireplaceThrob("fireplace_throb", fireplaceThrobFrames, sizeof(fireplaceThrobFrames));

const DWAnimationFrame fireplaceOffFrames[] = {
  {1,80},
  {30,70},
  {60,60},
  {90,50},
  {120,30},
  {150,20},
  {180,10},
  {210,5},
};
DWAnimation animFireplaceOff("fireplace_off", fireplaceOffFrames, sizeof(fireplaceOffFrames));


#endif // davelopwarexmasanimations_h
