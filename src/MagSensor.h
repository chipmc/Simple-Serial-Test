/**
 * Magnetometer Sensor Class
 * Author: Alex Bowen (referenced https://github.com/chipmc/TOF-Sensor-Demo/blob/master/src/TofSensor.h)
 * Date: July 2023
 * Description: Defines functions and instances of a singleton MagSensor class, which will
 *              read the data stream from the magnetometer and return 1 after a vehicle
 *              is sensed. Will be used with VehicleCounter to increment the vehicle count.
*/

#ifndef __MAGSENSOR_H
#define __MAGSENSOR_H

#include "Particle.h"

/**
 * This class is a singleton; you do not create one as a global, on the stack, or with new.
 * 
 * To initiate setup of singleton
 * MagSensor::instance().setup();
 * 
 * To initiate singleton loop, which will update values
 * MagSensor::instance().loop();
 */
class MagSensor {
public:

    /**
     * @brief Gets the singleton instance of this class, allocating it if necessary
     * 
     * Use MagSensor::instance() to instantiate the singleton.
     */
    static MagSensor &instance();

    /**
     * @brief Perform setup operations; call this from global application setup()
     * Returns 1 when setup is finished.
     * 
     * You typically use MagSensor::instance().setup();
     */
    void setup();

    /**
     * @brief Perform application loop operations; call this from global application loop()
     * Returns data if a vehicle has been fully sensed, returning the value
     * when a vehicle has STOPPED being sensed.
     * 
     * You typically use MagSensor::instance().update();
     */
    int loop();

    /**
     * @brief Returns the baseline value of the Magnetometer
    */
    float getBaseline();
    
    /**
     * @brief Recalibrates the baseline value of the Magnetometer. Returns baseline value if successful.
    */
    float recalibrateBaseline();

    /**
     * @brief Gets the threshold, which tells the magnetometer when to BEGIN sensing a vehicle
    */
    int getThreshold();

    /**
     * @brief Sets the threshold, which tells the magnetometer when to BEGIN sensing a vehicle
     * returns 1 if executed successfully for debugging purposes.
    */
    int setThreshold(int threshold);

    /**
     * @brief Sets the reset threshold, which tells the magnetometer when to STOP sensing a vehicle
    */
    int getResetThreshold();

    /**
     * @brief Sets the reset threshold, which tells the magnetometer when to STOP sensing a vehicle
     * returns 1 if executed successfully for debugging purposes.
    */
    int setResetThreshold(int reset_threshold);

protected:
    /**
     * @brief The constructor is protected because the class is a singleton
     * 
     * Use MagSensor::instance() to instantiate the singleton.
     */
    MagSensor();

    /**
     * @brief The destructor is protected because the class is a singleton and cannot be deleted
     */
    virtual ~MagSensor();

    /**
     * This class is a singleton and cannot be copied
     */
    MagSensor(const MagSensor&) = delete;

    /**
     * This class is a singleton and cannot be copied
     */
    MagSensor& operator=(const MagSensor&) = delete;

    /**
     * @brief Singleton instance of this class
     * 
     * The object pointer to this class is stored here. It's NULL at system boot.
     */
    static MagSensor *_instance;

};
#endif  /* __MAGSENSOR_H */