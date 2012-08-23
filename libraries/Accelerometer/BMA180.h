/*
  BMA180.h - Library for initializing and obtaining
  acceleration values from the BMA180 accelerometer
  Created by Morgan Catha, April 15, 2012.
  Released into the public domain.
*/
#ifndef BMA180_h
#define BMA180_h

#include "Arduino.h"

class BMA180
{
  public:
    BMA180();
void begin();
    float readAccel(int);
  private:
void initBMA180();
    int lsbregister;
    int msbregister;
    byte msb;
    byte lsb;
boolean negative;
int mover;
float accel;
int axis;
};

#endif
