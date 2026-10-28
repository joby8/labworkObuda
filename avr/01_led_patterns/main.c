/*
 * mecha2_02.c
 *
 * Created: 9/21/2023 1:43:33 PM
 * Author : F4-Laptop-11
 */ 

#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>
void led_init(void);
void led_out(uint8_t num);
void button_init(void);
void led_animation1(void);

int main(void)
{
	// T1
   uint8_t counter = 0;
   // T2
   uint8_t counter1 = 1;
   // T3
   uint8_t counter2 = 1;
   uint8_t dir = 0;	//0-L 1-R
   // T4
   uint8_t led_r = 0x01;	//1000|0001
   uint8_t led_l = 0x80;	//1000|____		10000000 = 0x80 L
							//____|0001		00000001 = 0x01 R
	// T5
	uint8_t counter5 = 0x0f;					
	// T7
	uint8_t button = 0;
	
		
	
   
   button_init();
    led_init();		//always important
	// led_out(255); //for test
	
    while (1) 
    {
		/*
		//---TASK 1---
		led_out(counter);
		_delay_ms(50);
		counter++;
		
		//---TASK 2---
		led_out(counter1);
		_delay_ms(100);
		if (counter1 == 0x80) counter1=1;
		counter1 <<= 1; //counter1 = counter1<<1;
		
		//---TASK 3---
		led_out(counter2);
		_delay_ms(20);
		if (!dir) //L <--
		{
			if (counter2 == 0x80) dir=1;
			else counter2 <<= 1;
		}
		if (dir) //R -->
		{
			if (counter2 == 0x01) dir=0;
			else counter2 >>= 1;
		}
		
		//---TASK 4---
		led_out(led_l| led_r);
		_delay_ms(50);
		//0001|1000 last state
		//0001|____ L - 00010000 = 0x10
		//____|1000 R - 00001000 = 0x08
		if (led_l==0x10)
		{
			led_l = 0x80;
			led_r = 0x01;
		}
		else
		{
			led_l >>= 1;
			led_r <<= 1;
		}
		*/
		//---TASK 5---
		led_out(counter5);
		_delay_ms(200);
		if (!dir) //L <--
		{
			if (counter5 == 0xF0) dir=1;
			else counter5 <<= 1;
		}
		if (dir) //R -->
		{
			if (counter5 == 0x01) dir=0, counter5 = 0x0f;
			else counter5 >>= 1;
		}
		/*
		//---TASK 6---
		led_out(led_l| led_r);
		_delay_ms(50);
		//0001|1000 last state
		//0001|____ L - 00010000 = 0x10
		//____|1000 R - 00001000 = 0x08
		,	if (!dir) //L <--
			{
				if (led_r == 0x01) dir=1;
				else 
				{
					led_r >>= 1;
					led_l <<= 1;
				}
			}
			if (dir) //R -->
			{
				if (led_r == 0x08) dir=0;
				else
				{ 
					led_l >>= 1;
					led_r <<= 1;
				}
			}
		*/
		//---TASK 7---
		//PIN - READ
		//	4	3	2	1	0
		//	0	0	0	0	1	-1	B0
		//	0	0	0	1	0	-2	B1
		//	0	0	1	0	0	-4	B2
		//	0	1	0	0	0	-8	B3
		//	1	0	0	0	0	-16	B4
		//	x x x B4 B3 B2 B1 B0
		//  0 0 0 1  1  1  1  1 -0x1F
		button = PING & 0x1F;
		switch(button)
		{
			case 1: led_out(170);/*code*/ break;
			case 2: led_animation1();/*led_out(0b00110011);*//*code*/ break;
			case 4: /*led_out(0b10101010);*//*code*/ break;
			case 8: /*led_out(0b01010101);*//*code*/ break;
			case 16: led_out(0xFF);/*code*/ break;
			default:break;
		}
    }
}

void led_init(void)
	{
		//8 LEDs
		//PORTB PORTD
		//LED3-0- PORTB7-4
		DDRB = 0xF0;
		//LED7-4- PORTD7-4
		DDRD = 0xF0;
	}
	
void led_out
	(uint8_t led)
	{
		//LEDs 7-4- led 7-4 bits
		PORTD &= 0x0F;
		PORTD |= led&0xF0;
		//LEDs 3-0- led 3-0 bits
		PORTB &= 0x0F;
		PORTB |= led<<4;
	}
	void button_init(void)
	{
		//5 PBUTTONS
		//G PORT 0-4 B0-B4
		//INPUT
		DDRG = 0;
		
	}
	
	void led_animation1()
{
	//B1
	uint8_t counter1 = 1;
	uint8_t button = 0;
	while(1)
	{
		led_out(counter1);
		_delay_ms(100);
		if (counter1 == 0x80) counter1=1;
		else counter1 <<= 1; //counter1 = counter1<<1;
		button = PING & 0x1F;
		if((button==1)||(button==4)||(button==8)||(button==16)) break;
	}
}
