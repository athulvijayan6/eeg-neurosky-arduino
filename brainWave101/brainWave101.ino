#include <SoftwareSerial.h>

SoftwareSerial ser(10,11);
#include <Brain.h>


 
// Set up the brain parser, pass it the hardware serial object you want to listen on.
Brain brain(Serial2);
 
void setup() {
    // Start the hardware serial.
    Serial2.begin(57600);
    Serial.begin(57600);
}
 
void loop() {
    // Expect packets about once per second.
    // The .readCSV() function returns a string (well, char*) listing the most recent brain data, in the following format:
    // "signal strength, attention, meditation, delta, theta, low alpha, high alpha, low beta, high beta, low gamma, high gamma"   
    //while (Serial.available()){
        //char c = Serial.read();
        //if (c == 'R') {
                //Serial.println('S');
                while (!brain.update());//brain.readCSV()
                Serial.println(brain.readDelta());
                //Serial.println('E');
        //}
    //}
}

