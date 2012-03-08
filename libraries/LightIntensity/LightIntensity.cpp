// File: LightIntensity.cpp

#include "Arduino.h"
#include "LightIntensity.h"

#define PORT 26
//#define PORT port

#define NUM_SAMPLES 100

LightIntensity::LightIntensity(int p, int i_ms) {
    port = p;
    interval = i_ms;
}

void LightIntensity::setup() {
    Serial.begin(9600);
    pinMode(PORT, INPUT);

    TCCR1A = 0;     // Force 16-bit timer mode
    //TCCR1B = 1;   // no prescaling
    TCCR1B = 2;     // 8x prescaling
}

void LightIntensity::run(Scheduler * scheduler) {
    scheduler -> schedule(this, interval);

    // Find average of NUM_SAMPLES successive reads
    long totTime = 0;

    for(int i=0; i<NUM_SAMPLES; i++) {
        int startState = digitalReadFast(PORT);

        // wait for start of next pulse
        while(digitalReadFast(PORT) == startState) {
        }

        int startPulse = TCNT1;

        // wait for end of next pulse
        while(digitalReadFast(PORT) != startState) {
        }

        int endPulse = TCNT1;

        totTime += endPulse - startPulse;
    }
    int pulseLength = totTime / NUM_SAMPLES;

    Serial.print("TSL235>>");
    Serial.println(pulseLength);
}
