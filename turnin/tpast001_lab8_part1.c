/*	Author: Tyler Pastor tpast001@ucr.edu
 *  Partner(s) Name: N/A
 *	Lab Section: 023
 *	Assignment: Lab #8  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: https://www.youtube.com/watch?v=-N47BX7ryoE
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

void set_PWM(double frequency){
	//static double current_frequency;

	//if(frequency != current_frequency0{
	
		if(!frequency) {TCCR3B &= 0x08;}
		else {TCCR3B |= 0x03;}
		
		if(frequency < 0.954){
			OCR3A = 0xFFFF;
		}
		else if(frequency > 31250){
			OCR3A = 0x0000;
		}
		else{
			OCR3A = (short)(8000000 / (128 * frequency)) - 1;
		}

		TCNT3 = 0;
}

void PWM_on(){
	TCCR3A = (1 << COM3A0);

	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);

	set_PWM(0);
}

void PWM_off(){
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

unsigned char A;

enum SM1_STATES {SM1_SMStart, SM1_c4, SM1_d4, SM1_e4, SM1_Wait, SM1_waitc4, SM1_waitd4, SM1_waite4} SM1_STATE;
void ButtonSound() {
	switch(SM1_STATE) {
     		case SM1_SMStart:
      			SM1_STATE = SM1_Wait;
         		break;

		case SM1_Wait:
			set_PWM(0.0);

			if((A & 0x07) == 0x01){
				SM1_STATE = SM1_c4;
			}
			else if((A & 0x07) == 0x02){
				SM1_STATE = SM1_d4;
			}
			else if((A & 0x07) == 0x04){
				SM1_STATE = SM1_e4;
			}
			else{
				SM1_STATE = SM1_Wait;
			}
			break;

		case SM1_c4:
			set_PWM(261.63);
			SM1_STATE = SM1_waitc4;
			break;

		case SM1_d4:
			set_PWM(293.66);
			SM1_STATE = SM1_waitd4;
			break;

		case SM1_e4:
			set_PWM(329.63);
			SM1_STATE = SM1_waite4;
			break;

		case SM1_waitc4:
			if(A == 0x01){
				SM1_STATE = SM1_waitc4;
			}
			else if(A == 0x00){
				SM1_STATE = SM1_Wait;
			}
			break;

		case SM1_waitd4:
			if(A == 0x02){
                                SM1_STATE = SM1_waitd4;
                        }
                        else if(A == 0x00){
                                SM1_STATE = SM1_Wait;
                        }
			break;

		case SM1_waite4:
			if(A == 0x04){
                                SM1_STATE = SM1_waite4;
                        }
                        else if(A == 0x00){
                                SM1_STATE = SM1_Wait;
                        }
			break;

      		default:
         		SM1_STATE = SM1_Wait;
         		break;
   	}

   	switch(SM1_STATE) {
      		case SM1_SMStart:
         		break;

		case SM1_Wait:
         		break;

		case SM1_c4:
         		break;

      		case SM1_d4:
        		break;

		case SM1_e4:
        		break;

		case SM1_waitc4:
        		break;

		case SM1_waitd4:
        		break;

		case SM1_waite4:
                        break;

	}
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0x40; PORTB = 0xBF;

	SM1_STATE = SM1_SMStart;

	PWM_on();
        
	TimerSet(120);
	TimerOn();

    	while (1) {
		A = ~PINA;

		while(!TimerFlag);
		TimerFlag = 0;
		ButtonSound();
    	}

    	return 0;
}
