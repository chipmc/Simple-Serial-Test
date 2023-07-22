/*
 * Vehicle Counter Class
 * Author: Alex Bowen (referenced https://github.com/chipmc/TOF-Sensor-Demo/blob/master/src/PeopleCounter.h)
 * Date: July 2023
 * Description: Counts vehicles as they com
*/

#ifndef __VEHICLECOUNTER_H
#define __VEHICLECOUNTER_H

#include "Particle.h"

/**
 * This class is a singleton; you do not create one as a global, on the stack, or with new.
 * 
 * From global application setup you must call:
 * VehicleCounter::instance().setup();
 * 
 * To initiate singleton loop, which will update values
 * VehicleCounter::instance().loop();
 */
class VehicleCounter {
public:

    /**
     * @brief Gets the singleton instance of this class, allocating it if necessary
     * 
     * Use VehicleCounter::instance() to instantiate the singleton.
     */
    static VehicleCounter &instance();

    /**
     * @brief Perform setup operations; call this from global application setup()
     * 
     * You typically use VehicleCounter::instance().setup();
     */
    void setup();

    /**
     * @brief Perform application loop operations; call this from global application loop()
     * 
     * You typically use VehicleCounter::instance().loop();
     */
    int loop();

    /**
     * @brief Reads the number of vehicles counted since last call to getCountAndReset(),
     *        then resets the count to 0. A Vehicle is added to the count once it has
     *        been fully sensed, as in, when the vehicle has STOPPED being sensed.
     * 
     * Will likely be used to send counts to particle over periods of time
    */
    int getCountAndReset();

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
     * Use peopleCounter::instance() to instantiate the singleton.
     */
    VehicleCounter();

    /**
     * @brief The destructor is protected because the class is a singleton and cannot be deleted
     */
    virtual ~VehicleCounter();

    /**
     * This class is a singleton and cannot be copied
     */
    VehicleCounter(const VehicleCounter&) = delete;

    /**
     * This class is a singleton and cannot be copied
     */
    VehicleCounter& operator=(const VehicleCounter&) = delete;

    /**
     * @brief Singleton instance of this class
     * 
     * The object pointer to this class is stored here. It's NULL at system boot.
     */
    static VehicleCounter *_instance;

};
#endif  /* __VEHICLECOUNTER_H */