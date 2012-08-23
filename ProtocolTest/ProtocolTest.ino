void softReset(void);  // prototype for later

void setup()
{
    Serial.begin(9600);
    Serial.println("UW-USLI Command Protocols Test");
    Serial.println("Command character is W");
}

void loop() 
{
    static int cmd_chars = 0;    // init only happens once

//---------------------------------------------------------------------------
    int buffered_chars = Serial.available();
    for(int i=0; i<buffered_chars; i++) {
        int cur_char = Serial.read();
        if (cur_char == 'W') {
            Serial.print("W... ");
            cmd_chars++;
        } else {
            // received a character other than upper case 'W'
            // if we noticed 3 command characters before this
            // enter command mode
            if (cmd_chars >= 3) {
              if (cur_char == 'A') {
                  Serial.print("(A)ctivating the bus... ");
                  // (A)ctivate the bus (TODO)
                  Serial.println("bus activated!!");
                  Serial.println("Rebooting in 100ms...");
                  Serial.flush();
                  delay(100);
                  cmd_chars = 0;
                  // Reset the microcontroller (s.t. all sensors initialized)
                  softReset(); // TODO only use this if we really have to
              } else if (cur_char == 'D') {
                  Serial.print("(D)eactivating the bus... ");
                  // (D)eactivate the bus (TODO)
                  Serial.println("bus deactivated!!");
                  //Serial.println("Rebooting in 100ms...");
                  //Serial.flush();
                  //delay(100);
                  cmd_chars = 0;
                  // Reset the microcontroller (s.t. all blocking sensors
                  //                            are offline)
                  //softReset(); // TODO only use this if we really have to
              } else {
                  // Print and log "UNKNOWN COMMAND"
                  cmd_chars = 0;
              }
            } else {
               // didn't notice 3 command characters before this
               // non-command character -- return to initial state
               cmd_chars = 0;
            }
        }
    }
//---------------------------------------------------------------------------
}

void softReset()
{
asm volatile ("  jmp 0");
}
