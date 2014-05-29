//main code based off of mspsci.blogspot.com 's interrupted-2_G2211.c

//	Header Files

#include <msp430.h> //include msp430 stuff

//#include <signal.h> //stuff for interrupts to work.

//	Preprocessor Definitions
#define LED1 BIT0;

//	Global Variables
volatile char i=0;

//	Function Prototypes/Definitions

int main(void) {
	WDTCTL = WDTPW + WDTHOLD; //disable watchdog
	
	//initialization stuff here
		//remember to enable any pull-up/downs!
	
	P1OUT = 0;
	P1DIR |= LED1;
	
	BCSCTL1 = CALBC1_1MHZ;	// Set DCO to calibrated 1 MHz
	DCOCTL = CALDCO_1MHZ;
	
	TACCR0 = 62500 - 1;		// A period of 62,500 cycles is 0 to 62,499.
	TACCTL0 = CCIE;        // Enable interrupts for CCR0.
	TACTL = TASSEL_2 + ID_3 + MC_1 + TACLR;  // SMCLK, div 8, up mode, 
	                                         // clear timer
	//_enable_interrupt(); 
	_BIS_SR(GIE);
	//infinite loop
	for(;;) {
			//just waiting for interrupts. should use a LPM instead
	}
	return 0; //should never reach this	
}

//	Functions

//	Interrupt Service Routines
//mspgcc way (nicer I think)
__attribute__((interrupt(TIMER0_A0_VECTOR))) //notice! for the 20pin chips:
											//had to change TIMERA0_VECTOR
											//to TIMER0_A0_VECTOR 
void CCR0_ISR(void){

	if (++i == 120) {
		P1OUT ^= LED1; //toggle led
		i=0;
	}
}
//#pragma vector = TIMERA0_VECTOR //this is CCS stuff
//__interrupt void CCR0_ISR(void) {
//	if (++i == 120) {
//		P1OUT ^= LED1; //toggle led
//		i=0;
//	}
//}
