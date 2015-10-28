// This #include statement was automatically added by the Particle IDE.
#include "SparkJson/SparkJson.h"

// This #include statement was automatically added by the Particle IDE.
#include "neopixel/neopixel.h"


// ---- CUSTOMIZE ME ----
#define PIXEL_PIN D1
#define PIXEL_COUNT 2
#define PIXEL_TYPE WS2812B

// blackout means don't run during "the day", define what times that means here
int lightupStart = 18;  // time in 24h to display - 6pm
int lightupHours = 9;   // hours to light         - 3am
// I'd love to use the do time ranges here but it's too hard to
// implement without a real time/date library.  You can't easily
// represent like 3am -- meaning the next day with a number.

// ---- END CUSTOMIZE ME ----


Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

int doorState = -1;
bool lightupEndNextDay;
int currentHour;
int lightupEnd;

void setup() {
  // Serial.begin(9600);

  Time.zone(-7);  // PST

  strip.begin();
  strip.show();   // Initialize all pixels to 'off'

  Particle.subscribe("squarism/garage/open", eventHandler);

  // figure out if operating time is actually next day
  lightupEndNextDay = endTimeIsNextDay();
  //   Serial.println("I'm booted!");
}

void loop() {
  if (Particle.connected() == false) {
    Particle.connect();
  }
  Particle.process();

  // Do you guys like TDD?  Do you guys like testing?  Well listen to this!
  // ** Johnny B. Goode plays while testers cry blood **
  // Time.setTime(1446023974);  // 2am
  // Time.setTime(1446029100);  // 3:45am
  // Time.setTime(1446031100);  // 4:18am

  if (isOperatingTime()) {
    switch(doorState)
    {
      case 1:
        fireRing(200);
        break;
      case 0:
        iceRing(200);
        break;
    }
  } else {
    // sort of a sleep mode here
    blankStrip();
    delay(60000);
  }

  delay(5000);
}

void eventHandler(const char *event, const char *data) {
  // Parse the event as json into something structured so we can react to it.
  // Saved by example!
  // https://github.com/glowfishAPI/glowfish-particle/blob/master/firmware/examples/anomaly-detect-webhook/anomaly-detect-webhook.ino
  int len = strlen(data);
  char dataCopy[len+1];
  strcpy(dataCopy, data);

  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(dataCopy);

  if (json["door_open"] == true) {
    doorState = 1;
  }

  if (json["door_open"] == false) {
    doorState = 0;
  }
}

bool endTimeIsNextDay() {
  if (lightupStart + lightupHours >= 24) {
    return true;
  } else {
    return false;
  }
}

bool isOperatingTime() {
  currentHour = Time.hour();
  lightupEnd = lightupStart + lightupHours;

  // same day calculation
  if ( (lightupEndNextDay == false) && ((currentHour >= lightupStart) && (currentHour < lightupEnd))) {
    return true;
  }

  // End time is tomorrow.
  // Let's say that operating time is 6pm to 3am.  That's 18 --> 27, 27 representing the next day.
  // We will count time from 18 on up and then from 3 to 0.  So we measure the bleed over with the or statement below
  // here to cover the "next day" case.
  if ( (lightupEndNextDay == true) && (currentHour >= lightupStart) || (currentHour <= (lightupEnd - 24))) {
    return true;
  }

  return false;
}

// Turn all the neopixels off
void blankStrip() {
  uint32_t black = strip.Color(0, 0, 0);
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, black);
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

// Sort of a red to orange fade effect.  Pretty sweet.
void fireRing(uint16_t wait) {
  uint16_t i, j;
  int colorStart = 95;  // a pure red
  int colorEnd = 125;   // the led starts to blink purple

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

// Sort of a blue to light blue fade effect.  Soothing.
void iceRing(uint16_t wait) {
  uint16_t i, j;
  int colorStart = 166;  // blue
  int colorEnd = 199;    // blue-green

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

