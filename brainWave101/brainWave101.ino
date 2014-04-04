#include <Brain.h>
 
// Set up the brain parser, pass it the hardware serial object you want to listen on.
Brain brain(Serial2);
 
void setup() {
    // Start the hardware serial.
    Serial2.begin(57600);
    Serial.begin(57600);
}
 
void loop() {
    while (!brain.update());   //wait for new packet
    Serial.println(brain.readCSV());  //print packet
}

