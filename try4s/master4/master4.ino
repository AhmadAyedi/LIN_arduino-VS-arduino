#include <SoftwareSerial.h>


#define TX_PIN 2  // Connect to UART to LIN module TX
#define RX_PIN 3  // Connect to UART to LIN module RX
#define SLP_PIN 4 // Sleep pin to wake up the LIN module


SoftwareSerial linSerial(RX_PIN, TX_PIN); // RX, TX


// Function to calculate checksum (simplified for the example)
byte calculateChecksum(byte* data, byte length) {
  byte checksum = 0;
  for (byte i = 0; i < length; i++) {
    checksum += data[i];
  }
  return (checksum ^ 0xFF) + 1; // LIN checksum formula (negate and add 1)
}


void setup() {
  pinMode(SLP_PIN, OUTPUT);
  digitalWrite(SLP_PIN, HIGH); // Wake up LIN module


  linSerial.begin(9600); // LIN bus communication
  Serial.begin(9600);    // Debugging


  Serial.println("Master Ready");
}


void loop() {
  // Sending a LIN frame
  byte id = 0x01; // Example ID (1)
  byte data[] = {0x11, 0x22, 0x33, 0x99, 0x55, 0x66, 0x77, 0x88}; // Example data
  byte dataLength = sizeof(data) / sizeof(data[0]);
  Serial.println(dataLength);
  // Debugging print to show checksum being calculated
  byte checksum = calculateChecksum(data, dataLength);
  Serial.print("Calculated checksum: ");
  Serial.println(checksum, HEX);




  // Construct the frame: Sync byte + ID byte + Data bytes + Checksum byte
  byte frame[11]; // 1 byte for sync, 1 byte for ID, 8 bytes for data, 1 byte for checksum
  frame[0] = 0x55;  // Sync byte
  frame[1] = id;    // ID byte
  for (byte i = 0; i < dataLength; i++) {
    frame[2 + i] = data[i]; // Add data
  }
  frame[2 + dataLength] = checksum; // Add checksum


  // Send the frame
  for (byte i = 0; i < sizeof(frame); i++) {
    linSerial.write(frame[i]);
  }


  // Debugging: Print sent frame
  Serial.print("Sent frame: ");
  for (byte i = 0; i < sizeof(frame); i++) {
    Serial.print(frame[i], HEX);
    Serial.print(" ");
  }
  Serial.println();


  delay(2000); // Send frame every 2 seconds
}















