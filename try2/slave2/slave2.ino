#include <SoftwareSerial.h>

#define TX_PIN 2  // TX to LIN module
#define RX_PIN 3  // RX from LIN module
#define SLP_PIN 4 // Sleep pin

SoftwareSerial linSerial(RX_PIN, TX_PIN); // RX, TX

void setup() {
    pinMode(SLP_PIN, OUTPUT);
    digitalWrite(SLP_PIN, HIGH); // Wake up LIN module

    linSerial.begin(9600);
    Serial.begin(9600);

    Serial.println("Slave Ready");
}

void loop() {
    if (linSerial.available() > 4) { // LIN frame size: 5 bytes minimum
        uint8_t breakField = linSerial.read(); // Read break (ignored)
        uint8_t sync = linSerial.read(); // Sync byte
        uint8_t id = linSerial.read();   // Identifier
        uint8_t data = linSerial.read(); // Data byte
        uint8_t receivedChecksum = linSerial.read(); // Checksum

        uint8_t calculatedChecksum = ~(id + data); // Compute checksum

        if (sync == 0x55 && receivedChecksum == calculatedChecksum) {
            Serial.print("Valid LIN Frame: ID :  ");            
            Serial.print(id);
            Serial.println("");
            Serial.print("  Data: ");
            Serial.print(data);
            Serial.println("");
            Serial.print(" receivedChecksum: ");
            Serial.print(receivedChecksum);
            Serial.println("");

        } else {
            Serial.println("Invalid LIN Frame");
        }
    }
}
