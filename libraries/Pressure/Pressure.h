/* Pressure.h - Library for interfacing with Bosch BMP085 pressure sensor
 */

#ifndef Pressure_h
#define Pressure_h

#include "Arduino.h"
#include <Wire.h>
#include <Scheduler.h>
#include <SD.h>

#define BMP085_ADDRESS 0x77   // I2C address of BMP085

class Pressure : public ITask
{
    public:
        Pressure(int);
        void setup();
        void run(Scheduler *);
        void set_interval(Scheduler *, int);

    private:
        void bmp085Calibration();
        short bmp085GetTemperature(unsigned int);
        long bmp085GetPressure(unsigned long);
        char bmp085Read(unsigned char);
        int bmp085ReadInt(unsigned char);
        unsigned int bmp085ReadUT();
        unsigned long bmp085ReadUP();


        int interval;   // Sampling interval in ms
        unsigned char OSS;    // Oversampling setting
        int ac1;                    // Calibration values
        int ac2; 
        int ac3; 
        unsigned int ac4;
        unsigned int ac5;
        unsigned int ac6;
        int b1; 
        int b2;
        int mb;
        int mc;
        int md;
        long b5;
        short temperature;
        long pressure;
};

#endif
