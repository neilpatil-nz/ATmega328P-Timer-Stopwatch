/***** Project Header *****/
// Project Name:
// Version: 1.1
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
//Hardware macros for outputs

#define SET_ALARM PORTD |= (1<<PD1)
#define CLR_ALARM PORTD &= ~(1<<PD1)

#define CLR_LED_1   PORTD |= (1<<PD6)
#define SET_LED_1   PORTD &= ~(1<<PD6)

#define CLR_LED_2   PORTD |= (1<<PD5)
#define SET_LED_2   PORTD &= ~(1<<PD5)

#define CLR_LED_3   PORTD |= (1<<PD4)
#define SET_LED_3   PORTD &= ~(1<<PD4)

#define CLR_LED_4   PORTD |= (1<<PD7)
#define SET_LED_4   PORTD &= ~(1<<PD7)

//Hardware macros for inputs
//Hardware macros for ADC inputs

/***** User macros *****/
#define DEL_TIME 5
#define ALARM_DEL_TIME 100

/***** States *****/
#define PAUSE 0
#define RUN 1
#define DEFAULT 2
#define TIMER 3
#define ALERT 4


/***** Declare & initialise global variables *****/

uint8_t i =0;
uint16_t count = 300;
uint8_t state_clock = RUN;
uint8_t state_setting = DEFAULT;

/***** Interrupt Service Routines *****/
 ISR(TIMER1_COMPA_vect){
	 if (state_clock == RUN){
		count++;
	 }
 }
 ISR(INT0_vect){
	 if (state_clock == PAUSE){
		 state_clock = RUN;
	 }
	 else{
		 state_setting = DEFAULT;
		 state_clock = PAUSE;
	 }
	 
 }
 
/***** Prototypes  for Functions *****/
void call_num();
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
	DDRD |= (1 << PD7);
	DDRD |= (1 << PD6);
	DDRD |= (1 << PD5);
	DDRD |= (1 << PD4);
	DDRD |= (1 << PD1);
	
	DDRB |= (1 << PB1); //setup OC1A pin (B1) as output

	/***** External Interrupt Setup *****/
	DDRD &=~ (1 << PD2);
	PORTD |= (1 << PD2);//Interrupt0 
	
	//EICRA |= (1 <<  ISC00);
	EICRA |= (1 <<  ISC01);
	EIMSK |= (1 << INT0);
	
	/***** Timer Config *****/
	TCCR1B |= (1 << WGM12); // CTC mode
	TCCR1A |= (1 << COM1A0); // toggle OC1A (B1 )output on match
	TCCR1B |= (1 << CS12); // Prescaler /256
	OCR1A = 62499;
	TIMSK1 |= (1 << OCIE1A);
	sei(); 
	 
	/***** LED Segment Ouput *****/ 
	DDRC = 0xff;
	DDRB = 0xff;
	
    // configure the internal pullup resistors for these pins

    /***** Main variables go here *****/

    /***** Run once code goes here *****/

    /***** Loop code *****/
    while (1) {
		CLR_ALARM;
		if (count == 320){
			state_clock = PAUSE;
			state_setting = ALERT;
			while(state_setting == ALERT){
				SET_ALARM;
				_delay_ms(ALARM_DEL_TIME);
				CLR_ALARM;
				_delay_ms(ALARM_DEL_TIME);
			}
		}
		_delay_ms(DEL_TIME);
		call_num(count/60);
		CLR_LED_1;
		CLR_LED_2;
		SET_LED_3;
		CLR_LED_4;
		
		
		_delay_ms(DEL_TIME);
		if ((count%60)/10 >= 6){
			call_num(0);
			CLR_LED_1;
			SET_LED_2;
			CLR_LED_3;
			CLR_LED_4;
		}
		else {
			call_num((count%60)/10);
			CLR_LED_1;
			SET_LED_2;
			CLR_LED_3;
			CLR_LED_4;
		}
		
	
		_delay_ms(DEL_TIME);
		call_num(count%10);
		SET_LED_1;
		CLR_LED_2;
		CLR_LED_3;
		CLR_LED_4;
		
    } //while end
} //main end
/***** Functions *****/
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

void num_zero(){
	PORTC = 0b0101111;
	PORTB = 0b11111;
}
void num_one(){
	PORTC = 0b0000100;
	PORTB = 0b11111;
}
void num_two(){
	PORTC = 0b111010;
	PORTB = 0b11111;
}
void num_three(){
	PORTC = 0b111100;
	PORTB = 0b11111;
}
void num_four(){
	PORTC = 0b0010101;
	PORTB = 0b11111;
}
void num_five(){
	PORTC = 0b111101;
	PORTB = 0b11110;
}
void num_six(){
	PORTC = 0b111111;
	PORTB = 0b11110;
}
void num_seven(){
	PORTC = 0b100100;
	PORTB = 0b11111;
}
void num_eight(){
	PORTC = 0b111111;
	PORTB = 0b11111;
}                                                                       
void num_nine(){
	PORTC = 0b110101;
	PORTB = 0b11111;
}