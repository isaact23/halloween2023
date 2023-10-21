// Node 2 - The Nun

#include "painlessMesh.h"
#include "Skull.hpp"

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

#include <Wire.h>

Skull skull = NULL;
Scheduler userScheduler;
painlessMesh mesh;
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void send_Message();
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
unsigned long interval = 5UL;

Task task_Send_Message  (interval, TASK_FOREVER, &send_Message);
Task task_mode_Idle     (interval, TASK_FOREVER, &mode_Idle);
Task task_mode_Attract  (interval, TASK_FOREVER, &mode_Attract);
Task task_mode_Approach (interval, TASK_FOREVER, &mode_Approach);
Task task_mode_Scare    (interval, TASK_FOREVER, &mode_Scare);

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

//Idle mode, default state
void mode_Idle() {

  millisElapsed += interval;
  float aPos = (sin((((float) millisElapsed +   0) / 3000)) + 1) / 2;
  float bPos = (sin((((float) millisElapsed + 250) / 3000)) + 1) / 2;
  float cPos = (sin((((float) millisElapsed + 500) / 3000)) + 1) / 2;
  float dPos = (sin((((float) millisElapsed + 750) / 3000)) + 1) / 2;

  skull.setWireLength(0, aPos);
  skull.setWireLength(1, bPos);
  skull.setWireLength(2, cPos);
  skull.setWireLength(3, dPos);
}

//Attract mode, get the people going
void mode_Attract() {

  millisElapsed += interval;
  float jawPos = sin(((float) millisElapsed / 100) + 1) / 2;
  skull.setJaw(jawPos);

}

void mode_Approach() {
  millisElapsed += interval;
  float eyeX = cos((float) millisElapsed / 300);
  float eyeY = sin((float) millisElapsed / 300);
  skull.setEyeRot(eyeX, eyeY);
  skull.setEyelids(1.0);
}

void mode_Scare() {
  skull.setEyeRot(0, 0);
  skull.setEyelids(1.0);
}

/**
 * Set PWM value for a servo.
 */
void setPWM(int servo, int value) {
  //Serial.printf("%i\n", value);
  pwm.setPWM(servo, 0, value);
}

// Disable all tasks.
void disableAllTasks() {
  task_mode_Approach.disable();
  task_mode_Attract.disable();
  task_mode_Scare.disable();
  task_mode_Idle.disable();
}

// Log messages received, change mode
void receivedCallback( uint32_t from, String &msg ) {
  
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
  String new_mode = msg.c_str();
  
  if (new_mode == "IDLE") { 
    disableAllTasks();
    task_mode_Idle.enable();
  }
  else if (new_mode == "ATTRACT") {
    disableAllTasks();
    task_mode_Attract.enable();
  }
  else if (new_mode == "SCARE") {
    disableAllTasks();
    task_mode_Scare.enable();
  }
  else if (new_mode == "WELCOME") {
    disableAllTasks();
    task_mode_Approach.enable();  
  }
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
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
  userScheduler.addTask( task_mode_Idle );
  userScheduler.addTask( task_mode_Attract );
  userScheduler.addTask( task_mode_Approach );
  userScheduler.addTask( task_mode_Scare );
  userScheduler.addTask( task_update_servos );
  //userScheduler.addTask( task_manual_Calibration );
  
  //task_manual_Calibration.enable();
  task_mode_Scare.enable();
  task_update_servos.enable();
  //task_Send_Message.enable();

  skull = Skull(&setPWM);
}

void loop() {
  // it will run the user scheduler as well
  mesh.update();
  //Serial.printf("Working\n");
}