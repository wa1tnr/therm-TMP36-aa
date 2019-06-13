// Wed Jun 12 18:34:01 UTC 2019 as flashed last
// Thu Jun 13 03:31:54 UTC 2019 - compiles, only (not tested)

// ceradda quirado tanque jacob marley this-a-way

// fittoria

// Mon May 27 21:29:05 UTC 2019
// Wed  4 Jul 19:52:31 UTC 2018

// filomena

// TODO: hysteresis not yet programmed 12 June 2019.
//       If it were present, the system would be able
//       to 'know' if the temperature were recently
//       rising .. or falling.
//       Right now, it does not - and that shows.

// 0.73 volts at 73 F

extern void interpret_setup(void); // interpret_m4.cpp:619:void interpret_setup() {
extern void push(int n);
extern void dotS(void);
extern void show_master_pixel();

#include "neo_pixel.h"

int setpoint = 0;
int color_toggle = 0; // -1 is true, 0 is false

//TMP36 Pin Variables
int sensorPin = A0; //the analog pin the TMP36's Vout (sense) pin is connected to
                        //the resolution is 10 mV / degree centigrade with a
                        //500 mV offset to allow for negative temperatures
 
int pwmPin = A2;    // cannot use A0 nor A1 for PWM.

uint8_t hourvoltage;

int hysteresis = millis(); // time_monotonic();

#define TURNKEY_COMPILE

void setup()
{
  Serial.begin(9600);  //Start the serial connection with the computer
  pinMode(pwmPin, OUTPUT);
  // turnkey - do not wait for serial connection // while(!Serial) { }
#ifndef TURNKEY_COMPILE
  while(!Serial) { }
#endif
  Serial.println("Alive and running code. ");
  
  setup_neoPixel();
  interpret_setup(); // Shattuck's interpreter

#ifdef AVR_MCU
  analogReference(EXTERNAL);
#endif

#ifdef ARDUINO_SAMD_ZERO
  analogReference(AR_EXTERNAL);
#endif
}
 
void color_toggled(void) {
    if (color_toggle == -1) color_toggle = 0; // if true make false
    if (color_toggle ==  0) color_toggle = -1; // if false make true
    // no error test for third condition 'other'
}

void timex(void) {
    int new_hysteresis = millis(); // now there is a new reading of the clock to compare to.
    int difference = new_hysteresis - hysteresis ;

    if (difference > (60*3*1000)) { // elapsed time 3 minutes (or larger)
        hysteresis = new_hysteresis;

        illuminate();
        dotS();

#ifdef TESTING_ONLY
        color_toggled();

        if (color_toggle) {
            glow_Blue1(); // cool
        } else {
            glow_Red1(); // warm
        }
#endif

    }
    Serial.print(" delta hysteresis is  (should be 5695 ms): ");
    Serial.println(difference);
}


void pwm_sender(void) {
  analogWrite(pwmPin, hourvoltage);
}

void loop()
{
  uint8_t hourvalue;
  int reading = analogRead(sensorPin);  // 233 to 264 - hand warming to ambient 72F
  float intmd_val = 0;

  hourvalue = (100 * reading); // as in 55 deg F

  // map(value, fromLo, fromHi, toLo, toHi);
  // hourvoltage = map(reading, 218, 274, 21, 234);     // Convert hour to PWM duty cycle
  // pwm_sender(); // call the sender
 
#define VCC 3.3
 // converting that reading to voltage, for 3.3v arduino use 3.3
 float voltage = reading * VCC;
 voltage /= 1024.0; 
 
 float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
                                               //to degrees ((voltage - 500mV) times 100)
 
 // now convert to Fahrenheit
 float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
 Serial.print(temperatureF); Serial.println(" degrees F");
 
 // setpoint = 55 ; // 20 second response time after cycle OFF air conditioner compressor!

// for fridge: #define SETPOINT_F 58 // as flashed 23:57z 28 May 2019
#define SETPOINT_F 52 // 51 // as flashed 02:57z 10 Jun 2019

 setpoint = SETPOINT_F ; // 72 gave either blue or green
                         // 73 gave blue
                         // 71 gave green (middle ground indication)
 // experimental
 glow_Green1(); // indeterminate
 delay(20);
 show_master_pixel();

 if (temperatureF < setpoint) {
    glow_Blue1(); // cool
    show_master_pixel();
 }

 if (temperatureF > (setpoint + 3)) {
    glow_Red1(); // warm
    show_master_pixel();
 }

 // override for test:
 intmd_val = 50 * 48.2; // temp in fahrenheit
 intmd_val = 50 * 76.7; //

 intmd_val = 50 * 76.7; //
 intmd_val = 50 * 62.4; //

 intmd_val = 50 * 56.1; // 2605 for midscale
 intmd_val = 50 * 54.1; // 2605 for midscale
 intmd_val = 50 * 53.1; // 2605 for midscale
 intmd_val = 50 * 52.1; // 2605 for midscale
 intmd_val = 50 * 44.1; // 2605 for midscale
 intmd_val = 50 * 59.1; // 2605 for midscale
 intmd_val = 50 * 54.1; // 2605 for midscale
 intmd_val = 50 * 75.1; // 2605 for midscale
 intmd_val = 50 * 75.7; // 2605 for midscale
 intmd_val = 50 * 52.5; // 2605 for midscale
 intmd_val = 50 * 86.5; // 2605 for midscale
 intmd_val = 50 * 82.5; // 2605 for midscale
 intmd_val = 50 * 63.25;// 2605 for midscale

 // map(value, fromLo, fromHi, toLo, toHi);
 // hourvoltage = map(intmd_val, 2405, 2770, 21, 234);     // Convert hour to PWM duty cycle

 // octave: music analogy for wrap-around (modulus) 0-255 &c.

    // maxscale (octave) 40.1 (40.2 enters mainscale)
    // maxscale (octave) 86.5 (86.6 minscale next octave)

    // minscale (fundam) 40.2
    // midscale (fundam) 52.5 for 2105, 3070, 21, 234 mapping.
    // maxscale (funda?) 63.2

    // midscale (octave) 75.7 for 2105, 3070, 21, 234 mapping.

 intmd_val = 50 * temperatureF;
    hourvoltage = map(intmd_val, 2105, 3070, 21, 234);     // Convert hour to PWM duty cycle
 pwm_sender();

 timex(); // speed up things a bit 0018z 28 may // delay(2400);

#ifdef OOPS_NOPE
 dotS(); // show the stack
 illuminate(); // perform the glowance action itself
#endif

 delay(800); // new kludge 0133z 28 may
}
