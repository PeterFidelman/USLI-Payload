// File: LightIntensity.h

#ifndef LightIntensity_h
#define LightIntensity_h

#include <digitalWriteFast.h>
#include <Scheduler.h>
#include <SD.h>

class LightIntensity : public ITask
{
    public:
        void setup();
        void run(Scheduler *);
        LightIntensity(int, int, boolean);
    private:
        int port;
        int interval;
        boolean useMorse;
};

#endif
