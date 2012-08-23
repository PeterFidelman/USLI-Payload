/*

// User sends 1 or 0 and arduino returns status of LED on pin 13, all other values returned as "invalid"

int ledPin = 13;
void setup ()
{
  //Create Serial Object
  Serial1.begin(9600); 
  pinMode(ledPin, OUTPUT);
}
void loop()
{
  //Arduino waits to recieve input
  while (Serial1.available() == 0);
  //Read Input
  int val = Serial1.read() - '0';
  if (val == 1) 
  {
    Serial1.println("Led is on");
    digitalWrite(ledPin, HIGH);
  }
  else if (val == 0)
  {
    Serial1.println("Led is off");
    digitalWrite(ledPin, LOW);
  }
  else
  {
    Serial1.println("Invalid");
  }
  while (Serial1.available()>0);
  {
    Serial1.read();
  }
}

*/

void setup()  {
  Serial.begin(9600);   // USB-Serial
  Serial1.begin(9600);  // Xbee
}

void loop() {
  
  if (Serial1.available() > 0) {
    int inByte = Serial1.read();
    Serial.write(inByte);
  }
  
  Serial.println("testing... (sending N)");
  Serial1.write(78); // "N"
  delay(1000);

}
