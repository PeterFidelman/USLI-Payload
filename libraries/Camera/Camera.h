/* Camera.h - Library for interfacing with LinkSprite LS-Y201 JPEG camera
 * Created by the University of Washington USLI Team, 2010-2011
 * Released into the public domain - use at your own risk!
 */

#ifndef Camera_h
#define Camera_h

#include "Arduino.h"
#include <SD.h>
#include <Scheduler.h>
#include <Accelerometer.h>    // god that hurt to write
#include "Servo.h"  // so did that

class Camera : public ITask
{
    public:
        void setup();
        void run(Scheduler *);
        void set_interval(Scheduler *, int);
        Camera(int, Accelerometer *);    // god that hurt to write

    private:
        void rotateCamera(void);
        Accelerometer * accmeter;
        File logFile_CAM;
        int interval;
};

#endif
