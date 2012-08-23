/* GPS.h - Library for interfacing with Locosys LS2003x-series GPS receiver
 * Created by the University of Washington USLI Team, 2010-2011
 * Released into the public domain - use at your own risk!
 */

#include "Arduino.h"
#include "GPS.h"
#include "TinyGPS.h"

TinyGPS gps;
#define GPS_SERIAL Serial3

static void gpsdump(TinyGPS &gps);
static bool feedgps();
static void print_float(float val, float invalid, int len, int prec);
static void print_int(unsigned long val, unsigned long invalid, int len);
static void print_date(TinyGPS &gps);
static void print_str(const char *str, int len);

// Global
File logFile_GPS;

GPS::GPS(int i_ms) {
  interval = i_ms;
}

void GPS::setup()
{
  Serial1.begin(9600);
  //GPS_SERIAL.begin(57600);   // LinkSprite LS20031
  GPS_SERIAL.begin(4800);      // Parallax PMB-648

  logFile_GPS = SD.open("GPS.log", FILE_WRITE);

  Serial1.print("Testing TinyGPS library v. "); Serial1.println(TinyGPS::library_version());
  Serial1.println("by Mikal Hart");
  Serial1.println();
  Serial1.print("Sizeof(gpsobject) = "); Serial1.println(sizeof(TinyGPS));
  Serial1.println();
  Serial1.println("Sats HDOP Latitude Longitude Fix  Date       Time       Date Alt     Course Speed Card  Distance Course Card  Chars Sentences Checksum");
  Serial1.println("          (deg)    (deg)     Age                        Age  (m)     --- from GPS ----  ---- to London  ----  RX    RX        Fail");
  Serial1.println("--------------------------------------------------------------------------------------------------------------------------------------");
  Serial1.flush();

  logFile_GPS.print("Testing TinyGPS library v. "); logFile_GPS.println(TinyGPS::library_version());
  logFile_GPS.println("by Mikal Hart");
  logFile_GPS.println();
  logFile_GPS.print("Sizeof(gpsobject) = "); logFile_GPS.println(sizeof(TinyGPS));
  logFile_GPS.println();
  logFile_GPS.println("Sats HDOP Latitude Longitude Fix  Date       Time       Date Alt     Course Speed Card  Distance Course Card  Chars Sentences Checksum");
  logFile_GPS.println("          (deg)    (deg)     Age                        Age  (m)     --- from GPS ----  ---- to London  ----  RX    RX        Fail");
  logFile_GPS.println("--------------------------------------------------------------------------------------------------------------------------------------");

  logFile_GPS.close();

}

void GPS::run(Scheduler * scheduler)
{
  scheduler -> schedule(this, interval);
  bool newdata = false;
  unsigned long start = millis();

//  while (GPS_SERIAL.available()) {
//      Serial.print(GPS_SERIAL.read());
//  }

  logFile_GPS = SD.open("GPS.log", FILE_WRITE);  // SD: Open file anew
  
  //while (millis() - start < 1000)
  {
    if (feedgps())
      newdata = true;
  }
  
  gpsdump(gps);

  // Make sure updates to the file were saved
  logFile_GPS.flush();
  logFile_GPS.close();
  
}

static void gpsdump(TinyGPS &gps)
{
  float flat, flon;
  unsigned long age, date, time, chars = 0;
  unsigned short sentences = 0, failed = 0;
  static const float LONDON_LAT = 51.508131, LONDON_LON = -0.128002;
  
  print_int(gps.satellites(), TinyGPS::GPS_INVALID_SATELLITES, 5);
  print_int(gps.hdop(), TinyGPS::GPS_INVALID_HDOP, 5);
  gps.f_get_position(&flat, &flon, &age);
  print_float(flat, TinyGPS::GPS_INVALID_F_ANGLE, 9, 5);
  print_float(flon, TinyGPS::GPS_INVALID_F_ANGLE, 10, 5);
  print_int(age, TinyGPS::GPS_INVALID_AGE, 5);

  print_date(gps);

  print_float(gps.f_altitude(), TinyGPS::GPS_INVALID_F_ALTITUDE, 8, 2);
  print_float(gps.f_course(), TinyGPS::GPS_INVALID_F_ANGLE, 7, 2);
  print_float(gps.f_speed_kmph(), TinyGPS::GPS_INVALID_F_SPEED, 6, 2);
  print_str(gps.f_course() == TinyGPS::GPS_INVALID_F_ANGLE ? "*** " : TinyGPS::cardinal(gps.f_course()), 6);
  print_int(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0UL : (unsigned long)TinyGPS::distance_between(flat, flon, LONDON_LAT, LONDON_LON) / 1000, 0xFFFFFFFF, 9);
  print_float(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : TinyGPS::course_to(flat, flon, 51.508131, -0.128002), TinyGPS::GPS_INVALID_F_ANGLE, 7, 2);
  print_str(flat == TinyGPS::GPS_INVALID_F_ANGLE ? "*** " : TinyGPS::cardinal(TinyGPS::course_to(flat, flon, LONDON_LAT, LONDON_LON)), 6);

  gps.stats(&chars, &sentences, &failed);
  print_int(chars, 0xFFFFFFFF, 6);
  print_int(sentences, 0xFFFFFFFF, 10);
  print_int(failed, 0xFFFFFFFF, 9);

  Serial1.print(millis()); // system uptime
  Serial1.println();
}



/************************************************
    Other Functions
************************************************/


static void print_int(unsigned long val, unsigned long invalid, int len)
{
  char sz[32];
  if (val == invalid)
    strcpy(sz, "*******");
  else
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i=strlen(sz); i<len; ++i)
    sz[i] = ' ';
  if (len > 0) 
    sz[len-1] = ' ';
  Serial1.print(sz);
  logFile_GPS.print(sz);
  feedgps();
}

static void print_float(float val, float invalid, int len, int prec)
{
  char sz[32];
  if (val == invalid)
  {
    strcpy(sz, "*******");
    sz[len] = 0;
        if (len > 0) 
          sz[len-1] = ' ';
    for (int i=7; i<len; ++i)
        sz[i] = ' ';
    Serial1.print(sz);
    logFile_GPS.print(sz);
  }
  else
  {
    Serial1.print(val, prec);
    logFile_GPS.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1);
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i) {
      Serial1.print(" ");
      logFile_GPS.print(" ");
    }
  }
  feedgps();
}

static void print_date(TinyGPS &gps)
{
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned long age;
  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  if (age == TinyGPS::GPS_INVALID_AGE) {
    Serial1.print("*******    *******    ");
    logFile_GPS.print("*******    *******    ");
  } else
  {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d %02d:%02d:%02d   ",
        month, day, year, hour, minute, second);
    Serial1.print(sz);
    logFile_GPS.print(sz);
  }
  print_int(age, TinyGPS::GPS_INVALID_AGE, 5);
  feedgps();
}

static void print_str(const char *str, int len)
{
  int slen = strlen(str);
  for (int i=0; i<len; ++i) {
    Serial1.print(i<slen ? str[i] : ' ');
    logFile_GPS.print(i<slen ? str[i] : ' ');
  }
  feedgps();
}

static bool feedgps()
{
  while (GPS_SERIAL.available())
  {
    if (gps.encode(GPS_SERIAL.read()))
      return true;
  }
  return false;
}
