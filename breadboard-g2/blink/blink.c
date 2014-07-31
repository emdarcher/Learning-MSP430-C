//blink red led

#include <msp430.h>

void main(void){
	
	WDTCTL = WDTPW + WDTHOLD; //disable watchdog	

	P1DIR |= 0x01;
	
	while(1){
		P1OUT ^= 0x01;
		
		int i=10;
		//delay
		//for(i = 0; i < 0xFFFF; i++){
		//}
        while(i--){
        __delay_cycles(50000);
        }	
    }


}
