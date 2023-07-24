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


SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

int totalVehicles = 0;

void setup() {  

  MagSensor::instance().setup();
  //Serial.println("MagSensor instance initialized.");

  VehicleCounter::instance().setup();
  //Serial.println("Vehicle Counter Initiated");
}

void loop() {
  float* vehicleData = MagSensor::instance().getData();
  MagSensor::instance().loop();
  if(vehicleData != 0){
    Particle.publish("Vehicle Detected!");
    Particle.publish("Detected at", String::format("[mag_RMS: %f, mag_x: %f, mag_y: %f, mag_z: %f]", 
                        vehicleData[3],
                        vehicleData[0],
                        vehicleData[1],
                        vehicleData[2] ));
    Particle.publish("Released at", String::format("[mag_RMS: %f, mag_x: %f, mag_y: %f, mag_z: %f]", 
                        vehicleData[7],
                        vehicleData[4],
                        vehicleData[5],
                        vehicleData[6]));
    Particle.publish("Total Vehicles", String(VehicleCounter::instance().loop())); // output vehicle count              
  }
}