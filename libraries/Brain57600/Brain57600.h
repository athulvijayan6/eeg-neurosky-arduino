#ifndef Brain_h
#define Brain_h

#include "Arduino.h"

#define MAX_PACKET_LENGTH 4

class Brain {
    public:
        Brain(Stream &_brainStream);    

        // Run this in the main loop.
        boolean update();

        // String with most recent error.
        char* readErrors();

        // Returns comme-delimited string of all available brain data.
        // Sequence is as below.
        uint32_t readRaw();
        char* printPacket(); 
        
    private:
        Stream* brainStream;        
        uint8_t packetData[MAX_PACKET_LENGTH];
        boolean inPacket;
        uint8_t latestByte;
        uint8_t lastByte;
        uint8_t packetIndex;
        uint8_t packetLength;
        uint8_t checksum;
        uint8_t checksumAccumulator;
        void clearPacket();
        boolean parsePacket();
        void init();

        // With current hardware, at most we would have...
        // 3 x 3 char uint8_t
        // 8 x 10 char uint32_t
        // 10 x 1 char commas
        // 1 x 1 char 0 (string termination)
        // -------------------------
        // 100 characters       
        char packetBuffer[12];
        
        // Longest error is
        // 22 x 1 char uint8_ts
        // 1 x 1 char 0 (string termination)
        char latestError[23];       
        
        uint16_t rawValue;
        boolean freshPacket;
        
};

#endif
