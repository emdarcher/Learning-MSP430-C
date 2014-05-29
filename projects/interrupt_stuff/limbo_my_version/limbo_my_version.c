//main code

//demonstration of LPM and interrupts
//based mspsci.blogspot.com 's limboG2211.c

//	Header Files

#include <msp430.h> //include msp430 stuff

//	Preprocessor Definitions
#define LED1 BIT0
#define LED2 BIT6

//	Global Variables

//	Function Prototypes/Definitions

int main(void) {
	WDTCTL = WDTPW + WDTHOLD; //disable watchdog
	
	//initialization stuff here
		//remember to enable any pull-up/downs!
	P1OUT |= LED1;
	P1OUT &= ~LED2;
	
	P1DIR |= (LED1 | LED2);
	
	BCSCTL1 = CALBC1_1MHZ;		// Running at 1 MHz
	DCOCTL = CALDCO_1MHZ;

	TACCR0 = 144;           // With the Timer using SMCLK div 8 (125 kHz), this
							// value gives a frequency of 125000/(TACCR0+1) Hz.
							// For TACCR0 = 144, that's 862 Hz.
							
	TACCTL0 = CCIE;         // Enable interrupts for CCR0.
	TACTL = TASSEL_2 + ID_3 + MC_1 + TACLR;  // SMCLK, div 8, up mode,
											 // clear timer.
	
	_BIS_SR(LPM0_bits + GIE);	// Enter LPM0 and enable interrupts
	
	/*
	//infinite loop
	for(;;) {

	}
	return 0; //should never reach this	
	*/
} //main

//	Functions 

//	Interrupt Service Routines

//  mspgcc way (nicer I think)
__attribute__((interrupt(TIMER0_A0_VECTOR))) //notice! for the 20pin chips:
											//had to change TIMERA0_VECTOR
											//to TIMER0_A0_VECTOR 
void CCR0_ISR(void){
	P1OUT ^= (LED1 | LED2);
}

