// Node 2 - The Nun

#include "painlessMesh.h"
#include "Skull.hpp"
#include <Wire.h>

#define   NODE_NAME       "Node 2 (The Nun)"

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

Skull skull = NULL;
Scheduler userScheduler;
painlessMesh mesh;
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void send_Message();
void mode_Standby();
void mode_Idle();
void mode_Attract(); 
void mode_Approach();
void mode_Scare();
void update_Servos();
void setPWM(int servo, int value);
//void manual_Calibration();
//void move_Eyes();

//update frequency counter
unsigned long millisElapsed = 0UL;
unsigned long const INTERVAL = 5UL;

Task task_Send_Message  (INTERVAL, TASK_FOREVER, &send_Message);
Task task_mode_Standby  (INTERVAL, TASK_FOREVER, &mode_Standby);
Task task_mode_Idle     (INTERVAL, TASK_FOREVER, &mode_Idle);
Task task_mode_Attract  (INTERVAL, TASK_FOREVER, &mode_Attract);
Task task_mode_Approach (INTERVAL, TASK_FOREVER, &mode_Approach);
Task task_mode_Scare    (INTERVAL, TASK_FOREVER, &mode_Scare);

Task task_update_servos (5, TASK_FOREVER, &update_Servos);

//Task task_manual_Calibration (1000, TASK_FOREVER, &manual_Calibration);
//Task task_move_Eyes (100, TASK_FOREVER, &move_Eyes);

void update_Servos() {
  skull.updateServos();
}

// Broadcast message
void send_Message() {
  String msg = "The nun says hi";
  mesh.sendBroadcast( msg );
  Serial.println(msg);
}

// No movement
void mode_Standby() {

}

// Idle mode, default state
void mode_Idle() {

  // Jiggle slow
  millisElapsed += INTERVAL;
  float leftPos = (sin((((float) millisElapsed) / 300)) + 1) / 2;
  float rightPos = -leftPos;

  skull.setWireLength(0, leftPos);
  skull.setWireLength(1, rightPos);
  skull.setWireLength(2, rightPos);
  skull.setWireLength(3, leftPos);

  skull.setEyelids(0.0);
  skull.setLeftEyeRot(0, 0);
  skull.setRightEyeRot(0, 0);

  skull.setJaw(0);

  //Serial.printf("%0.4f\n", aPos);
}

// Attract mode, get the people going
void mode_Attract() {

  // Jiggle fast
  millisElapsed += INTERVAL;
  float leftPos = (sin((((float) millisElapsed) / 30)) + 1) / 2;
  float rightPos = -leftPos;

  skull.setWireLength(0, leftPos);
  skull.setWireLength(1, rightPos);
  skull.setWireLength(2, rightPos);
  skull.setWireLength(3, leftPos);

  float eyePos = sin(((float) millisElapsed) / 200);
  skull.setEyelids(1.0f);
  skull.setLeftEyeRot(eyePos, 0);
  skull.setRightEyeRot(eyePos, 0);

  skull.setJaw((eyePos + 1) / 2);
}

void mode_Approach() {
  millisElapsed += INTERVAL;

  // Lean forward
  skull.setWireLength(0, 0.0);
  skull.setWireLength(1, 0.0);
  skull.setWireLength(2, 1.0);
  skull.setWireLength(3, 1.0);

  float wave1 = sin(((float) millisElapsed) / 140);
  float wave2 = cos(((float) millisElapsed) / 175);
  float wave3 = -sin(((float) millisElapsed) / 133);
  float wave4 = -cos(((float) millisElapsed) / 206);

  int BLINK_INTERVAL = 2000;

  int modulo = millisElapsed % BLINK_INTERVAL;
  
  if (modulo < 100 || (modulo > 200 && modulo < 300)) {
    skull.setEyelids(0.0);
  } else {
    skull.setEyelids(1.0);
  }

  skull.setLeftEyeRot(wave1, wave2);
  skull.setRightEyeRot(wave3, wave4);

  float jawVal = (round(sin(millisElapsed / 300) * 2) + 2) / 4;
  skull.setJaw(jawVal);
}

void mode_Scare() {

  millisElapsed += INTERVAL;

  if (millisElapsed < 3000) {
    skull.setEyelids(0.7);
    if (((millisElapsed / 400) % 2) == 0) {
      skull.setEyeRot(-1, 0);
    } else {
      skull.setEyeRot(1, 0);
    }
    skull.setJaw(0);
  }
  else {
    // Jiggle fast
    millisElapsed += INTERVAL;
    float leftPos = (sin((((float) millisElapsed) / 50)) + 1) / 2;
    float rightPos = -(sin((((float) millisElapsed) / 56)) + 1) / 2;

    skull.setWireLength(0, leftPos);
    skull.setWireLength(1, rightPos);
    skull.setWireLength(2, rightPos);
    skull.setWireLength(3, leftPos);

    skull.setEyelids(1.0);
    float x = cos(((float) millisElapsed) / 70);
    float y = sin(((float) millisElapsed) / 70);
    skull.setEyeRot(x, y);

    skull.setJaw(1);
  }
}

/**
 * Set PWM value for a servo.
 */
void setPWM(int servo, int value) {
  /*if (servo > 10) {
    Serial.printf("Set %i to %i", servo, value);
  }
  else if (servo > 7) {
    Serial.printf("Set %i to %i\n", servo, value);
  }*/
  pwm.setPWM(servo, 0, value);
}

// Disable all tasks.
void disableAllTasks() {
  task_mode_Standby.disable();
  task_mode_Approach.disable();
  task_mode_Attract.disable();
  task_mode_Scare.disable();
  task_mode_Idle.disable();

  millisElapsed = 0;
}

// Log messages received, change mode
void receivedCallback( uint32_t from, String &msg ) {
  
  Serial.printf("%s: Received from %u msg=%s\n", NODE_NAME, from, msg.c_str());
  String new_mode = msg.c_str();
  
  bool didChangeMode = false;
  if (new_mode == "STANDBY") {
    disableAllTasks();
    task_mode_Standby.enable();
    didChangeMode = true;
  }
  else if (new_mode == "IDLE") { 
    disableAllTasks();
    task_mode_Idle.enable();
    didChangeMode = true;
  }
  else if (new_mode == "ATTRACT") {
    disableAllTasks();
    task_mode_Attract.enable();
    didChangeMode = true;
  }
  else if (new_mode == "SCARE") {
    disableAllTasks();
    task_mode_Scare.enable();
    didChangeMode = true;
  }
  else if (new_mode == "WELCOME") {
    disableAllTasks();
    task_mode_Approach.enable();  
    didChangeMode = true;
  }

  if (didChangeMode) {
    char* broadcast = (char*) malloc(sizeof(char) * 100);
    sprintf(broadcast, "%s: Mode changed to %s\n", NODE_NAME, new_mode);
    mesh.sendBroadcast(broadcast);
    free(broadcast);
  }
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("%s: New Connection, nodeId = %u\n", NODE_NAME, nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
  //Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

void setup() {
  Serial.begin(115200);

//mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  //Initiate PWM module for Servo Control
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

  userScheduler.addTask( task_Send_Message );
  userScheduler.addTask( task_mode_Standby );
  userScheduler.addTask( task_mode_Idle );
  userScheduler.addTask( task_mode_Attract );
  userScheduler.addTask( task_mode_Approach );
  userScheduler.addTask( task_mode_Scare );
  userScheduler.addTask( task_update_servos );
  //userScheduler.addTask( task_manual_Calibration );
  
  //task_manual_Calibration.enable();
  task_mode_Standby.enable();
  task_update_servos.enable();
  //task_Send_Message.enable();

  skull = Skull(&setPWM);
}

void loop() {
  // it will run the user scheduler as well
  mesh.update();
  //Serial.printf("Working\n");
}