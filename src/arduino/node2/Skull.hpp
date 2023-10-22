#ifndef SKULL_HPP
#define SKULL_HPP

#include <Adafruit_PWMServoDriver.h>
#include <Arduino.h>
#include <cstdlib>
#include "painlessMesh.h"

#define SERVOMIN  140 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  520 // this is the 'maximum' pulse length count (out of 4096)
#define SERVO_COUNT 12 // number of servos to update each loop

#define L_JAW_OPEN 410
#define L_JAW_CLOSED 500
#define R_JAW_OPEN 470
#define R_JAW_CLOSED 380

#define L_EYE_UP 350
#define L_EYE_DOWN 150
#define L_EYE_LEFT 250
#define L_EYE_RIGHT 150

#define R_EYE_UP 150
#define R_EYE_DOWN 350
#define R_EYE_LEFT 250
#define R_EYE_RIGHT 150

#define TOP_EYELID_OPEN 500
#define TOP_EYELID_CLOSED 250
#define BOTTOM_EYELID_OPEN 500
#define BOTTOM_EYELID_CLOSED 250

#define WIRE_A_LO 500
#define WIRE_A_HI 150
#define WIRE_B_LO 150
#define WIRE_B_HI 500
#define WIRE_C_LO 500
#define WIRE_C_HI 150
#define WIRE_D_LO 150
#define WIRE_D_HI 500

struct _eyePos {
  int horizontal;
  int vertical;
};
typedef struct _eyePos EyePos;

/**
 * The Skull class controlls the servos on the nun.
*/
class Skull {

public:

  /**
  * Initialize Skull with callback to setPWM.
  */
  Skull(void (*setPWM) (int servo, int value));

  /**
   * Set the jaw servos to a certain percent open.
   * 0.0 is closed, 1.0 is open.
  */
  void setJaw(float value);

  /**
   * Set both eye rotations.
   * Both values must be between -1.0 and 1.0.
  */
  void setEyeRot(float x, float y);

  /**
   * Set left eye rotation.
   * Both values must be between -1.0 and 1.0.
  */
  void setLeftEyeRot(float x, float y);


  /**
   * Set right eye rotation.
   * Both values must be between -1.0 and 1.0.
  */
  void setRightEyeRot(float x, float z);

  /**
   * Set the eyelid positions.
   * 0.0 is closed, 1.0 is open.
  */
  void setEyelids(float value);

  /**
   * Set wire length.
   * Wire numbers are 0, 1, 2, 3.
   * 0.0 is minimum, 1.0 is maximum.
  */
  void setWireLength(int wire, float percent);

  /**
   * Send servo data to servos.
  */
  void updateServos();

private:

  // Callback function to update servo values
  void (*_setPWM) (int servo, int value);

  // Store current servo values
  int _l_jaw_curr = L_JAW_CLOSED;
  int _r_jaw_curr = R_JAW_CLOSED;
  EyePos _l_eye_curr = { L_EYE_LEFT, L_EYE_UP };
  EyePos _r_eye_curr = { R_EYE_LEFT, R_EYE_UP };
  int _top_eyelid_curr = TOP_EYELID_OPEN;
  int _bottom_eyelid_curr = BOTTOM_EYELID_OPEN;
  int _wire_a_curr = WIRE_A_LO;
  int _wire_b_curr = WIRE_B_LO;
  int _wire_c_curr = WIRE_C_LO;
  int _wire_d_curr = WIRE_D_LO;
  float _eyelid_space = 1.0;
};

#endif
