/**
 * Magnetometer Sensor Class
 * Author: Alex Bowen (referenced https://github.com/chipmc/TOF-Sensor-Demo/blob/master/src/TofSensor.cpp)
 * Date: July 2023
 * Description: MagModel is a model that interfaces with the Magnetometer and the Controller. Contains data
 *              about a vehicle detection as well as the business logic for parsing and updating that data
 *              using the serial stream coming from the Magnetometer. Observed by the Controller.
 *              
*/

#include "Particle.h"
#include "MagModel.h"
#include "Settings.h"

MagModel *MagModel::_instance; // Singleton instance of the model

/**
 * Stores data coming from a single vehicle passover.
 * Data at the time of vehicle's initial detection as well as its release. 
 * 
 * indexes:
 * 
 *       0: mag_x;
 *       1: mag_y;
 *       2: mag_z;
 *       3: mag_RMS;
 *       4: reset_mag_x;
 *       5: reset_mag_y;
 *       6: reset_mag_z;
 *       7: reset_mag_RMS;
 * 
 */
static float passData[8] = {};

static int threshold = 0; // current threshold setting (when to START sensing)
static int reset_threshold = 0; // current reset_threshold setting (when to STOP sensing)
static float baseline = 0; // Baseline magnetic field mag_RMS(v0.3.0)

// Magnetic field vector magnitudes in x, y, and z direction relative to the sensor (uT)
static float mag_x = 0;
static float mag_y = 0;
static float mag_z = 0;
// Root mean square of above vector magnitudes (uT)
static float mag_RMS = 0;

static int state = 0; // Device state. States are: 0 (WAITING), 1 (SENSING), and 2 (DONE). Set to -1 for baseline recalibration.
static int vehicle_sensed = 0; // Set to 1 if vehicle is detected, transitions 'state' to SENSING.
static int ready = 0; // Flag set to 1 in the DONE state. Resets to 0 after passData has been retrieved by the Controller

pin_t Light = D7; // Blue light on front of Boron, which lights up during the SENSING sate.

static int vehicleCount = 0; // Count since last call to getCountAndReset();
static int totalVehicleCount = 0; // Total count since model was initialized

// [static]
MagModel &MagModel::instance() {
  if (!_instance) {
      _instance = new MagModel();
  }
  return *_instance;
}

MagModel::MagModel() {
}

MagModel::~MagModel() {
}

void MagModel::setup(){
    
    pinMode(Light, OUTPUT);
    // Serial.begin(115200);
    Serial1.begin(115200);

    //waitUntil(Serial.isConnected);
    //Serial.println("Serial is connected");

    while (!Serial1.available()){
        delay(10); // Wait until serial connection is established
    }
    delay(1000);  // Make sure the serial monitor can connect

    Serial1.readStringUntil('\n'); //wait for clean line
    Serial1.parseInt(); //parse over vehicle_sensed

    threshold = Serial1.parseInt();
    reset_threshold = Serial1.parseInt();
    baseline = Serial1.parseFloat();
    delay(2000);
    Particle.publish("Initialized", String::format("Threshold: %d, Reset Threshold: %d, Baseline %f", threshold, reset_threshold, baseline));
}

/**
 * Serial data format coming from magnetometer:
 * 
 * threshold(int),reset_threshold(int),baseline(float),mag_x(float),
 * mag_y(float),mag_z(float),mag_RMS(float),vehicle_sensed(int)/n
 */
void MagModel::loop(){       // Returns 1 if a vehicle has been fully sensed.
    
    if(ready == 0) {

        Serial1.readStringUntil('\n'); //wait for clean line        
        vehicle_sensed = Serial1.parseInt(); //used to check if vehicle_sensed
        threshold = Serial1.parseInt(); //parse threshold
        reset_threshold = Serial1.parseInt(); //parse reset_threshold
        baseline = Serial1.parseFloat(); //parse baseline
        mag_x = Serial1.parseFloat(); //store mag_x
        mag_y = Serial1.parseFloat(); //store mag_y
        mag_z = Serial1.parseFloat(); //store mag_z
        mag_RMS = Serial1.parseFloat(); //store mag_RMS

        // Simple FSM for vehicle_sensed state
        switch(state){

            // WAITING
            case 0: 

                if(vehicle_sensed == 1){ //capture trigger values

                    passData[0] = mag_x;
                    passData[1] = mag_y;
                    passData[2] = mag_z;
                    passData[3] = mag_RMS;

                    state = 1;
                }
                break;

            // SENSING
            case 1:

                digitalWrite(Light, HIGH);
                if(vehicle_sensed == 0){ //capture reset values

                    passData[4] = mag_x;
                    passData[5] = mag_y;
                    passData[6] = mag_z;
                    passData[7] = mag_RMS;

                    state = 2;
                }
                break;
                
            // DONE
            case 2:

                digitalWrite(Light, LOW);
                ++vehicleCount;
                ++totalVehicleCount;
                ready = 1;
                state = 0;
                break;

            default:
                break;
        }
    }
}

float* MagModel::getPassData(){
    if(ready == 1){
        ready = 0;
        return passData;
    }
    return {};
}

float MagModel::getBaseline(){
    if(baseline != 0) {
        return baseline;
    }
    return -1; // error
}

float MagModel::recalibrateBaseline(){
    Serial1.println("CONF:BAS");
    loop(); // Update the values
    return baseline;
}

int MagModel::getThreshold(){
    return threshold;
}

int MagModel::setThreshold(int thres){
    if(thres <= MAX_THRESHOLD) {
        Serial1.printlnf("CONF:THR %d", thres);
        loop(); // Update the values
        thres = getThreshold();
        Particle.publish("New Threshold Set", String(thres));
        return thres;
    }
    return -1; // invalid
}

int MagModel::getResetThreshold(){
    return reset_threshold;
}

int MagModel::setResetThreshold(int reset_thres){
    if(reset_thres <= MAX_RESET_THRESHOLD) {
        Serial1.printlnf("CONF:RES %d", reset_thres);
        loop(); // Update the values.
        reset_thres = getResetThreshold();
        Particle.publish("New Reset Threshold Set", String(reset_thres));
        return reset_thres;
    }
    return -1; // invalid
}

int MagModel::getCountAndReset(){ 
    int count = vehicleCount;
    vehicleCount = 0;
    Particle.publish("Count Retrieved", String(count));
    Particle.publish("Count Reset");
    return count;
}

int MagModel::getTotalVehicleCount(){ 
    Particle.publish("Total Vehicle Count", String(totalVehicleCount));
    return totalVehicleCount;
}