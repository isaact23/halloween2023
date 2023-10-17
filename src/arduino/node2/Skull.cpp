#include "Skull.hpp"

/**
 * Initialize Skull and PWM.
*/
Skull::Skull() {

  this._pwm = Adafruit_PWMServoDriver();

  // Initiate PWM module for Servo Control
  this._pwm.begin();
  this._pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
}

/**
 * Set the jaw servos to a certain percent open.
 * 0.0 is closed, 1.0 is open.
*/
void Skull::setJaw(float value) {
  
}

/**
 * Set both eye rotations.
 * Both values must be between -1.0 and 1.0.
*/
void Skull::setEyeRot(float x, float y) {

}

/**
 * Set left eye rotation.
 * Both values must be between -1.0 and 1.0.
*/
void Skull::setLeftEyeRot(float x, float y) {

}


/**
 * Set right eye rotation.
 * Both values must be between -1.0 and 1.0.
*/
void Skull::setRightEyeRot(float x, float z) {

}

/**
 * Set the eyelid positions.
 * 0.0 is closed, 1.0 is open.
*/
void Skull::setEyelids(float value) {

}

/**
  * Set wire length.
  * Wire numbers are 0, 1, 2, 3.
  * 0.0 is minimum, 1.0 is maximum.
*/
void Skull::setWireLength(int wire, float value) {

}

/**
 * Send servo data to servos.
*/
void Skull::updateServos() {

  this._pwm.setPWM(0,  0, this._l_eye_curr.horizontal);
  this._pwm.setPWM(1,  0, this._r_eye_curr.horizontal);
  this._pwm.setPWM(2,  0, this._l_eye_curr.vertical);
  this._pwm.setPWM(3,  0, this._r_eye_curr.vertical);
  this._pwm.setPWM(4,  0, this._top_eyelid_curr);
  this._pwm.setPWM(5,  0, this._bottom_eyelid_curr);
  this._pwm.setPWM(6,  0, this._l_jaw_curr);
  this._pwm.setPWM(7,  0, this._r_jaw_curr);
  this._pwm.setPWM(8,  0, this._wire_a_curr);
  this._pwm.setPWM(9,  0, this._wire_b_curr);
  this._pwm.setPWM(10, 0, this._wire_c_curr);
  this._pwm.setPWM(11, 0, this._wire_d_curr);


  for (int i = 0; i < SERVO_COUNT; i++) {
    this._pwm.setPWM(i, 0, pwm_val[i]);
  }
}

/**
 * Set top eyelid position.
 * 0.0 is down, 1.0 is up.
*/
void Skull::_setTopEyelid(float value) {

}

/**
  * Set bottom eyelid position.
  * 0.0 is up, 1.0 is down.
*/
void Skull::_setBottomEyelid(float value) {

}
