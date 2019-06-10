// Mon Jun 10 01:35:46 UTC 2019

// Wed  4 Jul 19:52:31 UTC 2018

// vrabli filomena

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
 
int hysteresis = millis(); // time_monotonic();

/*
 * setup() - this function runs once when you turn your Arduino on
 * We initialize the serial connection with the computer
 */
void setup()
{
  Serial.begin(9600);  //Start the serial connection with the computer
  // turnkey - do not wait for serial connection // while(!Serial) { }
  // while(!Serial) { }
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
  // Serial.println("setup() has completed. ");
}
 
void color_toggled(void) {
    if (color_toggle == -1) color_toggle = 0; // if true make false
    if (color_toggle ==  0) color_toggle = -1; // if false make true
    // no error test for third condition 'other'
}

// void timex(void) { } // top down // int hyster_time_a = millis(); // time_monotonic();
void timex(void) {
    // Serial.println("timex is reached.");
    int new_hysteresis = millis(); // now there is a new reading of the clock to compare to.
    int difference = new_hysteresis - hysteresis ;

    // every 25 seconds, toggle the color of the first NeoPixel in the 8x strip.
    // if (difference > 24999) {// elapsed time 25 seconds (or larger)
    // if (difference > (60*5*1000)) {// elapsed time 5 minutes (or larger)
    if (difference > (60*3*1000)) {// elapsed time 3 minutes (or larger)

    // if (difference > (3*6*1000)) {// elapsed time 18 seconds (or larger)
        hysteresis = new_hysteresis; // actual hysteresis record event here
        // copy the most recent reading into the history buffer, overwriting the past record
        // this is done only when the goal of 25 seconds of elapsed time has been reached.
        // meow.


        // these two were swapped.  Want an accurate stack report
        illuminate(); // perform the glowance action itself
        dotS(); // show the stack //

#ifdef TESTING_ONLY
        // testing only
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

void loop()                     // run over and over again
{
 //getting the voltage reading from the temperature sensor
 int reading = analogRead(sensorPin);  
 // int reading = 220;
 
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
 // Serial.print(temperatureF); Serial.println(" degrees F");
 
 // setpoint = 55 ; // 20 second response time after cycle OFF air conditioner compressor!

 // may 2019 - went to 5 volts probably 3.3v calib // setpoint = 51 ;
 setpoint = 49 ; // was: 51

 if (temperatureF < setpoint) {
    glow_Blue1(); // cool
    show_master_pixel();
 }
 if (temperatureF > (setpoint + 3)) {
    glow_Red1(); // warm
    show_master_pixel();
 }

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
