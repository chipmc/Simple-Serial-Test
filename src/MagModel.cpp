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
#include "Storage.h"

MagModel *MagModel::_instance;          // Singleton instance of the model
pin_t Light = D7;                       // Blue light on front of Boron, which lights up during the SENSING sate.

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

    Serial1.begin(115200);

    while (!Serial1.available()){
        delay(10); // Wait until serial connection is established
    }
    delay(1000);  // Make sure the serial monitor can connect

    Serial1.readStringUntil('\n'); //wait for clean line
    Serial1.parseInt(); //parse over vehicle_sensed

    current.state = 0;          // Set initial state to WAITING

    sysStatus.threshold = Serial1.parseInt();
    sysStatus.reset_threshold = Serial1.parseInt();
    sysStatus.baseline = Serial1.parseFloat();
    delay(2000);
    Particle.publish("Initialized", String::format("Threshold: %d, Reset Threshold: %d, Baseline %f", sysStatus.threshold, sysStatus.reset_threshold, sysStatus.baseline));
}

/**
 * Serial data format coming from magnetometer:
 * 
 * threshold(int),reset_threshold(int),baseline(float),mag_x(float),
 * mag_y(float),mag_z(float),mag_RMS(float),vehicle_sensed(int)/n
 */
void MagModel::loop(){       // Returns 1 if a vehicle has been fully sensed.

    Serial1.readStringUntil('\n');                          //wait for clean line        
    current.vehicle_sensed = Serial1.parseInt();            //used to check if vehicle_sensed
    sysStatus.threshold = Serial1.parseInt();               //parse threshold
    sysStatus.reset_threshold = Serial1.parseInt();         //parse reset_threshold
    sysStatus.baseline = Serial1.parseFloat();             //recalibrate baseline

    // Simple FSM for vehicle_sensed state
    switch(current.state){

        // WAITING
        case 0: 
            if(current.vehicle_sensed == 1  && current.sensing == false){    // Trigger values are the current values
                current.state = 1;
                current.sensing = true;
            }
            break;

        // SENSING
        case 1:
            digitalWrite(Light, HIGH);
            if(current.vehicle_sensed == 0 && current.done == false){   
                current.state = 2;
            }
            break;
            
        // DONE
        case 2:
            digitalWrite(Light, LOW);
            current.vehicleCount++;
            current.totalVehicleCount++;
            current.state = 0;
            current.done = true;
            break;

        default:
            break;
    }
}

int MagModel::recalibrateBaseline(){
    Serial1.println("CONF:BAS");
    return 1;
}

int MagModel::setThreshold(int thres){
    if(thres <= MAX_THRESHOLD) {
        char message[64];
        Serial1.printlnf("CONF:THR %d", thres);
        snprintf(message,sizeof(message),"New Threshold Set to %i", thres);
        Particle.publish("Update",message,PRIVATE);
        return 1;
    }
    return -1; // invalid
}

int MagModel::setResetThreshold(int reset_thres){
    if(reset_thres <= MAX_RESET_THRESHOLD) {
        char message[64];
        Serial1.printlnf("CONF:RES %d", reset_thres);
        snprintf(message,sizeof(message),"New Reset Threshold Set to: %i", reset_thres);
        Particle.publish("Update",message,PRIVATE);
        return 1;
    }
    return -1; // invalid
}

int MagModel::resetCount(){ 
    current.vehicleCount = 0;
    Particle.publish("Count Reset");
    return 1;
}