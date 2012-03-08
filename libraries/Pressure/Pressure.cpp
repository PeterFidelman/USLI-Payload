/* Pressure.cpp - Library for interfacing with Bosch BMP085 pressure sensor
 */

#include "Pressure.h"

//------------------------------------------------------------------------
Pressure::Pressure(int i_ms) {
    OSS = 0;  // Oversampling Setting

    interval = i_ms;
}

//------------------------------------------------------------------------
void Pressure::setup() {
    Serial.begin(9600);
    Wire.begin();
    Serial.println("BMP085: Wire has been initialized");
    Serial.print("BMP085: Calibrating... ");

    bmp085Calibration();
    Serial.println("done!");
}

//------------------------------------------------------------------------
void Pressure::run(Scheduler * scheduler) {
    // Schedule task to recur after "interval" ms
    scheduler -> schedule(this, interval);

    temperature = bmp085GetTemperature(bmp085ReadUT());
    pressure = bmp085GetPressure(bmp085ReadUP());
    Serial.print("BMP085...");
    Serial.print("Pressure>>");
    Serial.print(pressure, DEC);
    Serial.print(" Pa; ");
    Serial.print("Temperature>>");
    char buf[10];
    Serial.print(dtostrf(temperature*0.1, 2, 2, buf));
    Serial.println(" deg C");
}

// Stores all of the bmp085's calibration values into global variables
// Calibration values are required to calculate temp and pressure
// This function should be called at the beginning of the program
void Pressure::bmp085Calibration()
{
  ac1 = bmp085ReadInt(0xAA);
  ac2 = bmp085ReadInt(0xAC);
  ac3 = bmp085ReadInt(0xAE);
  ac4 = bmp085ReadInt(0xB0);
  ac5 = bmp085ReadInt(0xB2);
  ac6 = bmp085ReadInt(0xB4);
  b1 = bmp085ReadInt(0xB6);
  b2 = bmp085ReadInt(0xB8);
  mb = bmp085ReadInt(0xBA);
  mc = bmp085ReadInt(0xBC);
  md = bmp085ReadInt(0xBE);
}

//------------------------------------------------------------------------
// Calculate temperature given ut.
// Value returned will be in units of 0.1 deg C
short Pressure::bmp085GetTemperature(unsigned int ut)
{
  long x1, x2;
  
  x1 = (((long)ut - (long)ac6)*(long)ac5) >> 15;
  x2 = ((long)mc << 11)/(x1 + md);
  b5 = x1 + x2;

  return ((b5 + 8)>>4);  
}

//------------------------------------------------------------------------
// Calculate pressure given up
// calibration values must be known
// b5 is also required so bmp085GetTemperature(...) must be called first.
// Value returned will be pressure in units of Pa.
long Pressure::bmp085GetPressure(unsigned long up)
{
  long x1, x2, x3, b3, b6, p;
  unsigned long b4, b7;
  
  b6 = b5 - 4000;
  // Calculate B3
  x1 = (b2 * (b6 * b6)>>12)>>11;
  x2 = (ac2 * b6)>>11;
  x3 = x1 + x2;
  b3 = (((((long)ac1)*4 + x3)<<OSS) + 2)>>2;
  
  // Calculate B4
  x1 = (ac3 * b6)>>13;
  x2 = (b1 * ((b6 * b6)>>12))>>16;
  x3 = ((x1 + x2) + 2)>>2;
  b4 = (ac4 * (unsigned long)(x3 + 32768))>>15;
  
  b7 = ((unsigned long)(up - b3) * (50000>>OSS));
  if (b7 < 0x80000000)
    p = (b7<<1)/b4;
  else
    p = (b7/b4)<<1;
    
  x1 = (p>>8) * (p>>8);
  x1 = (x1 * 3038)>>16;
  x2 = (-7357 * p)>>16;
  p += (x1 + x2 + 3791)>>4;
  
  return p;
}

//------------------------------------------------------------------------
// Read 1 byte from the BMP085 at 'address'
char Pressure::bmp085Read(unsigned char address)
{
  unsigned char data;
  
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();
  
  Wire.requestFrom(BMP085_ADDRESS, 1);
  while(!Wire.available())
    ;
    
  return Wire.read();
}

//------------------------------------------------------------------------
// Read 2 bytes from the BMP085
// First byte will be from 'address'
// Second byte will be from 'address'+1
int Pressure::bmp085ReadInt(unsigned char address)
{
  unsigned char msb, lsb;
  
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();
  
  Wire.requestFrom(BMP085_ADDRESS, 2);
  while(Wire.available()<2)
    ;
  msb = Wire.read();
  lsb = Wire.read();
  
  return (int) msb<<8 | lsb;
}

//------------------------------------------------------------------------
// Read the uncompensated temperature value
unsigned int Pressure::bmp085ReadUT()
{
  unsigned int ut;
  
  // Write 0x2E into Register 0xF4
  // This requests a temperature reading
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF4);
  Wire.write(0x2E);
  Wire.endTransmission();
  
  // Wait at least 4.5ms
  // PSF Change: Quadrupled delay from 5 to 20
  delay(20);
  
  // Read two bytes from registers 0xF6 and 0xF7
  ut = bmp085ReadInt(0xF6);
  return ut;
}

//------------------------------------------------------------------------
// Read the uncompensated pressure value
unsigned long Pressure::bmp085ReadUP()
{
  unsigned char msb, lsb, xlsb;
  unsigned long up = 0;
  
  // Write 0x34+(OSS<<6) into register 0xF4
  // Request a pressure reading w/ oversampling setting
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF4);
  Wire.write(0x34 + (OSS<<6));
  Wire.endTransmission();
  
  // Wait for conversion, delay time dependent on OSS
  // PSF Change: Quadrupled delay
  delay((2 + (3<<OSS)) * 4);
  
  // Read register 0xF6 (MSB), 0xF7 (LSB), and 0xF8 (XLSB)
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF6);
  Wire.endTransmission();
  Wire.requestFrom(BMP085_ADDRESS, 3);
  
  // Wait for data to become available
  while(Wire.available() < 3)
    ;
  msb = Wire.read();
  lsb = Wire.read();
  xlsb = Wire.read();
  
  up = (((unsigned long) msb << 16) | ((unsigned long) lsb << 8) | (unsigned long) xlsb) >> (8-OSS);
  
  return up;
}
