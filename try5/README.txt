void loop() {
    uint8_t id = 0x12;  // Example LIN ID (must include valid parity bits)
    uint8_t data = 0x45; // Example data

    Serial.println("Sending LIN Frame...");
    sendLINFrame(id, data);

    delay(1000); // Send every second
}

format te3 frame yib3a4ha direct mrigla simple 