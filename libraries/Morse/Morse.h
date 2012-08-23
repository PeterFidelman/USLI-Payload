/*
 * File: Morse.h
 * Morse Code library for Arduino, by Peter Fidelman
 * 2012 - Written for UW-USLI Team
 * Public Domain
 */

#ifndef MorsePSF_h
#define MorsePSF_h

#include <Arduino.h>

class Morse
{
    public:
        Morse(int pc_spkr, int dit_len); // ms
        void beep(int invFreq, int duration);
        void fakeMorse(unsigned param);
        void morse(String message);
    private:
        void dit();
        void dah();
        void eol();
        void eow();
        void fakeMorse_helper(unsigned param);
        int pc_spkr;  // PC Speaker Port
        int dit_len;  // Dit length (ms)
        
};

#endif
