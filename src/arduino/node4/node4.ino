// Node 4 - The Poltergeist

#include <FastLED.h>
#include <math.h>
//#include "painlessMesh.h"

#define PIN 16
#define NUM_LEDS 100

#define   NODE_NAME       "Node 4 (The Poltergeist)"

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

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(120, 120, 0);
  }

  FastLED.show();
}
