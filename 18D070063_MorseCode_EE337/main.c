#pragma large
#include <at89c5131.h>
#include "lcd.h"							//Header file with LCD interfacing functions
#include "serial.c"						//C file with UART interfacing functions
//#include <morsecodes.h>				//Header file with morsecodes


sbit pulse = P0^3;						//Pulse input to speaker
sbit dotdashbar = P1^5;				//Signal toggles between ".", "-" and pause -- 1:".", 0:"-", toggles to opposite state during 1 sec pause
int cnt;

void genTone(int input[]);		//Tone generator function
void timer1_interrupt(void) interrupt 1  //Interrupt for timer1
{ //FFFF - 3CB0 = C34F = 49999 clk cycles = 0.025s
	TH0 = 0x3C;
	TL0 = 0x0B0;
	cnt--;
	if(cnt == 0)
	{	
		dotdashbar = ~dotdashbar;	// state toggle to break the inner while loop in genTone() function
		TF0 = 0;									// Clear Timer T0 flag
	}	
}

//------------------------------------------------Main function---------------------------------------------------------------------
void main(void)
{	
	unsigned char ch=0;
	pulse = 0;									//Set as Output
	dotdashbar = 0 ;							//Set as Output
	lcd_init();									//Initialize LCD							
	TMOD = 0x21;								//Run Timer 0 in Mode 1 - for count 1 or 2 sec and Timer 1 in Mode 2 - for UART
	EA = 1;											//Enable Global Interrupt
	ET0 = 1;										//Enable Timer 0 Interrupt
	
	while(1)
	{	uart_init();							//Initialize UART
		lcd_cmd(0x82);						//Place LCD pointer at third position from left in top row
		lcd_write_string("Input Please");
		ch = receive_char();			//Receive a character
		transmit_char(ch);				//Transmit the same character
		lcd_cmd(0x01);						//clear LCD
		ES = 0;										// Disable Serial Interrupt
		//call Tone generator function with argument as input character
		switch(ch)								
		{	//input to genTone is a int array declared in morsecodes.h
			case 'A':genTone(A);
							 break;
			case 'B':genTone(BB);
							 break;
			case 'C':genTone(C);
							 break;
			case 'D':genTone(D);
							 break;
			case 'E':genTone(E);
							 break;
			case 'F':genTone(F);
							 break;
			case 'G':genTone(G);
							 break;
			case 'H':genTone(H);
							 break;
			case 'I':genTone(I);
							 break;
			case 'J':genTone(J);
							 break;
			case 'K':genTone(K);
							 break;
			case 'L':genTone(L);
							 break;
			case 'M':genTone(M);
							 break;
			case 'N':genTone(N);
							 break;
			case 'O':genTone(O);
							 break;
			case 'P':genTone(P);
							 break;
			case 'Q':genTone(Q);
							 break;
			case 'R':genTone(R);
							 break;
			case 'S':genTone(S);
							 break;
			case 'T':genTone(T);
							 break;
			case 'U':genTone(U);
							 break;
			case 'V':genTone(V);
							 break;
			case 'W':genTone(W);
							 break;
			case 'X':genTone(X);
							 break;
			case 'Y':genTone(Y);
							 break;
			case 'Z':genTone(Z);
							 break;
			case '1':genTone(one);
							 break;
			case '2':genTone(two);
							 break;
			case '3':genTone(three);
							 break;
			case '4':genTone(four);
							 break;
			case '5':genTone(five);
							 break;
			case '6':genTone(six);
							 break;
			case '7':genTone(seven);
							 break;
			case '8':genTone(eight);
							 break;
			case '9':genTone(nine);
							 break;
			case '0':genTone(zero);
							 break;					
			default:lcd_cmd(0x82);
							lcd_write_string("Invalid Input");
							msdelay(1900);						//1.9 sec delay
							break;
		}
		msdelay(100);												//0.1+1.9 = 2 sec delay
		lcd_cmd(0x01);											//clear LCD
	}
}

//---------------------------------------------Tone Generator Function--------------------------------------------------------------
void genTone(int input[])
{	int i = 0;								//input[] iterator
	while(1)
	{		
		if (input[i] == 0)							//check if ith element of input array is 0 (".")
		{	dotdashbar = 1;								// indicates dot is playing
			cnt = 40;											// 1 sec Timer	
			TH0 = 0x3C;
			TL0 = 0xB0;
			TR0 = 1;  										//Start Timer 0 
			while(dotdashbar == 1)
			{	// keep toggling pulse input to speaker with frequency 100Hz for 1 sec
				// runs until Timer 0 generates Interrupt 40 times (after 1 sec)
				pulse = ~pulse;							
				msdelay(5);		
			}
			cnt = 40;											// 1 sec Timer for pause between two characters
			pulse = 0;										// Input to speaker is 0
			while(cnt!=0);
		}
		else if (input[i] == 1)					//check if ith element of input array is 1 ("-")
		{
			dotdashbar = 0;								// indicates dash is playing
			cnt = 80;											// 2 sec timer
			TH0 = 0x3C;
			TL0 = 0xB0;
			TR0 = 1;											//Start Timer 0 
			while(dotdashbar == 0)
			{ // keep toggling pulse input to speaker with frequency 250Hz for 1 sec
				// runs until Timer 0 generates Interrupt 80 times (after 2 sec)
				pulse = ~pulse;
				msdelay(2);
			}
			cnt = 40;											// 1 sec Timer for pause between two characters
			pulse = 0;										// Input to speaker is 0
			while(cnt!=0);
		}
		else {													//runs when entire morse code sequence is played
			break;												//break if input character is anything other than 0 or 1 
		}
		i = i+1;												//increment input[] iterator
	}
}




