/* program to read the internal temp sensor 
and show it on a 2 digit 7-segment display */


/*
segments
 ---A---
|       |
F       B
|___G___|
|       |
E       C
|___D___|

*/
/*

pinout of display
        ________     
C1    --| ---- |--  G1
E1    --|digit1|--  A1
D1    --| ---- |--  F1
Vcc1  --|      |--  B1
Vcc0  --| ---- |--  B0
D0    --|digit0|--  F0
E0    --| ---- |--  A0
C0    --|______|--  G0

common adode for each digit
pull segment's pin LOW to turn it on
*/

//includes
#include <msp430.h>

//defines
#define SEG_A BIT0
#define SEG_B BIT1
#define SEG_C BIT2
#define SEG_D BIT3
#define SEG_E BIT4
#define SEG_F BIT5
#define SEG_G BIT7

#define DIG_0 BIT0
#define DIG_1 BIT1

//add any defined digits to this array
unsigned char digit_bits[] = { DIG_0, DIG_1 };

int i;

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD; //disable watchdog

	//setup port 1 registers?
	P1DIR = (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G);
	
	//sets of digits
	for (i=0; i < sizeof(digit_bits); i++){
		P2DIR |= digit_bits[i];		
	}

}
