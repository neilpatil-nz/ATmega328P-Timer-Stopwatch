# ATmega328P-Timer-Stopwatch

A Timer/Stopwatch project that is based on a simple 1 second counter implemented to an ATmega328P. The limition of only having four 7-segment displays, means that the max time it can display is 99 minutes with 60 seconds, even though the counter can go up to 1092 minutes due to being a 16 bit unsigned variable.

How to use
------------
The main code is located in `main.c`, to access it:

`ATmega328P-Timer > ATmega328P-Timer > main.c`

The `main.c` is only required, as such it can be compiled in your favourite IDE (Atmel Studio/Arduino) and then uploaded to the microcontroller. 

Timer1 Setup
------------

Uses the ATmega328P 16 MHz clock with a prescale of 256 bits to count to 62499 (`OCR1A = 62499`), when reached it causes an interrupt to occur, which adds 1 to the `uint16_t count` variable.

7-Segment Display 
------------

Multiplexing through each 7-segment display.

Interrupt Setup
------------

Alarm Setup
------------

Since the volume of a piezoelectric buzzer is largerly dependent on the current and the ATmega328P can only supply a limited amount of current across the outputs, which is largely used by the LED 7-segment display. Hence, I used a NPN transistor to amplify the current by connecting the buzzer to the collector terminal of the transistor, so that more current could be driven through the transistor due to Beta.


