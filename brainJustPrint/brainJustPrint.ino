#include "Brain57600.h"

Brain brain(Serial2);
void setup() {
    // Start the hardware serial.
    Serial2.begin(57600);
    Serial.begin(57600);
    Serial.println("init");
}

void loop() {
    while(!brain.update()) ;
    Serial.println(brain.readRaw());
}
