#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
const unsigned char keys_array[12] = { 69, 14, 13, 11, 22, 21, 19, 38, 37, 35, 70, 67 };
//  0   1   2   3   4   5   6   7   8   9   *   #
uint8_t row=0x08; //Addressing of row 1

void led_init(void);
void led_out(uint8_t led);
void task1(void);
uint8_t matrix(void);
void sevenseg_digit(uint8_t digit, uint8_t num);

volatile uint8_t counter = 8;
volatile uint8_t start_counter = 0;

int main(void)
{
	uint8_t button = 0;
	led_init();
	DDRG = 0;		// BUTTON INIT
	DDRA=0xFF;
	while (1)
	{
		button = PING & 0x1F;

		if (button == 8) // BUTTON3
		start_counter = 1;
		
		if (button == 16) // BUTTON4
		{
			start_counter = 0;
			sevenseg_digit(0, 0xFF); // Turn off display
		}

		if (start_counter)
		{
			sevenseg_digit(0, counter); // Display current value of counter on Digit0
			if (counter > 2) counter--;
			else counter = 8; // Reset counter
			_delay_ms(500);   // Wait 0.5 seconds
		}
	}
}

void led_init(void)
{
	DDRB= 0xF0;
	DDRD= 0xF0;
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
	uint8_t led=0xC0;
	uint8_t dir=0;		//1-L	0-R
	uint8_t button=0;
	while(1)
	{
		button=PING&0x1F;	//x x x B4 B3 B2 B1 B0
							//0 0 0 1  1  1  1  1
							//0 0 0 0  0  0  1  0  - 2
		//STOP BUTTON1
		if(button==2)
		{
			led_out(0); //LEDs OFF
			break;
		}
		led_out(led);
		if(!dir)	//0-R
		{
			if(led==0x03) dir=1;
			else led>>=1;
		}
		if(dir)		//1-L
		{
			if(led==0xC0) dir=0;
			else led<<=1;
		}
		_delay_ms(200);
	}
}

uint8_t matrix(void)
{
	uint8_t key=255; //non-existent key
	uint8_t read = 0; //to read
	uint8_t i =0; //for indexing
	//ROW ADDRESSING
	PORTC = row;
	_delay_ms(1); //little delay
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
