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
#include <string.h>


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

#define NUM_DIGITS 2 //put number of digits/chars of displays here

//prototypes
void write_segs(unsigned int bits, unsigned char digit);
void write_char(unsigned char c, unsigned char position);
void write_string(unsigned char *out_string);

//typedefs
typedef struct {
	unsigned char p1; //store P1OUT values
	//unsigned char p2; //store P2OUT values
	unsigned char ab; //store wether A or B, 0 or 1 respectively.
						//give 2 if doesn't matter, like the debug LED.
} segment;

//delay thingy from this link: http://forum.43oh.com/topic/58-delay-function/
static void __inline__ msp_delay(register unsigned int n) 
{
   __asm__ __volatile__(
" 1: \n"
" dec %[n] \n"
" jne 1b \n"
      : [n] "+r"(n));
}

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

//GNDs for A on digits 
const unsigned int grounds_A[] = {GND0_A, GND1_A};
//GNDs for B on digits
const unsigned int grounds_B[] = {GND0_B, GND1_B};
//dps on A
const unsigned int dps_A[] = {dp0_A, dp1_A}; 

//ascii dec 32 to 127, so make sure that is dealt with when addressing 
//areas in the the arrow, ex. '$' = segs_for_char[4],
//NOT: segs_for_char[36], it is segs_for_char[ (<ascii char num> - 32) ];
//array to store values for segments for chars
const unsigned int segs_for_char[] = {
//           led       
//           ^   b   p
//  A:       ^KLJGCBDd
//  B:MIHGEFA^
//       a
    0b0000000000000000, //space
    0b0000000000001100, //'!'
    0b0100010000000000, //'"'
    0b1111111011111110, //'#'
    0b0101011001011010, //'$'
    0b1011010010111000, //'%'
    0b1010001010101010,//'&'
    0b0000000000100000,//'\''
    0b0000000010100000,//'('
    0b1010000000000000,//')'
    0b1111000011110000,//'*'
    0b0101000001010000, //'+'
    0b1000000000000000, //','
    0b0001000000010000, // '-'
    0b0000000000000001, //'.'
    0b1000000000100000, //'/'
    0b1000111000101110,//0
    0b0000000000001100, //1
    0b0001101000010110, //2
    0b0000001000011110, //3
    0b0001010000011100, //4
    0b0001011010000010, //5
    0b0001111000011010, //6
    0b0000001000001100, //7
    0b0001111000011110, //8
    0b0001011000011110, //9
    0b0100000001000000, //':'
    0b1100000000000000, //';'
    0b1000000000000010, //'<'
    0b0001000000010010, //'='
    0b0000000010000010, //'>'
    0b0000001001010100, //'?'
    0b0100111000010110, //'@'
    
//           led       
//           ^   b   p
//  A:       ^KLJGCBDd
//  B:MIHGEFA^
//       a
	//will continue ... 
	//enter ALL THE CHAR SEGS!
};
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


//an array of typedef'd struct segment type for segments
const segment seg_ports[] = {

	{0, 0}, //dp
	{D_A,0}, //D
	{B_A,0}, //B
	{C_A,0}, //C
	{Gb_A,0}, //Gb
	{J_A,0}, //J
	{L_A,0}, //L
	{K_A,0}, //K
	{LED_DEBUG, 2}, //led
	{A_B,1}, //A
	{F_B,1}, //F
	{E_B,1}, //E
	{Ga_B,1}, //Ga
	{H_B,1}, //H
	{I_B,1}, //I
	{M_B,1}, //M
	
	
	
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
	
	//infinite
	for(;;){
		//test string stuff
		//write_string("0042"); //show 42
		//write_char('*',0); //test if it can show '*' in position 0
        //unsigned char *outnum;
        unsigned char n = 0;
        for(n = 99; n >0; n--){
            int r;
            
            //outnum[1] = (unsigned char)((n % 10)+48);
            //outnum[0] = (unsigned char)((n / 10)+48);
            for(r = 0; r < 20; r++){ 
                //write_string(outnum);
                write_char(((unsigned char)((n%10)+48)), 0);
                write_char(((unsigned char)((n/10)+48)), 1);
            }
        }
	}
	

}

void write_string(unsigned char *out_string)
{
	int s = strlen(out_string); //finding out that strlen was needed
								// took way too long. :(
	int k;
	for(k = s; k > 0 ; k--){
		//go through chars in the string
		if(k >= 0){
			write_char(out_string[(s - k)], k - 1); 
		}
	}
	
	
}

void write_char(unsigned char c, unsigned char position)
{
	
	write_segs(segs_for_char[(c - 32)], position );
	
}

void write_segs(unsigned int bits, unsigned char digit)
{
if(digit < NUM_DIGITS){		
	int i;
	for (i = 0; i < 16; i++){
		unsigned char bit_state = ((bits & (1<<i)) == (1<<i)) ? 1:0;
		if(bit_state)
		{
			
			
		if(i == 0){ //dp stuff
			/*if(digit == 0){
				P1OUT = 0;
				P2OUT |= (dp0_A);
				P2OUT &= ~(GND0_A);
				//finish stuff here
			}
			else if(digit == 1){
				P1OUT = 0;
				P2OUT |= (dp1_A);
				P2OUT &= ~(GND1_A);
				
			}*/
			P1OUT = 0;
			P2OUT = 0x3F; //set all high to disable other GNDS
			P2OUT |= dps_A[digit];
			P2OUT &= ~(grounds_A[digit]);
		}
		else if(i == 8){
			//will put stuff for led here
			//disable GNDs and dps
			P2OUT = 0x0F;
			P1OUT = seg_ports[i].p1;
		}
		else {
			P1OUT = seg_ports[i].p1;
			if(seg_ports[i].ab == 0){ //for A
				P2OUT = 0x0F; //disable GNDs by putting them high
				P2OUT &= ~(grounds_A[digit]);
			}
			else if(seg_ports[i].ab == 1){ //for B
				P2OUT = 0x0F; //same
				P2OUT &= ~(grounds_B[digit]);
			}
			else { //for whatever
				P2OUT = 0x0F; //disable GNDs
			}
		}
		int j;
		//for(j = 0; j<100;j++){}
		msp_delay(500);
		} else {
		P1OUT = 0x00; //clear p1
		P2OUT = 0x0F; //disable GNDs and dps
		}
	}
} else {P1OUT = 0x00; P2OUT = 0x0F;}
}
