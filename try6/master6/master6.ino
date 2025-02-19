#include <SoftwareSerial.h>

#define TX_PIN 2  // TX to LIN module
#define RX_PIN 3  // RX from LIN module
#define SLP_PIN 4 // Sleep pin

SoftwareSerial linSerial(RX_PIN, TX_PIN); // RX, TX

// Function to calculate parity bits for LIN ID
uint8_t calculateParity(uint8_t id) {
    uint8_t p0 = (id & 0x01) ^ ((id & 0x02) >> 1) ^ ((id & 0x04) >> 2) ^ ((id & 0x10) >> 4);
    uint8_t p1 = ~(((id & 0x02) >> 1) ^ ((id & 0x08) >> 3) ^ ((id & 0x10) >> 4) ^ ((id & 0x20) >> 5));
    p1 = (p1 & 0x01) << 1;
    return id | p0 | p1;
}

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


void sendLINFrame(uint8_t id, uint8_t* data, uint8_t length) {
    id = calculateParity(id);

    // Simulate Break field by sending a series of zeros
    linSerial.write((uint8_t)0x00);

    linSerial.write((uint8_t)0x00);

    linSerial.write((uint8_t)0x00);

    Serial.println("Break field: 0x00");

    linSerial.write(0x55); // Sync byte
    Serial.println("Sync field: 0x55");

    linSerial.write(id);   // Identifier with parity bits
    Serial.print("ID (with parity): ");
    Serial.println(id, HEX);

    // Send Data bytes
    for (int i = 0; i < length; i++) {
        linSerial.write(data[i]);
        Serial.print("Data: ");
        Serial.println(data[i], HEX);
    }

    // Calculate and send checksum
    uint8_t checksum = calculateChecksum(id, data, length);
    linSerial.write(checksum);
    Serial.print("Checksum: ");
    Serial.println(checksum, HEX);

    Serial.println("------------------------------");
}

void setup() {
    pinMode(SLP_PIN, OUTPUT);
    digitalWrite(SLP_PIN, HIGH); // Wake up LIN module

    linSerial.begin(9600); // LIN baud rate
    Serial.begin(9600);    // Debugging

    Serial.println("Master Ready");
}

void loop() {
    uint8_t id = 0x17;  // Example LIN ID
    uint8_t message[] = {'h', 'e', 'l', 'l', 'o'}; // "hello"
    uint8_t length = sizeof(message);

    Serial.println("Sending LIN Frame...");
    sendLINFrame(id, message, length);

    delay(2000); // Send every 2 seconds
}
