// File: LightIntensity.cpp

#include "Arduino.h"
#include "LightIntensity.h"
#include<SD.h>

#define PORT 26
//#define PORT port

//#define NUM_SAMPLES 100
#define NUM_SAMPLES 16

LightIntensity::LightIntensity(int p, int i_ms, boolean m) {
    port = p;
    interval = i_ms;
    useMorse = m;
}

// TODO restructure this to not be a crime against humanity ---------
#define PC_SPKR 30
#define DIT_LEN 100
void beep(int invFreq, int duration)
{
  unsigned long start = millis();
  do {
    digitalWrite(PC_SPKR, HIGH);
    delayMicroseconds(invFreq);
    digitalWrite(PC_SPKR, LOW);
    delayMicroseconds(invFreq);
  } while(millis() - start < duration);
}

void dit() {
  beep(500,DIT_LEN);
  delay(DIT_LEN);  
}

void dah() {
  beep(500,DIT_LEN*3);
  delay(DIT_LEN);
}

void eol() {
  delay(DIT_LEN * (3-1));
}

void eow() {
  delay(DIT_LEN * (7-1));
}

boolean fakeMorse_helper(unsigned); //prototype

void fakeMorse(unsigned param)
{
  fakeMorse_helper(param);
  eow();
}

boolean fakeMorse_helper(unsigned param)
{
  if(param >= 10)
    fakeMorse(param / 10);
  
  for(int i=0; i<param%10; i++)
    dit();
  
  if(param%10 == 0)
    dah();
}

// TODO crime against humanity ends here -----------------------------

File logFile_LI;

void LightIntensity::setup() {
    Serial1.begin(9600);

    pinMode(PORT, INPUT);

    TCCR1A = 0;     // Force 16-bit timer mode
    //TCCR1B = 1;   // no prescaling
    TCCR1B = 2;     // 8x prescaling

    pinMode(PC_SPKR, OUTPUT);
}

void LightIntensity::run(Scheduler * scheduler) {
    scheduler -> schedule(this, interval);

    // SD - open file anew
    logFile_LI = SD.open("LIGHTINT.LOG", FILE_WRITE);

    // Find average of NUM_SAMPLES successive reads
    long totTime = 0;

    for(int i=0; i<NUM_SAMPLES; i++) {
    /*
        int startState = digitalReadFast(PORT);
        
        // wait for start of next pulse
        while(digitalReadFast(PORT) == startState) {
            if(TCNT1 - startState > 65000)
                return;
        }

        int startPulse = TCNT1;

        // wait for state to change twice (e.g. LOW -> HIGH -> LOW)
        // this yields the period of the pulse wave
        while(digitalReadFast(PORT) != startState) {
            if(TCNT1 - startState > 65000)
                return;
        }
        while(digitalReadFast(PORT) == startState) {
            if(TCNT1 - startState > 65000)
                return;
        }

        int endPulse = TCNT1;
        totTime += endPulse - startPulse;
    */
        int p1 = pulseIn(PORT, HIGH, 15000);
        int p2 = pulseIn(PORT, LOW, 15000);
        if(p1 == 0 || p2 == 0) return;
        totTime += (p1 + p2);
    }
    int pulseLength = totTime / NUM_SAMPLES;

    if(useMorse) {
        beep(50000, 50);
        fakeMorse((unsigned) pulseLength);
        beep(50000, 50);
    } else {
        Serial1.print("TSL235>>");
        logFile_LI.print("TSL235>>");   // SD
        Serial1.print(" ");
        logFile_LI.print(" ");   // SD
        Serial1.print(millis());
        logFile_LI.print(millis());   // SD
        Serial1.print(" ");
        logFile_LI.print(" ");   // SD
        Serial1.println(pulseLength);
        logFile_LI.println(pulseLength);   // SD
    }
    logFile_LI.close();  // SD - close file to ensure data is written
}

