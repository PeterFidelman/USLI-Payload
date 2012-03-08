// File: TempHumidity.h

#include <DHT22.h>
#include <Scheduler.h>

class TempHumidity : public ITask
{
    public: void setup();
            void run(Scheduler *);
            TempHumidity(int, int);

    private: int port;
             int interval;
             DHT22* myDHT22;

};
