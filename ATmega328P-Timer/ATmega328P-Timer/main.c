/***** Project Header *****/
// Project Name: Stopwatch and Timer
// Version: 1.4
// Author: Neil Patil


/***** Hardware defines *****/
//make sure this matches your oscillator setting
#define F_CPU 16000000//crystal

/***** Includes *****/
#include <avr/io.h>
//#include <stdint.h>
#include <util/delay.h>
//#include <avr/interrupt.h>
//#include <avr/eeprom.h>
//#include <stdio.h>
//#include <string.h>
//#include <avr/pgmspace.h>
#include <avr/interrupt.h>

/***** Hardware macros *****/

//Hardware macro for alarm

#define SET_ALARM PORTB |= (1<<PB5) 
#define CLR_ALARM PORTB &= ~(1<<PB5)

//Hardware macros for LED Segments

#define CLR_LED_1   PORTD |= (1<<PD6)
#define SET_LED_1   PORTD &= ~(1<<PD6)

#define CLR_LED_2   PORTD |= (1<<PD5)
#define SET_LED_2   PORTD &= ~(1<<PD5)

#define CLR_LED_3   PORTD |= (1<<PD4)
#define SET_LED_3   PORTD &= ~(1<<PD4)

#define CLR_LED_4   PORTD |= (1<<PD7)
#define SET_LED_4   PORTD &= ~(1<<PD7)

//Hardware macros for LEDs

#define SET_DEFAULT_LED PORTB |= (1<<PB4) //Turns ON the Default LED
#define CLR_DEFAULT_LED PORTB &=~ (1<<PB4) //Turns ON the Timer LED

//Hardware macros for input switches

#define ADD_SWITCH_IS_HIGH PIND & (1<<PD1)
#define ADD_SWITCH_IS_LOW ~PIND & (1<<PD1)

#define MINUS_SWITCH_IS_HIGH PIND & (1<<PD0)
#define MINUS_SWITCH_IS_LOW ~PIND & (1<<PD0)

#define CLEAR_SWITCH_IS_HIGH PINB & (1<<PB3)
#define CLEAR_SWITCH_IS_LOW ~PINB & (1<<PB3)

/***** User macros *****/
#define DEL_TIME 2
#define ALARM_DEL_TIME 100

/***** States *****/
#define PAUSE 0 // Blocks increments of count
#define RUN 1 // Allows increments of count
#define DEFAULT 2 // Initial Stopwatch Mode
#define TIMER 3 // Timer Mode
#define ALERT 4 // When Triggered


/***** Declare & initialise global variables *****/
volatile uint16_t count = 0;
volatile uint16_t timer_count = 0;
volatile uint8_t state_clock = RUN;
volatile uint8_t state_setting = DEFAULT;

/***** Interrupt Service Routines *****/
 ISR(TIMER1_COMPA_vect){
	 if (state_clock == RUN){ //exact 1 sec increment
		count++;
	 }
 }
 /***** Changes State (Stop/Continue Incrementing)*****/
 ISR(INT0_vect){ 
	 if (state_clock == PAUSE){
		 state_clock = RUN;
	 }
	 else if (state_setting == ALERT){ //Stops Alert
		 state_clock = PAUSE;
		 state_setting = TIMER;
		 count = 0;
	 }
	 else {
		 state_clock = PAUSE;
	 }
 }
 /***** Changes Mode (Timer/Stopwatch) *****/
 ISR(INT1_vect){
	 if (state_setting == DEFAULT){ 
		state_setting = TIMER;
		count = 0;
		CLR_DEFAULT_LED;
	 }
	 else{
		 state_setting = DEFAULT;
		 SET_DEFAULT_LED;
	 }
	 state_clock = PAUSE;
	 count = 0;
 }
 
 //Resets count
ISR(PCINT0_vect){
	if (CLEAR_SWITCH_IS_LOW){
		count = 0;
	}
}

 
/***** Prototypes  for Functions *****/

/***** Displays Segments *****/
void disp_function();
void call_num();

/***** Displays given number *****/
void num_zero();
void num_one();
void num_two();
void num_three();
void num_four();
void num_five();
void num_six();
void num_seven();
void num_eight();
void num_nine();

/***** Main function *****/
int main(void) {
    /***** Initial hardware setups go here *****/
	
    /***** IO Hardware Config *****/
    // configure these pins as outputs
	DDRD |= (1 << PD7); //LED Segment Display 4
	DDRD |= (1 << PD4); //LED Segment Display 3
	DDRD |= (1 << PD5); //LED Segment Display 2
	DDRD |= (1 << PD6); //LED Segment Display 1
	
	DDRB |= (1 << PB1); //setup OC1A pin (B1) as output
	
	/***** Pin Inputs *****/
	
	//Add Pin
	PORTB |= (1<<PD1);
	
	//Minus Pin
	PORTD |= (1<<PD0);
	
	/***** External Interrupt Setup *****/
	//Interrupt0 
	
	PORTD |= (1 << PD2);
	
	//Interrupt1 
	
	PORTD |= (1 << PD3);
	
	//PCInterrupt0
	PORTB |= (1<<PB3);
	
	//Interrupt0 and Interrupt1 (Rising Edge)
	EICRA |= (1 <<  ISC01)|(1 <<  ISC01); 
	EICRA |= (1 <<  ISC11)|(1 <<  ISC01);
	EIMSK |= (1 << INT0)|(1 << INT1);
	
	//Pin 3 Interrupt Setup
	PCICR |= (1 <<  PCIE0);
	PCMSK0 |= (1 << PCINT3);
	
	/***** Timer Config (Exact 1 sec increment)*****/
	TCCR1B |= (1 << WGM12); // CTC mode
	TCCR1A |= (1 << COM1A0); // toggle OC1A (B1)output for every 1 sec
	TCCR1B |= (1 << CS12); // Prescaler /256
	OCR1A = 62499;
	TIMSK1 |= (1 << OCIE1A); // Triggers Interrupt0
	sei(); 
	
	DDRC = 0xff;
	DDRB = 0xff;
    /***** Initial states of Buzzer and Mode LEDs *****/
	CLR_ALARM;
	SET_DEFAULT_LED;
	
    /***** Loop code *****/
    while (1) {
		if (state_setting == DEFAULT){ // Initial Stopwatch phase
			disp_function(count);
		}
		// Timer Mode while its incrementing
		else if (state_setting == TIMER && state_clock == RUN) { 
			disp_function(count);
			if (count == timer_count){ //Triggers Alarm
				state_clock = PAUSE; //Pauses the system
				state_setting = ALERT; //Alarm state
				while(state_setting == ALERT){ // >//= Button is required to change state
					SET_ALARM;
					_delay_ms(ALARM_DEL_TIME);
					CLR_ALARM;
					_delay_ms(ALARM_DEL_TIME);
				}
			}
		}
		// In Timer Mode while incrementing
		else if (state_setting == TIMER && state_clock == PAUSE){
			disp_function(timer_count);
			if (ADD_SWITCH_IS_LOW){
				timer_count++;
				disp_function(timer_count);
			}
			else if (MINUS_SWITCH_IS_LOW && timer_count !=0){
				timer_count--;
				disp_function(timer_count);
			}	
		}
    } 
} 
/***** Functions *****/

//Multiplexes through each LED segment display
void disp_function(uint16_t count_input){
	
	// 10 minute 
	_delay_ms(DEL_TIME);
	call_num(count_input / 600);
	CLR_LED_1;
	CLR_LED_2;
	CLR_LED_3;
	SET_LED_4;
	
	// 1 minute
	_delay_ms(DEL_TIME);
	call_num((count_input % 600)/60);
	CLR_LED_1;
	CLR_LED_2;
	SET_LED_3;
	CLR_LED_4;
	
	// 10 second
	_delay_ms(DEL_TIME);
	if ((count_input % 60)/10 >= 6){
		call_num(0);
		CLR_LED_1;
		SET_LED_2;
		CLR_LED_3;
		CLR_LED_4;
	}
	else {
		call_num((count_input % 60)/10);
		CLR_LED_1;
		SET_LED_2;
		CLR_LED_3;
		CLR_LED_4;
	}
	
	// 1 second
	_delay_ms(DEL_TIME);
	call_num(count_input % 10);
	SET_LED_1;
	CLR_LED_2;
	CLR_LED_3;
	CLR_LED_4;
}

// Selects which number to output
void call_num(uint16_t count_input){
	switch (count_input)
	{
		case 1:
		num_one();
		break;
		
		case 2:
		num_two();
		break;
		
		case 3:
		num_three();
		break;
		
		case 4:
		num_four();
		break;
		
		case 5:
		num_five();
		break;
		
		case 6:
		num_six();
		break;
		
		case 7:
		num_seven();
		break;
		
		case 8:
		num_eight();
		break;
		
		case 9:
		num_nine();
		break;
		
		default:
		num_zero();
	}
}

// Turns ON certain LED segments
void num_zero(){
	PORTC = 0b0101111;
	PORTB |= (1 << PB0)|(1 << PB2);
}
void num_one(){
	PORTC = 0b0000100;
	PORTB |= (1 << PB0)|(1 << PB2);
}
void num_two(){
	PORTC = 0b111010;
	PORTB |= (1 << PB0)|(1 << PB2);
}
void num_three(){
	PORTC = 0b111100;
	PORTB |= (1 << PB0)|(1 << PB2);
}
void num_four(){
	PORTC = 0b0010101;
	PORTB |= (1 << PB0)|(1 << PB2);
}
void num_five(){
	PORTC = 0b111101;
	PORTB &=~ (1 << PB0);
	PORTB |=  (1 << PB2);
}
void num_six(){
	PORTC = 0b111111;
	PORTB &=~ (1 << PB0);
	PORTB |=  (1 << PB2);
}
void num_seven(){
	PORTC = 0b100100;
	PORTB |= (1 << PB0)|(1 << PB2);
}
void num_eight(){
	PORTC = 0b111111;
	PORTB |= (1 << PB0)|(1 << PB2);

}                                                                       
void num_nine(){
	PORTC = 0b110101;
	PORTB |= (1 << PB0)|(1 << PB2);

}