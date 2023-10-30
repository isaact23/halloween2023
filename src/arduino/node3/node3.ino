// Node 3 - LEDs

#include "painlessMesh.h"
#include <FastLED.h>
#include <math.h>
#include <Wire.h>

#define PIN 19
#define NUM_LEDS 100

#define   NODE_NAME       "Node 3 (The Lights)"

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

Scheduler userScheduler;
painlessMesh mesh;

// Update frequency counter
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
  String msg = "The Lights say hi";
  mesh.sendBroadcast(msg);
  Serial.println(msg);
}

// No movement
void mode_Standby() {
  millisElapsed += interval;
  
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
}

// Idle mode, default state
void mode_Idle() {
  millisElapsed += interval;

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(0, 120, 0);
  }
  FastLED.show();
  pinMode(EYE_PIN, INPUT);
}

// Attract mode, get the people going
void mode_Attract() {
  millisElapsed += interval;

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(0, 0, 120);
  }
  FastLED.show();
  pinMode(EYE_PIN, OUTPUT);
}

void mode_Approach() {
  millisElapsed += interval;

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(120, 120, 0);
  }
  FastLED.show(); 
  pinMode(EYE_PIN, INPUT); 
}

void mode_Scare() {
  millisElapsed += interval;

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(120, 120, 120);
  }
  FastLED.show();
  pinMode(EYE_PIN, OUTPUT);
}