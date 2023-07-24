/*
 * Vehicle Counter Class
 * Author: Alex Bowen (referenced https://github.com/chipmc/TOF-Sensor-Demo/blob/master/src/PeopleCounter.h)
 * Date: July 2023
 * Description: Counts vehicles as they fully pass over the sensor. Stores total vehicle count
 *              as well as a vehicle count that can be read and reset to 0.
 */

#include "Particle.h"
#include "VehicleCounter.h"

// Count since last call to getCountAndReset();
static int vehicleCount = 0;

// Total vehicle count;
static int totalVehicleCount = 0;

VehicleCounter *VehicleCounter::_instance;

// [static]
VehicleCounter &VehicleCounter::instance() {
    if (!_instance) {
        _instance = new VehicleCounter();
    }
    return *_instance;
}

VehicleCounter::VehicleCounter() {
}

VehicleCounter::~VehicleCounter() {
}


void VehicleCounter::setup(){
}

int VehicleCounter::loop(){     // updates vehicle count using MagSensor
        ++vehicleCount;
        ++totalVehicleCount;
        return totalVehicleCount;
}

int VehicleCounter::getCountAndReset(){
    int count = vehicleCount;
    vehicleCount = 0;
    return count;
}

int VehicleCounter::getTotalVehicleCount(){
    return totalVehicleCount;
}