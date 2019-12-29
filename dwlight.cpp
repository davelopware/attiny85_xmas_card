#include "dwlight.h"
#include "dwattiny.h"

DWLight::DWLight(int pin, bool analog = true) {
  _pin = pin;
  _analog = analog;
};

void DWLight::setup(DWLightCallbackAnimationEnd callbackAnimationEnded = nullptr) {
  if (_pin == -1) {
    Serial.println("DWLight::setup() setting pinMode to OUTPUT");
  } else {
    pinMode(_pin, OUTPUT); 
  }
  if (callbackAnimationEnded != nullptr) {
    this->setAnimationEndCallback(callbackAnimationEnded);
  }
};

void DWLight::setModeSimple(DWLightMode mode, short forSteps) {
//  if (_mode != mode) {
    _modeChanged = true;
    _step = (abs(forSteps) * -1);
    _mode = mode;
    _panimation = nullptr;
//  }
};

void DWLight::setModeManual(int value, short forSteps = 0) {
  _modeChanged = true;
  _step = (abs(forSteps) * -1);
  _mode = DWLightModeManual;
  _manualValue = value;
  _panimation = nullptr;
};

void DWLight::setModeAnimate(DWAnimation* panimation) {
  if (_pin == -1) {
    Serial.print("DWLight::setModeAnimate() setting animation ");
    Serial.println(panimation->_name);
  }
  _modeChanged = true;
  _step = 0;
  _mode = DWLightModeAnimate;
  _panimation = panimation;
};

void DWLight::setModeAnimateInterpolate(DWAnimation* panimation) {
  if (_pin == -1) {
    Serial.print("DWLight::setModeAnimateInterpolate() setting animation ");
    Serial.println(panimation->_name);
  }
  _modeChanged = true;
  _step = 0;
  _mode = DWLightModeAnimateInterpolate;
  _panimation = panimation;
};

void DWLight::doStep() {
  _step++;

  if (_mode == DWLightModeOff || _mode == DWLightModeOn || _mode == DWLightModeManual) {
    if (_modeChanged) {
      _modeChanged = false;
      if (_mode == DWLightModeOff) {
        setPinValue(0);
        return;
      } else if (_mode == DWLightModeOn) {
        setPinValue(255);
        return;
      } else if (_mode == DWLightModeManual) {
        setPinValue(_manualValue);
        return;
      }
    }

    if (_step == 0) {
      sequenceEnded();
      return;
    }
  }

  if (_mode == DWLightModeAnimate || _mode == DWLightModeAnimateInterpolate) {
    byte iframe;
    for (iframe = _panimation->getFrameCount()-1; iframe >= 0; iframe--) {
      
      if (_step >= _panimation->getFrameStep(iframe)) {
        
        if (_mode == DWLightModeAnimateInterpolate) {
          byte nextFrame = (iframe != _panimation->getFrameCount()-1) ? iframe + 1 : iframe; 
          byte  fromValue = _panimation->getFrameValue(iframe);
          short fromStep = _panimation->getFrameStep(iframe);
          byte  toValue = _panimation->getFrameValue(nextFrame);
          short toStep = _panimation->getFrameStep(nextFrame);

//Serial.print("plight,");Serial.print((int)this);
//Serial.print(",st,");Serial.print(_step);
//Serial.print(",tf,");Serial.print(iframe);
//Serial.print(",fv,");Serial.print(fromValue);
//Serial.print(",fs,");Serial.print(fromStep);
//Serial.print(",nf,");Serial.print(nextFrame);
//Serial.print(",tv,");Serial.print(toValue);
//Serial.print(",ts,");Serial.print(toStep);
          if (fromValue == toValue) {
//Serial.print(",iv,");Serial.print(fromValue);
//Serial.print(",no,intereasy");
//Serial.println();
            setPinValue(fromValue);
          } else {
            byte interpolatedValue = fromValue + ((toValue - fromValue) * ( _step - fromStep) / (toStep - fromStep));
//Serial.print(",iv,");Serial.print(interpolatedValue);
//Serial.print(",no,inter");
//Serial.println();
            setPinValue(interpolatedValue);
          }
        } else {
//Serial.print("plight,");Serial.print((int)this);
//Serial.print(",st,");Serial.print(_step);
//Serial.print(",tf,");Serial.print(iframe);
//Serial.print(",fv,");Serial.print(_panimation->getFrameValue(iframe));
//Serial.print(",fs,");Serial.print(_panimation->getFrameStep(iframe));
//Serial.print(",nf,-");
//Serial.print(",tv,-");
//Serial.print(",ts,-");
//Serial.print(",iv,");Serial.print(_panimation->getFrameValue(iframe));
//Serial.print(",no,simple");
//Serial.println();
          setPinValue(_panimation->getFrameValue(iframe));
        }
        break;
      }
      
    }
    if (iframe == _panimation->getFrameCount()-1) {
      sequenceEnded();
    }
  }
}

void DWLight::setPinValue(int value) {
  if (_analog) {
    analogWrite(_pin, value);
    _pinValue = value;
  } else {
    int pinValue = (value == 0) ? LOW : HIGH;
    digitalWrite(_pin, pinValue);
    _pinValue = pinValue == LOW ? 0 : 255;
  }
}

bool DWLight::isSleepable() {
  return (_mode == DWLightModeOn || _mode == DWLightModeOff || _pinValue == 0 || _pinValue == 255);
}

void DWLight::test(int delayMillis) {
  if (isAnalog()) {
    for (byte i = 0; i < 3; i++) {
      setModeSimple(DWLightModeOn);
      doStep();
      delay(delayMillis);
      setModeManual(100);
      doStep();
      delay(delayMillis);
      setModeManual(30);
      doStep();
      delay(delayMillis);
      setModeSimple(DWLightModeOff);
      doStep();
      delay(delayMillis);
    }
  } else {
    for (byte i = 0; i < 6; i++) {
      setModeSimple(DWLightModeOn);
      doStep();
      delay(delayMillis);
      setModeSimple(DWLightModeOff);
      doStep();
      delay(delayMillis);
    }
  }
};


void DWLight::debugDump() {
//  Serial.print("Pin: ");          Serial.println(_pin);
//  Serial.print("Analog: ");       Serial.println(_analog);
//  Serial.print("Step: ");         Serial.println(_step);
//  Serial.print("Mode: ");         Serial.println(_mode);
//  Serial.print("Mode Changed: "); Serial.println(_modeChanged);
//  Serial.println();
}


void DWLight::sequenceEnded() {
  if (_callbackAnimationEnded) {
    _callbackAnimationEnded(this);
  }

//  _step = 0;
//  if (_mode == DWLightModeFlickerOn) {
//    setMode(DWLightModeOn);
//  }
}
