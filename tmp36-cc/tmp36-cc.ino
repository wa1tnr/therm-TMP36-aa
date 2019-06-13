// Wed Jun 12 18:34:01 UTC 2019 as flashed last
// Tue Jun 11 21:34:46 UTC 2019
// Mon Jun 10 02:57:54 UTC 2019

// quirado tanque jacob marley this-a-way

// Tue May 28 20:30:14 UTC 2019
// hownow
// turnkey - 2351z
// fittoria


// Things are getting out of hand, but there's some
// action to work with. ;)

// Mon May 27 21:29:05 UTC 2019
// Wed  4 Jul 19:52:31 UTC 2018

// filomena

// TODO: hysteresis not yet programmed 12 June 2019.
//       If it were present, the system would be able
//       to 'know' if the temperature were recently
//       rising .. or falling.
//       Right now, it does not - and that shows.
//       A classic example of why hysteresis is needed
//       for this kind of system: presently, the system
//       keeps reporting all three conditions, when near
//       the border between three states (on, off, maybe on/off).
//       What is desired (instead) is a trend towards
//       becoming warmer (or cooler) and a lock on that
//       trend.
//       If it was recently cooler, and is now warming,
//       then once it reliably reaches the warmer state,
//       the sytem should report 'it is now warm enough'
//       by lighting in red (only).
//       Similarly, blue, if it was recently a good deal
//       warmer, but has now reached a sufficently cold
//       state to warrant the announcement 'it is now
//       cold enough' (wherein it lights only the blue).
//       The 'maybe' state is a kludge, for purposes of
//       development - the completed project won't actively
//       use this (it'll be used, if at all, only to
//       debug or otherwise further refine things).
//       A mechanical thermostat (which is the basic
//       model for the project .. sans 'temperature history'
//       in the sense of reporting on past events, long
//       after they are no longer operative in any
//       meaningful way) .. a mechanical thermostat works
//       by means of a glass vial with a drop of Mercury
//       in it.  The mercury has the quality of bunching
//       up together (high surface tension?) and so it
//       tends to flow rapidly from one end of the vial
//       to the other, and does not want to stay anywhere
//       near the middle of the glass vial.
//       There is enough mechanical 'amplification' to
//       (mostly) prevent the in-between state, and at
//       any rate, the Mercury, in its job as a kind of
//       SPST switch, can only connect the two electrodes
//       at (one) far end of the vial.
//       The vial itself, of course, rides on a bi-metalic
//       coil, which gets tight or loose, depending on the
//       ambient temperature; as the two strips of (dissimilar)
//       metals have different rates of expansion, the coil
//       that they've been formed into tends to tighten or
//       to loosen, which translates to a rotation applied
//       to the Mercury switch (the glass vial) attached
//       to the outside of it.
//       So the switch rocks back and forth, as temperature
//       varies sufficiently to flip the Mercury blob to
//       one end of the vial (or the other, depending).
//       It won't oscillate, however - the Mercury blob has
//       just enough mass to deform the bi-metalic coil,
//       just a bit, which has the effect of 'amplifying'
//       the 'signal' (temperature state) to keep the system
//       from going into (thermal!) (mechanical!) oscillation,
//       near the set-point (desired temperature).
//       The system preserves the 'history' of temperature
//       change, by the weight of the Mercury blob, which
//       (again) gathers at one end of the glass vial, or
//       the other -- and does not want to (then) move back
//       to the opposite end, from small changes in thermal
//       input.
//       Thus, a certain amount of temperature change (in
//       either direction; they do not have to be exactly
//       equal) is the only way to cause the system to change
//       to the 'other' state.
//       So, in effect, the system 'remembers' from whence
//       it came:
//       If the switch is ON, that was caused by a falling
//       temperature regime (most recently), once the system
//       has reached equillibrium (either a steady temperature;
//       or a temperature regime that is mostly rising, or
//       mostly falling -- or if the set-point is changed by
//       an outside agent (usually, a human).
//       Conversely, if the switch is presently OFF, that state
//       also has a historic cause: the ambient temperature
//       was in a rising regime (the furnace was operating,
//       raising the temperature in the room the thermostat
//       is located in) and in this rising regime, a trip
//       point was reached where the Mercury blob slid over
//       to the other side of the vial, cutting off the
//       switch by opening the contact (formerly) made by
//       the Mercury blob (which itself is a good conductor
//       of electric current).



// narrative:  timex does okay to tick tock out 5.7 second intervals;
//        instead, what is wanted is 5 minute intervals (do not reset
//        hysteresis until 5 minutes has passed).

//        Test case will be for 25 seconds, as 5 min is too long for
//        testing purposes (takes up an hour pretty quickly, doing it
//        that way - 25 second intervals should be reliable enough for
//        extrapolation out to 5 mins.

// fedipha

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

/*
 * setup() - this function runs once when you turn your Arduino on
 * We initialize the serial connection with the computer
 */
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
  
                       //to view the result open the serial monitor 
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

    if (difference > (60*3*1000)) {// elapsed time 3 minutes (or larger)
        hysteresis = new_hysteresis; // actual hysteresis record event here
        // copy the most recent reading into the history buffer, overwriting the past record
        // this is done only when the goal of 25 seconds of elapsed time has been reached.

        illuminate(); // perform the glowance action itself
        dotS(); // show the stack //

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

// coefficient
#define COEFF 0.66

// 0.492  12.1%
// 0.481   0.00%
// 0.483

void loop()                     // run over and over again
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
 
 // print out the voltage
 // Serial.print(voltage); Serial.println(" volts");
 
 // now print out the temperature
 float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
                                               //to degrees ((voltage - 500mV) times 100)
 // Serial.print(temperatureC); Serial.println(" degrees C");
 
 // now convert to Fahrenheit
 float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
 Serial.print(temperatureF); Serial.println(" degrees F");
 
 // setpoint = 55 ; // 20 second response time after cycle OFF air conditioner compressor!

 // may 2019 - went to 5 volts probably 3.3v calib // setpoint = 51 ;
 // UNDO 28 May 2059z // setpoint = 51 ;

// for fridge: #define SETPOINT_F 58 // as flashed 23:57z 28 May 2019
#define SETPOINT_F 52 // 51 // as flashed 02:57z 10 Jun 2019
// result: 49 gave ambiguous condition

// derate these minus two 71 becomes 69 &c.

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


 // 52.7 * 50 = 2635
 // 72.4 * 50 = 3620
 // 83.7 * 50 = 4185

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

    // maxscale (octave) 40.1 (40.2 enters mainscale)
    // maxscale (octave) 86.5 (86.6 minscale next octave)

    // minscale (fundam) 40.2
    // midscale (fundam) 52.5 for 2105, 3070, 21, 234 mapping.
    // maxscale (funda?) 63.2

    // midscale (octave) 75.7 for 2105, 3070, 21, 234 mapping.

 intmd_val = 50 * temperatureF;
    hourvoltage = map(intmd_val, 2105, 3070, 21, 234);     // Convert hour to PWM duty cycle
 pwm_sender(); // call the sender and give it hourvoltage

 // Serial.println("pre-timex reached - loop iteration");

 timex(); // speed up things a bit 0018z 28 may // delay(2400);

#ifdef OOPS_NOPE
 dotS(); // show the stack
 illuminate(); // perform the glowance action itself
#endif

 // delay(12000);
 // delay(3200);
 delay(800); // new kludge 0133z 28 may
}
