#include "Arduino.h"
#include "Brain57600.h"

Brain::Brain(Stream &_brainStream) {
    brainStream = &_brainStream;
    
    // Keep the rest of the initialization process in a separate method in case
    // we overload the constructor.
    init();
}

void Brain::init() {
    // It's up to the calling code to start the stream
    // Usually Serial.begin(9600);
    freshPacket = false;
    inPacket = false;
    packetIndex = 0;
    packetLength = 0;
    checksum = 0;
	checksumAccumulator = 0;
    rawValue = 0;
}

boolean Brain::update() {
    if (brainStream->available()) {
        latestByte = brainStream->read();

        // Build a packet if we know we're and not just listening for sync bytes.
        if (inPacket) {
            // First byte after the sync bytes is the length of the upcoming packet.
            if (packetIndex == 0) {
                packetLength = latestByte;

                // Catch error if packet is too long
                if (packetLength > MAX_PACKET_LENGTH) {
                    // Packet exceeded max length
                    // Send an error
                    sprintf(latestError, "ERROR: Packet too long %i", packetLength);
                    inPacket = false;
                }
            }
            else if (packetIndex <= packetLength) {
                // Run of the mill data bytes.
                
                // Print them here

                // Store the byte in an array for parsing later.
                packetData[packetIndex - 1] = latestByte;

                // Keep building the checksum.
                checksumAccumulator += latestByte;
            }
            else if (packetIndex > packetLength) {
                // We're at the end of the data payload.
                
                // Check the checksum.
                checksum = latestByte;
                checksumAccumulator = 255 - checksumAccumulator;

                // Do they match?
                if (checksum == checksumAccumulator) {
                    boolean parseSuccess = parsePacket();
                    
                    if (parseSuccess) {
                        freshPacket = true;
                    }
                    else {
                        // Parsing failed, send an error.
                        sprintf(latestError, "ERROR: Could not parse");
                        // good place to print the packet if debugging
                    }
                }
                else {
                    // Checksum mismatch, send an error.
                    sprintf(latestError, "ERROR: Checksum");
                    // good place to print the packet if debugging
                }
                // End of packet
                
                // Reset, prep for next packet
                inPacket = false;
            }
            
            packetIndex++;
        }
        
        // Look for the start of the packet
        if ((latestByte == 170) && (lastByte == 170) && !inPacket) {
            // Start of packet
            inPacket = true;
            packetIndex = 0;
            checksumAccumulator = 0;
        }
        
        // Keep track of the last byte so we can find the sync byte pairs.
        lastByte = latestByte;
    }
    
    if (freshPacket) {
        freshPacket = false;
        return true;
    }
    else {
        return false;
    }
    
}

void Brain::clearPacket() {
    for (uint8_t i = 0; i < MAX_PACKET_LENGTH; i++) {
        packetData[i] = 0;
    }    
}

boolean Brain::parsePacket() {
    // Loop through the packet, extracting data.
    // Based on mindset_communications_protocol.pdf from the Neurosky Mindset SDK.
    // Returns true if passing succeeds
    boolean parseSuccess = true;
	
    for (uint8_t i = 0; i < packetLength; i++) {
        switch (packetData[i]) {
            case 0x80:
                // We dont' use this value so let's skip it and just increment i
                // uint8_t packetLength = packetData[++i];
                i++;
                rawValue = ((int)packetData[++i] << 8) | packetData[++i];
                break;
            default:
                parseSuccess = false;
                break;
        }
    }
    return parseSuccess;
}

char* Brain::readErrors() {
    return latestError;
}



// For debugging, print the entire contents of the packet data array.
char* Brain::printPacket() {
    sprintf(packetBuffer, "%d,%d,%d,%d", packetData[0], packetData[1], packetData[2], packetData[3]);
    return packetBuffer;
}

uint32_t Brain::readRaw(){
    return rawValue;
}
