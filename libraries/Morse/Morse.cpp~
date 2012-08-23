/*
 * File: Morse.cpp
 * Morse Code library for Arduino, by Peter Fidelman
 * 2012 - Written for UW-USLI Team
 * Public Domain
 */

#include "Morse.h"

Morse::Morse(int port, int speed)
{
    pc_spkr = port;
    dit_len = speed;
}

void Morse::beep(int invFreq, int duration)
{
  unsigned long start = millis();
  do {
    digitalWrite(pc_spkr, HIGH);
    delayMicroseconds(invFreq);
    digitalWrite(pc_spkr, LOW);
    delayMicroseconds(invFreq);
  } while(millis() - start < duration);
}

void Morse::dit() {
  beep(500,dit_len);
  delay(dit_len);  
}

void Morse::dah() {
  beep(500,dit_len*3);
  delay(dit_len);
}

/*
 * end-of-letter pause
 */
void Morse::eol() {
  delay(dit_len * (3-1));
}

/*
 * end-of-word pause
 */
void Morse::eow() {
  delay(dit_len * (7-1));
}

/*
 * For pseudo-Morse number output.
 * dah=0, dit=1, dit*2=2, dit*3=3, ...
 *
 * 4207 = dit dit dit dit ... dit dit ... dah ... dit dit dit dit dit dit
 */
void Morse::fakeMorse(unsigned param)
{
  fakeMorse_helper(param);
  eow();
}

void Morse::fakeMorse_helper(unsigned param)
{
  if(param >= 10)
    fakeMorse(param / 10);
  
  for(int i=0; i<param%10; i++)
    dit();
  
  if(param%10 == 0)
    dah();
}

void Morse::morse(String message)
{
  for(int i=0; i<message.length(); i++) {
    int letter = message.charAt(i);
    letter = letter | 32; // convert to lowercase
    switch(letter) {
      case 'a': dit(); dah(); break;
      case 'b': dah(); dit(); dit(); dit(); break;
      case 'c': dah(); dit(); dah(); dit(); eol(); break;
      case 'd': dah(); dit(); dit(); break;
      case 'e': dit(); break;
      case 'f': dit(); dit(); dah(); dit(); break;
      case 'g': dah(); dah(); dit(); break;
      case 'h': dit(); dit(); dit(); dit(); break;
      case 'i': dit(); dit(); break;
      case 'j': dit(); dah(); dah(); dah(); break;
      case 'k': dah(); dit(); dah(); break;
      case 'l': dit(); dah(); dit(); dit(); break;
      case 'm': dah(); dah(); break;
      case 'n': dah(); dit(); break;
      case 'o': dah(); dah(); dah(); break;
      case 'p': dit(); dah(); dah(); dit(); break;
      case 'q': dah(); dah(); dit(); dah(); break;
      case 'r': dit(); dah(); dit(); break;
      case 's': dit(); dit(); dit(); break;
      case 't': dah(); break;
      case 'u': dit(); dit(); dah(); break;
      case 'v': dit(); dit(); dit(); dah(); break;
      case 'w': dit(); dah(); dah(); break;
      case 'x': dah(); dit(); dit(); dah(); break;
      case 'y': dah(); dit(); dah(); dah(); break;
      case 'z': dah(); dah(); dit(); dit(); break;
      
      case '1': dit(); dah(); dah(); dah(); dah(); break;
      case '2': dit(); dit(); dah(); dah(); dah(); break;
      case '3': dit(); dit(); dit(); dah(); dah(); break;
      case '4': dit(); dit(); dit(); dit(); dah(); break;
      case '5': dit(); dit(); dit(); dit(); dit(); break;
      case '6': dah(); dit(); dit(); dit(); dit(); break;
      case '7': dah(); dah(); dit(); dit(); dit(); break;
      case '8': dah(); dah(); dah(); dit(); dit(); break;
      case '9': dah(); dah(); dah(); dah(); dit(); break;
      case '0': dah(); dah(); dah(); dah(); dah(); break;
    }
    eol();
  }
}
