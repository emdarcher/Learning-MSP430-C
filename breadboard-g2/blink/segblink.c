//blink red led and seg A

#include <msp430.h>

void main(void){
	
	WDTCTL = WDTPW + WDTHOLD; //disable watchdog	

	P1DIR |= 0x03;
    P1OUT |= 0x3;
    P2DIR |= 0b10;
    P2OUT &= ~(0b10);
	
	while(1){
		P1OUT ^= (1 << 0);
        P1OUT ^= (1 << 1);
        //P1OUT ^= 0b10;
		
		int i;
		//delay
		for(i = 0; i < 0xFFFF; i++){
		}
	}


}
