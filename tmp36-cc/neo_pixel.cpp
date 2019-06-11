// SO 27 May 2019: This sketch ran on the Metro M0 Express, at the time.

#include <Adafruit_NeoPixel.h> // REQUIRED: https://github.com/adafruit/Adafruit_NeoPixel
#include "neo_pixel.h"
#define PIN_SOLO        40 // peculiar to Metro M0, M4 Express.  Feather M0 Express: PIN 8
#define PIN             12 // Metro M4 Express - as found wired to 8x NeoPixel strip - May 2019
#define NUMPIXELSnope    1 // Metro M0 Express
#define NUMPIXELS        8 // external 8x npx strip - may 2019
#define HIGH_PIXEL       5

extern void push(int n);
extern int pop(void);

// kelloggs
int red   = 0;
int green = 0;
int blue  = 0;

int st_index = 0; // stack index

// oldcode may 2019: Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, 40, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel master_pixel = Adafruit_NeoPixel(NUMPIXELS, PIN_SOLO, NEO_GRB + NEO_KHZ800);

void pop_tuple(void) {
    int extra = pop();
    red = pop();
    green = pop();
    blue = pop();
}

void push_tuple(void) {
    push(blue);
    push(green);
    push(red); // will pop as RGB tuple
    push(0); // need 32 byte stack 4x8
}

/* void dimPixels(void) {
    pixels.setBrightness(25); } */

void glow_Dark(void) {
    pixels.setPixelColor(0, 0, 0, 0);
    delay(25);
    pixels.show();
    delay(50);
}

void glow_Green1(void) {
    red   =  0;
    green =  5;
    blue  = 0;
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

void _illuminate(void) {
    glow_Dark();
    pop_tuple(); // setup red green and blue values popped off the stack
    pixels.setPixelColor(st_index, red, green, blue);
    red = 0; green = 0; blue = 0;
    delay(20);
    pixels.show();
    // delay(50); // kludge debug 0122z 28 may
}

void Marche_des_Enfants_Rouges(void) {
    push_tuple(); // no clear idea where this should happen, but ..
                  // it was happening too often, prior.  The idea
                  // is that it updates the next interval-station.

    for (int i=8;i>0;i--) {
        st_index = i-1; // move this stack pointer-like entity
        // Serial.print("st_index: ");
        // Serial.println(st_index);
        _illuminate();
    }
}

void illuminate(void) { // alias
    Marche_des_Enfants_Rouges();
}

void show_master_pixel(void) {
    // Adafruit_NeoPixel master_pixel = Adafruit_NeoPixel(NUMPIXELS, PIN_SOLO, NEO_GRB + NEO_KHZ800);
    master_pixel.setPixelColor(0, 0, 0, 0);
    master_pixel.show();
    delay(88); // give human time to notice

    master_pixel.setPixelColor(0, red, green, blue);
    master_pixel.show();
}

void setup_neoPixel(void) {
    pixels.begin();
    master_pixel.begin();

    for (int i=8;i>0;i--) {
        Serial.println(i);
        pixels.setPixelColor((i-1), 2, 2, 8);
    }
    pixels.show(); delay(200);

    for (int i=8;i>0;i--) {
        pixels.setPixelColor((i-1), 0, 0, 0);
    }
    pixels.show(); delay(200);

    delay(20);
    pixels.setPixelColor(HIGH_PIXEL, 0, 0, 0);
    delay(20);
    pixels.show(); delay(200);

    delay(20);

    pixels.setPixelColor(HIGH_PIXEL, 9, 1, 1);
    delay(20);
    pixels.show(); delay(200);

    pixels.setPixelColor(HIGH_PIXEL, 0, 0, 0);
    delay(20);
    pixels.show(); delay(200);

    master_pixel.setPixelColor(0, 0, 0, 0);

    delay(20);
    pixels.show();
    delay(20);
    master_pixel.show();
    delay(20);

    // glow_Red1();
    // glow_Blue1();

    // illuminate();
}

