/* USLIPayload.ino - Main driver program 
 * Purpose: Constantly collect data and send via telemetry.
 * Created by the University of Washington USLI Team, 2010-2011
 * Released into the public domain - use at your own risk!
 */

//#include <Accelerometer.h>
//#include <Camera.h>
//#include <GPS.h>
//#include <LightIntensity.h>
//#include <Pressure.h>
//#include <TempHumidity.h>
//#include <Xbee.h>

#include <digitalWriteFast.h>
#include <DHT22.h>

unsigned long initTime;

// replace syntax
#define TSL235R_PORT 21
#define DHT22_PORT 22

DHT22 myDHT22(DHT22_PORT);

void setup() {
  Serial.begin(9600);
  Serial.println("Hello, world!"); 
}

void loop() {  
  DHT22_ERROR_t errorCode;

  delay(2000);
  Serial.print("Requesting data...");
  errorCode = myDHT22.readData();
  switch(errorCode)
  {
    case DHT_ERROR_NONE:
      Serial.print("Got Data ");
      Serial.print(myDHT22.getTemperatureC());
      Serial.print("C ");
      Serial.print(myDHT22.getHumidity());
      Serial.println("%");
      break;
    case DHT_ERROR_CHECKSUM:
      Serial.print("check sum error ");
      Serial.print(myDHT22.getTemperatureC());
      Serial.print("C ");
      Serial.print(myDHT22.getHumidity());
      Serial.println("%");
      break;
    case DHT_BUS_HUNG:
      Serial.println("BUS Hung ");
      break;
    case DHT_ERROR_NOT_PRESENT:
      Serial.println("Not Present ");
      break;
    case DHT_ERROR_ACK_TOO_LONG:
      Serial.println("ACK time out ");
      break;
    case DHT_ERROR_SYNC_TIMEOUT:
      Serial.println("Sync Timeout ");
      break;
    case DHT_ERROR_DATA_TIMEOUT:
      Serial.println("Data Timeout ");
      break;
    case DHT_ERROR_TOOQUICK:
      Serial.println("Polled to quick ");
      break;
  }
}
