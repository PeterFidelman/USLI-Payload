/* Xbee.cpp - Library for interfacing with XBee-PRO XSC RF Module
 * Created by the University of Washington USLI Team, 2010-2011
 * Released into the public domain - use at your own risk!
 */

#include "Arduino.h"
#include "UV.h"

File logFile_UV;

UV::UV(int p, int i_ms)
{
    port = p;
    interval = i_ms;
}

void UV::setup()
{
    Serial1.begin(9600);
    Serial1.println("UV has been set up.");
}

void UV::run(Scheduler * scheduler)
{
    logFile_UV = SD.open("UV.LOG", FILE_WRITE);

    // Schedule task to recur after "interval" ms
    scheduler -> schedule(this, interval);

    int sensorValue = analogRead(port);

    Serial1.print("UV>>");
    logFile_UV.print("UV>>");
    Serial1.print(" ");
    logFile_UV.print(" ");
    Serial1.print(millis());
    logFile_UV.print(millis());
    Serial1.print(" ");
    logFile_UV.print(" ");
    Serial1.print(sensorValue, DEC);
    logFile_UV.print(sensorValue, DEC);
    Serial1.println();
    logFile_UV.println();

    // Make sure updates to file were saved
    logFile_UV.flush();
    logFile_UV.close();
}

void UV::set_interval(Scheduler * scheduler, int new_interval)
{
    interval = new_interval;
    scheduler -> schedule(this, new_interval);
}
