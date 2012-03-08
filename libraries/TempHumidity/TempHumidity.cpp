// File: TempHumidity.cpp

#include<TempHumidity.h>
#include<Arduino.h>

TempHumidity::TempHumidity(int p, int i_ms) {
    port = p;
    interval = i_ms;
}

void TempHumidity::setup() {
    Serial.begin(9600);
    myDHT22 = new DHT22(port);
}

void TempHumidity::run(Scheduler * scheduler) {
    DHT22_ERROR_t errorCode;
    // Schedule task to recur after "interval" ms
    scheduler -> schedule(this, interval);

    Serial.print("DHT22...");
    errorCode = myDHT22->readData();
    switch(errorCode)
    {
        case DHT_ERROR_NONE:
            Serial.print("Got Data>>");
            Serial.print(myDHT22->getTemperatureC());
            Serial.print("C ");
            Serial.print(myDHT22->getHumidity());
            Serial.println("%");
        break;
        case DHT_ERROR_CHECKSUM:
            Serial.print("check sum error ");
            Serial.print(myDHT22->getTemperatureC());
            Serial.print("C ");
            Serial.print(myDHT22->getHumidity());
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
            Serial.println("Polled too quick ");
            break;
    }
}
