//main code
//alteration that lights led1 when input is above 0.5 * Vcc (1/2)
//and lights led2 when input is above 0.25 * Vcc (1/4)
//using the built in analog comparator
//this based on the exercise at the end ot Tutorial 12
// on mspsci.blogspot.com


//	Header Files

#include <msp430.h> //include msp430 stuffq

//	Preprocessor Definitions
#define LED1 BIT0
#define LED2 BIT6

#define AIN1 BIT1

//	Global Variables
char flash = 0;					// start with no flash on LED

//	Function Prototypes/Definitions

void main(void) {
	WDTCTL = WDTPW + WDTHOLD; //disable watchdog
	
	//initialization stuff here
		//remember to enable any pull-up/downs!
	P1OUT = 0;
	P1DIR |= (LED1|LED2);				// output on P1.0 for LED1
										// and P1.6 for LED2
						
	CACTL1 = CAREF1 + CARSEL + CAIE;	// 0.5 Vcc ref on - pin, enable
										// interrupts on rising edge.
	CACTL2 = P2CA4 + CAF;		// Input CA1 on + pin, filter output.
	CAPD = AIN1;				// disable digital I/O on P1.1 (technically
								// this step is redundant)
	
	TACCR0 = 60000;				// Timer delay for LED flash, 60000 cycles
	TACCTL0 = CCIE;     		// Enable interrupts for CCR0.
	TACTL = TASSEL_2 + ID_3 + MC_1 + TACLR;  // SMCLK, div 8, up mode, 
	                                         // clear timer
	
	CACTL1 |= CAON;				// turn on comparator	
	_BIS_SR(LPM0_bits + GIE);	// Enter LPM0 and enable interrupts
	/*
	//infinite loop
	for(;;) {

	}
	..return 0; //should never reach this	
	*/
}

//	Functions

//	Interrupt Service Routines

__attribute__((interrupt(TIMER0_A0_VECTOR))) //notice! for the 20pin chips:
											//had to change TIMERA0_VECTOR
											//to TIMER0_A0_VECTOR 
void CCR0_ISR(void){
	//P1OUT ^= flash;// if flash is zero, keep LED off
								// if flash is LED1, toggle LED.
	if (flash >0){
		P1OUT |= flash;
	} else {
		P1OUR &= flash;
	}
}

__attribute__((interrupt(COMPARATORA_VECTOR)))
void COMPA_ISR(void) {
		
		if ((CACTL2 & CAOUT)==0x01) {
		CACTL1 |= CAIES;		// value high, so watch for falling edge
		flash = LED1;			// let LED flash
	}
	else {
		CACTL1 &= ~CAIES;		// value low, so watch for rising edge
		flash = 0;				// turn LED off
		P1OUT = 0;
	}	
		
}
