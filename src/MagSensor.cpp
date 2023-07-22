/**
 * Magnetometer Sensor Class
 * Author: Alex Bowen (referenced https://github.com/chipmc/TOF-Sensor-Demo/blob/master/src/TofSensor.cpp)
 * Date: July 2023
 * Description: Defines functions and instances of a singleton MagSensor class, which
 *              reads the data stream from the magnetometer and returns 1 after a vehicle
 *              is sensed. Will be used with VehicleCounter to increment the vehicle count.
*/

#include "Particle.h"
#include "MagSensor.h"

MagSensor *MagSensor::_instance;

static float baseline = 0;
static int vehicle_sensed = 0;

// Device state. States are: 0 (WAITING), 1 (SENSING), and 2 (DONE)
static int state = 0;

pin_t Light = D7;

// [static]
MagSensor &MagSensor::instance() {
  if (!_instance) {
      _instance = new MagSensor();
  }
  return *_instance;
}

MagSensor::MagSensor() {
}

MagSensor::~MagSensor() {
}

/**
 * Serial data format coming from magnetometer:
 * [(float)baseline],[(int)vehicle_sensed]/n
 */
int MagSensor::setup(){

    Serial.begin(115200);   //  Open serial communications and wait for port to open:
    Serial1.begin(115200);  //  Open serial1 communications and wait for port to open:

    waitUntil(Serial.isConnected);

    Serial.println("Serial is connected");

    // wait until baseline comes in
    while (!Serial1.available()){
        delay(10);
    }  
    baseline = Serial1.parseFloat();
    Serial.printlnf("Baseline value is: %f", baseline);

    pinMode(Light, OUTPUT);

    return 1;
}

int MagSensor::loop(){       // Returns 1 if a vehicle has been fully sensed.
    
    // Simple FSM for vehicle_sensed state
    switch(state){
        
        // WAITING
        case 0:
            vehicle_sensed = Serial1.parseInt();
            Serial.print(vehicle_sensed);
            if(vehicle_sensed){
                Serial.println("WAITING");
                state = 1;
            }
            break;

        // SENSING
        case 1:
            vehicle_sensed = Serial1.parseInt();
            if(!vehicle_sensed){
                Serial.printlnf("SENSING");
                digitalWrite(Light, HIGH);
                state = 2;
            }
            break;
            
        // DONE
        case 2:
            Serial.println("DONE");
            digitalWrite(Light, LOW);
            return 1;  
    }

    return 0;
}

float MagSensor::getBaseline(){
    return baseline;
}

int MagSensor::getThreshold(){
    return 0; //TODO
}

int MagSensor::setThreshold(int threshold){
    return 0; //TODO
}

int MagSensor::getResetThreshold(){
    return 0; //TODO
}

int MagSensor::setResetThreshold(int reset_threshold){
    return 0; //TODO
}