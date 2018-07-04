// Wed  4 Jul 21:59:51 UTC 2018
// 4737-a3b-00f- // therm/tmp36-aa

int TMP36_PIN = A0; // temperature sensor TMP36 analog out

void setup() {
    Serial.begin(19200);
}
 
void loop() {
    int sensed_temp = analogRead(TMP36_PIN);
    Serial.print("Sensed temp - raw - ");
    Serial.println(sensed_temp);
    delay(3200);
}

