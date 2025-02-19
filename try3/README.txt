we improved our checksum now &&& we used a bigger payload 
it's a table with length=8 
this is the function we used to calculate our checksum:


// Function to calculate checksum (simplified for the example)
byte calculateChecksum(byte* data, byte length) {
  byte checksum = 0;
  for (byte i = 0; i < length; i++) {
    checksum += data[i];
  }
  return (checksum ^ 0xFF) + 1; // LIN checksum formula (negate and add 1)
}

WARNING: we didn't compare the received checksum with 
calculated checksum on the receiver side=using that method 