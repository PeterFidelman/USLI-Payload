/* Accelerometer.h - Library for interfacing with BMA180 accelerometer
 * Created by the University of Washington USLI Team, 2010-2011
 * Released into the public domain - use at your own risk!
 */

#ifndef Accelerometer_h
#define Accelerometer_h

#include <Scheduler.h>
#include <SD.h>
#include "Arduino.h"

#include <BMA180.h>
#include <Wire.h>

class Accelerometer : public ITask
{
    public:
        void setup();
        void run(Scheduler *);
        float readMagnitudeAccel(void); 
        Accelerometer(int);
        BMA180 bma;     // made this public
    private:
        File logFile_AC;
        int interval;
        void printFloat(float, int);
};

#endif
