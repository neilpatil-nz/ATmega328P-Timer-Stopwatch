# ATmega328P-Timer-Stopwatch

<p align="center">
  <img src="https://i.imgur.com/DsjpdyQ.jpg" width="700" alt="accessibility text">
</p>

A Timer/Stopwatch project that is based on a simple 1 second counter implemented to an ATmega328P. The limition of only having four 7-segment displays, means that the max time it can display is 99 minutes with 60 seconds, even though the counter can go up to 1092 minutes due to being a 16 bit unsigned variable.

Youtube Link:
https://youtu.be/yNI6ADch_Lk

How to use
------------
The main code is located in `main.c`, to access it:

`ATmega328P-Timer > ATmega328P-Timer > main.c`

The `main.c` is only required, as such it can be compiled in your favourite IDE (Atmel Studio/Arduino) and then uploaded to the microcontroller. 

Recent Implementation - Capacitors
------------

1 nF Capacitors were recently added in parallel with the interrupt switches due to polling issues brought on by using mechanical switches. Since often or not, clicking on the mechanical switch would cause slight repeated voltage changes accross the pins, making the state of the system unstable. By adding the capacitor in parallal with the switch inputs, the voltage going into the microcontroller cannot change as fast, thus preventing the unstable states.

Timer1 Setup
------------

The exact 1 second counter is based on the ATmega328P 16 MHz clock with a prescale of 256 bits to count to 62499 (`OCR1A = 62499`), when reached it causes an interrupt to occur, which adds 1 to the `uint16_t count` variable.

7-Segment Display 
------------

Multiplexing through each 7-segment display - each display is turned ON and OFF at a high frequency to display each number (0-9).

Interrupt Setup
------------

There are a total of 4 interrupts, with one being the basis for 1 second counter. The other interrupts are for the main control switches, being 'Interrupt0', 'Interrupt1', and 'PCInterrupt0'.

* Interrupt0: Controls the incrementing state of the system (PAUSE/RUN) and is used to cancel the ALERT state when the alarm is triggered.
* Interrupt1: Controls the mode of the system, allowing the user to switch between a simple stopwatch to a timer.
* PCInterrupt0: Due to a lack of interrupt pins, I implented another one to clear the `uint16_t count` variable.

Alarm Setup
------------

Since the volume of a piezoelectric buzzer is largerly dependent on the current, and the ATmega328P can only supply a limited amount of total current across the output pins, which is largely used by the LED 7-segment display. As such, I used a NPN transistor to amplify the current by connecting the buzzer to the collector terminal of the transistor, so that more current could be driven through the transistor by putting it in the Saturation region.



