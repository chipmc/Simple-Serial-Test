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
// 0.1 - Moved Particle functions out to their own class - Did not work - need shared storage
// 0.2 - ok, last rev was non-functional - this rev works but need to deconflict Particle functions
// 0.3 - Added Particle functions for setting baseline and threshold as integers

#include "Particle.h"
#include "MagModel.h"
#include "Particle_Functions.h"						// Where we put all the functions specific to Particle
#include "Storage.h"
#include "Settings.h"

char currentPointRelease[6] ="0.2";
PRODUCT_VERSION(1);									// For now, we are putting nodes and gateways in the same product group - need to deconflict #

struct current_structure current;
struct sysStatus_structure sysStatus;

void setup() {  
  // Initiate the MagModel.
  MagModel::instance().setup();

  // Initialize the Particle Functions
  Particle_Functions::instance().setup();			// Initialize Particle Functions and Variables
}

void loop() {
  // static int old_state = 0;

  // Loop MagModel for next iteration.
  MagModel::instance().loop();

  // If passData came in, publish events.
  if(current.sensing || current.done) {
    char stringMessage[128];
    if (current.sensing) {
      snprintf(stringMessage, sizeof(stringMessage), "Detecting Vehicle!");
      current.sensing = false;      
    } 
    if (current.done) {
      snprintf(stringMessage,sizeof(stringMessage),"Count: %i, Total Count: %i", current.vehicleCount, current.totalVehicleCount);
      current.done = false;
      delay(DELAY);      
    }  
    Particle.publish("Vehicle", stringMessage, PRIVATE);
  }
  // old_state = current.state;
}