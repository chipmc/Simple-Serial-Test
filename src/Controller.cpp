#include "Particle.h"
#include "MagModel.h"

/*
 * Controller for the Magnetometer Boron project.
 * Description: Cellular data exchange for a Magnetometer using a Particle Boron.
 * Author: Alex Bowen 
 * Date: July 2023
 * Description: Controller for the Magnetometer project. Observes changes in MagModel 
 *              and outputs results as Particle events. Interfaces with Particle and the
 *              MagModel through events as well as through Particle functions that take
 *              user input.            
 */

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

float getBaseline(){
  return MagModel::instance().getBaseline();
}

int recalibrateBaseline(String cmd){
  if(cmd == "1") {
    return MagModel::instance().recalibrateBaseline();
  }
  return -1; // invalid
}

int getThreshold(){
  return MagModel::instance().getThreshold();
}

int setThreshold(String threshold){
  return MagModel::instance().setThreshold(threshold.toInt()); // returns -1 if > MAX_THRESHOLD
}

int getResetThreshold(){
  return MagModel::instance().getResetThreshold();
}

int setResetThreshold(String reset_threshold){
  return MagModel::instance().setResetThreshold(reset_threshold.toInt()); // returns -1 if > MAX_RESET_THRESHOLD
}

int getCountAndReset(String cmd){
  if(cmd == "1") {
    return MagModel::instance().recalibrateBaseline();
  }
  return -1; // invalid
}

int getTotalVehicleCount(){
  return MagModel::instance().getTotalVehicleCount();
}

void setup() {  

  MagModel::instance().setup();
  
  // Particle variables
  Particle.variable("Baseline", getBaseline);
  Particle.variable("Threshold", getThreshold);
  Particle.variable("Reset Threshold", getResetThreshold);
  Particle.variable("Total Vehicle Count", getTotalVehicleCount);

  // Particle functions
  Particle.function("Recalibrate Baseline? (Enter \"1\")", recalibrateBaseline);
  Particle.function("Set the Threshold", setThreshold);
  Particle.function("Set the Reset Threshold", setResetThreshold);
  Particle.function("Retrieve Count And Reset? (Enter \"1\")", getCountAndReset);
}

void loop() {

  // Ask the magnetometer for passData.
  float* passData = MagModel::instance().getPassData();

  // Continue looping MagModel.
  MagModel::instance().loop();

  // If passData came in, publish events.
  if(passData != 0) {

    Particle.publish("Detecting Vehicle!", String::format("[mag_RMS: %f, mag_x: %f, mag_y: %f, mag_z: %f]", 
                        passData[3],
                        passData[0],
                        passData[1],
                        passData[2] ));

    Particle.publish("Released.", String::format("[mag_RMS: %f, mag_x: %f, mag_y: %f, mag_z: %f]", 
                        passData[7],
                        passData[4],
                        passData[5],
                        passData[6]));
    
    Particle.publish("Total Vehicles", String(MagModel::instance().getTotalVehicleCount())); // output vehicle count              
  }
}