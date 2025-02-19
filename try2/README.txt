now we improved a little bit
we tried to build the correct format of LIN frame :
Break field + Sync field + payload + checksum

we used like always the class SoftwareSerial from the library SoftwareSerial.h
we used the methods of the class SoftwareSerial (we created an object named
 LinSerial and we applyed the required methods like write and read to send
 and receive through UART interface )

we choose : Break:0x0   Sync:0x55  Payload:0x64   checksum:256 >>>> false

WARNING: the checksum is not accurate we didn't calculate it correctly

the logic of comparing the received=lijet 7a4ra min send=168 checksum &&&
 the calculated checksum= ReceivedData + ReceivedId 
