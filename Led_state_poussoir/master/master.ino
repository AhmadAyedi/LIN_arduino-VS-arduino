#include <SoftwareSerial.h>

#define TX_PIN 2  // TX to LIN module
#define RX_PIN 3  // RX from LIN module
#define SLP_PIN 4 // Sleep pin
#define BUTTON_PIN 5  // Button connected to pin 5

SoftwareSerial linSerial(RX_PIN, TX_PIN); // RX, TX

// Function to calculate parity bits for LIN ID
uint8_t calculateParity(uint8_t id) {
    uint8_t p0 = (id & 0x01) ^ ((id & 0x02) >> 1) ^ ((id & 0x04) >> 2) ^ ((id & 0x10) >> 4);
    uint8_t p1 = ~(((id & 0x02) >> 1) ^ ((id & 0x08) >> 3) ^ ((id & 0x10) >> 4) ^ ((id & 0x20) >> 5));
    p1 = (p1 & 0x01) << 1;
    return id | p0 | p1;
}

// Function to calculate checksum
uint8_t calculateChecksum(uint8_t id, uint8_t* data, uint8_t length) {
    uint16_t sum = id; // Start with the ID
    for (int i = 0; i < length; i++) {
        sum += data[i];
    }
    sum = (sum & 0xFF) + (sum >> 8);
    return ~sum;
}

void sendLINFrame(uint8_t id, uint8_t* data, uint8_t length) {
    id = calculateParity(id); // Calculate parity and update the id

    linSerial.write((uint8_t)0x00); // Break field
    linSerial.write((uint8_t)0x00);
    linSerial.write((uint8_t)0x00);

    linSerial.write(0x55); // Sync byte

    linSerial.write(id);   // Identifier with parity
    for (int i = 0; i < length; i++) {
        linSerial.write(data[i]); // Data
    }

    uint8_t checksum = calculateChecksum(id, data, length);
    linSerial.write(checksum); // Checksum
}

void setup() {
    pinMode(SLP_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP); // Button pin set to input pull-up
    digitalWrite(SLP_PIN, HIGH); // Wake up LIN module

    linSerial.begin(9600);  // LIN baud rate
    Serial.begin(9600);     // Debugging

    Serial.println("Master Ready");
}

void loop() {
    uint8_t id = 0x12;  // Example LIN ID
    uint8_t message[1];  // Data to send (ON or OFF)
    uint8_t length = sizeof(message);

    // Read button state
    if (digitalRead(BUTTON_PIN) == LOW) {  // Button pressed
        message[0] = 0x01;  // ON command
        Serial.println("Sending ON Command");
    } else {
        message[0] = 0x00;  // OFF command
        Serial.println("Sending OFF Command");
    }

    sendLINFrame(id, message, length);  // Send the command over LIN

    delay(500); // Adjust the delay as needed to avoid too many frame sends
}
