/*
 * Project Simple-Serial-Test
 * Description: A test to see if we can take in the messages from the magnetometer and print them to the serial monitor
 * Author: Chip McClelland  
 * Date: 22-July-2023
 */

#include "Particle.h"

SYSTEM_MODE(MANUAL);      // This example code is designed to be used with the cloud disconnected

pin_t Light = D7;

// setup() runs once, when the device is first turned on.
void setup() {

  Serial.begin(115200);   //  Open serial communications and wait for port to open:
  Serial1.begin(115200);  //  Open serial1 communications and wait for port to open:

  waitUntil(Serial.isConnected);

  Serial.println("Serial is connected");

  pinMode(Light, OUTPUT);

}

// loop() runs over and over again, as quickly as it can execute.
void loop() {

 if (Serial1.available()) {
   Serial.write(Serial1.read());
   digitalWrite(Light, !digitalRead(Light));
 }

}