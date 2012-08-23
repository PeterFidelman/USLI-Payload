/* Xbee.h - Library for interfacing with XBee-PRO XSC RF Module
 * Created by the University of Washington USLI Team, 2010-2011
 * Released into the public domain - use at your own risk!
 */

#ifndef UV_h
#define UV_h

#include "Arduino.h"
#include <Scheduler.h>
#include <SD.h>

class UV : public ITask{
    public:
        void setup();
        void run(Scheduler *);
        void set_interval(Scheduler *, int);
        UV(int, int);
    private:
        File logFile_UV;
        int interval;
        int port;
};

#endif
