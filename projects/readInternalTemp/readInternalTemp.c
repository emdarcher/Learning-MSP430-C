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
#define SEG_A BIT7
#define SEG_B BIT6
#define SEG_C BIT5
#define SEG_D BIT4
#define SEG_E BIT3
#define SEG_F BIT2
#define SEG_G BIT1

#define DIG_0 BIT0
#define DIG_1 BIT1

//prototypes
void write_number_digit(unsigned char num, unsigned char dig);

//add any defined digits to this array
unsigned char digit_bits[] = { DIG_0, DIG_1 };
//int i used for "for" loops
int i;

//array to store bytes of port 1 values to make numbers
unsigned char number_seg_bytes[] = {
//       unconfigured
//ABCDEFG^
0b00000010;//0
0b10011110;//1
0b00100100;//2
0b00001100;//3
0b10011000;//4
0b01001000;//5
0b01000000;//6
0b00011110;//7
0b00000000;//8
0b00011000;//9

};

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
