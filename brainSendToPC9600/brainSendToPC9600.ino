#include <Brain.h>
 
// Set up the brain parser, pass it the hardware serial object you want to listen on.
Brain brain(Serial2);
 
void setup() {
    // Start the hardware serial.
    Serial2.begin(9600);
    Serial.begin(9600);
}
 
void loop() {
    while (Serial.available()){
        char c = Serial.read();
        if (c == 'R') {
            Serial.println('S');
            delay(15);
            while (!brain.update());
            Serial.println(brain.readCSV());
            delay(10);
            Serial.println('E');
        }
    }
}

