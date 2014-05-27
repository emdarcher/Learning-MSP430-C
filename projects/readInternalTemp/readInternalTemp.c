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

#define ALL_SEGS ( SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G )

#define DIG_0 BIT0
#define DIG_1 BIT1

//remember to add any newly defines digits here
#define ALL_DIGS ( DIG_0 | DIG_1 )

//prototypes
void clear_display(void);
void write_digit(unsigned char num, unsigned char dig);
void write_number(unsigned char number);

//add any defined digits to this array
unsigned char digit_bits[] = { DIG_0, DIG_1 };
//int i used for "for" loops
int i;

//array to store bytes of port 1 values to make numbers
unsigned char number_seg_bytes[] = {
//       unconfigured
//ABCDEFG^
0b00000010,//0
0b10011110,//1
0b00100100,//2
0b00001100,//3
0b10011000,//4
0b01001000,//5
0b01000000,//6
0b00011110,//7
0b00000000,//8
0b00011000,//9

};

int tempVals;

/*
from here:
https://indiantinker.wordpress.com/2012/12/13/tutorial-using-the-internal-temperature-sensor-on-a-msp430/ 
*/
void tempInit()
{
    ADC10CTL0=SREF_1 + REFON + ADC10ON + ADC10SHT_3 ; //1.5V ref,Ref on,64 clocks for sample
    ADC10CTL1=INCH_10+ ADC10DIV_3; //temp sensor is at 10 and clock/4
}
int tempOut()
{
    int t=0;
    __delay_cycles(1000);              //wait 4 ref to settle
    ADC10CTL0 |= ENC + ADC10SC;      //enable conversion and start conversion
    while(ADC10CTL1 & BUSY);         //wait..i am converting..pum..pum..
    t=ADC10MEM;                       //store val in t
    ADC10CTL0&=~ENC;                     //disable adc conv
    return(int) ((t * 27069L - 18169625L) >> 16); //convert and pass
}

int main(void)
{
	
	WDTCTL = WDTPW + WDTHOLD; //disable watchdog

	//setup port 1 registers?
	P1DIR |= ALL_SEGS;
	
	//sets of digits
	P2DIR |= ALL_DIGS;
	//for (i=0; i < sizeof(digit_bits); i++){
	//	P2DIR |= digit_bits[i];		
	//}	P2OUT &= ~(ALL_DIGS); //set digits LOW so nothing goes on	
	
	clear_display();

	volatile int temp;
	temp=0;
	tempInit();//initialize adc	
	
	int avg = 0;	

	unsigned int x;
	for(;;){
		/*for(i = 99; i > 0 ; i--){
			for (x = 0; x < 2048; x++){ 
				//test
				write_number(i);
			}
		}*/
		for(x = 0; x < 10; x++){
		__delay_cycles(500);
		tempVals = tempVals + tempOut();
		write_number(avg);
		__delay_cycles(500);
		}
		avg = tempVals / 10;
		//write_number(tempVals / 10 );
	}

	return 0; //should never reach this point
}

//function to clear everything
void clear_display(void){
	P2OUT &= ~(ALL_DIGS); //set digits LOW so nothing goes on       


        P1OUT |= ALL_SEGS; //set all segs off by putting them high;

}

void write_digit(unsigned char num, unsigned char dig){
	
	clear_display();
	P2OUT |= digit_bits[dig];
	P1OUT = number_seg_bytes[num]; 
	__delay_cycles(500);
}

void write_number(unsigned char number){

	//formats number based on digits to correct digits on display
	write_digit((number%10), 0 );
	write_digit((number/10), 1 );		

}
