/* GPS.h - Library for interfacing with Locosys LS2003x-series GPS receiver
 * Created by the University of Washington USLI Team, 2010-2011
 * Released into the public domain - use at your own risk!
 */

#ifndef GPS_h
#define GPS_h

#include <Scheduler.h>
#include <SD.h>
#include "Arduino.h"

class GPS : public ITask
{
    public:
        void setup();
        void run(Scheduler *);
        GPS(int);
    private:
        File logFile_GPS;
        int interval;
};

#endif
