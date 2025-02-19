it works fine 

a beginner code for LIN communication

just send a "hello" message using softwareSerial library 

we create virtual RX TX ports that we used to communicate with LIN module through
UART then the module convert from UART to LIN then from LIN to UART then be received 
by the other arduino

we created an object named LinSerial(RX,TX) it's a constructor 
we create that instance from the class named SoftwareSerial which we import it 
from the library named SoftwareSerial.h

then that class named SoftwareSerial that we created an object from .. has many 
methods that we use to make some functionality like sending with UART 
so we use that methods of our class and apply them to our instance named LinSerial
