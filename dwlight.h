/*
 *     --== Davelopware Arduino Light library ==--
 * 
 * Animated light effects such as for led christmas card effects
 * 
 * Copyright: Dave Amphlett 2019
 * 
 */

#ifndef davelopwarelight_h
#define davelopwarelight_h

#include "Arduino.h"

struct DWAnimationFrame {
  short _step;
  byte _value;
  DWAnimationFrame(short step, byte value) { _step = step; _value = value; }
};


class DWAnimation {
  public:
    String            _name;
    DWAnimationFrame* _pframes;
    short             _count;

  public:
    DWAnimation(String name, DWAnimationFrame frames[], int arraySize) {
      _name = name;
      _pframes = (DWAnimationFrame*)frames;
      _count = arraySize / sizeof(DWAnimationFrame);
    }

    int getFrameCount() { return _count; }
    int getFrameStep(int frame) { return (frame >= 0 && frame < _count) ? _pframes[frame]._step : -1; }
    int getFrameValue(int frame) { return (frame >= 0 && frame < _count) ? _pframes[frame]._value : -1; }
};

enum DWLightMode {
  DWLightModeOff,
  DWLightModeOn,
  DWLightModeManual,
  DWLightModeAnimation
};

typedef void (*DWLightCallbackAnimationEnd) (void *);

class DWLight {
  private:
    byte _pin;
    bool _analog = true;
    short _step = 0;
    DWLightMode _mode = DWLightModeOff;
    DWAnimation* _panimation = NULL;
    DWLightCallbackAnimationEnd _callbackAnimationEnded;

    bool _modeChanged = false;
    byte  _manualValue = 0;

  public:
    DWLight(int pin, bool analog = true);
    isAnalog() { return _analog; };
    
    void setup();
    void setAnimationEndCallback(DWLightCallbackAnimationEnd callbackAnimationEnded) {_callbackAnimationEnded = callbackAnimationEnded;}

    DWLightMode getMode() { return _mode; };
    void setModeSimple(DWLightMode mode, int forSteps = 0);
    void setModeManual(int value, int forSteps = 0);
    void setModeAnimation(DWAnimation* panimation);
    DWAnimation* getAnimation() { return _panimation; };

    int getStep() { return _step; };
    void resetStep() { _step = 0; };
    void doStep();

    void test(int delayMillis);
    void debugDump();

  protected:
    void setPinValue(int value);

  private:
    void sequenceEnded();
};

#endif //davelopwarelight_h

