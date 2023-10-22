#include "Skull.hpp"

uint8_t servonum = SERVO_COUNT;

/**
 * Linear interpolation
*/
int _lerp(float percent, float lo, float hi) {
  return map(percent, 0, 1.0, lo, hi);
}

/**
 * Initialize Skull with callback to setPWM.
 */
Skull::Skull(void (*setPWM) (int servo, int value)) {

  this -> _setPWM = setPWM;

  // Run task constantly to update PWM
}

/**
 * Set the jaw servos to a certain percent open.
 * 0.0 is closed, 1.0 is open.
*/
void Skull::setJaw(float percent) {

  this -> _l_jaw_curr = _lerp(percent, L_JAW_CLOSED, L_JAW_OPEN);
  this -> _r_jaw_curr = _lerp(percent, R_JAW_CLOSED, R_JAW_OPEN);
}

/**
 * Set both eye rotations.
 * Both values must be between -1.0 and 1.0.
*/
void Skull::setEyeRot(float x, float y) {

  this -> setLeftEyeRot(x, y);
  this -> setRightEyeRot(x, y);

  /*
  this._eyelid_space 0.0 to 1.0
  y -1.0 to 1.0
  TOP_EYELID_CLOSED to TOP_EYE_OPEN
  BOTTOM_EYELID_CLOSED to BOTTOM_EYELID_OPEN

  eyelid_space       y           top_val           bottom_val
  0                 -1            0                   0
  0                 0             0                   0
  0                 1             0                   0
  1                 -1            0                   1
  1                 0             0.5                   0.5
  1                 1             1                   0
  */

  // Calculate how open/closed the top/bottom eyes should be
  float topVal = map(y, -1, 1, 0, 1);
  float bottomVal = map(y, -1, 1, 1, 0);
  topVal *= this -> _eyelid_space;
  bottomVal *= this -> _eyelid_space;

  this -> _top_eyelid_curr = _lerp(topVal, TOP_EYELID_CLOSED, TOP_EYELID_OPEN);
  this -> _bottom_eyelid_curr = _lerp(bottomVal, BOTTOM_EYELID_CLOSED, BOTTOM_EYELID_OPEN);
}

/**
 * Set left eye rotation.
 * Both values must be between -1.0 and 1.0.
*/
void Skull::setLeftEyeRot(float x, float y) {

  int horizontal = _lerp((x + 1) / 2, L_EYE_LEFT, L_EYE_RIGHT);
  int vertical   = _lerp((y + 1) / 2, L_EYE_DOWN, L_EYE_UP);

  this -> _l_eye_curr = {
    horizontal,
    vertical
  };
}


/**
 * Set right eye rotation.
 * Both values must be between -1.0 and 1.0.
*/
void Skull::setRightEyeRot(float x, float y) {

  int horizontal = _lerp((x + 1) / 2, R_EYE_LEFT, R_EYE_RIGHT);
  int vertical   = _lerp((y + 1) / 2, R_EYE_DOWN, R_EYE_UP);

  this -> _r_eye_curr = {
    horizontal,
    vertical
  };
}

/**
 * Set the eyelid positions.
 * 0.0 is closed, 1.0 is open.
*/
void Skull::setEyelids(float value) {
  this -> _eyelid_space = value;
}

/**
  * Set wire length.
  * Wire numbers are 0, 1, 2, 3.
  * 0.0 is minimum, 1.0 is maximum.
*/
void Skull::setWireLength(int wire, float percent) {

  switch (wire) {
    case 0: {
      this -> _wire_a_curr = _lerp(percent, WIRE_A_LO, WIRE_A_HI);
      break;
    }
    case 1: {
      this -> _wire_b_curr = _lerp(percent, WIRE_B_LO, WIRE_B_HI);
      break;
    }
    case 2: {
      this -> _wire_c_curr = _lerp(percent, WIRE_C_LO, WIRE_C_HI);
      break;
    }
    case 3: {
      this -> _wire_d_curr = _lerp(percent, WIRE_D_LO, WIRE_D_HI);
      break;
    }
  }
}

/**
 * Send servo data to servos.
*/
void Skull::updateServos() {
  this -> _setPWM(0,  this -> _l_eye_curr.horizontal);
  this -> _setPWM(1,  this -> _r_eye_curr.horizontal);
  this -> _setPWM(2,  this -> _l_eye_curr.vertical);
  this -> _setPWM(3,  this -> _r_eye_curr.vertical);
  this -> _setPWM(4,  this -> _top_eyelid_curr);
  this -> _setPWM(5,  this -> _bottom_eyelid_curr);
  this -> _setPWM(6,  this -> _l_jaw_curr);
  this -> _setPWM(7,  this -> _r_jaw_curr);
  this -> _setPWM(8,  this -> _wire_a_curr);
  this -> _setPWM(9,  this -> _wire_b_curr);
  this -> _setPWM(10, this -> _wire_c_curr);
  this -> _setPWM(11, this -> _wire_d_curr);
}
