
/*
Code to control 7 segment display using PCA9551 - 8 led controller i2c
Wire library uses 7 bit addressing
PCA9551 LED controller - address 1100+xxx x=external pulls high or low.
device address = 1100000 = 96 = 
000 AI 0 B2 B1 B0  
AI = 0 for auto increment off
B2 B1 B0 = 
LED0-3 - 101 - 00000101 = 5 / 21 with AI
LED4-7 - 110 - 00000110 = 6 / 22 with AI

bits 7->0
00 = LOW (on)
01 = HIGH (off)

00000000 = 0 = ----
01000000 = 32 = x---
01010000 = 80 = xx--
01010100 = 84 = xxx-
01010101 = 85 = xxxx
00010101 = 21 = -xxx
00000101 = 5 = --xx
00000001 = 1 = ---x
01000100 = 68 = x-x-
00010001 = 17 = -x-x
00010000 = 16 = -x--
00000100 = 4 = --x-
01000101 = 69 = x-xx
01010001 = 81 = xx-x
00010100 = 20 = -xx-
01000001 = 65 = x--x




 My seven seg display info:
 W I R I N G   G U I D E


=== Common Anode digits ===
Arduino  Display  Digit
 9         12       0 Leftmost
 10         9       1
 11         8       2
 12         6       3 Rightmost
 
=== Cathode Segments ===
Teensy    GPIOD    Display   Segment  PCA9551
  2        0        11         A		LED0-pin4
  14       1        7          B		pin5
  7        2        4          C		pin6
  8        3        2          D		pin7
  6        4        1          E		pin9
  20       5        10         F		pin 10
  21       6        5          G		pin 11
  5        7        3         DEC  		pin 12

===Segment Layout===
    A
  F   B
    G
  E   C
    D   
          byte .GFEDCBA
segment code, Bxxxxxxxx

i = 85, 84
2 = 00010000 , 01000100 = 16, 68
c = 00010101, 01000100 = 21, 68
*/

#include <Wire.h>
#include <elapsedMillis.h>

byte digPins[] = { 2, 3, 4, 5 };   //digit pins
byte pinCountDig = 4;         //number of digits
byte seg [] = { 21, 22 }; //21 is first led register with auto increment, so send 21, then 2 btyes for led0-3 and then led4-7
/*
byte seg0[] = {     
	0, // = 00000000 = ---- 
	1, // = 00000001 = ---x 
	4, // = 00000100 = --x-
	5, // = 00000101 = --xx	
	16, // = 00010000 = -x--
	17, // = 00010001 = -x-x
	20, // = 00010100 = -xx-
	21, // = 00010101 = -xxx
	32, // = 01000000 = x---	
	65, // = 01000001 = x--x			
	68, // = 01000100 = x-x-
	69, // = 01000101 = x-xx
	80, // = 01010000 = xx--
	81, // = 01010001 = xx-x
	84, // = 01010100 = xxx-
	85, // = 01010101 = xxxx
};

byte seg1[] = {     
	0, // = 00000000 = ---- //0
	1, // = 00000001 = ---x //1
	4, // = 00000100 = --x- //2
	5, // = 00000101 = --xx	//3
	16, // = 00010000 = -x-- //4
	17, // = 00010001 = -x-x //5
	20, // = 00010100 = -xx- //6
	21, // = 00010101 = -xxx //7
	32, // = 01000000 = x---	
	65, // = 01000001 = x--x			
	68, // = 01000100 = x-x- //10
	69, // = 01000101 = x-xx
	80, // = 01010000 = xx--
	81, // = 01010001 = xx-x
	84, // = 01010100 = xxx-
	85, // = 01010101 = xxxx //15
};
*/

//             0    1   2  3   4   5   6    7   8   9   A  b    c   d  E    F   g  h   i   j   L   m    n  o   p   q   r   s   t   u   y   Z   -    _   ^   .  " "
byte seg0 [] { 0,  65, 16, 0,  65, 4,  4,  64, 0,  64, 64, 5,  21, 1,  20, 84, 0,  69, 85, 1,  21, 68, 69, 5,  80, 64, 85, 4,  21, 5,  65, 16, 85, 21, 84, 85, 85 };
byte seg1 [] { 80, 85, 68, 69, 65, 65, 64, 85, 64, 65, 64, 64, 68, 68, 64, 64, 65, 64, 84, 84, 80, 84, 68, 68, 64, 65, 68, 65, 64, 84, 65, 68, 69, 85, 85, 21, 85};
elapsedMillis digWait; 

void setup() {
  Wire.begin();
  Serial.begin(9600);
  for ( int pinDig = 0; pinDig < pinCountDig; pinDig++ ) {    // set digit pins as outputs
    pinMode(digPins[pinDig], OUTPUT);
  }
  Serial.println("set up");
}

//write digits
void digitOn(int digit, int character, int character2){
  if (digit == 0){

	digitalWriteFast(digPins[0],HIGH); //first digit on
    digitalWriteFast(digPins[1],LOW); //other pins off
    digitalWriteFast(digPins[2],LOW);
    digitalWriteFast(digPins[3],LOW);
  	//i2c
  	Wire.beginTransmission(96);
  	Wire.write(seg[0]);
	Wire.write(seg0[character]);
	Wire.write(seg1[character2]);
  	Wire.endTransmission();
    delay(2);
  	Wire.beginTransmission(96);
  	Wire.write(seg[0]);
	Wire.write(seg0[36]);
	Wire.write(seg1[36]);
  	Wire.endTransmission();
 }
  else if (digit == 1) {
    digitalWriteFast(digPins[0],LOW); //other pins off
    digitalWriteFast(digPins[1],HIGH); //second digit on
    digitalWriteFast(digPins[2],LOW);
    digitalWriteFast(digPins[3],LOW);
  	Wire.beginTransmission(96);
  	Wire.write(seg[0]);
	Wire.write(seg0[character]);
	Wire.write(seg1[character2]);
  	Wire.endTransmission();
    //Serial.println(digit);
    delay(2);
  	Wire.beginTransmission(96);
  	Wire.write(seg[0]);
	Wire.write(seg0[36]);
	Wire.write(seg1[36]);
  	Wire.endTransmission();
  }
  else if (digit == 2) {
    digitalWriteFast(digPins[0],LOW); //other pins off
    digitalWriteFast(digPins[1],LOW); 
    digitalWriteFast(digPins[2],HIGH); //third digit on 
    digitalWriteFast(digPins[3],LOW);
  	Wire.beginTransmission(96);
  	Wire.write(seg[0]);
	Wire.write(seg0[character]);
	Wire.write(seg1[character2]);
  	Wire.endTransmission();
    delay(2);
  	Wire.beginTransmission(96);
  	Wire.write(seg[0]);
	Wire.write(seg0[36]);
	Wire.write(seg1[36]);
  	Wire.endTransmission();
  }  
  else if (digit == 3) {
    digitalWriteFast(digPins[0],LOW); //other pins off
    digitalWriteFast(digPins[1],LOW); 
    digitalWriteFast(digPins[2],LOW);  
    digitalWriteFast(digPins[3],HIGH); //Fourth digit on
  	Wire.beginTransmission(96);
  	Wire.write(seg[0]);
	Wire.write(seg0[character]);
	Wire.write(seg1[character2]);
  	Wire.endTransmission();
    delay(2);
  	Wire.beginTransmission(96);
  	Wire.write(seg[0]);
	Wire.write(seg0[36]);
	Wire.write(seg1[36]);
  	Wire.endTransmission();
  }
}


void loop(){
	int i;
	for ( i = 0; i < 37; i++ ) {
		if ( i > 36 ) {
			i = 0;
		}
		digWait = 0;
		while( digWait < 200 ){
		  digitOn(0, i, i);
	      digitOn(1, i, i);
	      digitOn(2, i, i);
	      digitOn(3, i, i);
		}
	}
}