#include <SoftwareSerial.h>

#define TX_PIN 2  // Connect to UART to LIN module TX
#define RX_PIN 3  // Connect to UART to LIN module RX
#define SLP_PIN 4 // Sleep pin to wake up the LIN module

SoftwareSerial linSerial(RX_PIN, TX_PIN); // RX, TX

void setup() {
    pinMode(SLP_PIN, OUTPUT);
    digitalWrite(SLP_PIN, HIGH); // Wake up LIN module

    linSerial.begin(9600); // LIN bus typically uses 9600 baud
    Serial.begin(9600); // For debugging
    
    Serial.println("Master Ready");
}

void loop() {
    String frame = "hello"; // Data to send
    linSerial.println(frame);   // Send over LIN
    Serial.println("Sent: " + frame); // Debugging output

    delay(500); // Send every 2 seconds
}
