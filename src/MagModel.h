/**
 * Magnetometer Sensor Class
 * Author: Alex Bowen (referenced https://github.com/chipmc/TOF-Sensor-Demo/blob/master/src/TofSensor.h)
 * Date: July 2023
 * Description: Defines functions and instances of a singleton MagModel class, which will contain data
 *              about a vehicle detection as well as the business logic for parsing and updating that data
 *              using the serial stream coming from the Magnetometer.
 *              
*/

#ifndef __MAGSENSOR_H
#define __MAGSENSOR_H

#include "Particle.h"

/**
 * This class is a singleton; you do not create one as a global, on the stack, or with new.
 * 
 * To initiate setup of singleton
 * MagModel::instance().setup();
 * 
 * To initiate singleton loop, which will update values
 * MagModel::instance().loop();
 */
class MagModel {
public:

    /**
     * @brief Gets the singleton instance of this class, allocating it if necessary
     * 
     * Use MagModel::instance() to instantiate the singleton.
     */
    static MagModel &instance();

    /**
     * @brief Perform setup operations; call this from global application setup()
     * Returns 1 when setup is finished.
     * 
     * You typically use MagModel::instance().setup();
     */
    void setup();

    /**
     * @brief Perform application loop operations; call this from global application loop()
     * Updates values of vehicleData
     */
    void loop();

    /**
     * @brief Returns the baseline value of the Magnetometer
    */
    float getBaseline();
    
    /**
     * @brief Recalibrates and returns the baseline value of the Magnetometer.
    */
    int recalibrateBaseline();

    /**
     * @brief Gets the threshold, which tells the magnetometer when to BEGIN sensing a vehicle
    */
    int getThreshold();

    /**
     * @brief Sets the threshold, which tells the magnetometer when to BEGIN sensing a vehicle
    */
    int setThreshold(int threshold);

    /**
     * @brief Gets the reset threshold, which tells the magnetometer when to STOP sensing a vehicle
    */
    int getResetThreshold();

    /**
     * @brief Sets the reset threshold, which tells the magnetometer when to STOP sensing a vehicle
    */
    int setResetThreshold(int reset_threshold);
    
    /**
     * @brief Reads the number of vehicles counted since last call to getCountAndReset(),
     *        then resets the count to 0. A Vehicle is added to the count once it has
     *        been fully sensed, as in, when the vehicle has STOPPED being sensed.
     * 
     * Will likely be used to send counts to particle over periods of time
    */
    int resetCount();

    /**
     * @brief Retrieves the total number of vehicles sensed. A Vehicle is added to 
     *        the count once it has been fully sensed, as in, when the vehicle has 
     *        STOPPED being sensed.
    */
    int getTotalVehicleCount();

protected:
    
    /**
     * @brief The constructor is protected because the class is a singleton
     * 
     * Use MagModel::instance() to instantiate the singleton.
     */
    MagModel();

    /**
     * @brief The destructor is protected because the class is a singleton and cannot be deleted
     */
    virtual ~MagModel();

    /**
     * This class is a singleton and cannot be copied
     */
    MagModel(const MagModel&) = delete;

    /**
     * This class is a singleton and cannot be copied
     */
    MagModel& operator=(const MagModel&) = delete;

    /**
     * @brief Singleton instance of this class
     * 
     * The object pointer to this class is stored here. It's NULL at system boot.
     */
    static MagModel *_instance;

};
#endif  /* __MAGSENSOR_H */