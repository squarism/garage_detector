// This #include statement was automatically added by the Particle IDE.
#include "SparkJson/SparkJson.h"

// This #include statement was automatically added by the Particle IDE.
#include "neopixel/neopixel.h"

#include <string.h>

// ---- CHANGE THIS ----
#define PIXEL_PIN D1
#define PIXEL_COUNT 2
#define PIXEL_TYPE WS2812B

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

int doorState = -1;

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  Particle.subscribe("garage/open", eventHandler);
  Serial.println("I'm booted!");
}

void loop() {
  if (Particle.connected() == false) {
    Particle.connect();
  }
  Particle.process();
  
  if (doorState == 1) {
    fireRing(200);
  } else if (doorState == 0) {
    iceRing(200);
  }
}

void eventHandler(const char *event, const char *data) {
    // Parse the event as json into something structured so we can react to it.
    // Saved by example!
    // https://github.com/glowfishAPI/glowfish-particle/blob/master/firmware/examples/anomaly-detect-webhook/anomaly-detect-webhook.ino
    int len = strlen(data);
    char dataCopy[len+1];
    strcpy(dataCopy, data);

    StaticJsonBuffer<500> jsonBuffer;
    JsonObject& json = jsonBuffer.parseObject(dataCopy);
    
    if (json["door_open"] == true) {
        Serial.println("door is open.");
        doorState = 1;
    }

    if (json["door_open"] == false) {
        Serial.println("door is closed.");
        doorState = 0;
    }

}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void fireRing(uint16_t wait) {
  uint16_t i, j;
  int colorStart = 95;  // a pure red
  int colorEnd = 125;    // the led starts to blink purple

  for(j=colorStart; j<colorEnd; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(j & 255));
    }
    strip.show();
    delay(wait);
  }
  for(j=colorEnd; j>colorStart; j--) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(j & 255));
    }
    strip.show();
    delay(wait);
  }

}

void iceRing(uint16_t wait) {
  uint16_t i, j;
  int colorStart = 166;
  int colorEnd = 199;

  for(j=colorStart; j<colorEnd; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(j & 255));
    }
    strip.show();
    delay(wait);
  }
  for(j=colorEnd; j>colorStart; j--) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(j & 255));
    }
    strip.show();
    delay(wait);
  }

}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

