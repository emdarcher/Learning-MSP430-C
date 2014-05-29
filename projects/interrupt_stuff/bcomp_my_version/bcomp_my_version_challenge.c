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

#define REF_SWITCH_DELAY 12 //number of cycles to delay between
								//comparator ref switching

//	Global Variables
char flash = 0;					// start with no flash on LED

volatile unsigned char current_led, other_led;
volatile unsigned char comp_state;

//	Function Prototypes/Definitions

void comp_ref_to_led(void);

void main(void) {
	WDTCTL = WDTPW + WDTHOLD; //disable watchdog
	
	//initialization stuff here
		//remember to enable any pull-up/downs!
	P1OUT = 0;
	P1DIR |= (LED1|LED2);				// output on P1.0 for LED1
										// and P1.6 for LED2
	
	current_led = LED1; //start with LED1 b/c it initializes with 0.5 Vcc
	comp_state = 1;					
						//clarification!, took me forever to figure out.
						//CAREF1 ---- CAREF0
						//bit5			bit4
						//look at datasheet for better details
						
	CACTL1 = CAREF1 + CARSEL + CAIE;	// 0.5 Vcc ref on - pin, enable
										// interrupts on rising edge.
	//CACTL1 &= CAREF0;
	
	CACTL2 = P2CA4 + CAF;		// Input CA1 on + pin, filter output.
	CAPD = AIN1;				// disable digital I/O on P1.1 (technically
								// this step is redundant)
	
	TACCR0 = REF_SWITCH_DELAY;				// Timer delay for comparator
											//reference switching
											// With the Timer using SMCLK div 8 (125 kHz), this
							// value gives a frequency of 125000/(TACCR0+1) Hz.
							// For TACCR0 = 144, that's 862 Hz.
							// at 14400 it is 8.7... Hz
							
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

void comp_ref_to_led(void){

	//if ((CACTL1 & CAREF1) == 0x01){ //if CAREF1 is 1, so 0.5 Vcc
	if (comp_state){
		current_led = LED1; //if higher that 0.5 Vcc is satisfies
										//both conditions
		other_led = LED2;
	} else {
		current_led = LED2;
		other_led = 0;
	}
}

//	Interrupt Service Routines

__attribute__((interrupt(TIMER0_A0_VECTOR))) //notice! for the 20pin chips:
											//had to change TIMERA0_VECTOR
											//to TIMER0_A0_VECTOR 
void CCR0_ISR(void){
	CACTL1 &= ~CAON; //turn off comparator for settings change
	
	//toggle CAREF1 to set between 0.5 to 0.25 Vcc
	CACTL1 ^= CAREF1;
	//toggle CAREF0 as well
	CACTL1 ^= CAREF0;
	/*
	if ((CACTL1 & CAREF1) == 0x01){ //if CAREF1 is 1, so 0.5 Vcc
		current_led = LED1;
	} else {
		current_led = LED2;
	} 
	*/
	comp_state ^= 1;
	CACTL1 |= CAON; //turn back on
}

__attribute__((interrupt(COMPARATORA_VECTOR)))
void COMPA_ISR(void) {
	
	comp_ref_to_led();
		
	if ((CACTL2 & CAOUT)==0x01) {
		CACTL1 |= CAIES;		// value high, so watch for falling edge
		P1OUT |= current_led + other_led;			
	}
	else {
		CACTL1 &= ~CAIES;		// value low, so watch for rising edge
		P1OUT &= ~(current_led);
	}	
		
}
