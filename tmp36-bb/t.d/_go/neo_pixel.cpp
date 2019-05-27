#include <Adafruit_NeoPixel.h> // REQUIRED: https://github.com/adafruit/Adafruit_NeoPixel
#include "neo_pixel.h"
#define PIN             40 // peculiar to Metro M0 Express.  Feather M0 Express: PIN 8 
#define NUMPIXELS        1 // Metro M0 Express

// kelloggs
int red   = 0;
int green = 0;
int blue  = 0;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, 40, NEO_GRB + NEO_KHZ800);

/* void dimPixels(void) {
    pixels.setBrightness(25); } */

void glow_Dark(void) {
    pixels.setPixelColor(0, 0, 0, 0);
    delay(25);
    pixels.show();
    delay(50);
}

void glow_Blue1(void) {
    red   =  0;
    green =  0;
    blue  = 5;
}

void glow_Red1(void) {
    red   =  2;
    green =  0;
    blue  =  0;
}

void illuminate(void) {
    glow_Dark();
    pixels.setPixelColor(0, red, green, blue);
    delay(20);
    pixels.show();
}

void setup_neoPixel(void) {
    pixels.begin();

    glow_Red1();
    glow_Blue1();

    illuminate();
}

