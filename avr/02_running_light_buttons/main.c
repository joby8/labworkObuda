/*
 * GccApplication5.c
 *
 * Created: 10/9/2023 10:05:03 AM
 * Author : F4-Laptop-18
 */ 

#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
void led_out(uint8_t led);
void led_init(void);
void task1(void);

int main(void)
{
	
	uint8_t button =0;

	led_init();
    DDRG=0;
	/* Replace with your application code */
    while (1) 
    {
		//start button
		button=PING&0x1f;
		if (button==1) task1();
		
		{
		}
    }
}

void led_init(void)
{
	DDRB = 0xF0;
	DDRD = 0xF0;
}
void led_out(uint8_t led)
{
	PORTD &= 0x0F;
	PORTD |= led&0xF0;
	PORTB &= 0x0F;
	PORTB |= led<<4;
}

void task1(void) 
{
	void led_out(uint8_t led);
	void led_init(void);
	uint8_t led=0xc0;
	uint8_t dir =0;
	uint8_t button =0;

while (1)
{
	button=PING&0x1f; //	x	x	x	B4	B3	B2	B1	B0
						//	
		//stop button 1
	if(button==2)
	{
		led_out(0);
		break;
	}
	led_out(led);
	if (!dir) //0=R
	{
		if(led==0x03) dir=1;
		else led>>=1;
	}
	if (dir) //0=R
	{
		if (led==0xc0) dir=0;
		else led<<=1;
	}
	_delay_ms(200);
}
}