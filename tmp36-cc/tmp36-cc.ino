// Wed  4 Jul 19:52:31 UTC 2018

// fedipha

// 0.73 volts at 73 F


#include "neo_pixel.h"

int setpoint = 0;

//TMP36 Pin Variables
int sensorPin = A0; //the analog pin the TMP36's Vout (sense) pin is connected to
                        //the resolution is 10 mV / degree centigrade with a
                        //500 mV offset to allow for negative temperatures
 
int hyster_time_a = millis(); // time_monotonic();

/*
 * setup() - this function runs once when you turn your Arduino on
 * We initialize the serial connection with the computer
 */
void setup()
{
  Serial.begin(9600);  //Start the serial connection with the computer
  
                       //to view the result open the serial monitor 
  setup_neoPixel();

#ifdef AVR_MCU
  analogReference(EXTERNAL);
#endif

#ifdef ARDUINO_SAMD_ZERO
  analogReference(AR_EXTERNAL);
#endif
}
 
void timex(void) { } // top down // int hyster_time_a = millis(); // time_monotonic();


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
 setpoint = 51 ;

 if (temperatureF < setpoint) {
    glow_Blue1(); // cool
 }
 if (temperatureF > (setpoint + 3)) {
    glow_Red1(); // warm
 }

 illuminate(); // perform the glowance action itself

 // delay(12000);
 delay(3200);

}
