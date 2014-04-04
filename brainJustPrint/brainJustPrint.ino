#include <SoftwareSerial.h>

SoftwareSerial brain(10,11);

int latestByte,lastByte;

void setup() {
    // Start the hardware serial.
    Serial2.begin(57600);
    Serial.begin(57600);
    Serial.println("init");
}

void loop() {
   
    while(Serial2.available()){
        latestByte = Serial2.read();
            if ((latestByte==170)&&(lastByte==170)){  //latestByte==170)&&(lastByte==170
                Serial.println("sync");
                while(Serial2.available()<2);
                Serial.println(Serial2.read());
                Serial.println(Serial2.read());
                latestByte = 0;
            }
        lastByte = latestByte;
    }
}
