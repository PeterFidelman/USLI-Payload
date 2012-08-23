/* Camera.cpp - Library for interfacing with LinkSprite LS-Y201 JPEG camera
 * Created by the University of Washington USLI Team, 2010-2011
 * Released into the public domain - use at your own risk!
 */

#include "Camera.h"
#include <Arduino.h>
#include <Adafruit_VC0706.h>

#define chipSelect 53

Adafruit_VC0706 cam = Adafruit_VC0706(&Serial2);
Servo pan;  // 11
Servo roll; // 12

    int panpos  = 55;    // variables to store the servo position
    int rollpos = 89;
    int panposfloat  = 55;
    int rollposfloat = 89;
    float panangle = 0;
    float rollangle = 0;
    float xacc = 0;     // accelerations reported by accelerometer
    float yacc = 0;
    float zacc = 1;


Camera::Camera(int i_ms, Accelerometer * a)
{
    interval = i_ms;
    accmeter = a;
}

void Camera::setup()
{
  // Initialize servos
    pan.attach(11);
    roll.attach(12);

  // Initialize camera
  delay(100); // DEBUG
  Serial1.begin(9600);
  delay(100); // DEBUG
  cam.begin();
  delay(100); // DEBUG
  Serial1.println("-- Camera has been set up --");
  delay(100); // DEBUG
}

void Camera::run(Scheduler * scheduler)
{
  // Schedule task to run again
  scheduler -> schedule(this, interval);

  // aim camera
  rotateCamera();    // TODO this was just added

  cam.reset();
  cam.setImageSize(VC0706_640x480);  // biggest
  Serial1.println("-- About to take a picture --");
  delay(3000);  // wait for camera to settle
  cam.takePicture();
  //delay(3000);  // wait for camera to settle

   // Create an image with the name IMAGExx.JPG
  char filename[13];
  strcpy(filename, "IMAGE00.JPG");
  for (int i = 0; i < 100; i++) {
    filename[5] = '0' + i/10;
    filename[6] = '0' + i%10;
    // create if does not exist, do not open existing, write, sync after write
    if (! SD.exists(filename)) {
      break;
    }
  }

  Serial1.print("-- Writing to "); Serial1.print(filename);
  Serial1.println(" --");

  // Open the file for writing
  File logFile_CAM = SD.open(filename, FILE_WRITE);

  // Get the size of the image (frame) taken  
  uint16_t jpglen = cam.frameLength();

  // Read all the data up to # bytes!
  while (jpglen > 0) {
    // read 32 bytes at a time;
    uint8_t *buffer;
    uint8_t bytesToRead = min(32, jpglen); // change 32 to 64 for a speedup but may not work with all setups!
    //uint8_t bytesToRead = min(64, jpglen); // change 32 to 64 for a speedup but may not work with all setups!
    buffer = cam.readPicture(bytesToRead);
    // Write to SD card
    logFile_CAM.write(buffer, bytesToRead);
    // Write to serial too
    Serial1.write(buffer, bytesToRead);
    jpglen -= bytesToRead;
  }
  logFile_CAM.flush();
  logFile_CAM.close();
}

void Camera::rotateCamera()
{
    // pan.attach() and roll.attach() are now called in setup only
    // Camera orientation
    /*

                // Orient cameras
                // ------------------------------------
                xacc = accmeter->bma.readAccel(1);
                yacc = accmeter->bma.readAccel(2);
                zacc = accmeter->bma.readAccel(3);

                panangle = yacc*90.0;

                rollangle = (180.0/3.14159)*acos(zacc);
                Serial1.print("rollangle = ");
                Serial1.println(rollangle);

                if (zacc >= 0) {
                    if (xacc < 0) {
                        rollangle = -rollangle;
                    }
                } else {
                    if (xacc >= 0) {
                        rollangle = rollangle + 90;
                    } else {
                        rollangle = -90 - rollangle;
                    }
                }

                // roll offset correction
          //    if (rollangle >= 0) {
          //        rollangle -= 27;
          //    } else {
          //        rollangle += 27;
          //    }
                // make sure roll angle is still within -180 to 180
                //rollangle = ((int((rollangle+180) + 180)) % 360) - 180;
          //    rollangle = ((int((rollangle+180)      )) % 360) - 180;

                panposfloat = panangle * (95.0/360.0) + 55.0;
                rollposfloat = rollangle * .5 + 90;

                panpos  = int(panposfloat);
                rollpos = int(rollposfloat);

                Serial1.println("About to orient servo...");
                Serial1.flush();
                pan.write(panpos);
                roll.write(rollpos);

                Serial1.println(panpos);
                Serial1.println(rollpos);
                // Done orienting cameras
    */

}

void Camera::set_interval(Scheduler * scheduler, int new_interval)
{
    interval = new_interval;
    scheduler -> schedule(this, new_interval);
}
