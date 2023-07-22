#include "Particle.h"
#include "MagSensor.h"
#include "VehicleCounter.h"

/*
 * Magnetometer Vehicle Counter
 * Description: Cellular data exchange for a Magnetometer using a Particle Boron
 * Author: Alex Bowen 
 * Date: July 2023
 * Description: High level demo class, instantiates a chain of singleton instance creation.
 *              After detecting a vehicle, returns total number of vehicles detected. 
 */

float count = 0; 

SYSTEM_MODE(MANUAL);
SYSTEM_THREAD(ENABLED);

void setup() {  
  //initiate vehicleCounter and start loop
  VehicleCounter::instance().setup();
  Serial.println("Vehicle Counter Initiated");
}

void loop() {
  count = VehicleCounter::instance().loop(); // if vehicle count has been updated
  if(count > 0){
    Serial.printlnf("%d Vehicles Detected", count); // output vehicle count
  }
}