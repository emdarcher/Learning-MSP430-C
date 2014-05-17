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
#define GND1_b BIT3
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

void main(void)
{

}
