#include <SoftwareSerial.h>

#define TX_PIN 2  // TX to LIN module
#define RX_PIN 3  // RX from LIN module
#define SLP_PIN 4 // Sleep pin
#define LED_PIN 13 // LED connected to pin 13

SoftwareSerial linSerial(RX_PIN, TX_PIN); // RX, TX

// Function to calculate checksum
uint8_t calculateChecksum(uint8_t id, uint8_t* data, uint8_t length) {
    uint16_t sum = id; // Start with the ID
    for (int i = 0; i < length; i++) {
        sum += data[i];
    }
    sum = (sum & 0xFF) + (sum >> 8);
    return ~sum;
}

void setup() {
    pinMode(SLP_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT); // Set LED pin as output
    digitalWrite(SLP_PIN, HIGH); // Wake up LIN module

    linSerial.begin(9600);  // LIN baud rate
    Serial.begin(9600);     // Debugging

    Serial.println("Slave Ready");
}

void loop() {
    if (linSerial.available() > 8) {  // Check if enough data is available
        // Read Break field (ignore)
        linSerial.read(); linSerial.read(); linSerial.read();

        uint8_t sync = linSerial.read();
        uint8_t id = linSerial.read();
        uint8_t data[1];  // Assuming one byte for ON/OFF
        data[0] = linSerial.read();

        uint8_t receivedChecksum = linSerial.read();

        // Calculate checksum
        uint8_t calculatedChecksum = calculateChecksum(id, data, 1);

        if (sync == 0x55 && receivedChecksum == calculatedChecksum) {
            // Check command and control LED
            if (data[0] == 0x01) {
                digitalWrite(LED_PIN, HIGH);  // Turn ON LED
                Serial.println("LED ON");
            } else if (data[0] == 0x00) {
                digitalWrite(LED_PIN, LOW);  // Turn OFF LED
                Serial.println("LED OFF");
            }
        } else {
            Serial.println("Invalid LIN Frame");
        }
    }
}
