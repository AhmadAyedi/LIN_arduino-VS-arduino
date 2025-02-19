#include <SoftwareSerial.h>

#define TX_PIN 2  // TX to LIN module
#define RX_PIN 3  // RX from LIN module
#define SLP_PIN 4 // Sleep pin

SoftwareSerial linSerial(RX_PIN, TX_PIN); // RX, TX

// Function to calculate checksum
// uint8_t calculateChecksum(uint8_t id, uint8_t* data, uint8_t length) {
//     uint16_t sum = id;
//     for (int i = 0; i < length; i++) {
//         sum += data[i];
//         if (sum > 0xFF) sum -= 0xFF; // Carry handling
//     }
//     return ~sum;
// }
uint8_t calculateChecksum(uint8_t id, uint8_t* data, uint8_t length) {
    uint16_t sum = id; // Start with the ID

    // Add each data byte
    for (int i = 0; i < length; i++) {
        sum += data[i];
    }

    // Take the lower 8 bits and add the carry using bitwise operations
    sum = (sum & 0xFF) + (sum >> 8); // This simulates the "carry" by adding the upper byte to the lower byte.

    // Perform the bitwise NOT (invert all bits)
    return ~sum;
}


void setup() {
    pinMode(SLP_PIN, OUTPUT);
    digitalWrite(SLP_PIN, HIGH); // Wake up LIN module

    linSerial.begin(9600);
    Serial.begin(9600);

    Serial.println("Slave Ready");
}

void loop() {
    if (linSerial.available() > 8) { // LIN frame size for "hello" message
        // Read Break field (ignore)
        linSerial.read(); linSerial.read(); linSerial.read(); 

        // Read remaining LIN frame
        uint8_t sync = linSerial.read();
        uint8_t id = linSerial.read();
        uint8_t data[5];
        
        for (int i = 0; i < 5; i++) {
            data[i] = linSerial.read();
            Serial.print("Data Received: ");
            Serial.println(data[i], HEX);
        }

        uint8_t receivedChecksum = linSerial.read();
        Serial.print("Received Checksum: ");
        Serial.println(receivedChecksum, HEX);

        // Calculate checksum on receiver side
        uint8_t calculatedChecksum = calculateChecksum(id, data, 5);

        Serial.print("Calculated Checksum: ");
        Serial.println(calculatedChecksum, HEX);

        // Compare checksums
        if (sync == 0x55 && receivedChecksum == calculatedChecksum) {
            Serial.print("LIN Frame Successfully Received: ");
            for (int i = 0; i < 5; i++) {
                Serial.print((char)data[i]);
            }
            Serial.println();
        } else {
            Serial.println("Invalid LIN Frame");
        }
        
        Serial.println("------------------------------");
    }
}
