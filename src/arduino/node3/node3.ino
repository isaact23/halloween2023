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
  millisElapsed += INTERVAL;

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
}

// Idle mode, default state
void mode_Idle() {
  millisElapsed += INTERVAL;

  const float FREQ = 0.1;
  const float SPEED = 0.1;

  for (int i = 0; i < NUM_LEDS; i++) {

    // Flip second half of strip
    int j = i;
    if (j >= NUM_LEDS / 2) {
      j = NUM_LEDS - i;
    }

    float x = j * FREQ;
    float offset = millisElapsed * SPEED;
    float param = x + offset;
    float wave = sin(param);
    int hue = round(20 * (wave + 1));

    leds[i].setHSV(hue, 255, 255);
  }
  FastLED.show();
}

// Attract mode, get the people going
void mode_Attract() {
  millisElapsed += INTERVAL;

  const float FREQ = 0.1;
  const float SPEED = 0.1;

  for (int i = 0; i < NUM_LEDS; i++) {

    // Flip second half of strip
    int j = i;
    if (j >= NUM_LEDS / 2) {
      j = NUM_LEDS - i;
    }

    float x = j * FREQ;
    float offset = millisElapsed * SPEED;
    float param = x + offset;
    float wave = sin(param);

    // Stripe effect
    CRGB color;
    if (wave < 0) {
      color = CRGB(255, 255, 0);
    } else {
      color = CRGB(255, 0, 0);
    }

    leds[i] = color;
  }

  FastLED.show();
}

void mode_Approach() {
  millisElapsed += INTERVAL;

  const float FREQ = 0.1;
  const float SPEED = 0.1;

  for (int i = 0; i < NUM_LEDS; i++) {

    // Flip second half of strip
    int j = i;
    if (j >= NUM_LEDS / 2) {
      j = NUM_LEDS - i;
    }

    float x = j * FREQ;
    float offset = millisElapsed * SPEED;
    float param = x + offset;
    float wave = sin(param);

    // Stripe effect
    CRGB color;
    if (wave < 0) {
      color = CRGB(255, 255, 255);
    } else {
      color = CRGB(0, 0, 0);
    }

    leds[i] = color;
  }
  FastLED.show();
}

void mode_Scare() {
  millisElapsed += INTERVAL;

  for (int i = 0; i < NUM_LEDS; i++) {

    // Do nothing for the first 3 seconds
    if (millisElapsed < 3000) {
      leds[i].setRGB(0, 0, 0);
      continue;
    }

    // Flip second half of strip
    int j = i;
    if (j >= NUM_LEDS / 2) {
      j = NUM_LEDS - i;
    }

    const int STRIPE_WIDTH = 30;
    const int STRIPE_SPEED = 0.1;

    int offset = millisElapsed * STRIPE_SPEED;
    int segment = ((j + offset) / STRIPE_WIDTH) % 8;
    int hue = segment * 30;

    leds[i].setHSV(hue, 255, 255);
  }
  FastLED.show();
}