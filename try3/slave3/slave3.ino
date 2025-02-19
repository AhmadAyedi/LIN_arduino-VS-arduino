#include <SoftwareSerial.h>


#define TX_PIN 2  // Connect to UART to LIN module TX
#define RX_PIN 3  // Connect to UART to LIN module RX
#define SLP_PIN 4 // Sleep pin to wake up the LIN module


SoftwareSerial linSerial(RX_PIN, TX_PIN); // RX, TX


void setup() {
  pinMode(SLP_PIN, OUTPUT);
  digitalWrite(SLP_PIN, HIGH); // Wake up LIN module


  linSerial.begin(9600); // LIN bus communication
  Serial.begin(9600);    // Debugging


  Serial.println("Slave Ready");
}


void loop() {
  if (linSerial.available()) {
    String receivedData = ""; // Buffer to store the entire frame


    // Read all available bytes
    while (linSerial.available()) {
      byte dataByte = linSerial.read(); // Read one byte
      receivedData += String(dataByte, HEX); // Convert to HEX and append
      receivedData += " "; // Add a space between bytes
      delay(10); // Small delay to ensure all bytes are read correctly
    }


    // Print the entire frame in one line
    Serial.print("Received: ");
    Serial.println(receivedData);
  }
}
 




