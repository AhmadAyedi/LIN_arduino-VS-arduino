#include <SoftwareSerial.h>


#define TX_PIN 2  // Connect to UART to LIN module TX
#define RX_PIN 3  // Connect to UART to LIN module RX
#define SLP_PIN 4 // Sleep pin to wake up the LIN module


SoftwareSerial linSerial(RX_PIN, TX_PIN); // RX, TX


// Function to calculate checksum based on LIN protocol
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


  Serial.println("Slave Ready");
}


void loop() {
  byte syncByte = 0;  // Variable to store the sync byte
  byte idByte = 0;    // Variable to store the ID byte
  byte data[8];       // Array to store the 8 data bytes
  
  // Wait for sync byte (0x55)
  while (true) {
    if (linSerial.available()) {
      syncByte = linSerial.read();  // Read the sync byte
      if (syncByte == 0x55) {
        Serial.print("Sync byte received: ");
        Serial.println(syncByte, HEX);
        break; // Exit the loop once sync byte is received
      }
    }
  }


  // Wait for and capture the ID byte
  while (!linSerial.available()); // Wait for ID byte
  idByte = linSerial.read();  // Read the ID byte
  Serial.print("ID byte received: ");
  Serial.println(idByte, HEX);


  // Read the data bytes (8 bytes expected)
  for (byte i = 0; i < 8; i++) {
    while (!linSerial.available()); // Wait for data byte to be available
    data[i] = linSerial.read();  // Read each data byte
  }


  // Print the data bytes
  Serial.print("Data: ");
  for (byte i = 0; i < 8; i++) {
    Serial.print(data[i], HEX);
    if (i < 7) {
      Serial.print(" "); // Add a space between data bytes
    }
  }
  Serial.println(); // End the line after printing all data


  // Create an array with the syncByte, idByte, and data bytes to calculate checksum
  byte checksumData[8]; // 1 sync byte, 1 id byte, 8 data bytes
  //checksumData[0] = syncByte;
  //checksumData[1] = idByte;
  for (byte i = 0; i < 8; i++) {
    checksumData[i] = data[i];
  }


  // Calculate checksum
  byte checksum = calculateChecksum(checksumData, 8);
  Serial.print("Calculated checksum: ");
  Serial.println(checksum, HEX);


  // Optional: Add a small delay to prevent flooding the serial monitor
  delay(1000); // Adjust the delay time as necessary
}


