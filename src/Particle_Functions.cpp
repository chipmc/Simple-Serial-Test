#include "Particle.h"
#include "Particle_Functions.h"
#include "MagModel.h"           

// Prototypes and System Mode calls
SYSTEM_MODE(SEMI_AUTOMATIC);                        // This will enable user code to start executing automatically.
SYSTEM_THREAD(ENABLED);                             // Means my code will not be held up by Particle processes.
STARTUP(System.enableFeature(FEATURE_RESET_INFO));

SerialLogHandler logHandler(LOG_LEVEL_INFO);     // Easier to see the program flow

// Battery Conect variables
// Battery conect information - https://docs.particle.io/reference/device-os/firmware/boron/#batterystate-
const char* batteryContext[7] = {"Unknown","Not Charging","Charging","Charged","Discharging","Fault","Diconnected"};

Particle_Functions *Particle_Functions::_instance;

// [static]
Particle_Functions &Particle_Functions::instance() {
    if (!_instance) {
        _instance = new Particle_Functions();
    }
    return *_instance;
}

Particle_Functions::Particle_Functions() {
}

Particle_Functions::~Particle_Functions() {
}

void Particle_Functions::setup() {
    Log.info("Initializing Particle functions and variables");     // Note: Don't have to be connected but these functions need to in first 30 seconds

    // Declare Particle variables.
    Particle.variable("Baseline", (double)MagModel::instance().getBaseline());
    Particle.variable("Threshold", (double)MagModel::instance().getThreshold());
    Particle.variable("Reset Threshold", (double)MagModel::instance().getResetThreshold());
    Particle.variable("Last Count", (double)MagModel::instance().getCountAndReset());
    Particle.variable("Total Vehicles", (double)MagModel::instance().getTotalVehicleCount());
    Particle.variable("Recalibrate Baseline", (double)MagModel::instance().recalibrateBaseline());

    // Declare Particle functions.
    Particle.function("Set the Threshold", &Particle_Functions::setThreshold, this);
    Particle.function("Set the Reset Threshold", &Particle_Functions::setResetThreshold, this);
}

void Particle_Functions::loop() {
    // Put your code to run during the application thread loop here
}

int Particle_Functions::setThreshold(String threshold){
  return MagModel::instance().setThreshold(threshold.toInt()); // returns -1 if > MAX_THRESHOLD
}

int Particle_Functions::setResetThreshold(String reset_threshold){
  return MagModel::instance().setResetThreshold(reset_threshold.toInt()); ;// returns -1 if > MAX_RESET_THRESHOLD
}

bool Particle_Functions::disconnectFromParticle() {                    // Ensures we disconnect cleanly from Particle
                                                                       // Updated based on this thread: https://community.particle.io/t/waitfor-particle-connected-timeout-does-not-time-out/59181                                                                      		// Updated based on this thread: https://community.particle.io/t/waitfor-particle-connected-timeout-does-not-time-out/59181
  time_t startTime = Time.now();
  Log.info("In the disconnect from Particle function");
  Particle.disconnect();                                               		// Disconnect from Particle
  waitForNot(Particle.connected, 15000);                               		// Up to a 15 second delay() 
  Particle.process();
  if (Particle.connected()) {                      							// As this disconnect from Particle thing can be a·syn·chro·nous, we need to take an extra step to wait, 
    Log.info("Failed to disconnect from Particle");
    return(false);
  }
  else Log.info("Disconnected from Particle in %i seconds", (int)(Time.now() - startTime));
  // Then we need to disconnect from Cellular and power down the cellular modem
  startTime = Time.now();
  Cellular.disconnect();                                               // Disconnect from the cellular network
  Cellular.off();                                                      // Turn off the cellular modem
  waitFor(Cellular.isOff, 30000);                                      // As per TAN004: https://support.particle.io/hc/en-us/articles/1260802113569-TAN004-Power-off-Recommendations-for-SARA-R410M-Equipped-Devices
  Particle.process();
  if (Cellular.isOn()) {                                               // At this point, if cellular is not off, we have a problem
    Log.info("Failed to turn off the Cellular modem");
    return(false);                                                     // Let the calling function know that we were not able to turn off the cellular modem
  }
  else {
    Log.info("Turned off the cellular modem in %i seconds", (int)(Time.now() - startTime));
    return true;
  }
}
