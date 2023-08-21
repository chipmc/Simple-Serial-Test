#ifndef STORAGE_H
#define STORAGE_H

#include "Particle.h"

struct current_structure {  
  int vehicleCount;                               // Number of vehicles in the system
  int totalVehicleCount;                          // Total number of vehicles that have passed through the system
  int vehicle_sensed;                             // Flag for vehicle sensed
  int state;                                      // State of the FSM; 0 = waiting, 1 = sensing, 2 = done
  bool sensing;                                   // Flag for sensing message
  bool done;                                      // Flag for done message
};

struct sysStatus_structure {
  int baseline;                                   // Baseline value for the magnetometer
  int threshold;                                  // Threshold value for the magnetometer
  int reset_threshold;                            // Reset threshold value for the magnetometer
};

extern struct current_structure current;

extern struct sysStatus_structure sysStatus;

#endif
