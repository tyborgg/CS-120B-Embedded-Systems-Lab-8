/*	Author: Tyler Pastor tpast001@ucr.edu
 *  Partner(s) Name: N/A
 *	Lab Section: 023
 *	Assignment: Lab #8  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: https://www.youtube.com/watch?v=xrsQvBqI9lc
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
int count;
int pwr = 0;
double freq[] = {261.63, 293.66, 329.63, 349.23, 392, 440, 493.88, 523.25};

enum SM1_STATES {SM1_SMStart, SM1_Wait, SM1_inc, SM1_dec, SM1_OnOff} SM1_STATE;
void IncDecSound() {
	switch(SM1_STATE) {
     		case SM1_SMStart:
      			SM1_STATE = SM1_Wait;
         		break;

		case SM1_Wait:
			if(A == 0x01){
				SM1_STATE = SM1_OnOff;
			}
			else if(A == 0x02){
				SM1_STATE = SM1_inc;
			}	
			else if(A == 0x04){
				SM1_STATE = SM1_dec;
			}
			else{
				SM1_STATE = SM1_Wait;
			}			
			break;

		case SM1_OnOff:
			if(A == 0x01){
				SM1_STATE = SM1_OnOff;
			}
			else{
				if(pwr == 0){
                                        PWM_on();
                                        pwr = 1;
					set_PWM(freq[count]);
                                }
                                else{
                                        PWM_off();
                                        pwr = 0;
                                }
				
				SM1_STATE = SM1_Wait;
			}
			break;



		case SM1_inc:
			if(A == 0x02){
				SM1_STATE = SM1_inc;
			}
			else if (A == 0x00){
				if(count < 7){
					count++;
				}
				set_PWM(freq[count]);	
				SM1_STATE = SM1_Wait;
			}
			break;

		case SM1_dec:
			if(A == 0x04){
				SM1_STATE = SM1_dec;
			}
			else if(A == 0x00){
				if(count > 0){
					count--;
				}
				set_PWM(freq[count]);
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

		case SM1_inc:
         		break;

      		case SM1_dec:
        		break;

		case SM1_OnOff:
			break;
	}
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0x40; PORTB = 0xBF;

	SM1_STATE = SM1_SMStart;
        
	TimerSet(50);
	TimerOn();

    	while (1) {
		A = ~PINA;

		while(!TimerFlag);
		TimerFlag = 0;
		IncDecSound();
    	}

    	return 0;
}
