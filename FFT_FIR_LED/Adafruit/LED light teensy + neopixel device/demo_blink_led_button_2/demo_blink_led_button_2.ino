#include <Adafruit_NeoPixel.h>
#include <Bounce.h>

#define BRIGHTNESS 200 // range 0 - 255
#define TIMEMS 500 // ms high state or low state

#define LED_PIN 16
#define BUTT_PIN 18

int counter = 0;
boolean lstate = true;
int br_level = 0;

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, LED_PIN, NEO_GRB + NEO_KHZ800);
Bounce pushbutton = Bounce(BUTT_PIN, 20);  // 20 ms debounce

uint32_t orange = strip.Color(255, 150, 0);
uint32_t blue = strip.Color(0, 0, 255);

unsigned long prevmillis; 
elapsedMillis sinceLed;

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  pinMode(BUTT_PIN, INPUT_PULLUP);
  counter = 0;
  /*
  Serial.begin(9600);
  while(!Serial);
  */
}

void loop() {
  pushbutton.update();
  if (pushbutton.fallingEdge()) {
    counter++;
    if (counter > 4) counter=0;
    //Serial.println(counter, DEC);
    sinceLed = 0;
  }
  
  switch (counter) {
    case 0: {
      strip.setPixelColor(0, 0);
      strip.show();
      break;
    }
    case 1: {
      strip.setBrightness(BRIGHTNESS); // 0-255
      strip.show();  
      strip.setPixelColor(0, blue);
      strip.show();
      break;
    }
    case 2: {
      if (!lstate) {
        br_level = BRIGHTNESS / (1 + int((TIMEMS - sinceLed)*0.03));
      } else {
        br_level = BRIGHTNESS / (1 + int((sinceLed - TIMEMS)*0.03));
      }    
      strip.setBrightness(br_level);
      strip.show(); 
      
      if ((sinceLed < TIMEMS) && (lstate)) {
        sinceLed = 0;
        strip.setBrightness(0); // 0-255
        strip.show();
        strip.setPixelColor(0, blue);
        strip.show();
        lstate = false;
        //Serial.println("h");
      } 
      if ((sinceLed >= TIMEMS) && (!lstate)) {
        //strip.setPixelColor(0, 0);
        //strip.show();
        lstate = true;
        //Serial.println("l");
      }
      if (sinceLed >= (TIMEMS*2)) sinceLed = 0;
      break;
    }
    case 3: {
      strip.setBrightness(BRIGHTNESS); // 0-255
      strip.show();
      strip.setPixelColor(0, orange);
      strip.show();
      break;
    }
    case 4: {
      if ((sinceLed < TIMEMS) && (lstate)) {
        sinceLed = 0;
        strip.setBrightness(0); // 0-255
        strip.show();
        strip.setPixelColor(0, orange);
        strip.show();
        lstate = false;
      }
      if ((sinceLed >= TIMEMS) && (!lstate)) {
        strip.setPixelColor(0, 0);
        strip.show();
        lstate = true;
      }
      if (sinceLed >= (TIMEMS*2)) sinceLed = 0;
      break;
    }
  }
}
