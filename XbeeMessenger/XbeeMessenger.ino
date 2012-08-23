/*
  Xbee serial test
 
 This example code is in the public domain.
 */

void setup() {
  Serial.begin(9600);  // USB-Serial Console
  Serial1.begin(9600); // Xbee
}

void loop() {
  if (Serial1.available() > 0) {
    int inByte = Serial1.read();
    Serial.write(inByte);
    Serial1.write(inByte); //echo
  }
  
  if (Serial.available() > 0) {
    int inByte = Serial.read();
    Serial.write(inByte);  // echo
    Serial1.write(inByte);
  }
}
