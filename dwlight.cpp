#include "dwlight.h"
#include "dwattiny.h"

DWLight::DWLight(int pin, bool analog = true) {
  _pin = pin;
  _analog = analog;
}

void DWLight::setup() {
  if (_pin == -1) {
    Serial.println("DWLight::setup() setting pinMode to OUTPUT");
  } else {
    pinMode(_pin, OUTPUT); 
  }
}

void DWLight::setModeSimple(DWLightMode mode, int forSteps) {
  if (_mode != mode) {
    _modeChanged = true;
    _step = (abs(forSteps) * -1);
    _mode = mode;
    _panimation = nullptr;
  }
}

void DWLight::setModeManual(int value, int forSteps = 0) {
  _modeChanged = true;
  _step = (abs(forSteps) * -1);
  _mode = DWLightModeManual;
  _manualValue = value;
  _panimation = nullptr;
}

void DWLight::setModeAnimation(DWAnimation* panimation) {
  if (_pin == -1) {
    Serial.print("DWLight::setModeAnimation() setting animation ");
    Serial.println(panimation->_name);
  }
  _modeChanged = true;
  _step = 0;
  _mode = DWLightModeAnimation;
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

  if (_mode == DWLightModeAnimation) {
    byte iframe;
    for (iframe = _panimation->getFrameCount()-1; iframe >= 0; iframe--) {
      if (_step >= _panimation->getFrameStep(iframe)) {
        setPinValue(_panimation->getFrameValue(iframe));
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
//    if (_pin == -1) {
//      Serial.print("Setting analog mode on pin to ");
//      Serial.println(value);
//    } else {
      analogWrite(_pin, value);
//    }
  } else {
    bool bValue = (value == 0) ? true : false;
//    if (_pin == -1) {
//      Serial.print("Setting digital mode on pin to ");
//      Serial.println(bValue);
//    } else {
      digitalWrite(_pin, bValue);
//    }
  }
}

void DWLight::test(int delayMillis) {
  if (isAnalog()) {
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
  } else {
    setModeSimple(DWLightModeOn);
    doStep();
    delay(delayMillis);
    setModeSimple(DWLightModeOff);
    doStep();
    delay(delayMillis);
    setModeSimple(DWLightModeOn);
    doStep();
    delay(delayMillis);
    setModeSimple(DWLightModeOff);
    doStep();
    delay(delayMillis);
    setModeSimple(DWLightModeOn);
    doStep();
    delay(delayMillis);
    setModeSimple(DWLightModeOff);
    doStep();
    delay(delayMillis);
  }
}


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
