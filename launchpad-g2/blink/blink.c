//blink red led

#include <msp430.h>

void main(void){

	P1DIR |= 0x01;
	
	while(1){
		P1OUT ^= 0x01;
		
		int i;
		//delay
		for(i = 0; i < 0xFFFF; i++){
		}
	}


}
