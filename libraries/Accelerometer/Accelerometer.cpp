/* Accelerometer.cpp - Library for interfacing with BMA180 accelerometer
 * Created by the University of Washington USLI Team, 2010-2011
 * Released into the public domain - use at your own risk!
 */

#include "Arduino.h"
#include "Accelerometer.h"

File logFile_AC;
BMA180 bma;

Accelerometer::Accelerometer(int i_ms)
{
    interval = i_ms;
}

void Accelerometer::setup()
{
    //pinMode(32, OUTPUT);    // Wakeup pin
    //digitalWrite(32, HIGH); // WAKE UP

    Serial1.begin(9600);
    bma.begin();

    //digitalWrite(32, LOW); // GO TO SLEEP
}

void Accelerometer::run(Scheduler * scheduler)
{
    // open log file anew
    logFile_AC = SD.open("ACCEL.LOG", FILE_WRITE);

    // Schedule task to recur after "interval" ms
    scheduler -> schedule(this, interval);

    //digitalWrite(32, HIGH); // WAKE UP
    //delay(5); // give device some time to wake up

    Serial1.print("Accel>> ");
    logFile_AC.print("Accel>> ");  // SD
    Serial1.print(millis());
    logFile_AC.print(millis());  // SD
    Serial1.print(" ");
    logFile_AC.print(" ");  // SD

    float xaccel = bma.readAccel(1);  // 1 == axis
    float yaccel = bma.readAccel(2);  // 2 == axis
    float zaccel = bma.readAccel(3);  // 3 == axis

    printFloat(xaccel,6);
    Serial1.print(" ");
    logFile_AC.print(" ");

    printFloat(yaccel,6);
    Serial1.print(" ");
    logFile_AC.print(" ");

    printFloat(zaccel,6);
    Serial1.println(" ");
    logFile_AC.println(" ");

    // Make sure updates to file were saved
    logFile_AC.flush();
    logFile_AC.close();
}

// printFloat prints out the float 'value' rounded to 'places' places after the decimal point
void Accelerometer::printFloat(float value, int places) {
    // this is used to cast digits
    int digit;
    float tens = 0.1;
    int tenscount = 0;
    int i;
    float tempfloat = value;

    // make sure we round properly. this could use pow from <math.h>, but doesn't seem worth the import
    // if this rounding step isn't here, the value  54.321 prints as 54.3209

    // calculate rounding term d:   0.5/pow(10,places)  
    float d = 0.5;
    if (value < 0)
        d *= -1.0;
    // divide by ten for each decimal place
    for (i = 0; i < places; i++)
        d/= 10.0;    
    // this small addition, combined with truncation will round our values properly
    tempfloat +=  d;

    // first get value tens to be the large power of ten less than value
    // tenscount isn't necessary but it would be useful if you wanted to know after this how many
    // chars the number will take

    if (value < 0)
        tempfloat *= -1.0;
    while ((tens * 10.0) <= tempfloat) {
        tens *= 10.0;
        tenscount += 1;
    }


    // write out the negative if needed
    if (value < 0) {
        Serial1.print('-');
        logFile_AC.print('-');
    }

    if (tenscount == 0) {
        Serial1.print(0, DEC);
        logFile_AC.print(0, DEC);
    }

    for (i=0; i< tenscount; i++) {
        digit = (int) (tempfloat/tens);
        Serial1.print(digit, DEC);
        logFile_AC.print(digit, DEC);
        tempfloat = tempfloat - ((float)digit * tens);
        tens /= 10.0;
    }

    // if no places after decimal, stop now and return
    if (places <= 0)
        return;

    // otherwise, write the point and continue on
      Serial1.print('.');  
      logFile_AC.print('.');  

    // now write out each decimal place by shifting digits one by one into the ones place and writing
    // the truncated value
    for (i = 0; i < places; i++) {
        tempfloat *= 10.0;
        digit = (int) tempfloat;
        Serial1.print(digit,DEC);  
        logFile_AC.print(digit,DEC);  
        // once written, subtract off that digit
        tempfloat = tempfloat - (float) digit;
    }
}

float Accelerometer::readMagnitudeAccel() 
{
    float magnitude = 0;

    float xComponent = bma.readAccel(1);
    float yComponent = bma.readAccel(2);
    float zComponent = bma.readAccel(3);

    magnitude = (xComponent * xComponent) +
                (yComponent * yComponent) +
                (zComponent * zComponent);

    magnitude = sqrt(magnitude);

    return magnitude;
}
