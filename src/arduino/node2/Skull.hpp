#ifndef SKULL_HPP
#define SKULL_HPP

#define L_JAW_OPEN 380
#define L_JAW_CLOSE 500
#define R_JAW_OPEN 500
#define R_JAW_CLOSE 380

#define L_EYE_UP 240
#define L_EYE_DOWN 140
#define R_EYE_UP 140
#define R_EYE_DOWN 240

#define L_EYE_LEFT 140
#define L_EYE_RIGHT 240
#define R_EYE_LEFT 140
#define R_EYE_RIGHT 240

#define TOP_EYELID_OPEN 500
#define TOP_EYELID_CLOSE 250
#define BOTTOM_EYELID_OPEN 250
#define BOTTOM_EYELID_CLOSE 500

#define WIRE_A_LO 500
#define WIRE_A_HI 150
#define WIRE_B_LO 150
#define WIRE_B_HI 500
#define WIRE_C_LO 500
#define WIRE_C_HI 150
#define WIRE_D_LO 150
#define WIRE_D_HI 500

/**
 * The Skull class controlls the servos on the nun.
*/
class Skull {

public:
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
  void setWireLength(int wire, float value);

private:

  /**
   * Set top eyelid position.
   * 0.0 is down, 1.0 is up.
  */
  void _setTopEyelid(float value);

  /**
    * Set bottom eyelid position.
    * 0.0 is up, 1.0 is down.
  */
  void _setBottomEyelid(float value);

};

#endif
