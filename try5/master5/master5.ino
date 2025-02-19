#include <SoftwareSerial.h>

#define TX_PIN 2  // TX to LIN module
#define RX_PIN 3  // RX from LIN module
#define SLP_PIN 4 // Sleep pin

SoftwareSerial linSerial(RX_PIN, TX_PIN); // RX, TX

void sendLINFrame(uint8_t id, uint8_t data) {
    linSerial.write((uint8_t)0x00); // Break field (explicit uint8_t)
    linSerial.write((uint8_t)0x55); // Sync byte
    linSerial.write(id);   // Identifier (with parity bits)
    linSerial.write(data); // Data byte
    uint8_t checksum = ~(id + data); // Classic LIN checksum
    linSerial.write(checksum); // Checksum byte
}

void setup() {
    pinMode(SLP_PIN, OUTPUT);
    digitalWrite(SLP_PIN, HIGH); // Wake up LIN module

    linSerial.begin(9600); // Typical LIN baud rate
    Serial.begin(9600);    // Debugging

    Serial.println("Master Ready");
}

void loop() {
    uint8_t id = 0x12;  // Example LIN ID (must include valid parity bits)
    uint8_t data = 0x45; // Example data

    Serial.println("Sending LIN Frame...");
    sendLINFrame(id, data);

    delay(1000); // Send every second
}
