// This #include statement was automatically added by the Particle IDE.
#include <neopixel.h>

SYSTEM_MODE(AUTOMATIC);

/* ======================= prototypes =============================== */

void clear_remainder(uint16_t head);
void clear_all();
uint16_t get_next_index(uint16_t head, uint16_t after);


// IMPORTANT: Set pixel COUNT, PIN and TYPE
#define PIXEL_COUNT 24
#define PIXEL_PIN D6
#define PIXEL_TYPE WS2812B

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
//               note: if not specified, D2 is selected for you.
// Parameter 3 = pixel type [ WS2812, WS2812B, WS2811, TM1803 ]
//               note: if not specified, WS2812B is selected for you.
//               note: RGB order is automatically applied to WS2811,
//                     WS2812/WS2812B/TM1803 is GRB order.
//
// 800 KHz bitstream 800 KHz bitstream (most NeoPixel products ...
//                         ... WS2812 (6-pin part)/WS2812B (4-pin part) )
//
// 400 KHz bitstream (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//                   (Radio Shack Tri-Color LED Strip - TM1803 driver
//                    NOTE: RS Tri-Color LED's are grouped in sets of 3)

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

int led2 = D7;

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  pinMode(led2, OUTPUT);
}

static uint16_t head = 0;
static const uint16_t TAIL_LENGTH = 21;
static const uint16_t FADE_DECREMENT = 5;


void loop() {
  uint8_t wait = 50;
  uint16_t i;
  uint16_t red_value;
  uint16_t index;
  
  red_value = TAIL_LENGTH*FADE_DECREMENT + 1;

  clear_all();
    
  for(i=0; i<TAIL_LENGTH; i++) {
    index = get_next_index(head,i);
    strip.setPixelColor(index, red_value, 0, 0);
    red_value -= FADE_DECREMENT;
  }

  strip.show();
  delay(wait);
  
  // get ready for next time
  head++;
  if (head >= strip.numPixels()) {
      head = 0;
  }
}

// Optimization to clear LED's that shouldn't be turned on instead
// of clearing all of them.

// void clear_remainder(uint16_t head)
// {
//   uint16_t  i;
//   for (i=0; i<24-TAIL_LENGTH; i++) {
//     strip.setPixelColor(get_next_index(head,i), 0, 0, 0);
//   }
// }

void clear_all() {
  uint16_t  i;
  for (i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, 0, 0, 0);
  }
}

// head after ret
//    0    0  0
//    0    1  23
//    0    2  22
//    0    3  21
//    0    4  20
//
//    1    0  1
//    1    1  0
//    1    2  23
//    1    3  22
//    1    4  21
//
//    2    0  2
//    2    1  1
//    2    2  0
//    2    3  23
//    2    4  22
//
//    3    0  3
//    3    1  2
//    3    2  1
//    3    3  0
//    3    4  23
//
//    4   0   4
//    4   1   3
//    4   2   2
//    4   3   1
//    4   4   0

uint16_t get_next_index(uint16_t head, uint16_t after)
{
    if ( head-after >= 0 )
       return head - after;

    return 23 - (after - head - 1);
}

