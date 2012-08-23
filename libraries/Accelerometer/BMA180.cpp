
/*
  BMA180.cpp - Library for initializing and obtaining
  acceleration values from the BMA180 accelerometer
  Created by Morgan Catha, April 15, 2012.
  Released into the public domain.
*/

#include <Arduino.h>
#include <BMA180.h>
#include <Wire.h>

#define address 0x40

BMA180::BMA180()
{

}

void BMA180::initBMA180()
{
Serial1.begin(9600);

Serial1.println("Initializing");
}

void BMA180::begin() 
{

Serial1.begin(9600);

Serial1.println("Beginning");
Wire.begin();



    // Connect to the ctrl_reg1 register and set the ee_w bit to enable writing.
    Wire.beginTransmission(address);
  
    Wire.write(0x0D);
    Wire.write(B00010000);
    Wire.endTransmission();
       delay(10);

    // Connect to the bw_tcs register and set the filtering level to 10hz.
    Wire.beginTransmission(address);
    Wire.write(0x20);
    Wire.write(B00001000);
    Wire.endTransmission();
        delay(10);

    // Connect to the offset_lsb1 register and set the range to +- 8.
    Wire.beginTransmission(address);
    Wire.write(0x35);
    Wire.write(byte(00001010));
    Wire.endTransmission();
        delay(10);
 

}

float BMA180::readAccel(int axis)
{

  int lsbregister;
  int msbregister;
  byte msb;
	byte lsb;
  float accel;
int mover;
boolean negative = false;

  if (axis == 1) {
    lsbregister = 2;
    msbregister = 3;
  }
    
     if (axis == 2) {
    lsbregister = 4;
    msbregister = 5;
     }
    
     if (axis == 3){
    lsbregister = 6;
    msbregister = 7;
     }
    
    Wire.beginTransmission(address);
    Wire.write(lsbregister);
    Wire.endTransmission();
    Wire.requestFrom(address, 1);
    lsb = Wire.read();
  Wire.beginTransmission(address);
  Wire.write(msbregister);
  Wire.endTransmission();
  Wire.requestFrom(address, 1);
    msb = Wire.read();
    if (bitRead(msb,7)) {
negative = true;
    msb = ~msb;
lsb = ~lsb;
}
mover = int(msb);
mover = mover << 5;
lsb &= B11111000;
lsb = lsb >> 3;
mover = mover + lsb;
accel = float(mover)*.00099;
if (negative) {
accel = -accel;
}
return accel;
}

