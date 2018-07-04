// Wed  4 Jul 23:02:04 UTC 2018
// 4737-a3b-013- // therm/tmp36-bb

// siska

// previous timestamp:
// Wed  4 Jul 22:55:39 UTC 2018
// 4737-a3b-011- // therm/tmp36-aa

#define VCC 3.3 // or 5.0 volts

int TMP36_PIN = A0; // temperature sensor TMP36 analog out

void setup() {
    Serial.begin(19200);
}
 
void loop() {
    int sensed_temp = analogRead(TMP36_PIN);
    float volts = sensed_temp * VCC;
    volts /= 1024.0;
    Serial.print("Sensed temp - raw - ");
    Serial.println(sensed_temp);
    Serial.print(volts); Serial.println(" volts");
    // convert: 10 mv per degree Celsius (with 500 mV offset)
    float temp_C = (volts - 0.5) * 100 ;
    Serial.print(temp_C); Serial.println(" degrees Celsius ");
    float temp_F = (temp_C * 9.0 / 5.0) + 32.0;
    Serial.print(temp_F); Serial.println(" degrees F\r\n");
    delay(3200);
}

