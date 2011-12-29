/* USLIPayload.ino - Main driver program 
 * Purpose: Constantly collect data and send via telemetry.
 * Created by the University of Washington USLI Team, 2010-2011
 * Released into the public domain - use at your own risk!
 */

#include <Accelerometer.h>
#include <Camera.h>
#include <GPS.h>
#include <LightIntensity.h>
#include <Pressure.h>
#include <TempHumidity.h>
#include <Xbee.h>

#include <digitalWriteFast.h>


unsigned long initTime;

// replace syntax
#define PORT 22

void setup() {
  Serial.begin(9600);
  Serial.println("Hello, world!");
  
  pinMode(PORT, INPUT);
  
//  TCCR0B = 2;   // 8x prescaling

  TCCR1A = 0;   // force 16 bit timer mode
//  TCCR1B = 1;   // no prescaling
  TCCR1B = 2;   // 8x prescaling
}

void loop() {  
//    int startState = (PORTA & 0x80);
    int startState = digitalReadFast(PORT);
    
    // wait for start of next pulse   
//    while((PORTA & 0x80) == startState) {
    while(digitalReadFast(PORT) == startState) {
    }
    
//   byte startPulse = TCNT0;
   int startPulse = TCNT1;
    
    // wait for end of next pulse
//    while((PORTA & 0x80) != startState) {
    while(digitalReadFast(PORT) != startState) {
    }
//    unsigned long endPulse = micros();
//    byte endPulse = TCNT0;
    int endPulse = TCNT1;  // DEBUG    
    
    if(endPulse > startPulse) {
      int pulseLength = endPulse - startPulse;
//      Serial.println((double) pulseLength / 8);
      Serial.println(pulseLength);
    }
      
//    delay(1000);

}
