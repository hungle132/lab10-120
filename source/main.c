/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn(){

TCCR1B = 0x0B;

OCR1A = 125;

TIMSK1 = 0x02;

TCNT1 = 0;

_avr_timer_cntcurr = _avr_timer_M;

SREG |= 0x80;

}

void TimerOff(){
	TCCR1B = 0x00;
}

void TimerISR(){
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect){
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0){
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M){
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}


enum ThreeLEDsSM{start,one,two,three} state1;
enum BlinkingLEDSM{start1,four,five} state2;
enum CombineLEDsSM{init,comb} state3;
unsigned char threeLEDS = 0x00;
unsigned char blinkLEDSM = 0x00;
void fsm(){

	switch(state1){
	case start:
		state1 = one;
		break;
	case one: 
		
		state1 = two;
		break;
		
	case two:
		state1 = three;
		break;
	case three:
		state1 = start;
	       break;	

	default:
		break;
	}
	switch(state1){
	case start:
		break;
	case one:
		threeLEDS = 1;
		break;
	case two:
		threeLEDS = 2;
		break;
	case three:
		threeLEDS = 4;
		break;
	}

}

void fsm1(){
	switch(state2){
		case start1:
			state2=four;
			break;
case four:
	state2 = five;
	break;
case five:
	state2 = four;
	break;
	}

	switch(state2){
	case start1:
		break;
	case four:
		blinkLEDSM = 0;
		break;
	case five:
		blinkLEDSM = 8;
		break;
	}
}

void fsm2(){
	switch(state3){
	case init:
	state3 = comb;
		break;
		
	case comb:
		state3 = comb;
		break;
	}
	switch(state3){
	case init:
		PORTB = 0;
		break;
	case comb:
		PORTB =  threeLEDS  | blinkLEDSM;
		break;
		}
	
	}

	












int main(void) {
    /* Insert DDR and PORT initializations */
DDRB = 0xFF; PORTB = 0x00;
TimerSet(125);
TimerOn();
state1 = start;
state2 = start1;
    /* Insert your solution below */
    while (1) {
	fsm();
	fsm1();
	fsm2();
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
