
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
  void setLeftRotRot(float x, float y);


  /**
   * Set right eye rotation.
   * Both values must be between -1.0 and 1.0.
  */
  void setRightEyeRotation(float x, float z);

  /**
   * Set the eyelid positions.
   * 0.0 is closed, 1.0 is open.
  */
  void setEyelids(float value);

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