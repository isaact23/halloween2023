// Node 3 - LEDs

#include <FastLED.h>
#include <math.h>
//#include "painlessMesh.h"

#define PIN 19
#define NUM_LEDS 100

#define   NODE_NAME       "Node 3 (The Lights)"

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

// Run the light program.
void program();

CRGB leds[NUM_LEDS];

void setup() {
  program();
}

void loop() {
}

// Run the SONUS light program.
void program() {
  Serial.begin(9600);

  FastLED.addLeds<WS2812B, PIN>(leds, NUM_LEDS);

  for (int i = 0; i < 20; i++) {
    leds[i].setRGB(120, 0, 0);
  }
  for (int i = 20; i < 40; i++) {
    leds[i].setRGB(0, 120, 0);
  }
  for (int i = 40; i < 60; i++) {
    leds[i].setRGB(0, 0, 120);
  }

  FastLED.show();
}
