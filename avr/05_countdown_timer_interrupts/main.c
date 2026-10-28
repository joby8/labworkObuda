/*
Make the timer count from 1000 to 0, decrementing the counter every 100ms.
Pushbutton4: START (Grade 4)
Pushbutton3: STOP (Grade 4)
Pushbutton2: Enable input (Grade 5)
Pushbutton1: Disable input (Grade 5)
When the input is enabled, it is possible to set the start value of the timer from the
matrix keyboard (Grade 5)
 */ 
#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>
#include <avr/interrupt.h>
									// 0   1   2   3   4   5   6   7   8    9  10   11
const unsigned char keys_array[12] = { 69, 14, 13, 11, 22, 21, 19, 38, 37, 35, 70, 67 };
									//  0   1   2   3   4   5   6   7   8   9   *   #
uint8_t row=0x08; //Addressing of row 1

void sevenseg_init(void);
void matrix_init(void);
void sevenseg_digit(uint8_t digit, uint8_t num);
void separate(uint16_t num, uint8_t * d);
uint8_t matrix(void);
uint8_t status(void);
void tim0_CTC_OC_IT_init(void);
void tim2_OVF_IT_init(void);

uint8_t tim = 0;
uint16_t number=1000;
uint8_t d[4]={0};
uint8_t i=0;
uint8_t s=0;
uint8_t button=0;
uint8_t key_p=255;
uint8_t counter=0;
uint8_t duty=0;

int main(void)
{
	
	sevenseg_init();
	matrix_init();
	tim0_CTC_OC_IT_init();
	tim2_OVF_IT_init;
	separate(number, d); 
    while (1) 
    {
		/*
		
		
		*/
		
    }
}

ISR(TIMER2_OVF_vect)
{
	//SW PWM
	//ON: 0-duty OFF: duty-255
	if(counter==0) /*LED ON*/
	if(counter==duty) /*LED OFF*/
	counter++;
}

ISR(TIMER0_COMP_vect)
{
	//1ms
	sevenseg_digit(i, d[i]);
	i=(i+1)%4; //%4 0,1,2,3
	if(status()) s=status();		//BUTTON4,3,2,1 //NO B0
	if(tim==199)	//100ms tim-99	200ms tim-199
		{	tim=0;
			if(s==3) //PWM
			{
				key_p = matrix();
				if(key_p==10)	//* +5%
				{
					if(duty<=87) duty+=13	//max 100%
				}
				if(key_p==11)	//# -5%
				{
					if(duty>=13) duty-=13	//min 0%
				}
			}
			if(s==4)		//START
			{
				if(number==0) number=1000;
				else number--;
				separate(number, d);
			}
			if (s==2)					//ENABLED MATRIX
			{
				key_p = matrix();
				if(key_p<=9) //PORTA = 0x80 | key_p; //call matrix function
				{
					number=d[2]*1000+d[1]*100+d[0]*10+key_p;
					separate(number, d);
				}
			}
		}
		else tim++;
}

void sevenseg_init(void)
{
	//A PORT
	// 7 6 5 4 3 2 1 0
	// 1 1 1 1 1 1 1 1 0b11111111 = 0xFF
	DDRA = 0xFF; 
}

void sevenseg_digit(uint8_t digit, uint8_t num)
{
	//A PORT
	//	7	6	5	4	3	2	1	0
	//  EN  S2  S1  S0  N3	N2	N1	N0
	//  1	_	_	_	_	_	_	_		SEVSEG ENABLE - 0x80
	//	_   x	x	x	_	_	_	_		0-3
	//S2S1S0:	000-Digit0		001-Digit1		010-Digit2		011-Digit3
	//N3N2N1N0: 0000-1001 BCD 0-9
	if(num<=9 && digit<=3) PORTA = 0x80 | num | (digit<<4);
}

void separate(uint16_t num, uint8_t * d)
{
	if(num<=9999)
	{
		d[0] = (num/1)%10;
		d[1] = (num/10)%10;
		d[2] = (num/100)%10;
		d[3] = (num/1000)%10;
	}
}

void matrix_init(void)
{
	DDRC = 0x78; //matrix keyboard init
}

uint8_t matrix(void)
{
	uint8_t key=255; //non-existent key
	uint8_t read = 0; //to read
	uint8_t i =0; //for indexing
	//ROW ADDRESSING
	PORTC = row;
	//_delay_ms(1); //little delay
	//READ
	read = PINC & 0x7F; //PC7 <-- RGB RED!!!
	//SEARCH
	while(i<12) //checking the elements of the array
	{
		if(keys_array[i]==read) //found a match
		{
			while(read==(PINC & 0x7F)){};
			key= i; //the array index is the pressed key
			return key;
			//display on the seven-segment display
			//PORTA = 0x80 | key; //PC7 <--RGB RED LED !!!
			//break; //exit the loop in case of a match
		}
		i++; //index increment
		key= 255; //255 indicates if no key is pressed
	}
	if(row==0x40) row=0x08; //set the address of the first row after the last row is addressed
	else row<<=1; //set the address of the next row
	return key;
}

uint8_t status(void)
{
	uint8_t button=PING&0x7F;
	switch (button)
	{
		case 16: return 4; break;
		case 8: return 3; break;
		case 4: return 2; break;
		case 2: return 1; break;
	}
	return 0;
}

void tim0_CTC_OC_IT_init(void)
{
	//-----1ms IT
	TCCR0=(1<<WGM01) | (0<<WGM00) | (0<<CS02) | (1<<CS01) | (1<<CS00);
	OCR0=250-1;
	TIMSK|=(1<<OCIE0); //Local IT Enable
	sei();
}

void tim2_OVF_IT_init(void)
{
	//-----1ms IT
	//TIM2 - Normal mode OVF IT /1
	TCCR2=(0<<WGM21) | (0<<WGM20) | (0<<CS22) | (0<<CS21) | (1<<CS20);
	TIMSK|=(1<<TOIE2); //Local IT Enable
	sei();
}