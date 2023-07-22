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

// Declare variables for storing parsed data

static int threshold = 0;
static int reset_threshold = 0;
static float baseline = 0;
static float mag_x = 0;
static float mag_y = 0;
static float mag_z = 0;
static float mag_RMS = 0;

static int vehicle_sensed = 0; //set to 1 if vehicle is being sensed

static int state = 0; // Device state. States are: 0 (WAITING), 1 (SENSING), and 2 (DONE)
static int state_written = 0; // Tracks whether data for current state has been written to serial monitor or not


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

void MagSensor::setup(){

    Serial.begin(115200);
    Serial1.begin(115200);

    waitUntil(Serial.isConnected);

    Serial.println("Serial is connected");

    while (!Serial1.available()){
        delay(10); // Wait until serial connection is established
    }
    delay(1000);  // Make sure the serial monitor can connect

    Serial1.readStringUntil('\r'); //wait for clean line
    threshold = Serial1.parseInt();
    reset_threshold = Serial1.parseInt();
    baseline = Serial1.parseFloat();

    Serial.printlnf("Threshold is set to: %d", threshold);
    Serial.printlnf("Reset Threshold is set to: %d", reset_threshold);
    Serial.printlnf("Baseline value is: %f", baseline);

    pinMode(Light, OUTPUT);
}

/**
 * Serial data format coming from magnetometer:
 * 
 * threshold(int),reset_threshold(int),baseline(float),mag_x(float),
 * mag_y(float),mag_z(float),mag_RMS(float),vehicle_sensed(int)/n
 */
int MagSensor::loop(){       // Returns 1 if a vehicle has been fully sensed.

    Serial1.parseInt(); //parse over threshold
    Serial1.parseInt(); //parse over reset_threshold
    Serial1.parseFloat(); //parse over baseline
    mag_x = Serial1.parseFloat(); //store mag_x
    mag_y = Serial1.parseFloat(); //store mag_y
    mag_z = Serial1.parseFloat(); //store mag_z
    mag_RMS = Serial1.parseFloat(); //store mag_RMS
    vehicle_sensed = Serial1.parseInt(); //used to check if vehicle_sensed
    
    // Simple FSM for vehicle_sensed state
    switch(state){
        // WAITING
        case 0:
            if(state_written == 0){
                Serial.println("WAITING...");
                state_written = 1;
            }
            if(vehicle_sensed == 1){
                state_written = 0;
                state = 1;
            }
            break;

        // SENSING
        case 1:
            if(state_written == 0){
                Serial.printlnf("SENSING.. (Triggered at mag_RMS=%f, mag_x=%f, mag_y=%f, mag_z=%f)",
                                                mag_RMS, 
                                                mag_x, 
                                                mag_y, 
                                                mag_z);
                state_written = 1;
            }
            digitalWrite(Light, HIGH);
            if(vehicle_sensed == 0){
                state_written = 0;
                state = 2;
            }
            break;
            
        // DONE
        case 2:
            digitalWrite(Light, LOW);
            Serial.printlnf("DONE. (Triggered at mag_RMS=%f, mag_x=%f, mag_y=%f, mag_z=%f)",
                                        mag_RMS, 
                                        mag_x, 
                                        mag_y, 
                                        mag_z);
            state = 0;
            return 1;  
    }

    return 0;
}

float MagSensor::recalibrateBaseline(){
    return 0; //TODO
}

float MagSensor::getBaseline(){
    return baseline;
}

int MagSensor::getThreshold(){
    return threshold;
}

int MagSensor::setThreshold(int threshold){
    return 0; //TODO
}

int MagSensor::getResetThreshold(){
    return reset_threshold;
}

int MagSensor::setResetThreshold(int reset_threshold){
    return 0; //TODO
}