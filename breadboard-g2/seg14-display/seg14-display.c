//code to display stuff on 14-segment display that is common cathode
//and is multiplexed .

/*

letters that correspond to segments
 ---A---
|\  |  /|
F H I J B
|_Ga|Gb_|
|  /|\  |
E M L K C
|/  |  \|  _
 ---D---  |_|-dp

*/

/*
pinout

# on package

IEE3785A
8504   3
           ____________
Gb_A/Ga_B -1         18-   J_A/H_B
empty     -2 ------- 17-   L_A/I_B
K_A/M_B   -3 digit 1 16-   empty
C_A/E_B   -4 ------- 15-   GND1_B
GND1_A    -5 o       14-   B_A/F_B
dp1_A     -6 ------- 13-   empty
empty     -7 digit 0 12-   D_A/A_B
GND0_B    -8 ------- 11-   empty
dp0_A     -9 o       10-   GND0_A
           ------------
*/
/*
mcu pins to display

P2.0 GND0_A
P2.1 GND0_B
P2.2 GND1_A
P2.3 GND1_B
P2.4 dp0_A
P2.5 dp1_A
P1.0 LED 	//for debugging
P1.1 D_A/A_B
P1.2 B_A/F_B
P1.3 C_A/E_B
P1.4 Gb_A/Ga_B
P1.5 J_A/H_B
P1.6 L_A/I_B
P1.7 K_A/M_B 
*/

//include stuff
#include <msp430.h>



#define GND0_A BIT0
#define GND0_B BIT1
#define GND1_A BIT2
#define GND1_B BIT3
#define dp0_A BIT4
#define dp1_A BIT5
#define LED_DEBUG BIT0
#define D_A BIT1
#define A_B BIT1
#define B_A BIT2
#define F_B BIT2
#define C_A BIT3
#define E_B BIT3
#define Gb_A BIT4
#define Ga_B BIT4
#define J_A BIT5
#define H_B BIT5
#define L_A BIT6
#define I_B BIT6
#define K_A BIT7
#define M_B BIT7


//prototypes
void write_segs(unsigned int bits, unsigned char digit);
void write_char(unsigned char c);

//typedefs
typedef struct {
	unsigned char p1; //store P1OUT values
	unsigned char p2; //store P2OUT values
	unsigned char ab; //store wether A or B, 0 or 1 respectively.
						//give 2 if doesn't matter, like the debug LED.
} segment;

/*again,

letters that correspond to segments
 ---A---
|\  |  /|
F H I J B
|_Ga|Gb_|
|  /|\  |
E M L K C
|/  |  \|  _
 ---D---  |_|-dp

*/


//ascii dec 32 to 127, so make sure that is dealt with when addressing 
//areas in the the arrow, ex. '$' = segs_for_char[4],
//NOT: segs_for_char[36], it is segs_for_char[ (<ascii char num> - 32) ];
//array to store values for segments for chars
unsigned int segs_for_char[] = {
//           led       
//           ^   b   p
//  A:       ^KLJGCBDd
//  B:MIHGEFA^
//       a
    0b0, //space
    0b0, //'!'
    0b0100010000000000, //'"'
    0b0, //'#'
    0b0101011001011010, //'$'
    0b1011010010011000, //'%'
    0b1010001010101010,//'&'
    0b0000000000100000,//'\''
    0b0000000010100000,//'('
    0b1010000000000000,//')'
    0b1111000011110000,//'*'
    0b0101000001010000, //'+'
	//will continue ... 
	//enter ALL THE CHAR SEGS!
};
/*
//array to store port output for segs int the bits in segs_for_char[] 
unsigned char p1_for_segs[] = {
//values for port1 for segs
//	  76543210
	0b
			
};*/

//an array of typedef'd struct segment type for segments
segment seg_ports[] = {

	{0, (dp0_A + dp1_A), 0}, //dp
	{D_A,0xF,0}, //D
	{B_A,0xF,0}, //B
	
	
};

void main(void)
{
	//disable watchdog timer
	WDTCTL = WDTPW + WDTHOLD;
	
	//setup ports for output
	P1OUT |= 0xFF;
	P2OUT |= 0x3F;
	P1DIR |= 0xFF;
	P2DIR |= 0x3F;


}

void write_segs(unsigned int bits, unsigned char digit)
{	
	int i;
	for (i = 0; i < 16; ++i){
		if(i == 0){ //dp stuff
			if(digit == 0){
				P1OUT = 0;
				P2OUT |= (dp0_A);
				P2OUT &= ~(GND0_A);
				//finish stuff here
			}
			else if(digit == 1){
				P1OUT = 0;
				P2OUT |= (dp1_A);
				P2OUT &= ~(GND1_A);
				
			}
		}
		else if(i == 8){
			//will put stuff for led here
		}
		else {
			
		}
	}
}
