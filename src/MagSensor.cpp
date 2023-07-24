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


/**
 * Storing data coming from magnetometer, indexes
 * 
 *       0: mag_x;
 *       1: mag_y;
 *       2: mag_z;
 *       3: mag_RMS;
 *       4: reset_mag_x;
 *       5: reset_mag_y;
 *       6: reset_mag_z;
 *       7: reset_mag_RMS;
 */
static float data[8] = {};

static int threshold = 0;
static int reset_threshold = 0;
static float baseline = 0;
static float mag_x = 0;
static float mag_y = 0;
static float mag_z = 0;
static float mag_RMS = 0;

static int ready = 0;
static int vehicle_sensed = 0; //set to 1 if vehicle is being sensed
static int state = 0; // Device state. States are: 0 (WAITING), 1 (SENSING), and 2 (DONE)

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
    
    pinMode(Light, OUTPUT);
    //Serial.begin(115200);
    Serial1.begin(115200);

    //waitUntil(Serial.isConnected);

    // Serial.println("Serial is connected");

    while (!Serial1.available()){
        delay(10); // Wait until serial connection is established
    }
    delay(1000);  // Make sure the serial monitor can connect

    Serial1.readStringUntil('\n'); //wait for clean line
    Serial1.parseInt(); //parse over vehicle_sensed
    threshold = Serial1.parseInt();
    reset_threshold = Serial1.parseInt();
    baseline = Serial1.parseFloat();
    Particle.publish("Device configured", String::format("Threshold: %d, ResetThreshold: %d, Baseline %f", threshold, reset_threshold, baseline));
}

/**
 * Serial data format coming from magnetometer:
 * 
 * threshold(int),reset_threshold(int),baseline(float),mag_x(float),
 * mag_y(float),mag_z(float),mag_RMS(float),vehicle_sensed(int)/n
 */
void MagSensor::loop(){       // Returns 1 if a vehicle has been fully sensed.
    if(ready == 0) {
        Serial1.readStringUntil('\n'); //wait for clean line        
        vehicle_sensed = Serial1.parseInt(); //used to check if vehicle_sensed
        Serial1.parseInt(); //parse over threshold
        Serial1.parseInt(); //parse over reset_threshold
        Serial1.parseFloat(); //parse over baseline
        mag_x = Serial1.parseFloat(); //store mag_x
        mag_y = Serial1.parseFloat(); //store mag_y
        mag_z = Serial1.parseFloat(); //store mag_z
        mag_RMS = Serial1.parseFloat(); //store mag_RMS
        // Serial.println(vehicle_sensed);
        // Simple FSM for vehicle_sensed state
        switch(state){
            // WAITING
            case 0: 
                if(vehicle_sensed == 1){ //capture trigger values
                    data[0] = mag_x;
                    data[1] = mag_y;
                    data[2] = mag_z;
                    data[3] = mag_RMS;
                    state = 1;
                }
                break;

            // SENSING
            case 1:
                digitalWrite(Light, HIGH);
                if(vehicle_sensed == 0){ //capture reset values
                    data[4] = mag_x;
                    data[5] = mag_y;
                    data[6] = mag_z;
                    data[7] = mag_RMS;
                    state = 2;
                }
                break;
                
            // DONE
            case 2:
                digitalWrite(Light, LOW);
                ready = 1;
                state = 0;
                break;
        }
    }
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

float* MagSensor::getData(){
    if(ready == 1){
        ready = 0;
        return data;
    }
    return {};
}