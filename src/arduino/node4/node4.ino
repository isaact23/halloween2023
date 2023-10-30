// Node 4 - The lost soul

#include "painlessMesh.h"
#include <FastLED.h>
#include <math.h>
#include <Wire.h>

#define LED_PIN 16
#define EYE_PIN 27
#define NUM_LEDS 10

#define   NODE_NAME       "Node 4 (Lost Soul)"

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

Scheduler userScheduler;
painlessMesh mesh;

//update frequency counter
unsigned long millisElapsed = 0UL;
unsigned long const INTERVAL = 5UL;

CRGB leds[NUM_LEDS];

void send_Message();
void mode_Standby();
void mode_Idle();
void mode_Attract(); 
void mode_Approach();
void mode_Scare();

Task task_Send_Message  (INTERVAL, TASK_FOREVER, &send_Message);
Task task_mode_Standby  (INTERVAL, TASK_FOREVER, &mode_Standby);
Task task_mode_Idle     (INTERVAL, TASK_FOREVER, &mode_Idle);
Task task_mode_Attract  (INTERVAL, TASK_FOREVER, &mode_Attract);
Task task_mode_Approach (INTERVAL, TASK_FOREVER, &mode_Approach);
Task task_mode_Scare    (INTERVAL, TASK_FOREVER, &mode_Scare);

// Broadcast message
void send_Message() {
  String msg = "The lost soul says hi";
  mesh.sendBroadcast( msg );
  Serial.println(msg);
}

// No movement
void mode_Standby() {

}

// Idle mode, default state
void mode_Idle() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(0, 120, 0);
  }
  FastLED.show();
  pinMode(EYE_PIN, INPUT);
}

// Attract mode, get the people going
void mode_Attract() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(0, 0, 120);
  }
  FastLED.show();
  pinMode(EYE_PIN, OUTPUT);
}

void mode_Approach() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(120, 120, 0);
  }
  FastLED.show(); 
  pinMode(EYE_PIN, INPUT); 
}

void mode_Scare() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(120, 120, 120);
  }
  FastLED.show();
  pinMode(EYE_PIN, OUTPUT);
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

  char* broadcast = (char*) malloc(sizeof(char) * 100);
  sprintf(broadcast, "%s: Mode changed to %s\n", NODE_NAME, new_mode);
  mesh.sendBroadcast(broadcast);
  free(broadcast);
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

  FastLED.addLeds<WS2812B, LED_PIN>(leds, NUM_LEDS);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(120, 0, 0);
  }
  FastLED.show();

  digitalWrite(EYE_PIN, LOW);
  pinMode(EYE_PIN, INPUT);

  userScheduler.addTask( task_Send_Message );
  userScheduler.addTask( task_mode_Standby );
  userScheduler.addTask( task_mode_Idle );
  userScheduler.addTask( task_mode_Attract );
  userScheduler.addTask( task_mode_Approach );
  userScheduler.addTask( task_mode_Scare );

  task_mode_Standby.enable();

}

void loop() {
  // it will run the user scheduler as well
  mesh.update();
  //Serial.printf("Working\n");
}