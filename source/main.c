/*	Author: Tyler Pastor tpast001@ucr.edu
 *  Partner(s) Name: N/A
 *	Lab Section: 023
 *	Assignment: Lab #8  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: 
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

int num = 0;
unsigned char A;
int wait[] = {80, 80, 100, 80, 80, 200, 150, 150, 100, 100, 100, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75};
int playTime[] = {50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50};
double freq[] = {659.26, 659.26, 659.26, 523.25, 659.26, 783.99, 392, 523.25, 392, 329.63, 440, 493.88, 466.16, 440, 392, 659.26, 783.99, 880, 698.46, 783.99, 659.26, 523.25, 587.33, 493.88, 523.25, 392, 329.63, 440, 493.88, 466.16, 440, 392, 659.26, 783.99, 880, 698.46, 783.99, 659.26, 523.25, 587.33, 493.88};

enum SM1_STATES {SM1_SMStart, SM1_Wait, SM1_melody, SM1_melodyWait, SM1_play, SM1_pause} SM1_STATE;
void Melody() {
	switch(SM1_STATE) {
     		case SM1_SMStart:
      			SM1_STATE = SM1_Wait;
         		break;

		case SM1_Wait:
			if(A == 0x01){
				SM1_STATE = SM1_melody;
			}			
			break;

		case SM1_melody:
			if(num < 41){
				set_PWM(freq[num]);
				SM1_STATE = SM1_play;
			}
			else{
				SM1_STATE = SM1_melodyWait;
			}
			break;

		case SM1_play:
			TimerSet(playTime[num]);
			SM1_STATE = SM1_pause;

			break;

		case SM1_pause:
			TimerSet(wait[num]);
			num++;
			SM1_STATE = SM1_melody;

			break;

		case SM1_melodyWait:
			set_PWM(0.0);
			num = 0;

			if(A == 0x01){
				SM1_STATE = SM1_melodyWait;
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

		case SM1_melody:
         		break;

      		case SM1_melodyWait:
        		break;

		case SM1_play:
			break;

		case SM1_pause:
			break;
	}
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0x40; PORTB = 0xBF;

	SM1_STATE = SM1_SMStart;
        
	PWM_on();
	TimerSet(100);
	TimerOn();

    	while (1) {
		A = ~PINA;

		while(!TimerFlag);
		TimerFlag = 0;
		Melody();
    	}

    	return 0;
}
