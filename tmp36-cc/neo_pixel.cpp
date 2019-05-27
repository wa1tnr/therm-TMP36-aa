// SO 27 May 2019: This sketch ran on the Metro M0 Express, at the time.

#include <Adafruit_NeoPixel.h> // REQUIRED: https://github.com/adafruit/Adafruit_NeoPixel
#include "neo_pixel.h"
#define PINnope         40 // peculiar to Metro M0 Express.  Feather M0 Express: PIN 8 
#define PIN             12 // Metro M4 Express - as found wired to 8x NeoPixel strip - May 2019
#define NUMPIXELSnope    1 // Metro M0 Express
#define NUMPIXELS        8 // external 8x npx strip - may 2019

extern void push(int n);
extern int pop(void);

// kelloggs
int red   = 0;
int green = 0;
int blue  = 0;

int st_index = 0; // stack index

// oldcode may 2019: Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, 40, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

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

void glow_Blue1(void) {
    red   =  0;
    green =  0;
    blue  = 5;
    push_tuple();
}

void glow_Red1(void) {
    red   =  2;
    green =  0;
    blue  =  0;
    push_tuple();
}

void _illuminate(void) {
    glow_Dark();
    pop_tuple(); // setup red green and blue values popped off the stack
    pixels.setPixelColor(st_index, red, green, blue);
    delay(20);
    pixels.show();
}

void Marche_des_Enfants_Rouges(void) {
    for (int i=8;i>-1;i--) {
        st_index = i; // move this stack pointer-like entity
        _illuminate();
    }
}

void illuminate(void) { // alias
    Marche_des_Enfants_Rouges();
}

void setup_neoPixel(void) {
    pixels.begin();

    glow_Red1();
    glow_Blue1();

    illuminate();
}

