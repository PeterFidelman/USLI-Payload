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
#include <UV.h>
#include <Xbee.h>

#include <Camera.h>
#include <SoftwareSerial.h>  // unused

#include <digitalWriteFast.h>
#include <DHT22.h>
#include <SD.h>
#include <Wire.h>
#include <Scheduler.h>

// Ports
#define DHT22_PORT 22
//BMP085 uses I2C and Wire
//Xbee uses Serial1
//Camera uses Serial2
//GPS uses Serial3
//SD uses SPI
#define TSL235_PORT 26       // unused: set in LightIntensity.c
//Accelerometer uses I2C and Wire
#define UV_PORT 1            // Analog pin 1
#define RAD_PORT 2           // Analog pin 2
#define RAD_INTERRUPT 0      // Lives on digital pin #2
#define RAD_INTERRUPT_PIN 2  // Interrupt #0 lives in digital pin #2
#define POWER_PIN 33         // For turning power to sensors on/off
// Servos are modularized with accelerometer

// Polling frequencies
#define DHT22_INTERVAL_MS 3000
#define BMP085_INTERVAL_MS 1000
//#define CAMERA_INTERVAL_INACTIVE_MS (1000*60*5)
#define CAMERA_INTERVAL_INACTIVE_MS (1000*30)
#define CAMERA_INTERVAL_ACTIVE_MS (1000*30)
#define GPS_INTERVAL 1000
#define TSL235_INTERVAL_MS 250
#define ACCEL_INTERVAL 500   // TODO adjust
#define UV_INTERVAL 250      // TODO
//#define RAD_INTERVAL 99999   // N/A
//#define SERVO_INTERVAL 99999 // N/A

// How long the payload should be at rest before it is considered
// "landed". Measured (more or less) in seconds.
#define REST_THRESHOLD 5
// How many G's should be considered launch TODO adjust
#define LAUNCH_THRESHOLD 4

// Flight events
bool hasLaunched = false;
bool hasLanded = false;

// Radiation detector state
volatile bool _particleReceived;
volatile int  _particleIntensity;
File logFile_RAD;
File logFile_EVT;


// Create a task scheduler singleton
Scheduler __scheduler;

// Set up interrupt
ISR(TIMER2_OVF_vect) {
  RESET_TIMER2;
  __scheduler.__ulCounter++;    
}

// Instantiate tasks
TempHumidity _tempHumidity(DHT22_PORT, DHT22_INTERVAL_MS);
//Camera _camera(CAMERA_INTERVAL_INACTIVE_MS);
GPS _gps(GPS_INTERVAL);
LightIntensity _lightIntensity(TSL235_PORT, TSL235_INTERVAL_MS,
    false); //morse true/false
Accelerometer _accelerometer(ACCEL_INTERVAL);
UV _uv(UV_PORT, UV_INTERVAL);
Pressure _pressure(BMP085_INTERVAL_MS);

void setup() {
    Serial1.begin(9600);
    Serial1.println("Start");

    // Power
    pinMode(POWER_PIN, OUTPUT);
    digitalWrite(POWER_PIN, HIGH);   // HIGH=sensors on   LOW=sensors off

    // Prepare SD card
    if(!SD.begin(53)) {
        Serial1.println("WARNING! SD CARD NOT FOUND");
    }

    // Prepare tasks
    _tempHumidity.setup();
    //_camera.setup();
    _gps.setup();
    _lightIntensity.setup();
    _accelerometer.setup();
    _uv.setup();
    _pressure.setup();   // this really does need to be at the end
    
    // Start the scheduler
    __scheduler.setup();
    
    // Start each task
    __scheduler.queue(&_tempHumidity);
    __scheduler.queue(&_pressure);
    //__scheduler.queue(&_camera);
    __scheduler.queue(&_gps);
    __scheduler.queue(&_lightIntensity);
    __scheduler.queue(&_accelerometer);
    __scheduler.queue(&_uv);

    // Set up radiation detector
    pinMode(RAD_INTERRUPT_PIN, INPUT);     // default state of interrupt-pin
    //digitalWrite(RAD_INTERRUPT_PIN, HIGH); // turn on pull-up resistor
    attachInterrupt(RAD_INTERRUPT, ParticleDataSetter, RISING);

    // Open log file for radiation detector
    logFile_RAD = SD.open("RADIATN.LOG", FILE_WRITE);

    // Open log file for "flight events" (launch/landing)
    logFile_EVT = SD.open("EVENTS.LOG", FILE_WRITE);
}

void loop() {  
    __scheduler.processMessages();

    static int rest_count = 0;    // init only happens once
    float current_g = _accelerometer.readMagnitudeAccel();

    // Execute "every second" more or less
    if(millis() % 1000 == 0) {
        Serial1.print("executing > current_g = ");
        Serial1.println(current_g);
        // Detect launch
        if(current_g > LAUNCH_THRESHOLD) {
            // Alert
            Serial1.println("WARNING: Launch detected");
            logFile_EVT.println("WARNING: Launch detected");  // SD

            // do stuff here that must occur only ONCE when launch is
            // detected
            if(!hasLaunched)
                _uv.set_interval(&__scheduler, 250);  // speed up TODO remove

            hasLaunched = true;
        }

        // Detect landing
        if (hasLaunched && (0.995 < current_g) && (current_g < 1.005)) {
            rest_count++;
            if(rest_count > REST_THRESHOLD) {
                Serial1.println("WARNING: Landing detected");
                logFile_EVT.println("WARNING: Landing detected");  // SD

                // do stuff here that must occur only ONCE when landing is
                // detected
                // if(!hasLaunched) { ...

                // TODO: Orient cameras
                hasLanded = true;
            }
        } else {
            rest_count = 0;
        }
    }

    /* Put checking for one-time events here */
    if(_particleReceived) {
        _particleReceived = false;
        LogParticle(_particleIntensity);
    }
    //if(millis() % 1000 == 0) {
        //Serial.print("heartbeat ->");
        //Serial.println(millis());
    //}
}

// For Teviso RD1014 radiation detector
void ParticleDataSetter()
{
    // Interrupt handler for Teviso RD1014
    _particleReceived = true;
    _particleIntensity = analogRead(RAD_PORT);
}

// For Teviso RD1014 radiation detector
void LogParticle(int intensity)
{
    Serial1.print("PARTICLE>>");
    logFile_RAD.print("PARTICLE>>");
    Serial1.print(" ");
    logFile_RAD.print(" ");
    Serial1.print(millis());
    logFile_RAD.print(millis());
    Serial1.print(" ");
    logFile_RAD.print(" ");
    Serial1.print(intensity, DEC);
    logFile_RAD.print(intensity, DEC);
    Serial1.println();
    logFile_RAD.println();

    // Make sure updates to the file were saved
    logFile_RAD.flush();
}


