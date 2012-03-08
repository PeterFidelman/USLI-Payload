/* USLIPayload.ino - Main driver program 
 * Purpose: Constantly collect data and send via telemetry.
 * Created by the University of Washington USLI Team, 2010-2011
 * Released into the public domain - use at your own risk!
 */

#include <Accelerometer.h>
#include <Camera.h>
#include <GPS.h>
#include <LightIntensity.h>
#include <TempHumidity.h>
#include <Pressure.h>
#include <Xbee.h>

#include <digitalWriteFast.h>
#include <DHT22.h>
#include <Wire.h>
#include <Scheduler.h>

// Ports
#define DHT22_PORT 22
//DHT22 uses I2C and Wire
#define TSL235_PORT 26   // unused: set in LightIntensity.c

// Polling frequencies
#define DHT22_INTERVAL_MS 3000
#define BMP085_INTERVAL_MS 1000 
#define TSL235_INTERVAL_MS 250

// Create a task scheduler singleton
Scheduler __scheduler;

// Set up interrupt
ISR(TIMER2_OVF_vect) {
  RESET_TIMER2;
  __scheduler.__ulCounter++;    
}

// Instantiate tasks
TempHumidity _tempHumidity(DHT22_PORT, DHT22_INTERVAL_MS);
Pressure _pressure(BMP085_INTERVAL_MS);
LightIntensity _lightIntensity(TSL235_PORT, TSL235_INTERVAL_MS);

void setup() {
    // Prepare tasks
    _tempHumidity.setup();
    //_pressure.setup();  // TODO: Solder, re-add
    _lightIntensity.setup();
    
    // Start the scheduler
    __scheduler.setup();
    
    // Start each task
    __scheduler.queue(&_tempHumidity);
    //__scheduler.queue(&_pressure);  // TODO: Solder, re-add
    __scheduler.queue(&_lightIntensity);
}

void loop() {  
    __scheduler.processMessages();
}
