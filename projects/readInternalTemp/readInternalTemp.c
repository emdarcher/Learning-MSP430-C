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


