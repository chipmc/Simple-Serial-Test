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

// Version history
// 0.1 - Moved Particle functions out to their own class

#include "Particle.h"
#include "MagModel.h"
#include "Particle_Functions.h"						// Where we put all the functions specific to Particle

char currentPointRelease[6] ="0.1";
PRODUCT_VERSION(1);									// For now, we are putting nodes and gateways in the same product group - need to deconflict #

void setup() {  
  // Initiate the MagModel.
  MagModel::instance().setup();

  // Initialize the Particle Functions
  Particle_Functions::instance().setup();			// Initialize Particle Functions and Variables
}

void loop() {

  // Ask the magnetometer for passData.
  float* passData = MagModel::instance().getPassData();

  // Loop MagModel for next iteration.
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
    
    MagModel::instance().getTotalVehicleCount();           
  }
}