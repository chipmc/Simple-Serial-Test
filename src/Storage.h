#ifndef STORAGE_H
#define STORAGE_H

#include "Particle.h"

struct current_structure {  
  int vehicleCount;                               // Number of vehicles in the system
  int totalVehicleCount;                          // Total number of vehicles that have passed through the system
  int vehicle_sensed;                             // Flag for vehicle sensed
  int state;                                      // State of the FSM; 0 = waiting, 1 = sensing, 2 = done
  bool ready;                                     // Flag for data ready
  float mag_x;                                    // X-axis magnetometer value
  float mag_y;                                    // Y-axis magnetometer value
  float mag_z;                                    // Z-axis magnetometer value
  float mag_RMS;                                  // RMS value of the magnetometer
  float resetMag_x;                               // X-axis reset magnetometer value
  float resetMag_y;                               // Y-axis reset magnetometer value
  float resetMag_z;                               // Z-axis reset magnetometer value
  float resetMag_RMS;                             // RMS value of the reset magnetometer
};

struct sysStatus_structure {
  double baseline;                                   // Baseline value for the magnetometer
  int threshold;                                  // Threshold value for the magnetometer
  int reset_threshold;                            // Reset threshold value for the magnetometer
};

extern struct current_structure current;

extern struct sysStatus_structure sysStatus;

#endif
